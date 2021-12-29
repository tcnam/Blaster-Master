#include "Reward.h"
CReward::CReward() :CGameObject()
{
	SetState(REWARD_STATE_ACTION);
	SetTypeReward(REWARD_TYPE_POWER);
}

void CReward::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + REWARD_BBOX_WIDTH;
	bottom = y + REWARD_BBOX_HEIGHT;
}

void CReward::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state == REWARD_STATE_DIE)
		return;

	//
	// TO-DO: make sure REWARD can interact with the world and to each of them too!
	// 
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
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
		if (nx != 0) vx = 0;
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
			CJason* player = dynamic_cast<CJason*>(e->obj);
			if (dynamic_cast<CJason*>(e->obj))
			{
				if (state == REWARD_STATE_ACTION)
				{
					SetState(REWARD_STATE_DIE);
					CGame::GetInstance()->SetHealth(CGame::GetInstance()->GetHealth() + 1);
				}

			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void CReward::WorldToRender()
{
	render_x = x;
	render_y = -(y + REWARD_BBOX_HEIGHT);
}
void CReward::Render()
{

	if (typeReward == REWARD_TYPE_NOTHING)
		return;
	if (state == REWARD_STATE_ACTION)
	{
		WorldToRender();
		switch (typeReward)
		{
		case REWARD_TYPE_HEALTH:
			animation_set->at(REWARD_ANI_HEALTH)->Render(round(render_x), round(render_y));
			break;
		case REWARD_TYPE_POWER:
			animation_set->at(REWARD_ANI_POWER)->Render(round(render_x), round(render_y));
			break;
		}
		
	}

	//RenderBoundingBox();
}

void CReward::SetState(int state)
{
	CGameObject::SetState(state);
	vx = 0;
	vy = 0;
}
CReward::~CReward()
{

}
