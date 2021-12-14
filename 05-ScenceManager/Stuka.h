#pragma once
#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Brick.h"


#define STUKA_BBOX_WIDTH 18
#define STUKA_BBOX_HEIGHT 18

#define STUKA_STATE_IDLE 100
#define STUKA_STATE_ACTION	200
#define STUKA_STATE_DIE 300

#define STUKA_ANI_RIGHT 0
#define STUKA_ANI_LEFT 1

#define STUKA_SPEED_X		0.05f

#define DY_FOR_CHANGE_STATE		320
class CStuka : public CGameObject
{

private:
	CJason* Jason;
public:
	CStuka();
	void SetJason(CJason* jason) { Jason = jason; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CStuka();
};


