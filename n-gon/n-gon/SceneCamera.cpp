#include "SceneCamera.h"
#include "emmet-engine\RenderPass.h"
#include "emmet-engine\State.h"
#include "emmet-engine\GraphicsCapture.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\GraphicsManager.h"


SceneCamera::SceneCamera(float viewWidthIn, float viewHeightIn)
{
	viewHeight = viewHeightIn;
	viewWidth = viewWidthIn;
}


SceneCamera::~SceneCamera()
{
}

void SceneCamera::update(UpdatePackage * package)
{
	//update the render pass
	RenderPass * pass = package->state->getFrontCaptureBuffer()->getRenderPass(PASS_SCENE);
	pass->setViewProjection(&XMMatrixOrthographicOffCenterLH(0, viewWidth, viewHeight, 0, -100, 100), &XMMatrixIdentity(), &XMVectorZero(), &XMQuaternionIdentity());
	pass->setConstantData(&XMMatrixOrthographicOffCenterLH(0, viewWidth, viewHeight, 0, -100, 100), sizeof(XMMATRIX));
}

float SceneCamera::getViewWidth()
{
	return viewWidth;
}

float SceneCamera::getViewHeight()
{
	return viewHeight;
}