#include "RainbowBullet.h"
#include "Camera.h"
#include "Stuka.h"
#include"Interrupt.h"
#include "Eyelet.h"
#include "Ballbot.h"
#include "WeakBrick.h"
#include "GX680.h"
#include "Drag.h"
#include "Laserguard.h"
#include "Ballcarry.h"
#include "Neoworm.h"

CRainbowBullet::CRainbowBullet() :CGameObject()
{
	vx = 0;
	vy = 0;
	nx = 1;
	ny = 0;
	SetState(RAINBOWBULLET_STATE_IDLE, 0, 0);
	dem = 0;
	start_x = start_y = 0;
}
void CRainbowBullet::SetStartPosition(float x, float y)
{
	start_x = x;
	start_y = y;
}
void CRainbowBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == RAINBOWBULLET_STATE_IDLE)
		return;

	if (abs(x - start_x) >= RAINBOWBULLET_DISTANCE_FOR_CHANGE_STATE || abs(y - start_y) >= RAINBOWBULLET_DISTANCE_FOR_CHANGE_STATE)
	{
		SetState(RAINBOWBULLET_STATE_IDLE, 0, 0);
		//return;
	}
	if (dem >= 180)
		dem = 0;
	if (state == RAINBOWBULLET_STATE_FIRE)
	{
		switch (nxORny)
		{
		case false:
			vy = RAINBOWBULLET_SPEED_WAVE * cos(M_PI + dem * M_PI / 180*20);
			dem=dem++;
			break;
		case true:
			vx= RAINBOWBULLET_SPEED_WAVE * cos(M_PI + dem * M_PI / 180*20);
			dem=dem++;
			break;
		}
	}
	// Calculate dx, dy 
	CGameObject::Update(dt, coObjects);
	if (GetTickCount64() - effect_start > EFFECT_TIME && state == RAINBOWBULLET_STATE_IMPACT)
	{
		SetState(RAINBOWBULLET_STATE_IDLE, 0, 0);
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
				//SetPosition(x + RAINBOWBULLET_BBOX_WIDTH, y);
			SetState(RAINBOWBULLET_STATE_IMPACT, 0, 0);
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
			else if (dynamic_cast<CGx680*>(e->obj))
			{
				CGx680* g = dynamic_cast<CGx680*>(e->obj);
				if (g->GetState() != GX680_STATE_DIE)
				{
					g->SetState(GX680_STATE_DIE);
					g->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}
			}
			else if (dynamic_cast<CDrag*>(e->obj))
			{
				CDrag* d = dynamic_cast<CDrag*>(e->obj);
				if (d->GetState() != DRAG_STATE_DIE)
				{
					d->SetState(DRAG_STATE_DIE);
					d->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}
			}
			else if (dynamic_cast<CLaserguard*>(e->obj))
			{
				CLaserguard* lg = dynamic_cast<CLaserguard*>(e->obj);
				if (lg->GetState() != LASERGUARD_STATE_DIE)
				{
					lg->SetState(LASERGUARD_STATE_DIE);
					lg->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}
			}
			else if (dynamic_cast<CBallcarry*>(e->obj))
			{
				CBallcarry* bc = dynamic_cast<CBallcarry*>(e->obj);
				if (bc->GetState() != BALLCARRY_STATE_DIE)
				{
					bc->SetState(BALLCARRY_STATE_DIE);
					bc->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}
			}
			else if (dynamic_cast<CNeoworm*>(e->obj))
			{
				CNeoworm* nw = dynamic_cast<CNeoworm*>(e->obj);
				if (nw->GetState() != NEOWORM_STATE_DIE)
				{
					nw->SetState(NEOWORM_STATE_DIE);
					nw->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y);
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				brick->GetPosition(collide_x, collide_y);
				if (e->nx < 0 && e->ny == 0)
					SetPosition(x + RAINBOWBULLET_BBOX_WIDTH / 2, y);
				if (e->ny < 0 && e->nx == 0)
					SetPosition(x, collide_y);
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* wbrick = dynamic_cast<CWeakBrick*>(e->obj);
				wbrick->GetPosition(collide_x, collide_y);
				if (wbrick->GetState() == WEAKBRICK_STATE_NORMAL)
					wbrick->SetState(WEAKBRICK_STATE_DISAPPEAR);
				if (e->nx < 0 && e->ny == 0)
					SetPosition(x + RAINBOWBULLET_BBOX_WIDTH / 2, y);
				if (e->ny != 0 && e->nx == 0)
					SetPosition(x, collide_y);
			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CRainbowBullet::WorldToRender()
{
	render_x = x;
	render_y = -y;
}
void CRainbowBullet::Render()
{
	if (state == RAINBOWBULLET_STATE_IDLE)
		return;
	WorldToRender();
	int alpha = 255;
	int ani = -1;
	switch (state)
	{
	case RAINBOWBULLET_STATE_IMPACT:
		ani = RAINBOWBULLET_ANI_IMPACT;
		break;
	case RAINBOWBULLET_STATE_FIRE:
	{
		ani = RAINBOWBULLET_ANI_FIRE;
	}
	break;
	}
	animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);
	RenderBoundingBox();
}
void CRainbowBullet::SetState(int state, int nx, int ny)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RAINBOWBULLET_STATE_IDLE:
		dem = 0;
		vx = 0;
		vy = 0;
		break;
	case RAINBOWBULLET_STATE_FIRE:
		vx = nx * RAINBOWBULLET_SPEED;
		vy = ny * RAINBOWBULLET_SPEED;
		if (vx == 0)
		{
			vx = RAINBOWBULLET_SPEED_WAVE * cos(M_PI + dem * M_PI / 180*10);
			nxORny = true;
		}

		if (vy == 0)
		{
			nxORny = false;
			vy = RAINBOWBULLET_SPEED_WAVE * cos(M_PI + dem * M_PI / 180*10);
		}			
		break;
		
	case RAINBOWBULLET_STATE_IMPACT:
		dem = 0;
		vx = 0;
		vy = 0;
		break;
	}
}
void CRainbowBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RAINBOWBULLET_BBOX_WIDTH;
	bottom = y + RAINBOWBULLET_BBOX_HEIGHT;
}
CRainbowBullet::~CRainbowBullet()
{

}