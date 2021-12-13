#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Wheel.h"
#include "Game.h"

CWheel::CWheel(float x, float y) : CGameObject()
{
	SetState(WHEEL_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}
void CWheel::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}
void CWheel::WorldToRender()
{
	render_x = x;
	render_y = -y;
}
void CWheel::Render()
{
	WorldToRender();
	int ani = -1;
	int alpha = 255;

		switch (state)
		{
		case WHEEL_STATE_MOVE_RIGHT:
			ani = WHEEL_ANI_MOVE_RIGHT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		case WHEEL_STATE_MOVE_LEFT:
			ani = WHEEL_ANI_MOVE_LEFT;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		case WHEEL_STATE_IDLE:
			ani = WHEEL_ANI_IDLE;
			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
			break;
		}
}

void CWheel::SetState(int state)
{
	CGameObject::SetState(state);
}

void CWheel::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}