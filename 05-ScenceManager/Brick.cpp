#include "Brick.h"

CBrick::CBrick(float x, float y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
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