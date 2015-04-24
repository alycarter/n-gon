/*
ObjLoader.cpp
author: Alexander Carter 
date: 06/11/2014
implementation file for loading .obj files into a format usable by OpenGL and DirectX
specifies vertex atributes in order of position coord - normal coord - texture coord

feel free to use/modify this for anything you want but please do not remove this comment block
*/

#include "ObjLoader.h"
#include <iostream>
#include <fstream>

#define VERTEX_ELEMENTS 8
#define POSITION_ELEMENTS 3
#define NORMAL_ELEMENTS 3
#define TEXTURE_ELEMENTS 2

ObjLoader::ObjLoader(void)
{
}


ObjLoader::~ObjLoader(void)
{
}

void ObjLoader::loadObj(char * fileName, vector<float> * vertexData, vector<unsigned int> * indexData, bool useIndexed)
{
	//clear any data in the output vectors
	vertexData->clear();
	indexData->clear();
	//load the file into loaded
	loadFile(fileName);
	// value that will store our file input
	char * val = loaded;
	//vectors to store the data read from file
	vector<float> vertexIn = vector<float>(); 
	vector<float> textureIn = vector<float>();
	vector<float> normalIn = vector<float>();
	vector<vector<Index *>*> * indexIn = new vector<vector<Index *>*>();
	//reset our index count
	nextIndex = 0;
	// while we arnt at the end of the file
	while(position < size && val != NULL) 
	{
		// read in a new value from the file
		val = readValue(); 
		//if its a vertex coord
		if(!strcmp(val, "v")) 
		{
			val = readAttributes(&vertexIn, POSITION_ELEMENTS, val, "v");
		}
		// if its a texture coord
		if(!strcmp(val, "vt")) 
		{
			val = readAttributes(&textureIn, TEXTURE_ELEMENTS, val, "vt");
		}
		// if its a normal
		if(!strcmp(val, "vn"))
		{
			val = readAttributes(&normalIn, NORMAL_ELEMENTS, val, "vn");
		}
		// if its a face
		if(!strcmp(val, "f")) 
		{
			//if we are indexing and havnt handled faces yet (3ds max causes this to be ran multiple times)
			if(indexIn->size() == 0 && useIndexed)
			{
				//st your first dimention to fin all our vertex indexes
				indexIn->resize(vertexIn.size()/POSITION_ELEMENTS, NULL);
			}
			//read in faces(in 3ds max this just loads one)
			val = readFaces(&vertexIn, &textureIn, &normalIn, indexIn, vertexData, indexData, val, useIndexed);
		}
	}
	//clear up memory
	delete indexIn;
	delete [] loaded;
}

void ObjLoader::loadFile(char * fileName)
{
	// open the file
	ifstream file(fileName, ios::in | ios::binary); 
	//move our position tracker to the start
	position = 0;
	//move to the end of the file
	file.seekg(0, ios::end);
	//set size to the amount of bytes we just skipped
	size = (unsigned int)file.tellg();
	//set our char array to the size of the array
	loaded = new char[size];
	//move to the begining of the file
	file.seekg(0,ios::beg);
	//read in the file into the char array
	file.read(loaded, size);
	//release the file
	file.close();	
}

char * ObjLoader::readValue()
{
	char * start = NULL;
	char read = 0;
	bool end = false;
	//while we havnt marked our place or havnt ended reading and we havnt reached the end of the file
	while(start == NULL || !end && position < size)
	{
		//read the next char
		read = loaded[position]; 
		// if its a character char
		if(read != ' ' && read != '\n' && read != '/'){
			//mark our start position
			if(start == NULL)
			{
				start = loaded + position;
			}
		}else
		{
			//if weve started reading character data
			if(start != NULL)
			{
				//stop reading characters and mark our place with a null termination char to turn it into a c string
				end = true;
				loaded[position] = '\0';
			}
		}
		//move on in the file
		position++;
	}
	//return the start of the c string
	return start;
}

