#pragma once
#include "GameObject.h"

#define WHEEL_STATE_IDLE			100
#define WHEEL_STATE_MOVE_RIGHT		200
#define WHEEL_STATE_MOVE_LEFT		300

#define WHEEL_ANI_IDLE				0
#define WHEEL_ANI_MOVE_RIGHT		1
#define WHEEL_ANI_MOVE_LEFT			2


class CWheel : public CGameObject
{
	float start_x;			
	float start_y;
public:
	CWheel(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void WorldToRender();
	virtual void Render();

	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
