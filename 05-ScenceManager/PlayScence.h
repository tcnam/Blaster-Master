#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Jason.h"
#include "Interrupt.h"
#include "Ballbot.h"
#include "TileMap.h"
#include "Camera.h"
#include "Quadtree.h"
#include "Background.h"
#include "Eyelet.h"
#include "Stuka.h"
#include "WeakBrick.h"
#include "GX680.h"
#include "Drag.h"
#include "Laserguard.h"
#include "EBullet.h"
#include "Ballcarry.h"
#include "Neoworm.h"
#include "Bomb.h"
#include "Hud.h"
#include "Singlebar.h"
#include "Reward.h"
#include "AutoDoor.h"

class CPlayScene: public CScene
{
protected: 
	CJason *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	//vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> tempObjects;			//store objects still inside camera;
	vector<LPGAMEOBJECT> permanentObjects;

	vector<LPGAMEOBJECT> gunEnemies;
	vector<LPGAMEOBJECT> interrupts;
	vector<LPGAMEOBJECT> ballcarries;
	vector<LPGAMEOBJECT> rewardlist;
	int rewardIndex;

	Map* map;
	vector<CBackground*> backgrounds;

	Camera* camera;
	CHud* hud;

	Quadtree* quadtree;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_BACKGROUND(string line);
	void _ParseSection_HUD(string line);

	bool IsInsideCamera(LPGAMEOBJECT obj);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CJason * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