Index * ObjLoader::sortIndex(Index * index, vector<vector<Index*>*> * indicies)
{
	//jump to the vector that we need to look through
	vector<Index*> * targetVector = indicies->at(index->vertex);
	//if its not been set to anything yet
	if(targetVector == NULL)
	{
		//our index is unique because nothing has the same vertex index
		index->index = getNextIndex();
		//set the null vector to a vector with our index in  
		targetVector = new vector<Index*>();
		indicies->at(index->vertex) = targetVector;
		targetVector->push_back(index);
		//return our index because its unique
		return index;
	}else
	{
		//loop through the vector looking for an index the same as ours
		for(unsigned int i = 0; i < targetVector->size(); i++)
		{
			Index * targetIndex = targetVector->at(i); 
			//if the target index is equal to ours
			if(targetIndex->texture == index->texture && targetIndex->normal == index->normal)
			{
				//set our index to its index
				index->index = targetIndex->index;
				//return the unique index that we found because our new index isnt unique
				return targetIndex;
			}
		}
		targetVector->push_back(index);
		index->index = getNextIndex();
		//return our index because its unique
		return index;
	}
}

unsigned int ObjLoader::getNextIndex()
{
	//move to the next index
	nextIndex++;
	//return the index we were on before
	return nextIndex - 1;
}

char * ObjLoader::readAttributes(vector<float> * attributeIn, unsigned int dimensions, char * value, char * match)
{
	//while we are reading our attribute type
	while(!strcmp(value, match))
	{
		// read in the attribute coordinates
		for(unsigned int i =0; i < dimensions; i++) 
		{
			value = readValue();
			//add them to our attibute list
			attributeIn->push_back((float)atof(value));
		}
		//read the next word
		value = readValue();
	}
	//return the value that broke the loop
	return value;
} 

char * ObjLoader::readFaces(vector<float> * vertexIn, vector<float> * textureIn, vector<float> * normalIn, vector<vector<Index *>*> * indexIn, vector<float> * vertexData, vector<unsigned int> * indexData, char * value, bool useIndexed)
{
	while(!strcmp(value, "f")){
		// read in the 3 vertex indexs
		for(int i =0; i < 3; i++) 
		{
			value = readValue();
			// get the vertex index
			int vertex = atoi(value)-1; 
			value = readValue();
			// get the texture index
			int texture = atoi(value)-1; 
			value = readValue();
			// get the normal index
			int normal = atoi(value)-1; 
			// create an index structure
			Index * seperateindex = new Index(vertex, texture,normal);
			// set the indexes index value and sort in into our index list if needed
			Index * uniqueIndex = seperateindex;
			if(useIndexed){
				uniqueIndex = sortIndex(seperateindex, indexIn);
			}else
			{
				seperateindex->index = getNextIndex();
			}
			//add the index value into our index array
			indexData->push_back(seperateindex->index);
			//if it was a unique index
			if(seperateindex == uniqueIndex)
			{
				//get the position of the vertex attributes
				unsigned int vert = seperateindex->vertex * POSITION_ELEMENTS;
				unsigned int norm = seperateindex->normal * NORMAL_ELEMENTS;
				unsigned int text = seperateindex->texture * TEXTURE_ELEMENTS;
				//add the attributes in
				for(unsigned int i = 0; i < POSITION_ELEMENTS; i++)
				{
					vertexData->push_back(vertexIn->at(vert+i));
				}
				for(unsigned int i = 0; i < NORMAL_ELEMENTS; i++)
				{
					vertexData->push_back(normalIn->at(norm+i));
				}
				for(unsigned int i = 0; i < TEXTURE_ELEMENTS; i++)
				{
					vertexData->push_back(textureIn->at(text+i));
				}
			}else
			{
				//delete out index because it wasnt added to the list
				delete seperateindex;
			}				
		}
		//read the next value
		value = readValue();
	}
	//return the value that causes the loop to stop
	return value;
}