#pragma once
#include "GameObject.h"
#include "Tank.h"
#include "Bullet.h"

#define JASON_WALKING_SPEED		0.1f 
//0.1f
#define JASON_JUMP_SPEED_Y		0.35f
#define JASON_GRAVITY			0.0008f

#define JASON_LEVEL_SMALL		1
#define JASON_LEVEL_BIG			2
#define JASON_LEVEL_TANK		3

#define JASON_STATE_IDLE			0
#define JASON_STATE_WALKING_RIGHT	100
#define JASON_STATE_WALKING_LEFT	200
#define JASON_STATE_JUMP			300
#define JASON_STATE_AUTO_GO			400
#define JASON_STATE_DIE				800

#define JASON_SMALL_ANI_IDLE_RIGHT				0
#define JASON_SMALL_ANI_IDLE_LEFT				1

#define JASON_SMALL_ANI_WALKING_RIGHT			2
#define JASON_SMALL_ANI_WALKING_LEFT			3

#define JASON_BIG_BBOX_WIDTH  26	
#define JASON_BIG_BBOX_HEIGHT 18

#define JASON_TANK_BBOX_WIDTH  26	
#define JASON_TANK_BBOX_HEIGHT 18

#define JASON_SMALL_BBOX_WIDTH	8
#define JASON_SMALL_BBOX_HEIGHT 16


#define JASON_UNTOUCHABLE_TIME 5000


class CJason : public CGameObject
{
	int level;
	int untouchable;

	bool isJumping;
	bool isFiring;

	DWORD untouchable_start;

	float start_x;			// initial position of JASON at scene
	float start_y; 

	CTank* Tank;
	vector<CBullet*> Bullets;
	int bulletIndex;
public: 
	CJason(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void WorldToRender();
	virtual void Render();

	void SetState(int state);

	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void SetTank(CTank* tank) { Tank = tank; }
	CTank* GetTank();
	bool GetIsJumping() { return isJumping; }

	bool GetIsFiring() { return isFiring; }
	void SetIsFiring(bool b) { isFiring = b; }

	void PushBullets(CBullet* bl) { Bullets.push_back(bl); }
	vector<CBullet*> GetBullets() { return Bullets; }
	
	void StartAttack();
	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};