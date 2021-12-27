#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Brick.h"
#include "WeakBrick.h"


#define NEOWORM_IDLE_BBOX_WIDTH			9
#define NEOWORM_IDLE_BBOX_HEIGHT		9
#define NEOWORM_BBOX_WIDTH				18
#define NEOWORM_BBOX_HEIGHT				10

#define NEOWORM_STATE_IDLE		100
#define NEOWORM_STATE_FALL		200		
#define NEOWORM_STATE_ACTION	300
#define NEOWORM_STATE_DIE		400

#define NEOWORM_ANI_IDLE		0
#define NEOWORM_ACTION_RIGHT	1
#define NEOWORM_ACTION_LEFT		2

#define NEOWORM_SPEED		0.05f
#define NEOWORM_GRAVITY		0.0008f

class CNeoworm : public CGameObject
{
private:
	CJason* Jason;
public:
	CNeoworm();
	void SetJason(CJason* jason) { Jason = jason; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CNeoworm();
};



