#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"

class CBackground;
typedef CBackground* LPBACKGROUND;
class CBackground
{
	LPSPRITE sprite;
	float x;
	float y;
	float width;
	float height;
public: 
	CBackground();
	void SetPosition(float tempx, float tempy) { x = tempx; y = tempy; }
	void CalculateWidthHeight();
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	LPSPRITE GetSprite() { return this->sprite; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	void Draw();
	~CBackground();

};
