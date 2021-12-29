#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Brick.h"
#include "WeakBrick.h"
#include "Reward.h"


#define DRAG_BBOX_WIDTH 18
#define DRAG_BBOX_HEIGHT 16

#define DRAG_STATE_IDLE 100
#define DRAG_STATE_ACTION	200
#define DRAG_STATE_DIE 300

#define DRAG_ANI_IDLE 0
#define DRAG_ANI_ACTION 1

#define DRAG_SPEED		0.1f

#define DISTANCE_FOR_CHANGE_STATE		320
class CDrag : public CGameObject
{
private:
	CJason* Jason;
	CReward* Reward;
public:
	CDrag();
	void SetJason(CJason* jason) { Jason = jason; }
	void SetReward(CReward* r) { Reward = r; }
	CReward* GetReward() { return Reward; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CDrag();
};
