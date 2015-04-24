#pragma once
#include "Object.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

#include "InstanceGroup.h"
#include <vector>
using namespace std;

struct Batch
{
	Batch()
	{
		shaderID = 0;
		meshID = 0;
		instance = new InstanceGroup();
	}
	~Batch()
	{
		delete instance;
	}
	unsigned int shaderID;
	unsigned int meshID;
	InstanceGroup * instance;
};

#define PASS_SCENE "scene"
#define PASS_UI "ui"

class GraphicsCapture;

class RenderPass :
	public Object
{
public:
	RenderPass(char * nameIn);
	~RenderPass();
	XMMATRIX * getViewProjection();
	void setViewProjection(XMMATRIX * projection, XMMATRIX * view, XMVECTOR * translation, XMVECTOR * rotation);
	bool clippingContains(BoundingSphere * sphere);
	void setConstantData(void * data, unsigned int size);
	void * getConstantData();
	unsigned int getConstantDataSize();
	void addInstance(InstanceGroup * instance, unsigned int shaderID, unsigned int meshID);
	void addInstance(void * instanceData, unsigned int instanceDataStride, unsigned int instanceCount, unsigned int shaderID, unsigned int meshID);
	vector<Batch*> * getBatches();
	void clear();
private:
	vector<Batch*> batches;
	vector<vector<vector<InstanceGroup*>>> batchGrid;
	void * constantData;
	unsigned int constantDataSize;
	XMMATRIX * viewProjection;
	BoundingFrustum clipping;
};

