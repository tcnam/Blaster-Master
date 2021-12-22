#include "Bullet.h"
#include "Camera.h"
#include "Stuka.h"
#include"Interrupt.h"
#include "Eyelet.h"
#include "Ballbot.h"
#include "WeakBrick.h"

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

	if (abs(x - start_x) >= BULLET_DISTANCE_FOR_CHANGE_STATE || abs(y - start_y) >= BULLET_DISTANCE_FOR_CHANGE_STATE )
	{
		SetState(BULLET_STATE_IDLE,0,0);
		//return;
	}

	// Calculate dx, dy 
	CGameObject::Update(dt, coObjects);
	if (GetTickCount64() - effect_start > EFFECT_TIME&&state==BULLET_STATE_IMPACT)
	{
		SetState(BULLET_STATE_IDLE, 0, 0);
	}
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
			//if (this->nx > 0)
				//SetPosition(x + BULLET_BBOX_WIDTH, y);
			SetState(BULLET_STATE_IMPACT,0,0);
			StartEffect();
			float collide_x, collide_y;
			if (dynamic_cast<CStuka*>(e->obj)) // if e->obj is Goomba 
			{
				CStuka* s = dynamic_cast<CStuka*>(e->obj);
				if (s->GetState() != STUKA_STATE_DIE)
				{
					s->SetState(STUKA_STATE_DIE);
					s->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}					
			}
			else if (dynamic_cast<CInterrupt*>(e->obj))
			{
				CInterrupt* i = dynamic_cast<CInterrupt*>(e->obj);
				if (i->GetState() != INTERRUPT_STATE_DIE)
				{
					i->SetState(INTERRUPT_STATE_DIE);
					i->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}
					
			}
			else if (dynamic_cast<CBallbot*>(e->obj))
			{
				CBallbot* b = dynamic_cast<CBallbot*>(e->obj);
				if (b->GetState() != BALLBOT_STATE_DIE)
				{
					b->SetState(BALLBOT_STATE_DIE);
					b->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}

			}
			else if (dynamic_cast<CEyelet*>(e->obj))
			{
				CEyelet* eye = dynamic_cast<CEyelet*>(e->obj);
				if (eye->GetState() != EYELET_STATE_DIE)
				{
					eye->SetState(EYELET_STATE_DIE);
					eye->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}			
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				brick->GetPosition(collide_x, collide_y);
				if(e->nx<0 && e->ny==0)
					SetPosition(x+BULLET_BBOX_WIDTH/2, y);
				if (e->ny != 0 && e->nx == 0)
					SetPosition(x, collide_y);
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* wbrick = dynamic_cast<CWeakBrick*>(e->obj);
				wbrick->GetPosition(collide_x, collide_y);
				if (wbrick->GetState() == WEAKBRICK_STATE_NORMAL)
					wbrick->SetState(WEAKBRICK_STATE_DISAPPEAR);
				if (e->nx < 0 && e->ny == 0)
					SetPosition(x + BULLET_BBOX_WIDTH / 2, y);
				if (e->ny != 0 && e->nx == 0)
					SetPosition(x, collide_y);
			}
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
	switch (state)
	{
	case BULLET_STATE_IMPACT:
		ani = BULLET_ANI_IMPACT;
		break;
	case BULLET_STATE_FIRE:
		{
			if (vy > 0)
				ani = BULLET_ANI_UP;
			else
			{
				if (nx == 1)
					ani = BULLET_ANI_RIGHT;
				else
					ani = BULLET_ANI_LEFT;
			}
		}
		break;
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
	case BULLET_STATE_IMPACT:
		vx = 0;
		vy = 0;
		break;
	}
}
void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case BULLET_STATE_FIRE:
		{
			if (vy == 0)
			{
				right = x + BULLET_BBOX_WIDTH;
				bottom = y + BULLET_BBOX_HEIGHT;

			}
			else
			{
				right = x + BULLET_BBOX_HEIGHT;
				bottom = y + BULLET_BBOX_WIDTH;
			}
		}	
		break;
	default:
		right = x + BULLET_IMPACT_BBOX_WIDTH;
		bottom = y + BULLET_IMPACT_BBOX_HEIGT;
		break;
	}

}
CBullet::~CBullet()
{

}