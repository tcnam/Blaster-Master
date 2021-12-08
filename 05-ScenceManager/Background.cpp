#include"Background.h"
CBackground::CBackground()
{
	x = 0;
	y = 0;
	sprite = NULL;
	width = height = 0.0f;
}
void CBackground::CalculateWidthHeight()
{
	if (sprite)
	{
		width = abs(sprite->GetRight() - sprite->GetLeft());
		height = abs(sprite->GetBottom() - sprite->GetTop());
	}
}
void CBackground::Draw()
{
	int alpha = 255;
	sprite->Draw(x, y, alpha);
}
CBackground::~CBackground()
{

}
