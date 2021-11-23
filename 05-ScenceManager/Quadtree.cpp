#include"Quadtree.h"


Quadtree::Quadtree()
{
	level = 0;
	x = 0.0f;
	y = 0.0f;
	width = 0.0f;
	height = 0.0f;
}
Quadtree::Quadtree(int level, float x, float y, float w, float h)
{
	this->level = level;
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}
Quadtree::~Quadtree()
{

}
void Quadtree::Clear()
{
	if (Nodes.size() != 0)
	{
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			Nodes[i]->Clear();
			delete Nodes[i];
		}
	}
	ObjectList.clear();
}

bool Quadtree::IsContain(CGameObject* obj)
{
	if (obj == NULL)
		return false;
	int type = obj->GetType();
	float l, t, r, b;
	switch (type)
	{
	case OBJECT_TYPE_JASON:
		obj = new CJason();
		((CJason*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_CANNON:
		obj = new CCannon();
		((CCannon*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_WHEEL:
		obj = new CWheel();
		((CWheel*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_BRICK:
		obj = new CBrick();
		((CBrick*)obj)->GetBoundingBox(l, t, r, b);
		break;
	}
	return CGame::GetInstance()->AABBCheck(l, t, r, b, this->x, this->y, this->x + this->width, this->y + this->height);

}
void Quadtree::Split()
{
	Nodes[0] = new Quadtree(level + 1, x, y, width / 2, height / 2);
	Nodes[1] = new Quadtree(level + 1, x + width / 2, y, width / 2, height / 2);
	Nodes[3] = new Quadtree(level + 1, x, y + height / 2, width / 2, height / 2);
	Nodes[4] = new Quadtree(level + 1, x + width / 2, y + height / 2, width / 2, height / 2);

}
void Quadtree::Insert(CGameObject* obj)
{
	if (Nodes.size() != 0)
	{
		if (Nodes[0]->IsContain(obj))
			Nodes[0]->Insert(obj);
		if (Nodes[1]->IsContain(obj))
			Nodes[1]->Insert(obj);
		if (Nodes[2]->IsContain(obj))
			Nodes[2]->Insert(obj);
		if (Nodes[3]->IsContain(obj))
			Nodes[3]->Insert(obj);
		return;
	}
	if (this->IsContain(obj))
		ObjectList.push_back(obj);
	if (ObjectList.size() > MAX_OBJECT_IN_REGION && level < MAX_LEVEL)
	{
		Split();
		while (!ObjectList.empty())
		{
			if (Nodes[0]->IsContain(ObjectList.back()))
				Nodes[0]->Insert(ObjectList.back());
			if (Nodes[1]->IsContain(ObjectList.back()))
				Nodes[1]->Insert(ObjectList.back());
			if (Nodes[2]->IsContain(ObjectList.back()))
				Nodes[2]->Insert(ObjectList.back());
			if (Nodes[3]->IsContain(ObjectList.back()))
				Nodes[3]->Insert(ObjectList.back());
			ObjectList.pop_back();
		}
	}
	
}