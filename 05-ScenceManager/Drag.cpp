#include "Drag.h"
CDrag::CDrag() :CGameObject()
{
	SetState(DRAG_STATE_IDLE);
}

void CDrag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DRAG_BBOX_WIDTH;
	bottom = y + DRAG_BBOX_HEIGHT;	
}

void CDrag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (Jason == NULL)
		return;
	if (state == DRAG_STATE_DIE)
		return;

	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state;
	switch (nx)
	{
	case 1:
		choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x, y, x + DISTANCE_FOR_CHANGE_STATE, y + DRAG_BBOX_HEIGHT);
		break;
	case -1:
		choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x - DISTANCE_FOR_CHANGE_STATE, y, x, y + DRAG_BBOX_HEIGHT);
		break;
	}
	if (choose_state&&state==DRAG_STATE_IDLE)
		SetState(DRAG_STATE_ACTION);
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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* b = dynamic_cast<CBrick*>(e->obj);
				if (e->nx != 0 && e->ny == 0)
				{
					this->nx = -this->nx;
					SetState(DRAG_STATE_IDLE);
				}
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* b = dynamic_cast<CWeakBrick*>(e->obj);
				if (e->nx != 0 && e->ny == 0)
				{
					this->nx = -this->nx;
					SetState(DRAG_STATE_IDLE);
				}
			}
			else if (dynamic_cast<CJason*>(e->obj))
			{
				SetState(DRAG_STATE_ACTION);
				x += dx;
				y += dy;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void CDrag::WorldToRender()
{
	render_x = x;
	render_y = -(y + DRAG_BBOX_HEIGHT);
}
void CDrag::Render()
{
	if (state == DRAG_STATE_DIE)
		return;
	WorldToRender();

	animation_set->at(0)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
}

void CDrag::SetState(int state)
{

	CGameObject::SetState(state);
	switch (state)
	{
	case DRAG_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case DRAG_STATE_ACTION:
		vx =nx* DRAG_SPEED;
		vy = 0;
		break;
	}
}
CDrag::~CDrag()
{

}
