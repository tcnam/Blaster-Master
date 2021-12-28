#pragma once
#include "Camera.h"
#include "Healthbar.h"
#define HUD_TYPE_HEALTHBAR			0
#define HUD_TYPE_SINGLEBAR			1

class CHud
{
	float render_cam_x;
	float render_cam_y;
	Camera* camera;
	CHealthbar* healthbar;
public:
	CHud();
	void SetCamera(Camera* cm) { camera = cm; }

	void SetHealthbar(CHealthbar* hb) { healthbar = hb; }
	CHealthbar* GetHealthbar() { return healthbar; }

	void Update();
	void Render();
	~CHud();
};
