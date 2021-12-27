#pragma once
#include "GameObject.h"
#include "Jason.h"

#define EBULLET_STATE_IDLE 100
#define EBULLET_STATE_FIRE 200
#define EBULLET_STATE_IMPACT	300

#define EBULLET_ANI	    0

#define EBULLET_BBOX_WIDTH		10
#define EBULLET_BBOX_HEIGHT		10

#define EBULLET_SPEED_X		0.2f			//0.2f
#define EBULLET_SPEED_Y		0.2f

#define EBULLET_DISTANCE_FOR_CHANGE_STATE	240.0f

#define EFFECT_TIME							800

#define ENEMY_OWNER_LASERGUARD				10
#define ENEMY_OWNER_GX680					20

class CEBullet : public CGameObject
{
private:
	float start_x;
	float start_y;
	int enemyOwner;				//15: Laser guard, 13: GX680	
	DWORD effect_start;
public:
	CEBullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartEffect() { effect_start = GetTickCount64(); }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void WorldToRender();
	void SetStartPosition(float x, float y);
	void SetState(int state, int nx, int ny);
	void SetEnemyOwner(int e) { enemyOwner = e; }
	int GetEnemyOwner() { return enemyOwner; }
	~CEBullet();

};
