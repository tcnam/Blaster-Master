#include "Interrupt.h"
CInterrupt::CInterrupt()
{
	SetState(INTERRUPT_STATE_IDLE);
}

void CInterrupt::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + INTERRUPT_BBOX_WIDTH;

	if (state == INTERRUPT_STATE_ACTION)
		bottom = y + INTERRUPT_BBOX_HEIGHT_ACTION;
	else
		bottom = y + INTERRUPT_BBOX_HEIGHT;
}

void CInterrupt::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure INTERRUPT can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state=CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x, y + INTERRUPT_BBOX_HEIGHT, x + INTERRUPT_BBOX_WIDTH, y + INTERRUPT_BBOX_HEIGHT + DY_FOR_CHANGE_STATE);
	if (choose_state)
		SetState(INTERRUPT_STATE_ACTION);
	else
		SetState(INTERRUPT_STATE_IDLE);

	/*if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
}

void CInterrupt::Render()
{
	int ani = INTERRUPT_ANI_IDLE;
	if (state == INTERRUPT_STATE_ACTION) {
		ani = INTERRUPT_ANI_ACTION;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CInterrupt::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case INTERRUPT_STATE_DIE:
		y += INTERRUPT_BBOX_HEIGHT - INTERRUPT_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case INTERRUPT_STATE_WALKING:
		vx = -INTERRUPT_WALKING_SPEED;
		break;
	}*/
}
CInterrupt::~CInterrupt()
{

}
