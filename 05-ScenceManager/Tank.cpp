#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Tank.h"
#include "Game.h"

CTank::CTank(float x, float y) : CGameObject()
{
	SetCannonUP(false);
	SetMoving(false);
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
	if (Wheels.size() != 0)
	{
		for (unsigned int i = 0; i < Wheels.size(); i++)
		{
			switch (state)
			{
			case TANK_STATE_RIGHT:
				if (isMoving == true)
					Wheels[i]->SetState(WHEEL_STATE_MOVE_RIGHT);
				else
					Wheels[i]->SetState(WHEEL_STATE_IDLE);
				switch (isCannonUp)
				{
				case false:
					Wheels[i]->SetPosition(x + i * 18, y + 7);
					break;
				case true:
					Wheels[i]->SetPosition(x + i * 18+3, y + 4);
					break;
				}				
				break;
			case TANK_STATE_LEFT:
				if (isMoving == true)
					Wheels[i]->SetState(WHEEL_STATE_MOVE_LEFT);
				else
					Wheels[i]->SetState(WHEEL_STATE_IDLE);
				switch (isCannonUp)
				{
				case false:
					if (i == 0)
						Wheels[i]->SetPosition(x - 8, y + 7);
					else
						Wheels[i]->SetPosition(x + i * 10, y + 7);
					break;
				case true:
					Wheels[i]->SetPosition(x + i *18-9 , y + 4);
					break;
				}
				break;
			}
		}
	}
	switch (isCannonUp)
	{
	case false:
	{
		Cannon->SetCannonUP(false);
		switch (state)
		{
		case TANK_STATE_RIGHT:
			Cannon->SetPosition(x + 18, y + 16);
			Cannon->SetState(CANNON_STATE_RIGHT);
			break;
		case TANK_STATE_LEFT:
			Cannon->SetPosition(x - 9, y + 16);
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
			Cannon->SetPosition(x + 10, y + 28);
			Cannon->SetState(CANNON_STATE_RIGHT);
			break;
		case TANK_STATE_LEFT:
			Cannon->SetPosition(x +6, y + 28);
			Cannon->SetState(CANNON_STATE_LEFT);
			break;
		}
		break;
	}
	}
}
void CTank::WorldToRender()
{
	render_x = x;
	render_y = -(y+TANK_BBOX_HEIGHT);
}
void CTank::Render()
{
	WorldToRender();
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
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		case TANK_STATE_LEFT:
			ani = TANK_ANI_LEFT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		}
		//DebugOut(L"[INFO] Tank has been rendered!\n");
		break;
	}
	case true:
	{
		switch (state)
		{
		case TANK_STATE_RIGHT:
			ani = TANK_ANI_CANNONUP_RIGHT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		case TANK_STATE_LEFT:
			ani = TANK_ANI_CANNONUP_LEFT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		}
		//DebugOut(L"[INFO] Tank has been rendered!\n");
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
	left = x;
	top = y;
	right = x + TANK_BBOX_WIDTH;
	bottom = y + TANK_BBOX_HEIGHT;
}

