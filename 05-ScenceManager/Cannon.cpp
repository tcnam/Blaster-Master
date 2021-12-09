#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Cannon.h"
#include "Game.h"

CCannon::CCannon(float x, float y) : CGameObject()
{
	SetCannonUP(false);
	SetState(CANNON_STATE_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}
void CCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CCannon::Render()
{
	WorldToRender();
	int ani = -1;
	int alpha = 255;
	switch (isUp)
	{
	case false:
	{
		switch (state)
		{
		case CANNON_STATE_RIGHT:
			ani = CANNON_ANI_RIGHT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		case CANNON_STATE_LEFT:
			ani = CANNON_ANI_LEFT;
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
		case CANNON_STATE_RIGHT:
			ani = CANNON_ANI_CANNONUP_RIGHT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		case CANNON_STATE_LEFT:
			ani = CANNON_ANI_CANNONUP_LEFT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		}
		//DebugOut(L"[INFO] Tank has been rendered!\n");
		break;
	}
	}



}
void CCannon::WorldToRender()
{
	render_x = x;
	render_y = -y;
}

void CCannon::SetState(int state)
{
	CGameObject::SetState(state);
}

void CCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}