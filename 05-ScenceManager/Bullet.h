#pragma once
#include "GameObject.h"

#define BULLET_STATE_IDLE 100
#define BULLET_STATE_FIRE 200

#define BULLET_ANI_UP	    0
#define BULLET_ANI_RIGHT	1
#define BULLET_ANI_LEFT		2

#define BULLET_BBOX_WIDTH	26	
#define BULLET_BBOX_HEIGHT  8

#define BULLET_SPEED_X		0.1f
#define BULLET_SPEED_Y		0.1f

class CBullet : public CGameObject
{
public:
	CBullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void WorldToRender();
	void SetState(int state, int nx, int ny);
	~CBullet();

}; 