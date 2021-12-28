#include "Singlebar.h"
CSinglebar::CSinglebar()
{
	render_x = render_y = 0.0f;
	sprite = NULL;
	showOrnot = false;
}
void CSinglebar::SetRenderPosition(float tempx, float tempy)
{
	render_x = tempx;
	render_y = tempy;
}
void CSinglebar::Draw()
{
	if(showOrnot==true)
		sprite->Draw(round(render_x), round(render_y), 255);
}
CSinglebar::~CSinglebar()
{

}