#pragma once
#include "GameObject.h"
#include "Cannon.h"
#include "Wheel.h"

#define TANK_STATE_RIGHT		100
#define TANK_STATE_LEFT			200

#define TANK_ANI_RIGHT					0
#define TANK_ANI_LEFT					1

#define TANK_ANI_CANNONUP_RIGHT			2
#define TANK_ANI_CANNONUP_LEFT			3

#define TANK_BBOX_WIDTH				26	
#define TANK_BBOX_HEIGHT			18

class CTank : public CGameObject
{
	CCannon* Cannon;
	vector <CWheel*> Wheels;

	bool isCannonUp;
	bool isMoving;

	float start_x;			// initial position of JASON at scene
	float start_y;
public:
	CTank(float x = 0.0f, float y = 0.0f);
	virtual void WorldToRender();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetCannonUP(bool UpOrNOt) { isCannonUp = UpOrNOt; }
	void SetMoving(bool move) { isMoving = move; }

	void SetCannon(CCannon* cannon) { Cannon = cannon; }
	CCannon* GetCannon() { return Cannon; }

	void PushWheels(CWheel* wheel) { Wheels.push_back(wheel); }
	vector<CWheel*> GetWheel() { return Wheels; }


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};