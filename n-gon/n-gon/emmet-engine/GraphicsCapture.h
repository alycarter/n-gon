#pragma once
#include <vector>
using namespace std;
#include <DirectXMath.h>
using namespace DirectX;

class RenderPass;

class GraphicsCapture
{
public:
	GraphicsCapture(vector<char *> * renderPassIDs);
	GraphicsCapture();
	~GraphicsCapture();
	RenderPass * getRenderPass(char * name);
	vector<RenderPass *> * getRenderPasses();
	void setClearColor(float r, float g, float b, float a);//sets the background colour on clear
	XMFLOAT4 * getClearColor();
private:
	XMFLOAT4 clearColor;//color to clear the screen to
	void addRenderPass(char * name);
	vector<RenderPass *> renderPasses;
};

