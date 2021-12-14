#include "Ballbot.h"
CBallbot::CBallbot():CGameObject()
{
	SetState(BALLBOT_STATE_IDLE);
	Jason = NULL;
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
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x-160.0f, y - DY_FOR_CHANGE_STATE, x + +160.0f+BALLBOT_BBOX_WIDTH, y  );
	if (choose_state == false)
		SetState(BALLBOT_STATE_IDLE);
	else
	{
		switch (isActived)
		{

		case false:
			isActived = true;
			SetState(BALLBOT_STATE_MOVE_DOWN);
			break;
		case true:
			if (abs(y - init_y) >= DY_TO_MOVE_UP)
			{
				SetState(BALLBOT_STATE_MOVE_UP);
			}
			break;
		}
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
	switch (state)
	{
	case BALLBOT_STATE_MOVE_UP:
		ani = BALLBOT_ANI_MOVE_UP;
		break;
	default:
		ani = BALLBOT_ANI_MOVE_DOWN;
		break;
	}

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
	case BALLBOT_STATE_MOVE_DOWN:
		vx = -BALLBOT_SPEED_X;
		vy = BALLBOT_SPEED_Y;
		break;
	case BALLBOT_STATE_MOVE_UP:
		vx = BALLBOT_SPEED_X;
		vy = -BALLBOT_SPEED_Y;
		break;
	}
}
CBallbot::~CBallbot()
{

}
