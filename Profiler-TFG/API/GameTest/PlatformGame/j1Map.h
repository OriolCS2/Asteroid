#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

struct ObjectData
{
	p2SString	name;
	int			x;
	int			y;
	int		width;
	int		height;

};

struct ObjectGroup
{
	p2SString				name;
	p2List<ObjectData*>	objects;
	~ObjectGroup();
};


struct map_layer {
	Properties	properties;
	p2SString name;
	uint width = 0u;
	uint height = 0u;
	uint* data = nullptr;
	bool Walkable = false;
	float ParallaxSpeed = 10.0f;
	~map_layer() {
		if (data != nullptr)
			delete data;
	};
	inline uint Get(int x, int y) const {
		return data[width* y + x];
	}

};


struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<map_layer*>  layers;
	p2List<ObjectGroup*>object_layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	TileSet * GetTilesetFromTileId(int id) const;

	iPoint WorldToMap(int x, int y) const;

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;
	bool ChangeMap(const p2SString* map);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, map_layer* layer);
	bool LoadObjects(pugi::xml_node& node, ObjectGroup* Obj);
	bool LoadPlayerProperties();

	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public:

	MapData data;
	bool LoadCoins();
	bool LoadEnemies();

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	p2List_item<map_layer*>* layer = nullptr;
	p2List_item<TileSet*>* tileset = nullptr;

};

#endif // __j1MAP_H__