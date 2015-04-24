#pragma once
#include "GraphicsResource.h"
#include <vector>
using namespace std;
#include <DirectXCollision.h>
using namespace DirectX;

class Mesh :
	public GraphicsResource
{
public:
	Mesh();
	~Mesh();
	unsigned int getVertexCount();
	unsigned int getVertexOffset();
	unsigned int getIndexCount();
	unsigned int getIndexOffset();
	void setName(char * name);
	void setVertexOffset(unsigned int vertexOffsetIn);
	void setIndexOffset(unsigned int indexOffsetIn);
	vector<float> * getVerticies();
	vector<unsigned int> * getIndicies();
	void calculateBoundingSphere();
	void getBoundingSphere(BoundingSphere * sphere);
private:
	unsigned int vertexOffset;
	unsigned int indexOffset;
	vector<float> verticies;
	vector<unsigned int > indicies;
	BoundingSphere boundingSphere;
};

