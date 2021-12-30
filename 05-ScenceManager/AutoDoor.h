#pragma once
#include "GameObject.h"

#define AUTODOOR_BBOX_WIDTH	10
#define AUTODOOR_BBOX_HEIGHT	32

class CAutodoor : public CGameObject
{
private:
	float x1, y1;
	float x2, y2;
	float x3, y3;
	float x4, y4;
public:
	CAutodoor(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3, float x_4, float y_4);
	~CAutodoor();
	void SwitchLocation();
	virtual void Render();
	virtual void WorldToRender();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

