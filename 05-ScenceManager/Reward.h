#pragma once
#include "GameObject.h"
#include "Jason.h"

#define REWARD_BBOX_WIDTH	16
#define REWARD_BBOX_HEIGHT	16

#define REWARD_TYPE_NOTHING	0
#define REWARD_TYPE_HEALTH	10
#define REWARD_TYPE_POWER	20

#define REWARD_STATE_IDLE 100
#define REWARD_STATE_ACTION	200
#define REWARD_STATE_DIE 300

#define REWARD_ANI_HEALTH		0
#define REWARD_ANI_POWER		1

#define DY_FOR_CHANGE_STATE		320
class CReward : public CGameObject
{
private:
	int typeReward;
public:
	CReward();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);
	void SetTypeReward(int r) { typeReward = r; }
	int GetTypeReward() { return typeReward; }
	~CReward();
};
