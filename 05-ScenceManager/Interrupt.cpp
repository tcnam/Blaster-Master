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

	if (Jason == NULL)
		return;
	if (state == INTERRUPT_STATE_DIE)
		return;
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure INTERRUPT can interact with the world and to each of them too!
	// 
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
	if (state == INTERRUPT_STATE_DIE)
		return;
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
	switch (state)
	{
	case INTERRUPT_STATE_ACTION:
		ActivateNeoworm();
		break;
	}
}
void CInterrupt::ActivateNeoworm()
{
	if (Worm == NULL)
		return;
	if (Worm->GetState() == NEOWORM_STATE_IDLE)
	{
		Worm->SetPosition(x + INTERRUPT_BBOX_WIDTH / 3, y );
		Worm->SetState(NEOWORM_STATE_FALL);
	}
}
CInterrupt::~CInterrupt()
{

}
