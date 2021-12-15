#pragma once
#pragma once
#include "GameObject.h"
#include "Jason.h"

#define  BALLBOT_SPEED_X		0.05f
#define	 BALLBOT_SPEED_Y		0.05f

#define BALLBOT_BBOX_WIDTH 18
#define BALLBOT_BBOX_HEIGHT 18

#define BALLBOT_STATE_IDLE		100
#define BALLBOT_STATE_ACTION	200
#define BALLBOT_STATE_DIE		400

#define BALLBOT_ANI_MOVE_DOWN	0
#define BALLBOT_ANI_MOVE_UP		1

#define DY_FOR_CHANGE_STATE		320.0f
#define DX_FOR_CHANGE_STATE		128.0f
class CBallbot : public CGameObject
{
private:
	CJason* Jason;
	bool isActived;
	int dem;
public:
	CBallbot();
	void SetJason(CJason* jason) { Jason = jason; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void WorldToRender();
	virtual void Render();
	virtual void SetState(int state);

	void SetIsActived(bool b) { isActived = b; }
	bool GetIsActived() { return isActived; }
	~CBallbot();
};

