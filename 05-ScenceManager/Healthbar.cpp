#include "Healthbar.h"
CHealthbar::CHealthbar()
{
	render_x = render_y = 0.0f;
	sprite = NULL;
}
void CHealthbar::SetRenderPosition(float tempx,float tempy)
{
	render_x = tempx + RELATIVE_DX; 
	render_y = tempy + RELATIVE_DY;
}
void CHealthbar::Update()
{
	if (singlebars.size() == 0)
		return;
	int player_health=CGame::GetInstance()->GetHealth();
	for (unsigned int i = 0; i < singlebars.size(); i++)
	{
		if (i < player_health )
			singlebars[i]->SetShowOrNot(true);
		else
			singlebars[i]->SetShowOrNot(false);
		singlebars[i]->SetRenderPosition(render_x+SINGLEBAR_RELATIVE_X, render_y+ (singlebars.size()-i-1)*SINGLEBAR_FACTOR+SINGLEBAR_RELATIVE_Y);
	}
}
void CHealthbar::Draw()
{
	sprite->Draw(round(render_x), round(render_y), 255);
	for (unsigned int i = 0; i < singlebars.size(); i++)
	{
		singlebars[i]->Draw();
	}
}
CHealthbar::~CHealthbar()
{

}