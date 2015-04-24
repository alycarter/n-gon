#pragma once
#include "LineRenderer.h"

class ShapeRenderer :
	public LineRenderer
{
public:
	ShapeRenderer(GraphicsManager * manager);
	~ShapeRenderer();
	void update(UpdatePackage * package);
	void buildShape(int sides, float radius, float lineWidth, XMFLOAT4 * color);
};

