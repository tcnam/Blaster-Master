#pragma once
#include "GameObject.h"

#define RAINBOWBULLET_STATE_IDLE 100
#define RAINBOWBULLET_STATE_FIRE 200
#define RAINBOWBULLET_STATE_IMPACT	300

#define RAINBOWBULLET_ANI_FIRE	    0
#define RAINBOWBULLET_ANI_IMPACT	1

#define RAINBOWBULLET_BBOX_WIDTH	8	
#define RAINBOWBULLET_BBOX_HEIGHT  8


#define RAINBOWBULLET_SPEED		0.2f			//0.2f

#define RAINBOWBULLET_DISTANCE_FOR_CHANGE_STATE	320.0f

#define EFFECT_TIME							500

class CRainbowBullet : public CGameObject
{
private:
	float start_x;
	float start_y;
	int ny;
	int dem;
	bool nxORny;		//true:ny,  false:nx
	DWORD effect_start;
public:
	CRainbowBullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartEffect() { effect_start = GetTickCount64(); }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void WorldToRender();
	void SetStartPosition(float x, float y);
	void SetState(int state, int nx, int ny);
	~CRainbowBullet();

};
