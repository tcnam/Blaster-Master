#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "Font.h"
#include "Singlebar.h"
#include "Game.h"

#define RELATIVE_DX		16.0f
#define RELATIVE_DY		144.0f

class CHealthbar
{
	float render_x;
	float render_y;
	LPSPRITE sprite;
	vector<CSinglebar*> singlebars;
public:
	CHealthbar();
	~CHealthbar();

	void PushSingleBar(CSinglebar* s) { singlebars.push_back(s); }
	vector<CSinglebar*> GetSingleBar(){ return singlebars; }

	void SetRenderPosition(float tempx, float tempy);
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }
	LPSPRITE GetSprite() { return this->sprite; }

	void Update();
	void Draw();
};
