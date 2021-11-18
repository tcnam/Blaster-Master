#pragma once
#include <iostream>
#include <fstream>
#include "GameObject.h"
#include "Camera.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Jason.h"
#include "Brick.h"

#define MAX_LEVEL 5
#define MAX_OBJECT_IN_REGION 4

using namespace std;

class Quadtree
{
private:
	int level;
	vector <CGameObject*> ObjectList;
	vector <Quadtree*> Nodes;
	float x, y, width, height;
public:
	Quadtree();
	Quadtree(int level, float x, float y, float w, float h);
	~Quadtree();

	void Split();
	bool IsContain(CGameObject* obj);
	void Clear();
	void Insert(CGameObject* object);
	void GetListObject(vector<CGameObject*>& Obj, Camera* camera);
};
