#pragma once
#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	camera = NULL;
	player = NULL;
	map = NULL;
	quadtree = NULL;
	background = NULL;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP		7
#define SCENE_SECTION_BACKGROUND	8



#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JASON:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] Jason object was created before!\n");
			return;
		}
		else
		{
			obj = new CJason(x, y);
			player = (CJason*)obj;
			player->SetType(OBJECT_TYPE_JASON);
			int level = atoi(tokens[4].c_str());
			player->SetLevel(level);
			DebugOut(L"[INFO] Player object created!\n");
			permanentObjects.push_back(obj);
		}
		break;
	case OBJECT_TYPE_TANK:
		obj = new CTank(x,y);
		obj->SetType(OBJECT_TYPE_TANK);
		player->SetTank((CTank*)obj);
		permanentObjects.push_back(obj);
		break;
	case OBJECT_TYPE_CANNON:
		obj = new CCannon(x, y);
		obj->SetType(OBJECT_TYPE_CANNON);
		player->GetTank()->SetCannon((CCannon*)obj);
		permanentObjects.push_back(obj);
		break;
	case OBJECT_TYPE_WHEEL:
		obj = new CWheel(x, y);
		obj->SetType(OBJECT_TYPE_WHEEL);
		player->GetTank()->PushWheels((CWheel*)obj);
		permanentObjects.push_back(obj);
		break;
	case OBJECT_TYPE_BULLET:
		obj = new CBullet();
		obj->SetType(OBJECT_TYPE_BULLET);
		player->PushBullets((CBullet*)obj);
		permanentObjects.push_back(obj);
		break;
	case OBJECT_TYPE_RAINBOWBULLET:
		obj = new CRainbowBullet();
		obj->SetType(OBJECT_TYPE_RAINBOWBULLET);
		player->PushRainBullets((CRainbowBullet*)obj);
		permanentObjects.push_back(obj);
		break;
	case OBJECT_TYPE_INTERRUPT: 
		obj = new CInterrupt(); 
		((CInterrupt*)obj)->SetInitPosition(x, y);
		((CInterrupt*)obj)->SetJason(player);
		((CInterrupt*)obj)->SetInitPosition(x, y);
		obj->SetType(OBJECT_TYPE_INTERRUPT);
		break;
	case OBJECT_TYPE_BALLBOT:
		obj = new CBallbot();
		((CBallbot*)obj)->SetInitPosition(x, y);
		((CBallbot*)obj)->SetJason(player);
		((CBallbot*)obj)->SetInitPosition(x, y);
		obj->SetType(OBJECT_TYPE_BALLBOT);
		break;
	case OBJECT_TYPE_EYELET:
		{
			obj = new CEyelet();
			((CEyelet*)obj)->SetInitPosition(x, y);
			((CEyelet*)obj)->SetJason(player);
			((CEyelet*)obj)->SetInitPosition(x, y);
			obj->SetType(OBJECT_TYPE_EYELET);
			int direction = atoi(tokens[4].c_str());
			obj->Setnx(direction);
			obj->SetState(EYELET_STATE_ACTION);
		}
		break;
	case OBJECT_TYPE_STUKA:
		{
			obj = new CStuka();
			((CStuka*)obj)->SetInitPosition(x, y);
			((CStuka*)obj)->SetJason(player);
			//((CStuka*)obj)->SetInitPosition(x, y);
			obj->SetType(OBJECT_TYPE_STUKA);
			int direction = atoi(tokens[4].c_str());
			obj->Setnx(direction);
			obj->SetState(STUKA_STATE_IDLE);
		}
		break;
	case OBJECT_TYPE_GX680:
	{
		obj = new CGx680();
		((CGx680*)obj)->SetInitPosition(x, y);
		((CGx680*)obj)->SetJason(player);
		obj->SetType(OBJECT_TYPE_GX680);
		obj->SetState(GX680_STATE_ACTION);
		gunEnemies.push_back(obj);
	}
		break;
	case OBJECT_TYPE_DRAG:
	{
		obj = new CDrag();
		((CDrag*)obj)->SetInitPosition(x, y);
		((CDrag*)obj)->SetJason(player);
		//((CStuka*)obj)->SetInitPosition(x, y);
		obj->SetType(OBJECT_TYPE_DRAG);
		int direction = atoi(tokens[4].c_str());
		obj->Setnx(direction);
		obj->SetState(DRAG_STATE_IDLE);
	}
	break;
	case OBJECT_TYPE_LASERGUARD:
	{
		obj = new CLaserguard();
		((CLaserguard*)obj)->SetInitPosition(x, y);
		((CLaserguard*)obj)->SetJason(player);
		obj->SetType(OBJECT_TYPE_LASERGUARD);
		int direction = atoi(tokens[4].c_str());
		obj->Setnx(direction);
		obj->SetState(LASERGUARD_STATE_IDLE);
		gunEnemies.push_back(obj);
	}
	break;
	case OBJECT_TYPE_BALLCARRY:
		{
			obj = new CBallcarry();
			((CBallcarry*)obj)->SetInitPosition(x, y);
			((CBallcarry*)obj)->SetJason(player);
			obj->SetType(OBJECT_TYPE_BALLCARRY);
		}
		break;
	case OBJECT_TYPE_ENEMYBULLET:
	{
		obj = new CEBullet();
		obj->SetType(OBJECT_TYPE_ENEMYBULLET);
		int enemyOwner = atoi(tokens[4].c_str());
		((CEBullet*)obj)->SetEnemyOwner(enemyOwner);
		switch (enemyOwner)
		{
		case OBJECT_TYPE_LASERGUARD:
			{
				for (unsigned int i = 0; i < gunEnemies.size(); i++)
				{
					if (gunEnemies[i]->GetType() == OBJECT_TYPE_LASERGUARD)
					{
						if (((CLaserguard*)gunEnemies[i])->GetEBullet() == NULL)
						{
							((CLaserguard*)gunEnemies[i])->SetEBullet((CEBullet*)obj);
							break;
						}
					}
					
				}
			}
			break;
		case OBJECT_TYPE_GX680:
			{

			}
			break;
		}
	}
		break;
	case OBJECT_TYPE_BRICK: 
		{
			int w = atoi(tokens[4].c_str());
			int h = atoi(tokens[5].c_str());
			obj = new CBrick(x,y,w,h);
			obj->SetType(OBJECT_TYPE_BRICK);
		}
		break;
	case OBJECT_TYPE_WEAKBRICK:
		{
			obj = new CWeakBrick(x, y);
			obj->SetType(OBJECT_TYPE_WEAKBRICK);
		}
		break;	
	case OBJECT_TYPE_PORTAL:
		{	
			float w = atof(tokens[4].c_str());
			float h = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, w, h, scene_id);
			((CPortal*)obj)->SetType(OBJECT_TYPE_PORTAL);
			permanentObjects.push_back(obj);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	//float l, t, r, b;
	//obj->GetBoundingBox(l, t, r, b);
	quadtree->Insert(obj);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 7) return;
	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
	
}
void CPlayScene::_ParseSection_BACKGROUND(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return;
	float x = (float)atof(tokens[0].c_str());
	float y = (float)atof(tokens[1].c_str());
	int sprite_id = atoi(tokens[2].c_str());
	
	background->SetPosition(x, y);
	LPSPRITE sprites = CSprites::GetInstance()->Get(sprite_id);
	background->SetSprite(sprites);
	background->CalculateWidthHeight();
	quadtree = new Quadtree(0.0f, 0.0f, 0.0f, background->GetWidth(), background->GetHeight());
}
void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	
	camera = new Camera();
	background = new CBackground();
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[BACKGROUND]") {
			section = SCENE_SECTION_BACKGROUND; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_BACKGROUND: _ParseSection_BACKGROUND(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	camera->SetJason(player);
	camera->SetBoundary(background->GetWidth(), background->GetHeight());
	quadtree->NumberOfObjectsInNodes();
	DebugOut(L"[INFO] Number of Objects %i\n", objects.size());
	DebugOut(L"[INFO] Screen Height %i\n",CGame::GetInstance()->GetScreenHeight());
	DebugOut(L"[INFO] Screen Width %i\n", CGame::GetInstance()->GetScreenWidth());
}

