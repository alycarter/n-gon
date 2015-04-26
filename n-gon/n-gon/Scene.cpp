#include "Scene.h"
#include "SceneCamera.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\GraphicsManager.h"
#include "emmet-engine\RenderPass.h"
#include "emmet-engine\Entity.h"
#include "ShipFactory.h"
#include "LevelController.h"

Scene::Scene(GraphicsManager * graphics)
{
	addRenderPass(PASS_SCENE);

	graphics->createMeshResourceFromFile("quad", "obj/quad.obj", this);
	graphics->createShaderResourceFromFile("line", L"line.hlsl", "line.hlsl", this);

	Entity * camera = new Entity();
	camera->addComponent(new SceneCamera());
	addEntity(camera);

	ShipFactory factory;

	Entity * player = new Entity();
	factory.createShip(player, graphics, SHIP_TYPE_PLAYER);
	player->getComponentOfType<Transform>()->setPosition(&XMVectorSet(1280/2, 720/2, 0, 0));
	addEntity(player);

	Entity * level = new Entity();
	level->addComponent(new LevelController());
	addEntity(level);
}


Scene::~Scene()
{
}
