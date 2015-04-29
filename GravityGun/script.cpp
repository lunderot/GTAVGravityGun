#include "script.h"
#include "Vector.h"

#include <string>
#include <ctime>
#include <sstream>

#pragma warning(disable : 4244 4305) // double <-> float conversions

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
{
	// default values
	int text_col[4] = {255, 255, 255, 255},
		rect_col[4] = {70, 95, 95, 255};
	float text_scale = 0.35;
	int font = 0;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;

		rect_col[0] = 218;
		rect_col[1] = 242;
		rect_col[2] = 216;

		if (rescaleText) text_scale = 0.40;
	}

	if (title)
	{
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;

		if (rescaleText) text_scale = 0.50;
		font = 1;
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_0x521FB041D93DD0E4("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f), 
		lineWidthScaled, ((((float)(num25)* UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);	
}

bool get_key_pressed(int nVirtKey)
{
	return (GetAsyncKeyState(nVirtKey) & 0x8000) != 0;
}

int pedBones[] =
{
	11816, //
	14201, //
	18905, //
	28252, //right elbow
	28422, //right palm
	31086, //
	36864, //
	39317, //
	40269, //
	42114, //
	45509, //
	51826, //
	52301, //
	57005, //right hand
	58271, //
	60309, //
	61163, //
	63931, //
};

void main()
{
	while (true)
	{
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		std::stringstream sstream;
		bool hoverEntity = get_key_pressed('E');

		Vector3 handPosition = PED::GET_PED_BONE_COORDS(playerPed, pedBones[13], 0.0f, 0.0f, 0.0f);
		Vector3 elbowPosition = PED::GET_PED_BONE_COORDS(playerPed, pedBones[3], 0.0f, 0.0f, 0.0f);
		Vector3 aim;
		aim.x = handPosition.x - elbowPosition.x;
		aim.y = handPosition.y - elbowPosition.y;
		aim.z = handPosition.z - elbowPosition.z;
		float aimMultiplier = 3.0f;
		float magnitude = sqrt(pow(aim.x, 2) + pow(aim.z, 2) + pow(aim.z, 2));
		aim.x /= magnitude;
		aim.y /= magnitude;
		aim.z /= magnitude;
		aim.x *= aimMultiplier;
		aim.y *= aimMultiplier;
		aim.z *= aimMultiplier;
		

		Vector up(0, 0, 1);

		Vector3 offset;


		Vector3 hoverPosition;
		hoverPosition.x = handPosition.x + aim.x;
		hoverPosition.y = handPosition.y + aim.y;
		hoverPosition.z = handPosition.z + aim.z;
		GRAPHICS::DRAW_LINE(handPosition.x, handPosition.y, handPosition.z, hoverPosition.x, hoverPosition.y, hoverPosition.z, 255, 0, 0, 255);
		
		

		if (PLAYER::IS_PLAYER_FREE_AIMING(player) &&
			WEAPON::GET_SELECTED_PED_WEAPON(playerPed) == GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN"))
		{
			Any temp = 0;
			PLAYER::_0x2975C866E6713290(player, &temp); //_GET_AIMED_ENTITY

			sstream << "target: "<<temp;
			if (temp)
			{
				Entity targetEntity = temp;
				Vector3 playerPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0f, 0.0f, 0.0f);
				Vector3 entityPos = ENTITY::GET_ENTITY_COORDS(targetEntity, true);
				GRAPHICS::DRAW_LINE(playerPos.x, playerPos.y, playerPos.z, entityPos.x, entityPos.y, entityPos.z, 255, 0, 0, 255);

				if (hoverEntity)
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(targetEntity, hoverPosition.x, hoverPosition.y, hoverPosition.z, 0, 0, 1);
				}
				
				if (PED::IS_PED_SHOOTING(playerPed))
				{
					sstream << " setting velocity";
					
					Vector3 velocity;
					float velocityMultiplier = 30.0f;

					//Get vector between player and entity
					velocity.x = entityPos.x - playerPos.x;
					velocity.y = entityPos.y - playerPos.y;
					velocity.z = entityPos.z - playerPos.z;

					//Normalize vector
					float magnitude = sqrt(pow(velocity.x, 2) + pow(velocity.z, 2) + pow(velocity.z, 2));
					velocity.x /= magnitude;
					velocity.y /= magnitude;
					velocity.z /= magnitude;

					//Multiply to add more power
					velocity.x *= velocityMultiplier;
					velocity.y *= velocityMultiplier;
					velocity.z *= velocityMultiplier;

					ENTITY::SET_ENTITY_VELOCITY(targetEntity, velocity.x, velocity.y, velocity.z);
				}
			}
		}
		draw_menu_line(sstream.str(), 250.0, 9.0, 60.0, 0.0, 9.0, false, false);

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
