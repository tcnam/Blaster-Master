#pragma once
#include "GameObject.h"
#include "Tank.h"

#define JASON_WALKING_SPEED		0.1f 
//0.1f
#define JASON_JUMP_SPEED_Y		0.5f
#define JASON_GRAVITY			0.002f

#define JASON_LEVEL_SMALL		1
#define JASON_LEVEL_BIG			2
#define JASON_LEVEL_TANK		3

#define JASON_STATE_IDLE			0
#define JASON_STATE_WALKING_RIGHT	100
#define JASON_STATE_WALKING_LEFT	200
#define JASON_STATE_JUMP			300
#define JASON_STATE_DIE				400

#define JASON_ANI_IDLE_RIGHT		0
#define JASON_ANI_IDLE_LEFT			1

#define JASON_ANI_WALKING_RIGHT			2
#define JASON_ANI_WALKING_LEFT			3

#define JASON_BIG_BBOX_WIDTH  26	
#define JASON_BIG_BBOX_HEIGHT 18


#define JASON_UNTOUCHABLE_TIME 5000


class CJason : public CGameObject
{
	int level;
	int untouchable;

	bool isJumping;

	DWORD untouchable_start;

	float start_x;			// initial position of JASON at scene
	float start_y; 

	CTank* Tank;
public: 
	CJason(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void SetTank(CTank* tank) { Tank = tank; }
	CTank* GetTank() { return Tank; }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};