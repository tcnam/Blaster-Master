#include "WeakBrick.h"

CWeakBrick::CWeakBrick(float location_x, float location_y)
{
	x = location_x;
	y = location_y;
	SetState(WEAKBRICK_STATE_NORMAL);
}

void CWeakBrick::Render()
{
	WorldToRender();
	if(state==WEAKBRICK_STATE_NORMAL)
		animation_set->at(0)->Render(render_x, render_y);
	//RenderBoundingBox();
}

void CWeakBrick::WorldToRender()
{
	
	render_x = x;
	render_y = -(y + WEAKBRICK_BBOX_HEIGHT);
}
void CWeakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WEAKBRICK_BBOX_WIDTH;
	b = y + WEAKBRICK_BBOX_HEIGHT;
}
void CWeakBrick::SetState(int state)
{
	CGameObject::SetState(state);
	
}
CWeakBrick::~CWeakBrick()
{

}