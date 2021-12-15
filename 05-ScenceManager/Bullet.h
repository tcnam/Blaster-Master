#pragma once
#include "GameObject.h"

#define BULLET_STATE_IDLE 100
#define BULLET_STATE_FIRE 200
#define BULLET_STATE_IMPACT	300

#define BULLET_ANI_UP	    0
#define BULLET_ANI_RIGHT	1
#define BULLET_ANI_LEFT		2
#define BULLET_ANI_IMPACT	3

#define BULLET_BBOX_WIDTH	26	
#define BULLET_BBOX_HEIGHT  8

#define BULLET_IMPACT_BBOX_WIDTH	16
#define BULLET_IMPACT_BBOX_HEIGT	16

#define BULLET_SPEED_X		0.2f
#define BULLET_SPEED_Y		0.2f

#define BULLET_DISTANCE_FOR_CHANGE_STATE	320.0f

#define EFFECT_TIME							500

class CBullet : public CGameObject
{
private:
	float start_x;
	float start_y;
	DWORD effect_start;
public:
	CBullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartEffect() { effect_start = GetTickCount64(); }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void WorldToRender();
	void SetStartPosition(float x, float y);
	void SetState(int state, int nx, int ny);
	~CBullet();

}; 