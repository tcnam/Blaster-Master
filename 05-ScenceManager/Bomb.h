#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Jason.h"
#include "WeakBrick.h"


#define BOMB_BBOX_WIDTH					10
#define BOMB_BBOX_HEIGHT				10

#define BOMB_STATE_IDLE		100
#define BOMB_STATE_JUMP		200		
#define BOMB_STATE_FALL		300
#define BOMB_STATE_ACTION	400
#define BOMB_STATE_DIE		500

#define BOMB_ANI_IDLE		0
#define BOMB_ANI_EXPLODE	1

#define BOMB_SPEED_X		0.008f
#define BOMB_SPEED_Y		0.05f

#define DISTANCE_TO_CHANGE_STATE	32.0f

#define EXPLODE_TIME				1000

class CBomb : public CGameObject
{
private:
	float start_x;
	float start_y;
	CJason* Jason;
	DWORD explode_start;
public:
	CBomb();
	void SetJason(CJason* j) { Jason = j; }
	void StartExplode() { explode_start = GetTickCount64(); }
	void SetStartPosition(float sx, float sy) { start_x = sx, start_y = sy; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CBomb();
};



