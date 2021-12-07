#pragma once
#include "Utils.h"
#include "Game.h"
#include "Jason.h"

#define BOUNDRRY_X_MAX 1792.0f
#define CAMX_IN_USE 330
#define CAMY_IN_USE 300


class Camera
{
private:
	float cam_x;
	float cam_y;

	float render_cam_x;
	float render_cam_y;

	DWORD dt;
	CJason* Jason;
public:
	Camera();
	~Camera();
	void SetJason(CJason* jason) { Jason = jason; }
	void SetPosition(float x, float y) { this->cam_x = x, this->cam_y = y; }
	float GetCamX() { return cam_x; }
	float GetCamY() { return cam_y; }
	void GetPosition(float& x, float& y) { x = this->cam_x; y = this->cam_y; }
	void Update(DWORD dt);
};
