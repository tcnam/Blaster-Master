#include "Portal.h"


CPortal::CPortal(float l, float t, float w, float h, int scene_id )
{
	this->scene_id = scene_id;
	x = l; 
	y = t;
	width = w;
	height = h;
}
void CPortal::WorldToRender()
{
	render_x = x;
	render_y = -(y+height);
}
void CPortal::Render()
{
	WorldToRender();
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}