#include "script.h"
#include "Vector.h"

#include <string>
#include <sstream>

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}

void DrawMenuLine(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
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

bool GetKeyPressed(int nVirtKey)
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

void DrawDebug(Vector o, int r, int g, int b, Vector startPosition)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector handPosition(PED::GET_PED_BONE_COORDS(playerPed, pedBones[13], 0.0f, 0.0f, 0.0f));
	GRAPHICS::DRAW_LINE(startPosition.x, startPosition.y, startPosition.z, startPosition.x + o.x, startPosition.y + o.y, startPosition.z + o.z, r, g, b, 255);
}

char* weaponName = "WEAPON_STUNGUN";
float hoverDistance = 6.0f;
float velocityMultiplier = 80.0f;
char keyCode = 'E';
int shootCooldown = 50; //In milliseconds

Vector attachOffset(0.2f, 0.05f, 0.0f);
Vector attachRotationOffset(0.0f, 0.0f, -95.0f);
Vector dummyObjectRestingPosition(0.0f, 0.0f, -1000.0f);


Any CreateDummyObject(Any playerPed)
{
	Any object = OBJECT::CREATE_OBJECT(0x848B8ABA, dummyObjectRestingPosition.x, dummyObjectRestingPosition.y, dummyObjectRestingPosition.z, 1, 1, 0);
	ENTITY::SET_ENTITY_ALPHA(object, 0, false);
	return object;
}

void AttachObjectToPlayer(Any playerPed, Any object)
{
	ENTITY::ATTACH_ENTITY_TO_ENTITY(object, playerPed, PED::GET_PED_BONE_INDEX(playerPed, 28422), attachOffset.x, attachOffset.y, attachOffset.z, attachRotationOffset.x, attachRotationOffset.y, attachRotationOffset.z, 0, 0, 0, 0, 2, 1);
}

void DetachObjectFromPlayer(Any object)
{
	ENTITY::DETACH_ENTITY(object, 1, 1);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(object, dummyObjectRestingPosition.x, dummyObjectRestingPosition.y, dummyObjectRestingPosition.z, 0, 0, 1);
}

Vector GetHoverPositionFromObject(Any object)
{
	Vector objectPosition = ENTITY::GET_ENTITY_COORDS(object, true);
	Vector objectForwardVector = ENTITY::GET_ENTITY_FORWARD_VECTOR(object);
	return objectPosition + objectForwardVector.normalized() * hoverDistance;
}

Vector GetShootDirectionFromObject(Any object)
{
	Vector objectPosition = ENTITY::GET_ENTITY_COORDS(object, true);
	Vector objectForwardVector = ENTITY::GET_ENTITY_FORWARD_VECTOR(object);
	return objectForwardVector.normalized() * velocityMultiplier;
}

void main()
{
	bool hoverButton = false;
	Any object = CreateDummyObject(PLAYER::PLAYER_PED_ID());
	Any hoverEntity = 0;
	Any shootEntity = 0;
	int shootTimer = 0;
	bool shootState = false;

	while (true)
	{
		std::stringstream debug;
		hoverButton = GetKeyPressed(keyCode);
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		
		if (shootTimer > 0)
		{
			shootTimer--; //TODO: Decrease with milliseconds passed since last frame
		}
		if (shootEntity && shootTimer == 0 && !shootState)
		{
			Vector velocity = GetShootDirectionFromObject(object);
			ENTITY::SET_ENTITY_VELOCITY(shootEntity, velocity.x, velocity.y, velocity.z);
			shootState = true;
			shootTimer = shootCooldown;
		}
		if (shootTimer == 0)
		{
			shootState = false;
			shootEntity = 0;
		}
		

		if (PLAYER::IS_PLAYER_FREE_AIMING(player) &&
			WEAPON::GET_SELECTED_PED_WEAPON(playerPed) == GAMEPLAY::GET_HASH_KEY(weaponName) &&
			!PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
		{
			AttachObjectToPlayer(playerPed, object);
			if (hoverButton && hoverEntity && hoverEntity != shootEntity)
			{
				Vector hoverPosition = GetHoverPositionFromObject(object);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(hoverEntity, hoverPosition.x, hoverPosition.y, hoverPosition.z, 0, 0, 1);
				if (PED::IS_PED_SHOOTING(playerPed))
				{
					shootEntity = hoverEntity;
				}
			}
			else
			{
				hoverEntity = 0;
				PLAYER::_0x2975C866E6713290(player, &hoverEntity); //_GET_AIMED_ENTITY
			}
		}
		else
		{
			hoverEntity = 0;
			DetachObjectFromPlayer(object);
		}
		debug	<< "   se: " << shootEntity
				<< "\t st: " << shootTimer
				<< "\t he: " << hoverEntity
				<< "\t hb: " << hoverButton;
		DrawMenuLine(debug.str(), 500.0, 9.0, 60.0, 0.0, 9.0, false, false);
		WAIT(0);
	}
}

void ScriptMain()
{
	main();
}
