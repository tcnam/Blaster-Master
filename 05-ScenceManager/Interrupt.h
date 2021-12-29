#pragma once
#include "GameObject.h"
#include "Jason.h"
#include "Neoworm.h"
#include "Reward.h"

#define INTERRUPT_BBOX_WIDTH 22
#define INTERRUPT_BBOX_HEIGHT 18
#define INTERRUPT_BBOX_HEIGHT_ACTION 15

#define INTERRUPT_STATE_IDLE 100
#define INTERRUPT_STATE_ACTION	200
#define INTERRUPT_STATE_DIE 300

#define INTERRUPT_ANI_IDLE 0
#define INTERRUPT_ANI_ACTION 1

#define DY_FOR_CHANGE_STATE		320
class CInterrupt : public CGameObject
{
	
private:
	CNeoworm* Worm;
	CJason* Jason;
	CReward* Reward;
public:
	CInterrupt();

	void SetJason(CJason* jason) { Jason = jason; }

	void SetWorm(CNeoworm* w) { Worm = w; }
	CNeoworm* GetWorm() { return Worm; }
	
	void ActivateNeoworm();
	void SetReward(CReward* r) { Reward = r; }
	CReward* GetReward() { return Reward; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	~CInterrupt();
};
