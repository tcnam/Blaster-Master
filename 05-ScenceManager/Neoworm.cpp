#include"Neoworm.h"
CNeoworm::CNeoworm() :CGameObject()
{
	SetState(NEOWORM_STATE_FALL);
	Jason = NULL;
}
void CNeoworm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case NEOWORM_STATE_IDLE:
		right = x + NEOWORM_IDLE_BBOX_WIDTH;
		bottom = y + NEOWORM_IDLE_BBOX_HEIGHT;
		break;
	case NEOWORM_STATE_FALL:
		right = x + NEOWORM_IDLE_BBOX_WIDTH;
		bottom = y + NEOWORM_IDLE_BBOX_HEIGHT;
		break;
	case NEOWORM_STATE_ACTION:
		right = x + NEOWORM_BBOX_WIDTH;
		bottom = y + NEOWORM_BBOX_HEIGHT;
		break;
	}

}

void CNeoworm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	//
	// TO-DO: make sure NEOWORM can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
	if (state == NEOWORM_STATE_DIE)
		return;
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	
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
				if (e->nx == 0 && e->ny > 0)
				{
					SetState(NEOWORM_STATE_ACTION);
				}
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* b = dynamic_cast<CWeakBrick*>(e->obj);
				if (e->nx != 0 && e->ny == 0)
				{
					this->nx = -this->nx;
					SetState(NEOWORM_STATE_ACTION);
				}
			}
			else if (dynamic_cast<CJason*>(e->obj))
			{
				SetState(NEOWORM_STATE_ACTION);
				x += dx;
				y += dy;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];



}
void CNeoworm::WorldToRender()
{
	render_x = x;
	render_y = -(y + NEOWORM_BBOX_HEIGHT);
}
void CNeoworm::Render()
{
	if (state == NEOWORM_STATE_DIE)
		return;
	WorldToRender();
	int ani = -1;
	switch (state)
	{
	case NEOWORM_STATE_IDLE:
		ani = NEOWORM_ANI_IDLE;
		break;
	case NEOWORM_STATE_FALL:
		ani = NEOWORM_ANI_IDLE;
		break;
	case NEOWORM_STATE_ACTION:
		{
			switch (nx)
			{
			case 1:
				ani = NEOWORM_ACTION_RIGHT;
				break;
			case -1:
				ani = NEOWORM_ACTION_LEFT;
				break;
			}
		}
		break;
	}
	animation_set->at(ani)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
}

void CNeoworm::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case NEOWORM_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case NEOWORM_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case NEOWORM_STATE_FALL:
		vx = 0;
		vy = -NEOWORM_SPEED;
		break;
	case NEOWORM_STATE_ACTION:
		vx = nx * NEOWORM_SPEED;
		break;
	}
}
CNeoworm::~CNeoworm()
{

}
