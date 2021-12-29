#include "EBullet.h"


CEBullet::CEBullet() :CGameObject()
{
	vx = 0;
	vy = 0;
	nx = 1;
	SetState(EBULLET_STATE_IDLE, 0, 0);
	start_x = start_y = 0;
	enemyOwner = 0;
}
void CEBullet::SetStartPosition(float x, float y)
{
	start_x = x;
	start_y = y;
}
void CEBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == EBULLET_STATE_IDLE)
		return;

	if (abs(x - start_x) >= EBULLET_DISTANCE_FOR_CHANGE_STATE || abs(y - start_y) >= EBULLET_DISTANCE_FOR_CHANGE_STATE)
	{
		SetState(EBULLET_STATE_IDLE, 0, 0);
		//return;
	}

	// Calculate dx, dy 
	CGameObject::Update(dt, coObjects);
	if (GetTickCount64() - effect_start > EFFECT_TIME && state == EBULLET_STATE_IMPACT)
	{
		SetState(EBULLET_STATE_IDLE, 0, 0);
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
			SetState(EBULLET_STATE_IMPACT, 0, 0);
			StartEffect();
			float collide_x, collide_y;
			if (dynamic_cast<CJason*>(e->obj)) // if e->obj is Goomba 
			{
				CJason* player = dynamic_cast<CJason*>(e->obj);
				if (player->GetState() != JASON_STATE_DIE)
				{
					//player->SetState(JASON_STATE_DIE);
					player->GetPosition(collide_x, collide_y);
					SetPosition(collide_x, collide_y + EBULLET_BBOX_HEIGHT);
					if (player->GetUntouchable() != 1)
						player->StartUntouchable();
				}
			}
	
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CEBullet::WorldToRender()
{
	render_x = x;
	render_y = -y;
}
void CEBullet::Render()
{
	if (state != EBULLET_STATE_FIRE)
		return;
	WorldToRender();
	animation_set->at(0)->Render(round(render_x), round(render_y), 255);
	RenderBoundingBox();
}
void CEBullet::SetState(int state, int nx, int ny)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EBULLET_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case EBULLET_STATE_FIRE:
		vx = nx * EBULLET_SPEED_X;
		vy = ny * EBULLET_SPEED_Y;
		break;
	}
}
void CEBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EBULLET_BBOX_WIDTH;
	bottom = y + EBULLET_BBOX_HEIGHT;
}
CEBullet::~CEBullet()
{

}