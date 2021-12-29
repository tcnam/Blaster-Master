#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Brick.h"


#define GX680_BBOX_WIDTH 18
#define GX680_BBOX_HEIGHT 18

#define GX680_STATE_IDLE	100
#define GX680_STATE_ACTION	200
#define GX680_STATE_DIE		300

#define GX680_ANI_MOVE		0

#define GX680_SPEED_X		0.03f

#define DISTANCE_TO_CHANGE_STATE	112.0f

class CGx680 : public CGameObject
{
private:
	CJason* Jason;
	int ny;
public:
	CGx680();
	void SetJason(CJason* jason) { Jason = jason; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CGx680();
};


