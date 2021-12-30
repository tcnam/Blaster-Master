#include "AUTODOOR.h"

CAutodoor::CAutodoor(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3, float x_4, float y_4)
{
	x = x_1;
	y = y_1;

	x1 = x_1;
	y1 = y_1;

	x2 = x_2;
	y2 = y_2;

	x3 = x_3;
	y3 = y_3;

	x4 = x_4;
	y4 = y_4;
}

void CAutodoor::Render()
{
	WorldToRender();
	//RenderBoundingBox();
}

void CAutodoor::WorldToRender()
{
	render_x = x;
	render_y = -(y + AUTODOOR_BBOX_HEIGHT);
}
void CAutodoor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + AUTODOOR_BBOX_WIDTH;
	b = y + AUTODOOR_BBOX_HEIGHT;
}
void CAutodoor::SwitchLocation()
{

	if (x == x1 && y == y1)
	{
		x = x3;
		y = y3;
	}
	else if(x==x3&& y==y3)
	{
		x = x2;
		y = y2;
	}
	else if (x == x2 && y == y2)
	{
		x = x4;
		y = y4;
	}
	else
	{
		x = x1;
		y = y1;
	}
}
void CAutodoor::Reset()
{
	x = x1;
	y = y1;
}
void CAutodoor::SetState(int state)
{
	CGameObject::SetState(state);

}
CAutodoor::~CAutodoor()
{

}