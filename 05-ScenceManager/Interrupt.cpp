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
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back JASON if collides with a moving objects, what if JASON is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		if (nx != 0) vx = 0;
		if (ny != 0)
		{
			vy = 0;
		}
		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CJason*>(e->obj))
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (Jason->GetUntouchable() != 1)
					Jason->StartUntouchable();
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

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
