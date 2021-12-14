#include "Interrupt.h"
CInterrupt::CInterrupt():CGameObject()
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
	bool choose_state=CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x, y- DY_FOR_CHANGE_STATE, x + INTERRUPT_BBOX_WIDTH, y);
	if (choose_state)
		SetState(INTERRUPT_STATE_ACTION);
	else
		SetState(INTERRUPT_STATE_IDLE);

}
void CInterrupt::WorldToRender()
{
	render_x = x;
	render_y = -(y+INTERRUPT_BBOX_HEIGHT);
}
void CInterrupt::Render()
{
	WorldToRender();
	int ani = INTERRUPT_ANI_IDLE;
	if (state == INTERRUPT_STATE_ACTION) {
		ani = INTERRUPT_ANI_ACTION;
	}

	animation_set->at(ani)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
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
