#include "Bullet.h"
#include "Camera.h"

CBullet::CBullet():CGameObject()
{
	vx = 0;
	vy = 0;
	nx = 1;
	SetState(BULLET_STATE_IDLE,0,0);
	start_x = start_y = 0;
}
void CBullet::SetStartPosition(float x, float y)
{
	start_x = x;
	start_y = y;
}
void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BULLET_STATE_IDLE)
		return;
	if (abs(x - start_x) >= BULLET_DISTANCE_FOR_CHANGE_STATE || abs(y - start_y) >= BULLET_DISTANCE_FOR_CHANGE_STATE)
	{
		SetState(BULLET_STATE_IDLE,0,0);
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

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
		if (nx != 0)
		{
			vx = 0;
		}			
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
		}
	}
	

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBullet::WorldToRender()
{
	render_x = x;
	render_y = -y;
}
void CBullet::Render()
{
	if (state == BULLET_STATE_IDLE)
		return;
	WorldToRender();
	int alpha = 255;
	int ani = -1;
	if (vy > 0)
		ani = BULLET_ANI_UP;
	else
	{
		if (nx == 1)
			ani = BULLET_ANI_RIGHT;
		else
			ani = BULLET_ANI_LEFT;
	}
	animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
	RenderBoundingBox();
}
void CBullet::SetState(int state,int nx, int ny)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BULLET_STATE_FIRE:
		switch (ny)
		{
		case 1:
			vx = 0;
			vy = ny * BULLET_SPEED_Y;
			break;
		default:
			vx = nx * BULLET_SPEED_X;
			vy = 0;
			break;
		}
		break;
	}
}
void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (vy > 0)
	{
		right = x + BULLET_BBOX_HEIGHT;
		bottom = y + BULLET_BBOX_WIDTH;
	}
	else
	{
		right = x + BULLET_BBOX_WIDTH;
		bottom = y + BULLET_BBOX_HEIGHT;
	}
}
CBullet::~CBullet()
{

}