#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include "Sprites.h"
#include "Font.h"
#include "Jason.h"

#define RELATIVE_DX		16.0f
#define RELATIVE_DY		144.0f

class CHealthbar
{
	float render_x;
	float render_y;
	LPSPRITE sprite;
public:
	CHealthbar();
	~CHealthbar();
	void SetRenderPosition(float tempx, float tempy) { render_x = tempx+RELATIVE_DX; render_y = tempy+RELATIVE_DY; }
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	LPSPRITE GetSprite() { return this->sprite; }
	void Update();
	void Draw();
};
