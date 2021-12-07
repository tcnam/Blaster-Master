#pragma once
#include "GameObject.h"


class CBrick : public CGameObject
{
private:
	int width;
	int height;
public:
	CBrick();
	CBrick(float x, float y, int width, int height);
	~CBrick();

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	virtual void Render();
	virtual void WorldToRender();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};