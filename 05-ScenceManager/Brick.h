#pragma once
#include "GameObject.h"


class CBrick : public CGameObject
{
private:
	int width;
	int height;
public:
	CBrick(float x, float y, int width, int height);
	~CBrick();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};