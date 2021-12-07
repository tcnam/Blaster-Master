#include "Brick.h"

CBrick::CBrick()
{
	width = 0;
	height = 0;
}
CBrick::CBrick(float x, float y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
void CBrick::Render()
{
	WorldToRender();
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::WorldToRender()
{
	render_x = x;
	render_y = -(y + height);
}
void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
CBrick::~CBrick()
{

}