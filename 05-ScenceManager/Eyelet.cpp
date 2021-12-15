#include"Eyelet.h"
CEyelet::CEyelet() :CGameObject()
{
	SetState(EYELET_STATE_ACTION);
	Jason = NULL;
	dem = 0;
}
void CEyelet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYELET_BBOX_WIDTH;
	bottom=y+ EYELET_BBOX_HEIGHT;
}

void CEyelet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	

	//
	// TO-DO: make sure EYELET can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
	if (dem >= 180)
		dem = 0;
	if (state == EYELET_STATE_ACTION)
	{
		//vx = EYELET_SPEED_X * sin(M_PI + dem * M_PI / 180);
		vy = EYELET_SPEED_X * cos(M_PI + dem * M_PI / 180);
		dem++;
	}
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != JASON_STATE_DIE)
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



		//if (nx != 0) vx = 0;
		//if (ny != 0)
		//{
		//	vy = 0;

		//}
		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	/*float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x, y - DY_FOR_CHANGE_STATE, x + EYELET_BBOX_WIDTH, y);
	if (choose_state)
		SetState(EYELET_STATE_ACTION);
	else
		SetState(EYELET_STATE_IDLE);*/

}
void CEyelet::WorldToRender()
{
	render_x = x;
	render_y = -(y + EYELET_BBOX_HEIGHT);
}
void CEyelet::Render()
{
	if (state == EYELET_STATE_IDLE)
		return;
	WorldToRender();
	int ani = -1;
	switch (nx)
	{
	case 1:
		ani = EYELET_ANI_RIGHT;
		break;
	case -1:
		ani = EYELET_ANI_LEFT;
		break;
	}
	animation_set->at(ani)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
}

void CEyelet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYELET_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BULLET_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case EYELET_STATE_ACTION:
		vx = nx * EYELET_SPEED_X;
		vy = EYELET_SPEED_X * cos(M_PI + dem * M_PI / 180);
		break;
	}
}
CEyelet::~CEyelet()
{

}
