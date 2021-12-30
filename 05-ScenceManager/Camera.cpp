#include "Camera.h"
Camera::Camera()
{
	cam_x = 0.0f;
	cam_y = 0.0f;
	boundary_x_min = boundary_x_max = 0.0f;
	boundary_y_min = boundary_y_max = 0.0f;
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
	if (Jason->GetIsCameraFollow() == true)
	{

		cx -= (float)CGame::GetInstance()->GetScreenWidth() / 2;
		cy -= (float)CGame::GetInstance()->GetScreenHeight() / 2;
		cam_x = cx;
		cam_y = cy;
	}
	else
	{
		switch (CGame::GetInstance()->GetSceneId())
		{
		case 1:
			cam_x = boundary_x_max;
			cy -= (float)CGame::GetInstance()->GetScreenHeight() / 2;
			cam_y = cy;
			break;
		case 2:
			if (cx > boundary_x_min + CGame::GetInstance()->GetScreenWidth())
			{
				cx -= (float)CGame::GetInstance()->GetScreenWidth() / 2;
				cy -= (float)CGame::GetInstance()->GetScreenHeight() / 2;
				cam_x = cx;
				cam_y = cy;
			}
			else
			{
				cam_x = boundary_x_min;
				cy -= (float)CGame::GetInstance()->GetScreenHeight() / 2;
				cam_y = cy;
			}

			break;
		}
	}


	if (cam_x <= boundary_x_min)
		cam_x = boundary_x_min;
	if (cam_x >= boundary_x_max)
		cam_x = boundary_x_max;
	if (cam_y <= boundary_y_min)
		cam_y = boundary_y_min;
	if (cam_y >= boundary_y_max)
		cam_y = boundary_y_max;
	render_cam_x = cam_x;
	render_cam_y = -(cam_y+(float)CGame::GetInstance()->GetScreenHeight());
	CGame::GetInstance()->SetCamPos(round(render_cam_x), round(render_cam_y));
}
void Camera::SetBoundary(float map_width, float map_height)
{
	boundary_x_min = 0;
	boundary_x_max = map_width-CGame::GetInstance()->GetScreenWidth();
	boundary_y_min = 0;
	boundary_y_max = map_height- CGame::GetInstance()->GetScreenHeight();
}
Camera::~Camera()
{

}