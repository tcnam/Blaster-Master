#include"Bomb.h"
CBomb::CBomb() :CGameObject()
{
	SetState(BOMB_STATE_IDLE);
	Jason = NULL;
	start_x = start_y = 0.0f;
	explode_start = 0;
}
void CBomb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOMB_BBOX_WIDTH;
	bottom = y + BOMB_BBOX_HEIGHT;

}

void CBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	//
	// TO-DO: make sure BOMB can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
	if (state == BOMB_STATE_DIE || state == BOMB_STATE_IDLE)
		return;
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);

	
	

	CGameObject::Update(dt, coObjects);
	switch (state)
	{
		case BOMB_STATE_JUMP:
		{
			if (abs(y - start_y) > DISTANCE_TO_CHANGE_STATE)
				SetState(BOMB_STATE_FALL);
		}
		break;
		case BOMB_STATE_ACTION:
		{
			if (GetTickCount64() - explode_start > EXPLODE_TIME)
			{
				SetState(BOMB_STATE_DIE);
				explode_start = 0;
			}
		}
		break;
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
		SetState(BOMB_STATE_ACTION);
		StartExplode();
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* b = dynamic_cast<CBrick*>(e->obj);
				if (e->ny != 0)
				{
					SetState(BOMB_STATE_ACTION);
					StartExplode();
				}
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* b = dynamic_cast<CWeakBrick*>(e->obj);
				if (e->nx != 0 && e->ny > 0&&state == BOMB_STATE_FALL)
				{
					SetState(BOMB_STATE_ACTION);
					StartExplode();
				}
			}
			else if (dynamic_cast<CJason*>(e->obj))
			{
				CJason* player = dynamic_cast<CJason*>(e->obj);
				if (e->ny != 0 && e->nx == 0)
					SetPosition(x, y + 0.1f);
				//x += dx;
				//y += dy;
				if(state==BOMB_STATE_ACTION)
					player->StartUntouchable();
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];



}
void CBomb::WorldToRender()
{
	render_x = x;
	render_y = -(y + BOMB_BBOX_HEIGHT);
}
void CBomb::Render()
{
	if (state == BOMB_STATE_DIE)
		return;
	if (state == BOMB_STATE_IDLE)
		return;
	WorldToRender();
	int ani = BOMB_ANI_IDLE;
	if (state == BOMB_STATE_ACTION)
		ani = BOMB_ANI_EXPLODE;
	animation_set->at(ani)->Render(round(render_x), round(render_y));

	//RenderBoundingBox();
}

void CBomb::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOMB_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BOMB_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BOMB_STATE_FALL:
		vy = -BOMB_SPEED_Y;
		vx = nx * BOMB_SPEED_X;
		break;
	case BOMB_STATE_ACTION:
		vx = 0;
		vy = 0;
		break;
	case BOMB_STATE_JUMP:
		vy = BOMB_SPEED_Y;
		vx = nx * BOMB_SPEED_X ;
		break;
	}
}
CBomb::~CBomb()
{

}
