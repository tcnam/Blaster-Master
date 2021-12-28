#include "Healthbar.h"
CHealthbar::CHealthbar()
{
	render_x = render_y = 0.0f;
	sprite = NULL;
}
void CHealthbar::Draw()
{
	sprite->Draw(round(render_x), round(render_y), 255);
}
CHealthbar::~CHealthbar()
{

}