#pragma once
#include <iostream>
#include <fstream>
#include "GameObject.h"
#include "Camera.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"


#define MAX_LEVEL 20
#define MAX_OBJECT_IN_REGION 40

using namespace std;

class Quadtree
{
private:
	int level;
	vector <CGameObject*> ObjectList;
	vector<CGameObject*> PermanentObjects;
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
	bool CheckNodeInsideCamera(Camera* camera);
	void GetListObject(vector<CGameObject*>& Obj, Camera* camera);

	void NumberOfObjectsInNodes();
};
