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
	CGame::GetInstance()->SetCamPos(round(cam_x), round(cam_y));
}
Camera::~Camera()
{

}