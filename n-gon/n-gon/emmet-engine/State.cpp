#include "State.h"
#include "UpdatePackage.h"
#include "GraphicsRenderer.h"
#include "Entity.h"
#include "GraphicsCapture.h"
#include "RenderPass.h"

State::State()
{
	frontCaptureBuffer = new GraphicsCapture();
	backCaptureBuffer = new GraphicsCapture();
	occluding = true;
}

State::~State()
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		delete entities.at(i);
	}
	entities.clear();
	delete frontCaptureBuffer;
	delete backCaptureBuffer;
}

void State::update(UpdatePackage * package)
{
	package->state = this;
	//update all entities
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entities.at(i)->update(package);
	}
	//delete any entities that need deleting
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities.at(i)->isDeleteRequested())
		{
			delete entities.at(i);
			entities.erase(entities.begin() + i);
			i--;
		}
	}
}

void State::render(GraphicsManager * graphics)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entities.at(i)->render(graphics, frontCaptureBuffer);
	}	
}

void State::addEntity(Entity * entity)
{
	entities.push_back(entity);
}

GraphicsCapture * State::getFrontCaptureBuffer()
{
	return frontCaptureBuffer;
}

void State::addRenderPass(char * name)
{
	frontCaptureBuffer->getRenderPasses()->push_back(new RenderPass(name));
}

void State::swapCaptureBuffers()
{
	//add any new render passes
	for (unsigned int i = backCaptureBuffer->getRenderPasses()->size(); i < frontCaptureBuffer->getRenderPasses()->size(); i++)
	{
		RenderPass * pass = new RenderPass(frontCaptureBuffer->getRenderPasses()->at(i)->getName());
		backCaptureBuffer->getRenderPasses()->push_back(pass);
	}
	for (unsigned int i = 0; i < frontCaptureBuffer->getRenderPasses()->size(); i++)
	{
		RenderPass * frontPass = frontCaptureBuffer->getRenderPasses()->at(i);
		RenderPass * backPass = backCaptureBuffer->getRenderPasses()->at(i);
		backPass->setConstantData(frontPass->getConstantData(), frontPass->getConstantDataSize());
	}
	//swap the capture buffers
	GraphicsCapture * temp = backCaptureBuffer;
	backCaptureBuffer = frontCaptureBuffer;
	frontCaptureBuffer = temp;
}

Entity * State::getContainerEntity(Component * component)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities.at(i)->isContainerOfComponent(component))
		{
			return entities.at(i);
		}
	}
	return NULL;
}

bool State::isOccluding()
{
	return occluding;
}

void State::setOccluding(bool occlude)
{
	occluding = occlude;
}