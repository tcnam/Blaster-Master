#include "Hud.h"

CHud::CHud()
{
	render_cam_x = render_cam_y = 0.0f;
	camera = NULL;
	healthbar = NULL;
}

void CHud::Update()
{
	if (camera == NULL)
		return;
	if (healthbar == NULL)
		return;
	camera->GetRenderPosition(render_cam_x, render_cam_y);
	healthbar->SetRenderPosition(render_cam_x, render_cam_y);
	healthbar->Update();
}
void CHud::Render()
{
	if (camera == NULL)
		return;
	if (healthbar == NULL)
		return;
	healthbar->Draw();
}
CHud::~CHud()
{

}