#include "SceneCamera.h"
#include "emmet-engine\RenderPass.h"
#include "emmet-engine\State.h"
#include "emmet-engine\GraphicsCapture.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\GraphicsManager.h"


SceneCamera::SceneCamera()
{
}


SceneCamera::~SceneCamera()
{
}

void SceneCamera::update(UpdatePackage * package)
{
	RenderPass * pass = package->state->getFrontCaptureBuffer()->getRenderPass(PASS_SCENE);
	pass->setViewProjection(&XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, -100, 100), &XMMatrixIdentity(), &XMVectorZero(), &XMQuaternionIdentity());
	pass->setConstantData(&XMMatrixOrthographicOffCenterLH(0, package->graphics->getScreenResolution()->x, package->graphics->getScreenResolution()->y, 0, -100, 100), sizeof(XMMATRIX));
}
