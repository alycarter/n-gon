#include "Shader.h"


Shader::Shader()
{
	setType(RESOURCE_TYPE_SHADER);
}


Shader::~Shader()
{
}

ID3D11VertexShader * Shader::getVertexShader()
{
	return vertexShader;
}

ID3D11PixelShader * Shader::getPixelShader()
{
	return pixelShader;
}

ID3D11InputLayout * Shader::getInput()
{
	return layout;
}

void Shader::setShader(ID3D11VertexShader * vShader, ID3D11PixelShader * pShader, ID3D11InputLayout * inputLayout, char * nameIn)
{
	vertexShader = vShader;
	pixelShader = pShader;
	layout = inputLayout;
	name = nameIn;
	setCommited(true);
}