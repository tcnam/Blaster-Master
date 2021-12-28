#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"

#define SINGLEBAR_STATE_SHOW		0
#define SINGLEBAR_STATE_HIDE		1

#define SINGLEBAR_RELATIVE_X		1.0f
#define SINGLEBAR_RELATIVE_Y		1.0f
#define SINGLEBAR_FACTOR			4

class CSinglebar
{
	float render_x;
	float render_y;
	LPSPRITE sprite;
	bool showOrnot;
public:
	CSinglebar();
	~CSinglebar();
	void SetRenderPosition(float tempx, float tempy);
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	LPSPRITE GetSprite() { return this->sprite; }
	void SetShowOrNot(bool b) { showOrnot = b; }
	bool GetShowOrNot() { return showOrnot; }
	void Draw();
};
