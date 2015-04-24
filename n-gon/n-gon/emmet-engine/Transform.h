#pragma once
#include "Component.h"

#include <DirectXMath.h>
using namespace DirectX;

#define TYPE_TRANSFORM "transform"

class Transform:
	public Component
{
public:
	Transform();
	~Transform();
	XMVECTOR getPosition();
	void setPosition(XMVECTOR * positionIn);
	XMVECTOR getRotation();
	void setRotation(XMVECTOR * rotationIn);
	XMVECTOR getScale();
	void setScale(XMVECTOR * scaleIn);
	XMVECTOR getForward();
	XMVECTOR getUp();
	XMVECTOR getLeft();
	void getRotationMatrix(XMMATRIX * matrix);
	XMMATRIX getRotationMatrix();
	void getTransformationMatrix(XMMATRIX * matrix);
	XMMATRIX getTransformationMatrix();
private:
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	XMFLOAT3 scale;
};

