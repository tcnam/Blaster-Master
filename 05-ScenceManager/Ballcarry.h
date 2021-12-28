
#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Brick.h"
#include "WeakBrick.h"
#include "Bomb.h"


#define BALLCARRY_BBOX_WIDTH	17
#define BALLCARRY_BBOX_HEIGHT	17

#define BALLCARRY_STATE_IDLE	100
#define BALLCARRY_STATE_ACTION	200
#define BALLCARRY_STATE_RUN		300
#define BALLCARRY_STATE_DIE		400

#define BALLCARRY_ANI_IDLE		0
#define BALLCARRY_ANI_ACTION	1
#define BALLCARRY_ANI_RUN		2

#define BALLCARRY_SPEED_X		0.08f
#define BALLCARRY_GRAVITY		0.0008f

#define ACTION_TIME				500

#define DISTANCE_FOR_CHANGE_STATE		48.0f
class CBallcarry : public CGameObject
{
private:
	CJason* Jason;
	DWORD action_start;
	vector<CBomb*> bombs;
public:
	CBallcarry();

	void SetJason(CJason* jason) { Jason = jason; }
	void PushBomb(CBomb* b) { bombs.push_back(b); }
	vector<CBomb*> GetBombs() { return bombs; }
	void ActivateBombs();

	void StartAction() { action_start = GetTickCount64(); }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CBallcarry();
};



