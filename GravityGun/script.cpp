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

void drawDebug(Vector o, int r, int g, int b)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector handPosition(PED::GET_PED_BONE_COORDS(playerPed, pedBones[13], 0.0f, 0.0f, 0.0f));
	GRAPHICS::DRAW_LINE(handPosition.x, handPosition.y, handPosition.z, handPosition.x + o.x, handPosition.y + o.y, handPosition.z + o.z, r, g, b, 255);
}

void main()
{
	Entity hoverEntity = 0;
	while (true)
	{
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		std::stringstream sstream;
		bool hoverEntityState = get_key_pressed('E');
		Vector handPosition(PED::GET_PED_BONE_COORDS(playerPed, pedBones[13], 0.0f, 0.0f, 0.0f));
		Vector elbowPosition(PED::GET_PED_BONE_COORDS(playerPed, pedBones[3], 0.0f, 0.0f, 0.0f));

		Vector aim = handPosition - elbowPosition;
		aim = aim.normalized();
		Vector up(0, 0, 1);
		Vector offset = aim.cross(up);
		offset = offset.normalized();

		float aimMul = 6.0f;
		float offsetMul = 0.8f;
		float upMul = -0.8f;
		float velocityMultiplier = 50.0f;

		Vector hoverPosition;
		hoverPosition = handPosition + aim * aimMul + offset * offsetMul + up * upMul;
		drawDebug(up, 255, 255, 0);
		drawDebug(offset, 0, 0, 255);
		drawDebug(aim, 255, 0, 255);

		if (PLAYER::IS_PLAYER_FREE_AIMING(player) &&
			WEAPON::GET_SELECTED_PED_WEAPON(playerPed) == GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN") &&
			!PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
		{
			Any targetEntity = 0;
			PLAYER::_0x2975C866E6713290(player, &targetEntity); //_GET_AIMED_ENTITY

			sstream << "target: " << targetEntity << "hover: " << hoverEntity;
			if (!hoverEntityState)
			{
				hoverEntity = 0;
			}
			if (hoverEntity)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(hoverEntity, hoverPosition.x, hoverPosition.y, hoverPosition.z, 0, 0, 1);
			}

			if (targetEntity)
			{
				Vector playerPos(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0f, 0.0f, 0.0f));
				Vector entityPos(ENTITY::GET_ENTITY_COORDS(targetEntity, true));

				//If the player wants to hover an entity
				if (hoverEntityState)
				{
					//And is currently not hovering an entity
					if (!hoverEntity)
					{
						//Set the hover entity to current target
						hoverEntity = targetEntity;
					}
				}
				
				if (PED::IS_PED_SHOOTING(playerPed))
				{	
					Vector velocity;
					velocity = hoverPosition - handPosition;
					velocity = velocity.normalized() * velocityMultiplier;

					ENTITY::SET_ENTITY_VELOCITY(targetEntity, velocity.x, velocity.y, velocity.z);
				}
			}
		}
		else
		{
			hoverEntity = 0;
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
