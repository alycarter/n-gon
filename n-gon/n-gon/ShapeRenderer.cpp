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
	lines.clear();
	width = lineWidth;
	float change = XMConvertToRadians(360.0f / sides);
	XMVECTOR up = XMVectorSet(0, radius, 0, 0);
	for (int i = 0; i < sides; i++)
	{
		LineInstance instance;
		XMStoreFloat3(&instance.p1, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * i)));
		XMStoreFloat3(&instance.p2, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * (i+1))));
		XMVECTOR normal;
		calculateLineNormal(&normal, &instance.p1, &instance.p2);

		XMVECTOR normalBefore;
		XMFLOAT3 prior;
		XMStoreFloat3(&prior, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * (i - 1))));

		calculateLineNormal(&normalBefore, &prior, &instance.p1);
		normalBefore = XMVectorLerp(normalBefore, normal, 0.5f);
		normalBefore = XMVector3Normalize(normalBefore);
		normalBefore = XMVector2Transform(normalBefore, XMMatrixScaling(width / 2.0f, width / 2.0f, width / 2.0f));

		XMVECTOR normalAfter;
		XMFLOAT3 post;
		XMStoreFloat3(&post, XMVector3Transform(up, XMMatrixRotationRollPitchYaw(0, 0, change * (i + 2))));

		calculateLineNormal(&normalAfter, &instance.p2, &post);
		normalAfter = XMVectorLerp(normal, normalAfter, 0.5f);
		normalAfter = XMVector3Normalize(normalAfter);
		normalAfter = XMVector2Transform(normalAfter, XMMatrixScaling(width / 2.0f, width / 2.0f, width / 2.0f));

		XMStoreFloat3(&instance.n1, normalBefore);
		XMStoreFloat3(&instance.n2, normalAfter);
		instance.color = *color;
		lines.push_back(instance);
	}
}

void ShapeRenderer::update(UpdatePackage * package)
{
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		XMStoreFloat4x4(&lines.at(i).model, package->entity->getComponentOfType<Transform>()->getTransformationMatrix());
	}
}
