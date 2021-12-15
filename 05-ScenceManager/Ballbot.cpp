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
	if (dem >= 360)
		dem = 0;
	if (state == BALLBOT_STATE_ACTION)
	{
		vx = BALLBOT_SPEED_X * sin(M_PI + dem * M_PI / 180);
		vy = BALLBOT_SPEED_X * cos(M_PI + dem * M_PI / 180);
		dem++;
	}
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x-DX_FOR_CHANGE_STATE, y - DY_FOR_CHANGE_STATE, x + BALLBOT_BBOX_WIDTH+DX_FOR_CHANGE_STATE, y);
	if (choose_state == false)
	{
		SetState(BALLBOT_STATE_IDLE);
	}		
	else
	{
		SetState(BALLBOT_STATE_ACTION);
	}
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;

}
void CBallbot::WorldToRender()
{
	render_x = x;
	render_y = -y;
}
void CBallbot::Render()
{
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
		vx = BALLBOT_SPEED_X * sin(M_PI + dem * M_PI / 180);
		vy = BALLBOT_SPEED_X * cos(M_PI + dem * M_PI / 180);
		break;
	}
}
CBallbot::~CBallbot()
{

}
