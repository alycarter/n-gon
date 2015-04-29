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
}


PlayerController::~PlayerController()
{
}

void PlayerController::update(UpdatePackage * package)
{
	//get the ship controller
	ShipController * controller = package->entity->getComponentOfType<ShipController>();
	//if w is pressed then thrust
	if (package->controls->isKeyPressed('W'))
	{
		controller->thrust();
	}
	//get the direction the player wants to turn
	float direction = 0;
	if (package->controls->isKeyPressed('A'))
	{
		direction--;
	}
	if (package->controls->isKeyPressed('D'))
	{
		direction++;
	}
	//turn the ship
	controller->turn(direction);
	//if space is pressed then shoot
	if (package->controls->isKeyPressed(VK_SPACE))
	{
		controller->fire();
	}
}