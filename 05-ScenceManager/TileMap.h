#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Sprites.h"

#define	TILE_WIDTH 16
#define TILE_HEIGHT 16
#define IN_USE_WIDTH 330
#define IN_USE_HEIGHT 300

class Map
{
private: 
	int** TileMap;
	vector <LPSPRITE> Tiles;
	LPDIRECT3DTEXTURE9 TileSet; //map1.png
	int TotalColumnsOfMap, TotalRowsOfMap;
	int TotalColumnsOfTileSet, TotalRowsOfTileSet;
	int TotalTiles; //total tiles of tileset
public:
	Map(int _idTileSet, int _totalRowTileSet, int _totalColumnsTileSet, int _totalRowsMap, int _totalColumnsMap, int totalTiles);
	~Map();
	void Render();
	void Render(int camX, int camY);
	void ExtractTileFromTileSet();
	void LoadMap(LPCWSTR file_path);

	int GetMapHeight();
	int GetMapWidth();
};