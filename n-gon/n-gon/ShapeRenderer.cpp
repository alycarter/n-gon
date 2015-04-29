#include "ShapeRenderer.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\Entity.h"



ShapeRenderer::ShapeRenderer(GraphicsManager * manager) : LineRenderer(manager)
{
	
}

ShapeRenderer::~ShapeRenderer()
{
}


void ShapeRenderer::buildShape(int sides, float radius, float lineWidth, XMFLOAT4 * color)
{
	//remove any existing lines
	lines.clear();
	//set the line width
	width = lineWidth;
	//set the angle to step between each corner
	float change = XMConvertToRadians(360.0f / sides);
	//get the up world vector * the radius of the shape
	XMVECTOR up = XMVectorSet(0, radius, 0, 0);
	//for each side of the shape
	for (int i = 0; i < sides; i++)
	{
		//get a line representing the side starting at the current angle
		LineInstance instance;
		XMStoreFloat3(&instance.p1, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * i)));
		XMStoreFloat3(&instance.p2, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * (i+1))));
		XMVECTOR normal;
		//get the normal of our line
		calculateLineNormal(&normal, &instance.p1, &instance.p2);

		//get the corner before the current side
		XMVECTOR normalBefore;
		XMFLOAT3 prior;
		XMStoreFloat3(&prior, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * (i - 1))));
		//get the normal of the side before
		calculateLineNormal(&normalBefore, &prior, &instance.p1);
		//interpolate between the current and prevoius sides normal
		normalBefore = XMVectorLerp(normalBefore, normal, 0.5f);
		normalBefore = XMVector3Normalize(normalBefore);
		normalBefore = XMVector2Transform(normalBefore, XMMatrixScaling(width / 2.0f, width / 2.0f, width / 2.0f));

		//get the corner after our current side
		XMVECTOR normalAfter;
		XMFLOAT3 post;
		XMStoreFloat3(&post, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * (i + 2))));
		//get the normal of the next side
		calculateLineNormal(&normalAfter, &instance.p2, &post);
		//get the interpolated value between the current and last side
		normalAfter = XMVectorLerp(normal, normalAfter, 0.5f);
		normalAfter = XMVector3Normalize(normalAfter);
		normalAfter = XMVector2Transform(normalAfter, XMMatrixScaling(width / 2.0f, width / 2.0f, width / 2.0f));

		//store ther normals in the line segment
		XMStoreFloat3(&instance.n1, normalBefore);
		XMStoreFloat3(&instance.n2, normalAfter);
		instance.color = *color;
		//add the line
		lines.push_back(instance);
	}
}

void ShapeRenderer::update(UpdatePackage * package)
{
	//update the matrix of all the lines
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		XMStoreFloat4x4(&lines.at(i).model, package->entity->getComponentOfType<Transform>()->getTransformationMatrix());
	}
}
