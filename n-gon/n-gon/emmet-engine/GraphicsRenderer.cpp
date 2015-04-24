#include "GraphicsRenderer.h"
#include "GraphicsManager.h"
#include "RenderPass.h"
#include "GraphicsCapture.h"

GraphicsRenderer::GraphicsRenderer()
{
}


GraphicsRenderer::~GraphicsRenderer()
{
}


void GraphicsRenderer::drawGraphicsCaptures(vector<GraphicsCapture *> * captures, GraphicsManager * manager)
{
	if (!captures->empty())
	{
		manager->setClearColor(captures->at(captures->size() - 1)->getClearColor());
	}
	manager->clearRenderBuffer();
	for (int i = captures->size() - 1; i >= 0; i--)
	{
		drawGraphicsCapture(captures->at(i), manager);
	}
	manager->swapBuffers();
}

void GraphicsRenderer::drawGraphicsCapture(GraphicsCapture * capture, GraphicsManager * manager)
{
	for (unsigned int pass = 0; pass < capture->getRenderPasses()->size(); pass++)
	{
		RenderPass * renderPass = capture->getRenderPasses()->at(pass);
		manager->setConstantBufferData(renderPass->getConstantData(), renderPass->getConstantDataSize());
		vector<Batch*> * batches = renderPass->getBatches();
		for (unsigned int i = 0; i < batches->size(); i++)
		{
			Batch * batch = batches->at(i);
			manager->bindShader(batch->shaderID);
			manager->setInstanceBufferData(batch->instance->getInstanceData(), batch->instance->getInstanceStride() * batch->instance->getInstanceCount(), batch->instance->getInstanceStride());
			manager->drawMesh(batch->meshID, batch->instance->getInstanceCount());
		}
		renderPass->clear();
		manager->clearDepthBuffer();
	}
}
