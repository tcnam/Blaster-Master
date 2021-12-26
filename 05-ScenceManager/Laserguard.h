
#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Brick.h"
#include "WeakBrick.h"


#define LASERGUARD_BBOX_WIDTH	17
#define LASERGUARD_BBOX_HEIGHT	18

#define LASERGUARD_STATE_IDLE	100
#define LASERGUARD_STATE_ACTION	200
#define LASERGUARD_STATE_DIE	300

#define LASERGUARD_ANI			0


#define LASERGUARD_SPEED_X		0.05f

#define DY_FOR_CHANGE_STATE		320
class CLaserguard : public CGameObject
{

private:
	CJason* Jason;
public:
	CLaserguard();
	void SetJason(CJason* jason) { Jason = jason; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CLaserguard();
};


