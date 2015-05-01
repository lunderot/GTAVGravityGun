#include "script.h"
#include "Vector.h"

#include <string>
#include <ctime>
#include <sstream>
#include <DirectXMath.h>

#define PI 3.14159265

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

void drawDebug(Vector o, int r, int g, int b, Vector startPosition)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector handPosition(PED::GET_PED_BONE_COORDS(playerPed, pedBones[13], 0.0f, 0.0f, 0.0f));
	GRAPHICS::DRAW_LINE(startPosition.x, startPosition.y, startPosition.z, startPosition.x + o.x, startPosition.y + o.y, startPosition.z + o.z, r, g, b, 255);
}

void main()
{
	Entity hoverEntity = 0;
	bool attachedObject = false;
	Any handObject = 0;
	Vector attachOffset(0.2f, 0.05f, 0.0f);

	float hoverDistance = 6.0f;
	float velocityMultiplier = 50.0f;

	while (true)
	{
		std::stringstream sstream;
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		if (get_key_pressed(VK_NUMPAD2) && !attachedObject)
		{
			Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0f, 0.0f, 1.0f);
			handObject = OBJECT::CREATE_OBJECT(0x848B8ABA, offset.x, offset.y, offset.z, 1, 1, 0);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(handObject, playerPed, PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 28422), attachOffset.x, attachOffset.y, attachOffset.z, 0.0f, 0.0f, -90.0f, 0, 0, 0, 0, 2, 1);
			attachedObject = true;
		}
		
		Vector handObjectPosition;
		Vector handObjectForwardVector;

		if (handObject)
		{
			handObjectPosition = ENTITY::GET_ENTITY_COORDS(handObject, true);
			handObjectForwardVector = ENTITY::GET_ENTITY_FORWARD_VECTOR(handObject);
			drawDebug(handObjectForwardVector, 255, 128, 0, handObjectPosition);
			drawDebug(handObjectForwardVector * hoverDistance, 255, 0, 0, handObjectPosition);
		}
		
		bool hoverEntityState = get_key_pressed('E');
		
		Vector hoverPosition = handObjectPosition + handObjectForwardVector.normalized() * hoverDistance;

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
					Vector velocity = handObjectForwardVector.normalized() * velocityMultiplier;
					ENTITY::SET_ENTITY_VELOCITY(targetEntity, velocity.x, velocity.y, velocity.z);
				}
			}
		}
		else
		{
			hoverEntity = 0;
		}
		draw_menu_line(sstream.str(), 350.0, 9.0, 60.0, 0.0, 9.0, false, false);

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
