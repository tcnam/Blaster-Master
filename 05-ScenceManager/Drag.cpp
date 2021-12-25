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

	if (state == DRAG_STATE_ACTION)
		bottom = y + DRAG_BBOX_HEIGHT_ACTION;
	else
		bottom = y + DRAG_BBOX_HEIGHT;
}

void CDrag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (Jason == NULL)
		return;
	if (state == DRAG_STATE_DIE)
		return;
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure DRAG can interact with the world and to each of them too!
	// 
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);
	bool choose_state = CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, x, y - DY_FOR_CHANGE_STATE, x + DRAG_BBOX_WIDTH, y);
	if (choose_state)
		SetState(DRAG_STATE_ACTION);
	else
		SetState(DRAG_STATE_IDLE);

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
	int ani = DRAG_ANI_IDLE;
	if (state == DRAG_STATE_ACTION) {
		ani = DRAG_ANI_ACTION;
	}

	animation_set->at(ani)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
}

void CDrag::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case DRAG_STATE_DIE:
		y += DRAG_BBOX_HEIGHT - DRAG_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case DRAG_STATE_WALKING:
		vx = -DRAG_WALKING_SPEED;
		break;
	}*/
}
CDrag::~CDrag()
{

}
