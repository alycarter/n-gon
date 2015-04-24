#include "RenderPass.h"
#include "GraphicsCapture.h"
#include <cstring>

RenderPass::RenderPass(char * nameIn)
{
	name = nameIn;
	constantData = NULL;
	constantDataSize = 0;
	viewProjection = (XMMATRIX*)_aligned_malloc(sizeof(XMMATRIX), 16);
}

RenderPass::~RenderPass()
{

}

XMMATRIX * RenderPass::getViewProjection()
{
	return viewProjection;
}

bool RenderPass::clippingContains(BoundingSphere * sphere)
{
	return clipping.Intersects(*sphere);
}

void RenderPass::setViewProjection(XMMATRIX * projection, XMMATRIX * view, XMVECTOR * translation, XMVECTOR * rotation)
{
	clipping = BoundingFrustum(*projection);
	XMStoreFloat4(&clipping.Orientation, *rotation);
	XMStoreFloat3(&clipping.Origin, *translation);
	*viewProjection = *view * *projection;
}

void RenderPass::setConstantData(void * data, unsigned int size)
{
	if (constantData != NULL)
	{
		delete[] constantData;
	}
	constantDataSize = size;
	constantData = new char[size];
	memcpy(constantData, data, size);
}

void * RenderPass::getConstantData()
{
	return constantData;
}

unsigned int RenderPass::getConstantDataSize()
{
	return constantDataSize;
}

void RenderPass::addInstance(InstanceGroup * instance, unsigned int shaderID, unsigned int meshID)
{
	if (batchGrid.size() <= shaderID)
	{
		batchGrid.resize(shaderID + 1);
	}
	if (batchGrid.at(shaderID).size() <= meshID)
	{
		batchGrid.at(shaderID).resize(meshID + 1);
	}
	vector<InstanceGroup*> * batch = &batchGrid.at(shaderID).at(meshID);
	if (batch->empty())
	{
		Batch * newBatch = new Batch();
		newBatch->shaderID = shaderID;
		newBatch->meshID = meshID;
		batches.push_back(newBatch);
	}
	batchGrid.at(shaderID).at(meshID).push_back(instance);
}

void RenderPass::addInstance(void * instanceData, unsigned int instanceDataStride, unsigned int instanceCount, unsigned int shaderID, unsigned int meshID)
{
	InstanceGroup * instance = new InstanceGroup();
	instance->setInstance(instanceCount, instanceDataStride, instanceData);
	addInstance(instance, shaderID, meshID);
}

vector<Batch*> * RenderPass::getBatches()
{
	for (unsigned int i = 0; i < batches.size(); i++)
	{
		Batch * batch = batches.at(i);
		batch->instance->setToInstanceGroups(&batchGrid.at(batch->shaderID).at(batch->meshID));
	}
	return &batches;
}

void RenderPass::clear()
{
	for (unsigned int b = 0; b < batches.size(); b++)
	{
		Batch * batch = batches.at(b);
		vector<InstanceGroup*> * group = &batchGrid.at(batch->shaderID).at(batch->meshID);
		for (unsigned int i = 0; i < group->size(); i++)
		{
			delete group->at(i);
		}
		group->clear();
		delete batch;
	}
	batches.clear();
}