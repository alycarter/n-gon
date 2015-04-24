#include "PlayerController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Controls.h"
#include "emmet-engine\Entity.h"
#include "ShapeRenderer.h"
#include "Physics.h"
#include "ShipController.h"
#include "Scene.h"
#include "emmet-engine\StateMachine.h"
#include "emmet-engine\State.h"
#include "emmet-engine\RenderPass.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\GraphicsManager.h"
#include "emmet-engine\Transform.h"

PlayerController::PlayerController()
{
	fireDelay = 0;
}


PlayerController::~PlayerController()
{
}

void PlayerController::update(UpdatePackage * package)
{
	ShipController * controller = package->entity->getComponentOfType<ShipController>();
	if (package->controls->isKeyPressed('W'))
	{
		controller->thrust();
	}
	int direction = 0;
	if (package->controls->isKeyPressed('A'))
	{
		direction--;
	}
	if (package->controls->isKeyPressed('D'))
	{
		direction++;
	}
	if (direction == 1)
	{
		controller->turn(TURN_COUNTER_CLOCKWISE);
	}
	if (direction == -1)
	{
		controller->turn(TURN_CLOCKWISE);
	}
	if (package->controls->isKeyPressed(VK_SPACE))
	{
		controller->fire();
	}
	if (package->controls->isKeyReleased(VK_ESCAPE))
	{
		package->stateMachine->requestPop();
	}
}