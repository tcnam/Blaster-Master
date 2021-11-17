#pragma once
#include "GameObject.h"
#include "Cannon.h"


#define TANK_STATE_RIGHT		100
#define TANK_STATE_LEFT			200

#define TANK_ANI_RIGHT					0
#define TANK_ANI_LEFT					1

#define TANK_ANI_CANNONUP_RIGHT			2
#define TANK_ANI_CANNONUP_LEFT			3


class CTank : public CGameObject
{
	CCannon* Cannon;

	bool isCannonUp;

	float start_x;			// initial position of JASON at scene
	float start_y;
public:
	CTank(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetCannonUP(bool UpOrNOt) { isCannonUp = UpOrNOt; }

	void SetCannon(CCannon* cannon) { Cannon = cannon; }
	CCannon* GetCannon() { return Cannon; }


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};