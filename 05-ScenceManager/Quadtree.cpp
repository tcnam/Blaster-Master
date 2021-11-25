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
	obj->GetBoundingBox(l, t, r, b);
	/*switch (type)
	{
	case OBJECT_TYPE_JASON:
		((CJason*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_TANK:
		((CTank*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_CANNON:
		((CCannon*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_WHEEL:
		((CWheel*)obj)->GetBoundingBox(l, t, r, b);
		break;
	case OBJECT_TYPE_BRICK:
		((CBrick*)obj)->GetBoundingBox(l, t, r, b);
		break;
	}*/
	return CGame::GetInstance()->AABBCheck(l, t, r, b, this->x, this->y, this->x + this->width, this->y + this->height);

}
void Quadtree::Split()
{
	Nodes.push_back(new Quadtree(level + 1, x, y, width / 2, height / 2));
	Nodes.push_back(new Quadtree(level + 1, x + width / 2, y, width / 2, height / 2));
	Nodes.push_back(new Quadtree(level + 1, x, y + height / 2, width / 2, height / 2));
	Nodes.push_back(new Quadtree(level + 1, x + width / 2, y + height / 2, width / 2, height / 2));
	//DebugOut(L"New Nodes level %i has been created with %i\n", level + 1, Nodes.size());

}
void Quadtree::Insert(CGameObject* obj)
{
	/*if (obj->GetType() == OBJECT_TYPE_INTERRUPT)
		return;*/
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
	if (this->IsContain(obj) == true && obj->GetIsInserted() == false)
	{
		ObjectList.push_back(obj);
		obj->SetIsInserted(true);
	}
		
	if (ObjectList.size() > MAX_OBJECT_IN_REGION && level < MAX_LEVEL)
	{
		Split();
		while (!ObjectList.empty())
		{
			if (Nodes[0]->IsContain(ObjectList.back()))
			{
				ObjectList.back()->SetIsInserted(false);
				Nodes[0]->Insert(ObjectList.back());
			}
				
			if (Nodes[1]->IsContain(ObjectList.back()))
			{
				ObjectList.back()->SetIsInserted(false);
				Nodes[1]->Insert(ObjectList.back());
			}
				
			if (Nodes[2]->IsContain(ObjectList.back()))
			{
				ObjectList.back()->SetIsInserted(false);
				Nodes[2]->Insert(ObjectList.back());
			}
				
			if (Nodes[3]->IsContain(ObjectList.back()))
			{
				ObjectList.back()->SetIsInserted(false);
				Nodes[3]->Insert(ObjectList.back());
			}
				
			ObjectList.pop_back();
		}
	}
	
}
bool Quadtree::CheckNodeInsideCamera(Camera* camera)
{
	float cam_x, cam_y;
	camera->GetPosition(cam_x, cam_y);
	float left, top, right, bottom;
	left = cam_x;
	top = cam_y;
	right = cam_x + CAMX_IN_USE;
	bottom = cam_y + CAMY_IN_USE;
	return CGame::GetInstance()->AABBCheck(this->x, this->y, this->x + this->width, this->y + this->height, left, top, right, bottom);
}
void Quadtree::GetListObject(vector<CGameObject*>& Obj, Camera* camera)
{
	
	if (Nodes.size() != 0)
	{
		if (Nodes[0]->CheckNodeInsideCamera(camera))
			Nodes[0]->GetListObject(Obj, camera);
		if (Nodes[1]->CheckNodeInsideCamera(camera))
			Nodes[1]->GetListObject(Obj, camera);
		if (Nodes[2]->CheckNodeInsideCamera(camera))
			Nodes[2]->GetListObject(Obj, camera);
		if (Nodes[3]->CheckNodeInsideCamera(camera))
			Nodes[3]->GetListObject(Obj, camera);
		return;
	}
	//DebugOut(L"List object wrong 1:%i\n", Obj.size());
	if (this->CheckNodeInsideCamera(camera))
	{
		for (unsigned int i = 0; i < ObjectList.size(); i++)
		{
			Obj.push_back(ObjectList[i]);
		}
		//DebugOut(L"List object wrong 2:%i\n", Obj.size());
	}

}

void Quadtree::NumberOfObjectsInNodes()
{
	if (Nodes.size() != 0)
	{
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			Nodes[i]->NumberOfObjectsInNodes();
		}
	}
	DebugOut(L"Nodes level %i has %i objects\n", level, ObjectList.size());
}