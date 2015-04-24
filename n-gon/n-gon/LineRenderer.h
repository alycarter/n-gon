#pragma once
#include "emmet-engine\Component.h"

#include <vector>
using namespace std;
#include <DirectXMath.h>
using namespace DirectX;

struct LineInstance
{
	XMFLOAT3 p1;
	XMFLOAT3 p2;
	XMFLOAT3 n1;
	XMFLOAT3 n2;
	XMFLOAT4 color;
	XMFLOAT4X4 model;
};

class LineRenderer :
	public Component
{
public:
	LineRenderer(GraphicsManager * manager);
	~LineRenderer();
	void render(GraphicsManager * graphics, GraphicsCapture * capture);
	void addLine(LineInstance line);
	void clear();
protected:
	void calculateLineNormal(XMVECTOR * normal, XMFLOAT3 * p1, XMFLOAT3 * p2);
	vector<LineInstance> lines;
	float width;
private:
	unsigned int mesh;
	unsigned int shader;
};

