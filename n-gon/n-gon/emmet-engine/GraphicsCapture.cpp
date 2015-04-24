#include "GraphicsCapture.h"
#include "RenderPass.h"


GraphicsCapture::GraphicsCapture(vector<char *> * renderPassIDs)
{
	for (unsigned int i = 0; i < renderPassIDs->size(); i++)
	{
		addRenderPass(renderPassIDs->at(i));
	}
}

GraphicsCapture::GraphicsCapture()
{
}

GraphicsCapture::~GraphicsCapture()
{
}

void GraphicsCapture::addRenderPass(char * name)
{
	RenderPass * pass = new RenderPass(name);
	renderPasses.push_back(pass);
}

RenderPass * GraphicsCapture::getRenderPass(char * name)
{
	for (unsigned int i = 0; i < renderPasses.size(); i++)
	{
		if (strcmp(renderPasses.at(i)->getName(), name) == 0)
		{
			return renderPasses.at(i);
		}
	}
	printf(name);
	printf(" is not a render pass \n");
	return NULL;
}

vector<RenderPass *> * GraphicsCapture::getRenderPasses()
{
	return &renderPasses;
}

XMFLOAT4 * GraphicsCapture::getClearColor()
{
	return &clearColor;
}

void GraphicsCapture::setClearColor(float r, float g, float b, float a)
{
	//set the clear colour channels
	clearColor.x = r;
	clearColor.y = g;
	clearColor.z = b;
	clearColor.w = a;
}