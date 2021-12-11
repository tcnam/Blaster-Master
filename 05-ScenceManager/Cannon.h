#pragma once
#include "GameObject.h"


#define CANNON_STATE_RIGHT			100
#define CANNON_STATE_LEFT			200

#define CANNON_ANI_RIGHT					0
#define CANNON_ANI_LEFT						1

#define CANNON_ANI_CANNONUP_RIGHT			2
#define CANNON_ANI_CANNONUP_LEFT			3


class CCannon : public CGameObject
{

	bool isUp;

	float start_x;			
	float start_y;
public:
	CCannon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void WorldToRender();
	virtual void Render();

	void SetState(int state);
	void SetCannonUP(bool UpOrNOt) { isUp = UpOrNOt; }
	bool CannonUpOrNot() { return isUp; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};