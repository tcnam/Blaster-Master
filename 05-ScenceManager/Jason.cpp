#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Jason.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CJason::CJason(float x, float y) : CGameObject()
{
	//level = JASON_LEVEL_BIG;
	untouchable = 0;
	SetState(JASON_STATE_IDLE);
	SetLevel(JASON_LEVEL_TANK);
	isJumping = false;
	isFiring = false;

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	bulletIndex = 0;
}

void CJason::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	// Simple fall down
	vy -= JASON_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=JASON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > JASON_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
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
		if (ny > 0)
		{
			isJumping = false;
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

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						//vy = -JASON_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							/*if (level > JASON_LEVEL_SMALL)
							{
								level = JASON_LEVEL_SMALL;
								StartUntouchable();
							}
							else */
								SetState(JASON_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}
	if (level == JASON_LEVEL_TANK)
	{
		if (Tank != NULL)
		{
			switch (nx)
			{
			case 1:
				switch (Tank->GetCannon()->CannonUpOrNot())
				{
				case TRUE:
					Tank->SetPosition(round(x), round(y+2.0f));
					break;
				case FALSE:
					Tank->SetPosition(round(x), round(y));
					break;
				}
				break;
			case -1:
				switch (Tank->GetCannon()->CannonUpOrNot())
				{
				case TRUE:
					Tank->SetPosition(round(x + 10.0f), round(y + 2.0f));
					break;
				case FALSE:
					Tank->SetPosition(round(x + 10.0f), round(y));
					break;
				}
				
				break;
			}
		}
	}
	
		

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CJason::Render()
{
	WorldToRender();
	int ani = JASON_SMALL_ANI_WALKING_LEFT;
	switch (level)
	{
	case JASON_LEVEL_TANK:
		{
			RenderBoundingBox();
			break;
		}
	case JASON_LEVEL_SMALL:
		{
			if (vx == 0)
			{
				if (nx > 0) ani = JASON_SMALL_ANI_IDLE_RIGHT;
				else ani = JASON_SMALL_ANI_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = JASON_SMALL_ANI_WALKING_RIGHT;
			else ani = JASON_SMALL_ANI_WALKING_LEFT;

			int alpha = 255;
			if (untouchable) alpha = 128;

			animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);

			RenderBoundingBox();
			break;
		}
	case JASON_LEVEL_BIG:
		{
			break;
		}
	}

	
}

void CJason::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case JASON_STATE_AUTO_GO:
		nx = 1;
		vx = nx * JASON_WALKING_SPEED;
		//vy = JASON_WALKING_SPEED* cos(M_PI + dem * M_PI / 180);
		break;
	case JASON_STATE_WALKING_RIGHT:		
		nx = 1;
		vx = nx*JASON_WALKING_SPEED;		
		if (Tank != NULL && level==JASON_LEVEL_TANK)
		{
			Tank->SetState(TANK_STATE_RIGHT);
			Tank->SetMoving(true);
		}
		break;
	case JASON_STATE_WALKING_LEFT: 
		nx = -1;
		vx = nx*JASON_WALKING_SPEED;		
		if (Tank != NULL && level==JASON_LEVEL_TANK)
		{
			Tank->SetState(TANK_STATE_LEFT);
			Tank->SetMoving(true);
		}
		break;
	case JASON_STATE_JUMP:
		// TODO: need to check if JASON is *current* on a platform before allowing to jump again
		if (isJumping == false)
		{
			isJumping = true;
			vy = JASON_JUMP_SPEED_Y;
		}
		//vy = JASON_JUMP_SPEED_Y;
		break; 
	case JASON_STATE_IDLE:
		vy -=JASON_GRAVITY*dt;
		vx = 0;
		if (Tank != NULL)
		{
			Tank->SetMoving(false);
		}
		break;
	case JASON_STATE_DIE:
		//vy = -JASON_DIE_DEFLECT_SPEED;
		break;
	}
}

void CJason::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	switch (level)
	{
	case JASON_LEVEL_TANK:
		left = x;
		top = y;
		right = x + JASON_BIG_BBOX_WIDTH;
		bottom = y + JASON_BIG_BBOX_HEIGHT;
		break;
	case JASON_LEVEL_SMALL:
		left = x;
		top = y;
		right = x + JASON_SMALL_BBOX_WIDTH;
		bottom = y + JASON_SMALL_BBOX_HEIGHT;
		break;

	}
	

}

void CJason::WorldToRender()
{
	render_x = x;
	switch (level)
	{
	case JASON_LEVEL_TANK:
		render_y = -(y + JASON_TANK_BBOX_HEIGHT);
		break;
	case JASON_LEVEL_BIG:
		render_y = -(y + JASON_BIG_BBOX_HEIGHT);
		break;
	case JASON_LEVEL_SMALL:
		render_y = -(y + JASON_SMALL_BBOX_HEIGHT);
		break;
	}
}

CTank* CJason::GetTank()
{
	if (Tank != NULL)
		return Tank;
}
/*
	Reset JASON status to the beginning state of a scene
*/
void CJason::Reset()
{
	SetState(JASON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CJason::StartAttack()
{
	if (Bullets.size() == 0)
		return;
	if (isFiring == true)
		return;
	if (bulletIndex >= Bullets.size() )
		bulletIndex = 0;
	if (Bullets[bulletIndex]->GetState() == BULLET_STATE_IDLE)
	{
		switch (Tank->GetCannon()->CannonUpOrNot())
		{
		case false:
			switch (nx)
			{
			case 1:
				Bullets[bulletIndex]->SetStartPosition(x + JASON_TANK_BBOX_WIDTH, y + JASON_TANK_BBOX_HEIGHT);
				Bullets[bulletIndex]->SetPosition(x + JASON_TANK_BBOX_WIDTH, y + JASON_TANK_BBOX_HEIGHT);
				break;
			case -1:
				Bullets[bulletIndex]->SetStartPosition(x - JASON_TANK_BBOX_WIDTH / 2, y + JASON_TANK_BBOX_HEIGHT);
				Bullets[bulletIndex]->SetPosition(x - JASON_TANK_BBOX_WIDTH / 2, y + JASON_TANK_BBOX_HEIGHT);
				break;
			}
			break;
		case true:
			Bullets[bulletIndex]->SetStartPosition(x + JASON_TANK_BBOX_WIDTH / 2, y + JASON_TANK_BBOX_HEIGHT);
			Bullets[bulletIndex]->SetPosition(x + JASON_TANK_BBOX_WIDTH / 2, y + JASON_TANK_BBOX_HEIGHT);
			break;
		}
		Bullets[bulletIndex]->SetState(BULLET_STATE_FIRE,nx,(int)Tank->GetCannon()->CannonUpOrNot());
	}
	bulletIndex++;
}

