#include "Scene.h"
#include "SceneCamera.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\GraphicsManager.h"
#include "emmet-engine\RenderPass.h"
#include "emmet-engine\Entity.h"
#include "ShipFactory.h"
#include "LevelController.h"

#define WIDTH 1920
#define HEIGHT 1080


Scene::Scene(GraphicsManager * graphics)
{
	//add a render pass
	addRenderPass(PASS_SCENE);

	//load asset files needed
	graphics->createMeshResourceFromFile("quad", "obj/quad.obj", this);
	graphics->createShaderResourceFromFile("line", L"line.hlsl", "line.hlsl", this);

	//create a camera
	Entity * camera = new Entity();
	camera->addComponent(new SceneCamera(WIDTH, HEIGHT));
	addEntity(camera);

	//create the player
	ShipFactory factory;
	Entity * player = new Entity();
	factory.createShip(player, graphics, SHIP_TYPE_PLAYER);
	player->getComponentOfType<Transform>()->setPosition(&XMVectorSet(WIDTH/2, HEIGHT/2, 0, 0));
	addEntity(player);

	//create the level controller
	Entity * level = new Entity();
	level->addComponent(new LevelController());
	addEntity(level);
}


Scene::~Scene()
{
}
