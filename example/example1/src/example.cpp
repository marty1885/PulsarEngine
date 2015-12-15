#include <Pulsar.hpp>
#include <PulsarMesh.hpp>
#include <PulsarShader.hpp>
#include <PulsarScene.hpp>

using namespace Pulsar;

#include <iostream>
#include <string>
using namespace std;

#include <ExampleUtils.hpp>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

class MainWindow : public Window
{
public:
	MainWindow();
	~MainWindow();
	void render();

	void update();

protected:
	Renderer renderer;
	Mesh* mesh;
	Model* model;
	MaterialShader* shader;
	MaterialShader* normalShader;
	Texture* texture;
	Camera* camera;
	SceneNode* rootNode;
	SceneMeshItem* meshItem;
	SceneModelItem* modelItem;
};

MainWindow::MainWindow()
{
	createWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"PusarEngine Example 1");

	renderer.init();
	renderer.setClearColor(vec3(0,0,0.2));

	bool result = true;
	shader = new MaterialShader;
	result &= shader->addVertexShader(File::readAllText("data/shader/test.vs"));
	result &= shader->addFragmentShader(File::readAllText("data/shader/test.fs"));
	result &= shader->compile();

	normalShader = new MaterialShader;
	result &= normalShader->addVertexShader(File::readAllText("data/shader/test.vs"));
	result &= normalShader->addFragmentShader(File::readAllText("data/shader/Normal.fs"));
	result &= normalShader->compile();
	if(result == false)
	{
		terminate();
		exit(0);
	}

	GLfloat vertices[] =
	{
		-5,0,-5,
		-5,0,5,
		5,0,5,
		5,0,-5
	};

	GLfloat normals[] =
	{
		0,1,0,
		0,1,0,
		0,1,0,
		0,1,0
	};

	GLuint indices[] =
	{
		0,1,2,
		2,3,0
	};

	GLfloat uv[] =
	{
		0,0,
		0,1,
		1,1,
		1,0
	};

	mesh = new Mesh;
	mesh->setVertices((vec3*)vertices,4);
	mesh->setIndices(indices,6);
	mesh->setTextureCoord((vec2*)uv,8);
	mesh->setNormals((vec3*)normals,4);

	Image image;
	image.load("data/texture/planks_oak.png");

	texture = new Texture;
	texture->load(&image);
	texture->enableMipmap(true);

	shader->setTexture(texture);

	model = new Model;
	model->load("data/model/monkey.obj");

	rootNode = new SceneNode;
	meshItem = new SceneMeshItem(mesh);
	meshItem->setShader(shader);
	rootNode->addItem(meshItem);

	modelItem = new SceneModelItem(model,normalShader);
	rootNode->addItem(modelItem);

	Projection projection;
	projection.setProjection(radians(70.0f),1200.0f,720.0f,0.01f,50.0f);

	camera = new Camera(vec3(0,0.1,5),vec3(0,0,-1),vec3(0,1,0));
	camera->setProjection(projection);
}

MainWindow::~MainWindow()
{
	delete shader;
	delete normalShader;
	delete mesh;
	delete model;
	delete camera;
	delete texture;
	delete rootNode;
	delete meshItem;
	delete modelItem;
}

void MainWindow::render()
{
	renderer.clearScreen();
	renderer.initFrame();

	static float val = 1;
	static float scal = 1;

	Transform transform;

	//transform.scale(vec3(1,1,1));
	//transform.translate(vec3(0,0,0));
	//transform.rotate(vec3(cos(val*3.14)*3.14,sin(val*3.14)*3.14,0));
	//transform.rotate(vec3(0.5*3.14f,0,0));

	meshItem->setTransform(transform);

	val += 0.004*scal;
	rootNode->render(camera);
}

void MainWindow::update()
{
	static vec2 lastMousePos = getMousePos();

	float moveSpeed = 0.025;
	//Move Camera
	if(getKeyState(Key::W) == true)
		camera->move(camera->getDirection()*moveSpeed);
	if(getKeyState(Key::S) == true)
		camera->move(-camera->getDirection()*moveSpeed);
	if(getKeyState(Key::D) == true)
		camera->move(camera->getRight()*moveSpeed);
	if(getKeyState(Key::A) == true)
		camera->move(-camera->getRight()*moveSpeed);
	if(getKeyState(Key::E) == true)
		camera->move(camera->getUp()*moveSpeed);
	if(getKeyState(Key::C) == true)
		camera->move(-camera->getUp()*moveSpeed);

	// if(getKeyState(Key::Esc) == true)
	// 	exit(0);

	//Mouse movements
	vec2 mousePos = getMousePos();
	if(getMouseState(Button::ButtonLeft) == true)
	{
		vec2 diff = mousePos - lastMousePos;

		//FIXME: fixed
		Quatf rotateVector = diff.x * camera->getUp() + diff.y * camera->getRight();
		Quatf rotor = exp(0.003 * rotateVector);

		vec3 direction =  rotor * camera->getDirection() * ~rotor;
		camera->setDirection(direction);

		//HACK : This works but not well
		/*camera->rotateX(diff.y/3.14/50);
		camera->rotateY(diff.x/3.14/50);
		vec3 direction = camera->getDirection();
		vec3 up = normalize(cross(cross(direction,vec3(0,1,0)),direction));
		camera->setUp(up);
		camera->setDirection(direction);*/

		//cout << direction.x << "\t" << direction.y << "\t" << direction.z << endl;
	}
	lastMousePos = mousePos;

	//cout << camera->getPosition().y << endl;
}

int main()
{
	MainWindow window;
	window.loop();
	window.terminate();

	return 0;
}
