#pragma once
#include "GraphicsResource.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

class Shader :
	public GraphicsResource
{
public:
	Shader();
	~Shader();
	ID3D11VertexShader * getVertexShader();
	ID3D11PixelShader * getPixelShader();
	ID3D11InputLayout * getInput();
	void setShader(ID3D11VertexShader * vShader, ID3D11PixelShader * pShader, ID3D11InputLayout * inputLayout, char * name);
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout * layout;

};

