#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Tank.h"
#include "Game.h"

CTank::CTank(float x, float y) : CGameObject()
{
	SetCannonUP(false);
	SetState(TANK_STATE_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}
void CTank::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Cannon == NULL)
		return;
	switch (isCannonUp)
	{
	case false:
	{
		Cannon->SetCannonUP(false);
		switch (state)
		{
		case TANK_STATE_RIGHT:
			Cannon->SetPosition(x + 18, y + 1);
			Cannon->SetState(CANNON_STATE_RIGHT);
			break;
		case TANK_STATE_LEFT:
			Cannon->SetPosition(x - 9, y + 1);
			Cannon->SetState(CANNON_STATE_LEFT);
			break;
		}
		break;
	}
	case true:
	{
		Cannon->SetCannonUP(true);
		switch (state)
		{
		case TANK_STATE_RIGHT:
			Cannon->SetPosition(x + 10, y -10);
			Cannon->SetState(CANNON_STATE_RIGHT);
			break;
		case TANK_STATE_LEFT:
			Cannon->SetPosition(x +6, y -10);
			Cannon->SetState(CANNON_STATE_LEFT);
			break;
		}
		break;
	}
	}
}

void CTank::Render()
{
	int ani = -1;
	int alpha = 255;
	switch (isCannonUp)
	{
	case false:
	{
		switch (state)
		{
		case TANK_STATE_RIGHT:
			ani = TANK_ANI_RIGHT;
			animation_set->at(ani)->Render(round(x), round(y), alpha);
			break;
		case TANK_STATE_LEFT:
			ani = TANK_ANI_LEFT;
			animation_set->at(ani)->Render(round(x), round(y), alpha);
			break;
		}
		DebugOut(L"[INFO] Tank has been rendered!\n");
		break;
	}
	case true:
	{
		switch (state)
		{
		case TANK_STATE_RIGHT:
			ani = TANK_ANI_CANNONUP_RIGHT;
			animation_set->at(ani)->Render(round(x), round(y), alpha);
			break;
		case TANK_STATE_LEFT:
			ani = TANK_ANI_CANNONUP_LEFT;
			animation_set->at(ani)->Render(round(x), round(y), alpha);
			break;
		}
		DebugOut(L"[INFO] Tank has been rendered!\n");
		break;
	}
	}
	


}

void CTank::SetState(int state)
{
	CGameObject::SetState(state);
}

void CTank::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}