void CPlayScene::Update(DWORD dt)
{
	if (player == NULL) return;
	// We know that Jason is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	//camera->Update(dt);
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coObjectsOfJason;			//Objects for collidding of Jason
	vector<LPGAMEOBJECT> coObejctsOfBullets;
	vector<LPGAMEOBJECT> coObjectsOfEnemies1;		//not include bircks
	vector<LPGAMEOBJECT> coObjectsOfEnemies2;		//include bricks
	for (unsigned int i = 0; i < permanentObjects.size(); i++)
	{
		coObjects.push_back(permanentObjects[i]);
	}
	if (quadtree != NULL)
		quadtree->GetListObject(coObjects, camera);
	for (unsigned int i = 0; i < coObjects.size(); i++)
	{
		switch (coObjects[i]->GetType())
		{
		case OBJECT_TYPE_BRICK:
			coObjectsOfJason.push_back(coObjects[i]);
			coObejctsOfBullets.push_back(coObjects[i]);
			coObjectsOfEnemies2.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_WEAKBRICK:
			if (coObjects[i]->GetState() == WEAKBRICK_STATE_NORMAL)
			{
				coObjectsOfJason.push_back(coObjects[i]);
				coObejctsOfBullets.push_back(coObjects[i]);
				coObjectsOfEnemies2.push_back(coObjects[i]);
			}
			break;
		case OBJECT_TYPE_JASON:
			coObjectsOfEnemies1.push_back(coObjects[i]);
			coObjectsOfEnemies2.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_BALLBOT:
			if(coObjects[i]->GetState()!=BALLBOT_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_BALLCARRY:						
			if (coObjects[i]->GetState() != BALLBOT_STATE_DIE)		
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_EYELET:
			if (coObjects[i]->GetState() != EYELET_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_INTERRUPT:
			if (coObjects[i]->GetState() != INTERRUPT_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_STUKA:
			if (coObjects[i]->GetState() != STUKA_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_GX680:
			if(coObjects[i]->GetState()!=GX680_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_DRAG:
			if (coObjects[i]->GetState() != DRAG_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_LASERGUARD:
			if (coObjects[i]->GetState() != LASERGUARD_STATE_DIE)
				coObejctsOfBullets.push_back(coObjects[i]);
			break;
		case OBJECT_TYPE_PORTAL:
			coObjectsOfJason.push_back(coObjects[i]);
			break;
		}
	}
	DebugOut(L"coObjects size:%i\n", coObjects.size());
	// skip the rest if scene was already unloaded (Jason::Update might trigger PlayScene::Unload)
	player->Update(dt, &coObjectsOfJason);
	if (player == NULL) return;
	for (size_t i = 0; i < coObjects.size(); i++)
	{
		switch (coObjects[i]->GetType())
		{
		case OBJECT_TYPE_JASON:
			//continue;
			//coObjects[i]->Update(dt, &coObjectsOfJason);
			break;
		case OBJECT_TYPE_BULLET:
			coObjects[i]->Update(dt, &coObejctsOfBullets);
			break;
		case OBJECT_TYPE_RAINBOWBULLET:
			coObjects[i]->Update(dt, &coObejctsOfBullets);
			break;
		case OBJECT_TYPE_EYELET:
			coObjects[i]->Update(dt, &coObjectsOfEnemies1);
			break;
		case OBJECT_TYPE_STUKA:
			coObjects[i]->Update(dt, &coObjectsOfEnemies2);
			break;
		case OBJECT_TYPE_BALLBOT:
			coObjects[i]->Update(dt, &coObjectsOfEnemies1);
			break;
		case OBJECT_TYPE_GX680:
			coObjects[i]->Update(dt, &coObjectsOfEnemies2);
			break;
		case OBJECT_TYPE_DRAG:
			coObjects[i]->Update(dt, &coObjectsOfEnemies2);
			break;
		case OBJECT_TYPE_LASERGUARD:
			coObjects[i]->Update(dt, &coObjectsOfEnemies2);
			break;
		case OBJECT_TYPE_ENEMYBULLET:
			coObjects[i]->Update(dt, &coObjectsOfEnemies1);
			break;
		case OBJECT_TYPE_BALLCARRY:
			coObjects[i]->Update(dt, &coObjectsOfEnemies2);
			break;
		default:
			if (player == NULL)
				return;
			if (coObjects[i] == NULL)
				return;
			coObjects[i]->Update(dt, NULL);
			break;
		}
		
	}
	// Update camera to follow Jason
	//CGame::GetInstance()->SetCamPos(0.0f, -64.0f);
	camera->Update(dt);
	if (player == NULL)
		return;
}

void CPlayScene::Render()
{
	/*if (map)
	{
		this->map->Render((int)camera->GetCamX(), (int)camera->GetCamY());
	}*/
	if (background)
	{
		background->Draw();
	}
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	player = NULL;
	camera = NULL;

	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	permanentObjects.clear();
	gunEnemies.clear();
	background = NULL;
	quadtree = NULL;
	
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CJason *Jason = ((CPlayScene*)scence)->GetPlayer();
	if (Jason == NULL)
		return;
	switch (KeyCode)
	{
	case DIK_S:
		CGame::GetInstance()->SwitchScene(2);
		break;
	case DIK_SPACE:
		Jason->SetState(JASON_STATE_JUMP);
		break;
	case DIK_A: 
		Jason->Reset();
		break;
	case DIK_X:
		float l1, t1, r1, b1, l2, t2, r2, b2;
		Jason->GetBoundingBox(l1, t1, r1, b1);
		Jason->GetTank()->GetBoundingBox(l2, t2, r2, b2);
		if (CGame::GetInstance()->AABBCheck(l1, t1, r1, b1, l2, t2, r2, b2) == true)
		{
			switch (Jason->GetLevel())
			{
			case JASON_LEVEL_TANK:
				Jason->SetLevel(JASON_LEVEL_SMALL);
				break;
			case JASON_LEVEL_SMALL:
				Jason->SetLevel(JASON_LEVEL_TANK);
				Jason->SetPosition(l1, t1 + 10.0f);
				break;
			}
		}		
		break;
	case DIK_Z:		
		Jason->StartAttack();
		Jason->SetIsFiring(true);
		break;
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CJason* Jason = ((CPlayScene*)scence)->GetPlayer();
	if (Jason == NULL)
		return;
	switch (KeyCode)
	{
	case DIK_UP:
		Jason->GetTank()->SetCannonUP(false);
		break;
	case DIK_Z:
		Jason->SetIsFiring(false);
		break;
	}
}


void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CJason *Jason = ((CPlayScene*)scence)->GetPlayer();
	if (Jason == NULL)
		return;

	// disable control key when Jason die 
	if (Jason->GetState() == JASON_STATE_DIE) return;
	switch (Jason->GetLevel())
	{
	case JASON_LEVEL_BIG:
		{
			if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
			{
				Jason->SetState(JASON_STATE_WALKING_RIGHT);
				Jason->SetLastState(JASON_STATE_WALKING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT))
			{
				Jason->SetState(JASON_STATE_WALKING_LEFT);
				Jason->SetLastState(JASON_STATE_WALKING_LEFT);
			}
			else if (game->IsKeyDown(DIK_UP) && !game->IsKeyDown(DIK_DOWN))
			{
				Jason->SetState(JASON_STATE_WALKING_UP);
				Jason->SetLastState(JASON_STATE_WALKING_UP);
			}
			else if (game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_UP))
			{
				Jason->SetState(JASON_STATE_WALKING_DOWN);
				Jason->SetLastState(JASON_STATE_WALKING_DOWN);
			}
			else
			{
				Jason->SetState(JASON_STATE_IDLE);
			}
		}
		break;
	default:
		{
			if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
			{
				if (game->IsKeyDown(DIK_UP))
					Jason->GetTank()->SetCannonUP(true);
				else
				{
					Jason->GetTank()->SetCannonUP(false);
				}
				Jason->SetState(JASON_STATE_WALKING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT))
			{
				Jason->SetState(JASON_STATE_WALKING_LEFT);
				if (game->IsKeyDown(DIK_UP))
					Jason->GetTank()->SetCannonUP(true);
				else
				{
					Jason->GetTank()->SetCannonUP(false);
				}
			}
			else
			{
				if (game->IsKeyDown(DIK_UP) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
				{
					Jason->GetTank()->SetCannonUP(true);
					Jason->SetState(JASON_STATE_IDLE);
				}
				else
					Jason->SetState(JASON_STATE_IDLE);
			}
		}

		break;
	}

		
}