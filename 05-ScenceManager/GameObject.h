#pragma once
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <cmath>
#include <math.h>
#include"Game.h"
#include "Sprites.h"
#include "Animations.h"


using namespace std;



#define OBJECT_TYPE_JASON		0
#define OBJECT_TYPE_TANK		1
#define OBJECT_TYPE_CANNON		2
#define OBJECT_TYPE_WHEEL		3
#define OBJECT_TYPE_BRICK		4
#define OBJECT_TYPE_INTERRUPT	5
#define OBJECT_TYPE_BALLBOT		6
#define OBJECT_TYPE_EYELET		7
#define OBJECT_TYPE_BALLCARRY	8
#define OBJECT_TYPE_STUKA		9
#define OBJECT_TYPE_BULLET		10

#define OBJECT_TYPE_PORTAL	50

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
protected:

	float x; 
	float y;

	float render_x;
	float render_y;

	float init_x;
	float init_y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	int state;

	int type;

	bool isInserted;

	DWORD dt; 

	LPANIMATION_SET animation_set;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	void Setnx(int direction) { nx = direction; }

	int GetState() { return this->state; }

	int GetType() { return this->type; }
	void SetType(int t) { this->type = t; }

	void SetIsInserted(bool insertOrnot) { this->isInserted = insertOrnot; }
	bool GetIsInserted(){ return isInserted; }

	void SetInitPosition(float tempx, float tempy) { this->init_x = tempx, this->init_y = tempy; }
	void GetInitPosition(float& tempx, float& tempy) { tempx = this->init_x; tempy = this->init_y; }

	
	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();

	virtual void WorldToRender()=0;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	~CGameObject();
};

