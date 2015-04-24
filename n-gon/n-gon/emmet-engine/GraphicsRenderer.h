#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

class GraphicsCapture;
class GraphicsManager;

class GraphicsRenderer
{
public:
	GraphicsRenderer();
	~GraphicsRenderer();
	void drawGraphicsCaptures(vector<GraphicsCapture *> * captures, GraphicsManager * manager);
private:
	void drawGraphicsCapture(GraphicsCapture * capture, GraphicsManager * manager);
};

