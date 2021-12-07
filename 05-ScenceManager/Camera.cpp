#include "Camera.h"
Camera::Camera()
{
	cam_x = 0.0f;
	cam_y = 0.0f;
	dt = 0;
	Jason = NULL;
}
void Camera::Update(DWORD time)
{
	if (Jason == NULL)
		return;
	dt = time;
	float cx, cy;
	Jason->GetPosition(cx, cy);
	cx -= (float)CGame::GetInstance()->GetScreenWidth() / 2;
	cy -= (float)CGame::GetInstance()->GetScreenHeight() /2;
	cam_x = cx;
	cam_y = cy;

	if (cam_x >= BOUNDRRY_X_MAX - (float)CGame::GetInstance()->GetScreenWidth())
		cam_x = BOUNDRRY_X_MAX - (float)CGame::GetInstance()->GetScreenWidth();
	render_cam_x = cam_x;
	render_cam_y = -(cam_y+(float)CGame::GetInstance()->GetScreenHeight());
	CGame::GetInstance()->SetCamPos(round(render_cam_x), round(render_cam_y));
}
Camera::~Camera()
{

}