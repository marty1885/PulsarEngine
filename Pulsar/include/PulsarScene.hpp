#ifndef PULSARSCENE_H
#define PULSARSCENE_H

#include <PulsarMesh.hpp>

#include <Pulsar.hpp>

#include <vector>
namespace Pulsar
{

using namespace std;

/*class Object
{
public:
	Object(Object* parent = NULL)
	{
		if(parent != NULL)
			parent->addChildObject(this);
	}

	void setParent(Object* ptr)
	{
		parent = ptr;
	}

	virtual void childDeleted(Object* ptr)
	{
		int size = child.size();
		for(int i=0;i<size;i++)
		{
			if(child[i] == ptr)
			{
				child.erase(child.begin() + i);
				break;
			}
		}
	}

	virtual ~Object()
	{
		if(parent != NULL)
			parent->childDeleted(this);

		int size = child.size();
		for(int i=size-1;i>=0;i--)//This is faster because we are using vector
			delete child[i];
	}

	virtual void addChildObject(Object* obj)
	{
		child.push_back(obj);
	}
protected:
	Object* parent = NULL;
	vector<Object*> child;
};*/

class SceneItem
{
public:
	virtual void render(Camera* camera, mat4 preTransformMatrix = mat4()){}
	virtual ~SceneItem(){}

	void setTransform(Transform transformation);
protected:
	Transform transform;
};

class ThreeDShader;

class SceneMeshItem : public SceneItem
{
public:
	SceneMeshItem(Mesh* m = NULL, ThreeDShader* s = NULL);
	void render(Camera* camera, mat4 preTransformMatrix = mat4());
	void setShader(ThreeDShader* ThreeDShader);
protected:
	Mesh* mesh = NULL;
	ThreeDShader* shader = NULL;
};

class SceneModelItem : public SceneItem
{
public:
	SceneModelItem(Model* m = NULL, ThreeDShader* s = NULL);
	virtual ~SceneModelItem();

	void setModel(Model* m);
	void setShader(ThreeDShader* ThreeDShader);
	void render(Camera* camera, mat4 preTransformMatrix = mat4());
	void releaseMeshItem();
protected:
	Model* model = NULL;
	ThreeDShader* shader = NULL;
	vector<SceneMeshItem*> meshItems;
};

class SceneNode : public SceneItem
{
public:
	void render(Camera* camera, mat4 preTransformMatrix = mat4());
	void addItem(SceneItem* item);
	void addNode(SceneNode* node);

protected:
	vector<SceneItem*> childItems;
	vector<SceneNode*> childNodes;
};

}

#endif
