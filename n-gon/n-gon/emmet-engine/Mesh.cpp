#include "Mesh.h"


Mesh::Mesh()
{
	setType(RESOURCE_TYPE_MESH);
	vertexOffset = 0;
	indexOffset = 0;
	boundingSphere = BoundingSphere(XMFLOAT3(0, 0, 0), 0);
	setCommited(false);
}

Mesh::~Mesh()
{

}

unsigned int Mesh::getVertexCount()
{
	return verticies.size();
}

unsigned int Mesh::getVertexOffset()
{
	return vertexOffset;
}

unsigned int Mesh::getIndexCount()
{
	return indicies.size();
}

unsigned int Mesh::getIndexOffset()
{
	return indexOffset;
}

void Mesh::setName(char * nameIn)
{
	name = nameIn;
}

void Mesh::setVertexOffset(unsigned int vertexOffsetIn)
{
	vertexOffset = vertexOffsetIn;
}

void Mesh::setIndexOffset(unsigned int indexOffsetIn)
{
	indexOffset = indexOffsetIn;
}

vector<float> * Mesh::getVerticies()
{
	return &verticies;
}

vector<unsigned int> * Mesh::getIndicies()
{
	return &indicies;
}

void Mesh::calculateBoundingSphere()
{
	XMVECTOR center = XMVectorSet(0,0,0,0);
	unsigned int i = 0;
	for (i = 0; i < verticies.size() / 8; i++)
	{
		center += XMVectorSet(verticies.at((i * 8) + 0), verticies.at((i * 8) + 1), verticies.at((i * 8) + 2), 0);
	}
	center /= (float)i;
	float max = 0;
	for (i = 0; i < verticies.size() / 8; i++)
	{
		XMVECTOR pos = XMVectorSet(verticies.at((i * 8) + 0), verticies.at((i * 8) + 1), verticies.at((i * 8) + 2), 0);
		float dist = XMVectorGetX(XMVector3Length(pos - center));
		if (dist > max)
		{
			max = dist;
		}
	}
	XMFLOAT3 cen;
	XMStoreFloat3(&cen, center);
	boundingSphere = BoundingSphere(cen, max);
}

void Mesh::getBoundingSphere(BoundingSphere * sphere)
{
	*sphere = boundingSphere;
}