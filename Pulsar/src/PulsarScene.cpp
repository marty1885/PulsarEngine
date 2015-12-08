#include <PulsarScene.hpp>
#include <PulsarShader.hpp>

using namespace Pulsar;

void SceneItem::setTransform(Transform transformation)
{
	transform = transformation;
}

void SceneNode::render(Camera* camera, mat4 preTtansformMatrix)
{
	mat4 currentMatrix = transform.getTransformation()*preTtansformMatrix;
	for(SceneItem*& item : childItems)
		item->render(camera,currentMatrix);

	for(SceneNode*& node : childNodes)
		node->render(camera,currentMatrix);
}

void SceneNode::addItem(SceneItem* item)
{
	childItems.push_back(item);
}

void SceneNode::addNode(SceneNode* node)
{
	childNodes.push_back(node);
}

SceneMeshItem::SceneMeshItem(Mesh* m, MaterialShader* s)
{
	mesh = m;
	shader = s;
}

void SceneMeshItem::render(Camera* camera, mat4 preTtansformMatrix)
{
	if(shader != NULL)
	{
		shader->setCamera(camera);
		shader->setTransformation(transform.getTransformation()*preTtansformMatrix);
		Projection projection = camera->getProjection();
		shader->setProjection(&projection);
		shader->bind();
		mesh->render();
		shader->unbind();
	}
}

void SceneMeshItem::setShader(MaterialShader* materialShader)
{
	shader = materialShader;
}

SceneModelItem::SceneModelItem(Model* m, MaterialShader* s)
{
	shader = s;
	setModel(m);
}

SceneModelItem::~SceneModelItem()
{
	releaseMeshItem();
}
void SceneModelItem::setModel(Model* m)
{
	model = m;
	releaseMeshItem();
	if(m != NULL)
	{
		int size = m->getMeshNum();
		Mesh** meshes = (Mesh**)m->getMeshes();
		for(int i=0;i<size;i++)
		{
			SceneMeshItem* item = new SceneMeshItem(meshes[i],shader);
			meshItems.push_back(item);
		}
	}
}

void SceneModelItem::setShader(MaterialShader* materialShader)
{
	for(SceneMeshItem*& item : meshItems)
		item->setShader(materialShader);
}

void SceneModelItem::render(Camera* camera, mat4 preTtansformMatrix)
{
	if(shader != NULL)
	{
		mat4 currentMatrix = transform.getTransformation()*preTtansformMatrix;
		for(SceneMeshItem*& item : meshItems)
			item->render(camera,currentMatrix);
	}
}
void SceneModelItem::releaseMeshItem()
{
	for(SceneMeshItem*& item : meshItems)
		delete item;
	meshItems.clear();
}
