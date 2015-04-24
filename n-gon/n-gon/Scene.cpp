#include "Scene.h"
#include "SceneCamera.h"
#include "ShapeRenderer.h"
#include "PlayerController.h"
#include "Physics.h"
#include "ShipController.h"
#include "ParticleEmmiter.h"
#include "SimpleEnemyController.h"
#include "AimingEnemyController.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\GraphicsManager.h"
#include "emmet-engine\RenderPass.h"
#include "emmet-engine\Entity.h"


Scene::Scene(GraphicsManager * graphics)
{
	addRenderPass(PASS_SCENE);

	graphics->createMeshResourceFromFile("quad", "obj/quad.obj", this);
	graphics->createShaderResourceFromFile("line", L"line.hlsl", "line.hlsl", this);

	Entity * camera = new Entity();
	camera->addComponent(new SceneCamera());
	addEntity(camera);

	Entity * player = new Entity();
	player->addComponent(new Physics());
	player->addComponent(new ShipController(10, 15, 100, XMConvertToRadians(90), 0.25f, 5));
	player->addComponent(new ShapeRenderer(graphics));
	player->addComponent(new ParticleEmmiter(graphics, 100));
	player->addComponent(new PlayerController());
	Transform * transform = player->getComponentOfType<Transform>();
	player->getComponentOfType<Physics>()->addCollisionListener(player->getComponentOfType<ShipController>());
	transform->setPosition(&XMVectorSet(100, 100, 0, 0));
	addEntity(player);

	for (unsigned int i = 0; i < 5; i++)
	{
		Entity * enemy = new Entity();
		enemy->addComponent(new Physics());
		enemy->addComponent(new ShipController(5, 15, 100, XMConvertToRadians(90), 1.5f, 5));
		enemy->addComponent(new ShapeRenderer(graphics));
		enemy->addComponent(new ParticleEmmiter(graphics, 100));
		enemy->addComponent(new AimingEnemyController());
		enemy->getComponentOfType<Physics>()->addCollisionListener(enemy->getComponentOfType<ShipController>());
		transform = enemy->getComponentOfType<Transform>();
		transform->setPosition(&XMVectorSet(200.0f + (rand() % 1000), 200.0f + (rand() % 500), 0, 0));
		addEntity(enemy);
	}
}


Scene::~Scene()
{
}
