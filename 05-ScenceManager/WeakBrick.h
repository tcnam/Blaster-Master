#pragma once
#include "GameObject.h"

#define WEAKBRICK_BBOX_WIDTH	16
#define WEAKBRICK_BBOX_HEIGHT	16

#define WEAKBRICK_STATE_NORMAL	0
#define WEAKBRICK_STATE_DISAPPEAR	1


class CWeakBrick : public CGameObject
{
public:
	CWeakBrick(float loaction_x, float location_y);
	~CWeakBrick();
	virtual void Render();
	virtual void WorldToRender();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};
