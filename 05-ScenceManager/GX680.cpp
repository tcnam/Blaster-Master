#include"GX680.h"
CGx680::CGx680() :CGameObject()
{
	SetState(GX680_STATE_ACTION);
	Jason = NULL;
}
void CGx680::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GX680_BBOX_WIDTH;
	bottom = y + GX680_BBOX_HEIGHT;
}

void CGx680::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	//
	// TO-DO: make sure GX680 can interact with the world and to each of them too!
	// 
	if (Jason == NULL)
		return;
	if (state == GX680_STATE_DIE)
		return;
	float jason_x, jason_y;
	float l1, t1, r1, b1;
	Jason->GetPosition(jason_x, jason_y);
	Jason->GetBoundingBox(l1, t1, r1, b1);

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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* b = dynamic_cast<CBrick*>(e->obj);
				if (e->nx != 0 && e->ny == 0)
				{
					vx = GX680_SPEED_X * e->nx;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];



}
void CGx680::WorldToRender()
{
	render_x = x;
	render_y = -(y + GX680_BBOX_HEIGHT);
}
void CGx680::Render()
{
	if (state == GX680_STATE_DIE)
		return;
	WorldToRender();
	int ani = -1;
	switch (state)
	{
	case GX680_STATE_ACTION:
		ani = GX680_ANI_MOVE;
		break;
	}
	animation_set->at(0)->Render(round(render_x), round(render_y));

	RenderBoundingBox();
}

void CGx680::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GX680_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BULLET_STATE_IDLE:
		vx = nx * GX680_SPEED_X;
		vy = 0;
		break;
	case GX680_STATE_ACTION:
		vx = 0;
		vy = 0;
		//vx = nx * GX680_SPEED_X;
		//vy = GX680_SPEED_X * cos(M_PI + dem * M_PI / 180);
		break;
	}
}
CGx680::~CGx680()
{

}
