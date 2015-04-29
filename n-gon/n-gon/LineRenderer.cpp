#include "LineRenderer.h"
#include "emmet-engine\GraphicsCapture.h"
#include "emmet-engine\GraphicsManager.h"
#include "emmet-engine\RenderPass.h"

LineRenderer::LineRenderer(GraphicsManager * manager)
{
	//set the shader and mesh values to the loaded resources 
	shader = manager->getGraphicsResourceID(RESOURCE_TYPE_SHADER, "line");
	mesh = manager->getGraphicsResourceID(RESOURCE_TYPE_MESH, "quad");
}


LineRenderer::~LineRenderer()
{
}

void LineRenderer::calculateLineNormal(XMVECTOR * normal, XMFLOAT3 * p1, XMFLOAT3 * p2)
{
	//get the vector between the points
	*normal = XMLoadFloat3(p1) - XMLoadFloat3(p2);
	//rotate it by 90 degrees
	*normal = XMVector3Transform(*normal, XMMatrixRotationRollPitchYaw(0, 0, XMConvertToRadians(-90.0f)));
	//normalize the vector
	*normal = XMVector3Normalize(*normal);
}

void LineRenderer::render(GraphicsManager * graphics, GraphicsCapture * capture)
{
	//add all of the line instances to the render pass if there is a line
	if (lines.size() > 0)
	{
		RenderPass * pass = capture->getRenderPass(PASS_SCENE);
		pass->addInstance(&lines.at(0), sizeof(LineInstance), lines.size(), shader, mesh);
	}
}

void LineRenderer::addLine(LineInstance line)
{
	lines.push_back(line);
}

void LineRenderer::clear()
{
	lines.clear();
}