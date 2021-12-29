#include "Ballbot.h"
CBallbot::CBallbot():CGameObject()
{
	SetState(BALLBOT_STATE_IDLE);
	Jason = NULL;
	dem = 0;
}

void CBallbot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BALLBOT_BBOX_WIDTH;
	bottom = y + BALLBOT_BBOX_HEIGHT;
}

void CBallbot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	

	//
	// TO-DO: make sure BALLBOT can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
	if (state == BALLBOT_STATE_DIE)
		return;
	if (dem >= 360)
	{
		dem = 0;
		SetState(BALLBOT_STATE_IDLE);
	}		
	if (state == BALLBOT_STATE_ACTION)
	{
		vx = 0;
		vx = BALLBOT_SPEED_X * sin(M_PI/2+dem * M_PI / 180);
		vy = BALLBOT_SPEED_X * cos(M_PI/2+dem * M_PI / 180);
		dem++;
	}
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, init_x-DX_FOR_CHANGE_STATE, init_y - DY_FOR_CHANGE_STATE, init_x + BALLBOT_BBOX_WIDTH+DX_FOR_CHANGE_STATE, init_y);
	if (choose_state == false)
	{
		if(dem==0&&state==BALLBOT_STATE_ACTION)
			SetState(BALLBOT_STATE_IDLE);
	}		
	else
	{
		SetState(BALLBOT_STATE_ACTION);
	}
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
void CBallbot::WorldToRender()
{
	render_x = x;
	render_y = -y;
}
void CBallbot::Render()
{
	if (state == BALLBOT_STATE_DIE)
		return;
	WorldToRender();

	int ani = -1;
	if (vy > 0)
		ani = BALLBOT_ANI_MOVE_UP;
	else
		ani = BALLBOT_ANI_MOVE_DOWN;

	animation_set->at(ani)->Render(round(render_x), round(render_y));

	//RenderBoundingBox();
}

void CBallbot::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BALLBOT_STATE_DIE:
		//y += BALLBOT_BBOX_HEIGHT - BALLBOT_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case BALLBOT_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BALLBOT_STATE_ACTION:
		vx = BALLBOT_SPEED_X * sin(M_PI / 2 + dem * M_PI / 180);
		vy = BALLBOT_SPEED_X * cos(M_PI / 2 + dem * M_PI / 180);
		break;
	}
}
CBallbot::~CBallbot()
{

}
