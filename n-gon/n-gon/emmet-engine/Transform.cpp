#include "Transform.h"

Transform::Transform()
{
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT4(0, 0, 0, 1);
	scale = XMFLOAT3(1, 1, 1);
}


Transform::~Transform()
{

}

XMVECTOR Transform::getPosition()
{
	return XMLoadFloat3(&position);
}

void Transform::setPosition(XMVECTOR * positionIn)
{
	XMStoreFloat3(&position, *positionIn);
}

XMVECTOR Transform::getRotation()
{
	return XMLoadFloat4(&rotation);
}

void Transform::setRotation(XMVECTOR * rotationIn)
{
	XMStoreFloat4(&rotation, *rotationIn);
}

XMVECTOR Transform::getScale()
{
	return XMLoadFloat3(&scale);
}

void Transform::setScale(XMVECTOR * scaleIn)
{
	XMStoreFloat3(&scale, *scaleIn);
}

XMVECTOR Transform::getForward()
{
	return XMVector3Transform(XMVectorSet(0, 0, 1, 0), getRotationMatrix());
}

XMVECTOR Transform::getUp()
{
	return XMVector3Transform(XMVectorSet(0, 1, 0, 0), getRotationMatrix());
}

XMVECTOR Transform::getLeft()
{
	return XMVector3Transform(XMVectorSet(1, 0, 0, 0), getRotationMatrix());
}

void Transform::getRotationMatrix(XMMATRIX * matrix)
{
	*matrix = XMMatrixRotationQuaternion(getRotation());
}

XMMATRIX Transform::getRotationMatrix()
{
	return XMMatrixRotationQuaternion(getRotation());
}

void Transform::getTransformationMatrix(XMMATRIX * matrix)
{
	*matrix = XMMatrixAffineTransformation(getScale(), XMVectorSet(0, 0, 0, 1), getRotation(), getPosition());
}

XMMATRIX Transform::getTransformationMatrix()
{
	return XMMatrixAffineTransformation(getScale(), XMVectorSet(0, 0, 0, 1), getRotation(), getPosition());
}