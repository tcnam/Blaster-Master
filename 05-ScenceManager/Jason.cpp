#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Jason.h"
#include "Game.h"
#include "Brick.h"

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
	if (this == NULL)
		return;
	// Calculate dx, dy 
	CGameObject::Update(dt, coObjects);
	
	// Simple fall down
	
	if (level != JASON_LEVEL_BIG)
		vy -= JASON_GRAVITY*dt;
	//DebugOut(L"vy:%f\n", vy);

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
		if (nx != 0)
		{
			vx = 0;
			//dx = vx * dt;
		}			
		if (ny != 0)
		{
			vy = 0;
			//dy = vy * dt;
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
			if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				return;
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{

			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	dx = dy = 0;
	if (this != NULL) //in case collide with portal
	{
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
						Tank->SetPosition(round(x), round(y + 2.0f));
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
	}


}

void CJason::Render()
{
	WorldToRender();
	int ani = JASON_SMALL_ANI_WALKING_LEFT;
	switch (level)
	{
	case JASON_LEVEL_TANK:
		{
			
			//RenderBoundingBox();
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

			//RenderBoundingBox();
			break;
		}
	case JASON_LEVEL_BIG:
		ani = JASON_BIG_ANI_IDLE_DOWN;
		switch (state)
		{
		case JASON_STATE_WALKING_RIGHT:
			ani = JASON_BIG_ANI_WALKING_RIGHT;
			break;
		case JASON_STATE_WALKING_LEFT:
			ani = JASON_BIG_ANI_WALKING_LEFT;
			break;
		case JASON_STATE_WALKING_UP:
			ani = JASON_BIG_ANI_WALKING_UP;
			break;
		case JASON_STATE_WALKING_DOWN:
			ani = JASON_BIG_ANI_WALKING_DOWN;
			break;
		case JASON_STATE_IDLE:
			switch (laststate)
			{
			case JASON_STATE_WALKING_RIGHT:
				ani = JASON_BIG_ANI_IDLE_RIGHT;
				break;
			case JASON_STATE_WALKING_LEFT:
				ani = JASON_BIG_ANI_IDLE_LEFT;
				break;
			case JASON_STATE_WALKING_UP:
				ani = JASON_BIG_ANI_IDLE_UP;
				break;
			case JASON_STATE_WALKING_DOWN:
				ani = JASON_BIG_ANI_IDLE_DOWN;
				break;
			}
			break;
		}
		int alpha = 255;
		if (untouchable) alpha = 128;
		animation_set->at(ani)->Render(round(render_x), round(render_y), alpha);

		//RenderBoundingBox();
		break;
	}

	
}

void CJason::SetState(int state)
{
	CGameObject::SetState(state);
	if (level == JASON_LEVEL_BIG)
	{
		switch (state)
		{
		case JASON_STATE_WALKING_RIGHT:
			nx = 1;
			vx = nx * JASON_WALKING_SPEED;
			vy = 0;
			break;
		case JASON_STATE_WALKING_LEFT:
			nx = -1;
			vx = nx * JASON_WALKING_SPEED;
			vy = 0;
			break;
		case JASON_STATE_WALKING_UP:
			ny = 1;
			vy = ny * JASON_WALKING_SPEED;
			vx = 0;
			break;
		case JASON_STATE_WALKING_DOWN:
			vx = 0;
			ny = -1;
			vy = ny * JASON_WALKING_SPEED;
			break;
		case JASON_STATE_IDLE:
			vx = 0;
			vy = 0;
			break;
		}
	}
	else
	{
		switch (state)
		{
		case JASON_STATE_AUTO_GO:
			nx = 1;
			vx = nx * JASON_WALKING_SPEED;
			//vy = JASON_WALKING_SPEED* cos(M_PI + dem * M_PI / 180);
			break;
		case JASON_STATE_WALKING_RIGHT:
			nx = 1;
			vx = nx * JASON_WALKING_SPEED;
			//vy -= JASON_GRAVITY * dt;
			if (Tank != NULL && level == JASON_LEVEL_TANK)
			{
				Tank->SetState(TANK_STATE_RIGHT);
				Tank->SetMoving(true);
			}
			break;
		case JASON_STATE_WALKING_LEFT:
			nx = -1;
			vx = nx * JASON_WALKING_SPEED;
			//vy -= JASON_GRAVITY * dt;
			if (Tank != NULL && level == JASON_LEVEL_TANK)
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
			if (level == JASON_LEVEL_BIG)
				vy = 0;
			/*else
				vy -= JASON_GRAVITY * dt;*/
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
}

void CJason::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	switch (level)
	{
	case JASON_LEVEL_TANK:
		left = x;
		top = y;
		right = x + JASON_TANK_BBOX_WIDTH;
		bottom = y + JASON_TANK_BBOX_HEIGHT;
		break;
	case JASON_LEVEL_SMALL:
		left = x;
		top = y;
		right = x + JASON_SMALL_BBOX_WIDTH;
		bottom = y + JASON_SMALL_BBOX_HEIGHT;
		break;
	case JASON_LEVEL_BIG:
		left = x;
		top = y;
		right = x + JASON_BIG_BBOX_WIDTH;
		bottom = y + JASON_BIG_BBOX_HEIGHT;
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
void CJason::SetLastState(int s)
{
	laststate = s;
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
void CJason::StartUntouchable()
{
	CGame::GetInstance()->SetHealth(CGame::GetInstance()->GetHealth() - 1);
	untouchable = 1; 
	untouchable_start = GetTickCount64();
}

void CJason::StartAttack()
{

	if (isFiring == true)
		return;
	if (this == NULL)
		return;
	switch (level)
	{
	case JASON_LEVEL_TANK:
	{
		if (Bullets.size() == 0)
			return;
		if (bulletIndex >= Bullets.size())
			bulletIndex = 0;
		if (Bullets[bulletIndex]->GetState() == BULLET_STATE_IDLE)
		{
			switch (Tank->GetCannon()->CannonUpOrNot())
			{
			case false:
				switch (nx)
				{
				case 1:
					Bullets[bulletIndex]->SetStartPosition(x, y + JASON_TANK_BBOX_HEIGHT- JASON_TANK_BBOX_HEIGHT/9);
					Bullets[bulletIndex]->SetPosition(x, y + JASON_TANK_BBOX_HEIGHT- JASON_TANK_BBOX_HEIGHT / 9);
					break;
				case -1:
					Bullets[bulletIndex]->SetStartPosition(x, y + JASON_TANK_BBOX_HEIGHT- JASON_TANK_BBOX_HEIGHT / 9);
					Bullets[bulletIndex]->SetPosition(x, y + JASON_TANK_BBOX_HEIGHT- JASON_TANK_BBOX_HEIGHT / 9);
					break;
				}
				break;
			case true:
				Bullets[bulletIndex]->SetStartPosition(x + JASON_TANK_BBOX_WIDTH / 2, y + JASON_TANK_BBOX_HEIGHT);
				Bullets[bulletIndex]->SetPosition(x + JASON_TANK_BBOX_WIDTH / 2, y + JASON_TANK_BBOX_HEIGHT);
				break;
			}
			Bullets[bulletIndex]->SetState(BULLET_STATE_FIRE, nx, (int)Tank->GetCannon()->CannonUpOrNot());
		}
		bulletIndex++;
	}

		break;
	case JASON_LEVEL_BIG:
	{
		if (rBullets.size() == 0)
			return;
		if (rBulletIndex >= rBullets.size())
			rBulletIndex = 0;
		if (rBullets[rBulletIndex]->GetState() == RAINBOWBULLET_STATE_IDLE)
		{
			rBullets[rBulletIndex]->SetStartPosition(x + JASON_BIG_BBOX_WIDTH / 2, y + JASON_BIG_BBOX_HEIGHT / 2);
			rBullets[rBulletIndex]->SetPosition(x + JASON_BIG_BBOX_WIDTH / 2, y + JASON_BIG_BBOX_HEIGHT / 2);
			if (laststate == JASON_STATE_WALKING_RIGHT || laststate == JASON_STATE_WALKING_LEFT)
				rBullets[rBulletIndex]->SetState(RAINBOWBULLET_STATE_FIRE, nx, 0);
			else
				rBullets[rBulletIndex]->SetState(RAINBOWBULLET_STATE_FIRE, 0, ny);
		}
		rBulletIndex++;
	}

		break;
	}

}

