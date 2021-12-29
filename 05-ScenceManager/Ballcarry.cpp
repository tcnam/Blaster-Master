#include"Ballcarry.h"
CBallcarry::CBallcarry() :CGameObject()
{
	SetState(BALLCARRY_STATE_IDLE);
	Jason = NULL;
}
void CBallcarry::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BALLCARRY_BBOX_WIDTH;
	bottom = y + BALLCARRY_BBOX_HEIGHT;
}

void CBallcarry::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	//
	// TO-DO: make sure BALLCARRY can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
 	if (state == BALLCARRY_STATE_DIE)
		return;
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x-DISTANCE_FOR_CHANGE_STATE, y, x + BALLCARRY_BBOX_WIDTH+DISTANCE_FOR_CHANGE_STATE, y+BALLCARRY_BBOX_HEIGHT);
	if (choose_state && state == BALLCARRY_STATE_IDLE)
	{
		SetState(BALLCARRY_STATE_ACTION);		
	}

	//vy -= BALLCARRY_GRAVITY * dt;
	CGameObject::Update(dt, coObjects);
	if (GetTickCount64() - action_start > ACTION_TIME && state == BALLCARRY_STATE_ACTION)
	{
		if (jason_x > x)
			Setnx(-1);
		else
		{
			Setnx(1);
		}
		SetState(BALLCARRY_STATE_RUN);
		action_start = 0;
	}
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
					SetState(BALLCARRY_STATE_RUN);
				}
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* b = dynamic_cast<CWeakBrick*>(e->obj);
				if (e->nx != 0 && e->ny == 0)
				{
					this->nx = -this->nx;
					SetState(BALLCARRY_STATE_RUN);
				}
			}
			else if (dynamic_cast<CJason*>(e->obj))
			{
				SetState(BALLCARRY_STATE_RUN);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if(Jason->GetUntouchable()!=1)
					Jason->StartUntouchable();
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];



}
void CBallcarry::WorldToRender()
{
	render_x = x;
	render_y = -(y + BALLCARRY_BBOX_HEIGHT);
}
void CBallcarry::Render()
{
	if (state == BALLCARRY_STATE_DIE)
		return;
	WorldToRender();
	int ani = -1;
	switch (state)
	{
	case BALLCARRY_STATE_IDLE:
		ani = BALLCARRY_ANI_IDLE;
		break;
	case BALLCARRY_STATE_ACTION:
		ani = BALLCARRY_ANI_ACTION;
		break;
	case BALLCARRY_STATE_RUN:
		ani = BALLCARRY_ANI_RUN;
		break;
	}
	animation_set->at(ani)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
}
void CBallcarry::ActivateBombs()
{
	if (bombs.size() == NULL)
		return;
	for (unsigned int i = 0; i < bombs.size(); i++)
	{
		switch (i)
		{
		case 0:
			bombs[i]->Setnx(-1);
			bombs[i]->SetStartPosition(x , y);
			bombs[i]->SetPosition(x , y);
			break;
		case 1:
			bombs[i]->Setnx(-1);
			bombs[i]->SetStartPosition(x + BALLCARRY_BBOX_WIDTH / 2, y);
			bombs[i]->SetPosition(x + BALLCARRY_BBOX_WIDTH / 2 , y);
			break;
		case 2:
			bombs[i]->Setnx(1);
			bombs[i]->SetStartPosition(x + BALLCARRY_BBOX_WIDTH / 2 , y);
			bombs[i]->SetPosition(x + BALLCARRY_BBOX_WIDTH / 2 , y);
			break;
		case 3:
			bombs[i]->Setnx(1);
			bombs[i]->SetStartPosition(x + BALLCARRY_BBOX_WIDTH , y);
			bombs[i]->SetPosition(x + BALLCARRY_BBOX_WIDTH , y);
			break;
		}

		bombs[i]->SetState(BOMB_STATE_JUMP);
	}
}
void CBallcarry::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BALLCARRY_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BALLCARRY_STATE_IDLE:
		vx = 0;
		break;
	case BALLCARRY_STATE_ACTION:
		vx = 0;
		ActivateBombs();
		StartAction();
		break;
	case BALLCARRY_STATE_RUN:
		vx =  nx*BALLCARRY_SPEED_X;
		break;
	}
}
CBallcarry::~CBallcarry()
{

}
