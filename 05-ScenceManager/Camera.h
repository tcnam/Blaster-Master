#pragma once
#include "Utils.h"
#include "Game.h"
#include "Jason.h"

#define CAMX_IN_USE 272
#define CAMY_IN_USE 256


class Camera
{
private:
	float cam_x;
	float cam_y;

	float boundary_x_min;
	float boundary_x_max;
	float boundary_y_min;
	float boundary_y_max;

	float render_cam_x;
	float render_cam_y;

	DWORD dt;
	CJason* Jason;
public:
	Camera();
	~Camera();
	void SetJason(CJason* jason) { Jason = jason; }
	void SetBoundary(float map_width, float map_height);
	void SetPosition(float x, float y) { this->cam_x = x, this->cam_y = y; }
	float GetCamX() { return cam_x; }
	float GetCamY() { return cam_y; }
	void GetRenderPosition(float& x, float& y) { x = render_cam_x; y = render_cam_y; }
	void GetPosition(float& x, float& y) { x = this->cam_x; y = this->cam_y; }
	void Update(DWORD dt);
};
