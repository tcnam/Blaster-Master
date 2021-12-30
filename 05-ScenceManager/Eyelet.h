#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Reward.h"

#define EYELET_BBOX_WIDTH 18
#define EYELET_BBOX_HEIGHT 18

#define EYELET_STATE_IDLE 100
#define EYELET_STATE_ACTION	200
#define EYELET_STATE_DIE 300

#define EYELET_ANI_RIGHT 0
#define EYELET_ANI_LEFT 1

#define EYELET_SPEED_X		0.08f

#define DY_FOR_CHANGE_STATE		320
class CEyelet : public CGameObject
{

private:
	CJason* Jason;
	int dem;
	CReward* Reward;
public:
	CEyelet();
	void SetJason(CJason* jason) { Jason = jason; }
	void SetReward(CReward* r) { Reward = r; }
	CReward* GetReward() { return Reward; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CEyelet();
};
