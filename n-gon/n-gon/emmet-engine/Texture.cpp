#include "Texture.h"

Texture::Texture()
{
	setType(RESOURCE_TYPE_TEXTURE);
	textureIndex = 0;
	setCommited(false);
}

Texture::~Texture()
{
	delete[] textureData;
}

void Texture::setTexture(unsigned int textureIndexIn, char * data, char * nameIn)
{
	textureIndex = textureIndexIn;
	textureData = data;
	name = nameIn;
	setCommited(false);
}

char * Texture::getTextureData()
{
	return textureData;
}

unsigned int Texture::getTextureIndex()
{
	return textureIndex;
}