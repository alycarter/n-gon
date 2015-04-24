/*
ObjLoader.h
author: Alexander Carter 
date: 06/11/2014
interface file for loading .obj files into a format usable by OpenGL and DirectX
specifies vertex atributes in order of position coord - normal coord - texture coord

feel free to use/modify this for anything you want but please do not remove this comment block
*/
#pragma once
#include <vector>
using namespace std;

struct Index;

class ObjLoader
{
public:
	ObjLoader(void);
	~ObjLoader(void);
	/*	loads a model from a .obj file at the file path specified.
		perameters:
		fileName - c string that specifies the file path to the .obj file
		vertexData - pointer to a vector that will be filled with the vertex data (3 position floats, 3 normal floats, 2 texture coord floats)
		indexData - pointer to a vector that will be filled with the index data
		useIndexed - boolean that says weither to optimize the index data. if false the mesh is unindexed but will still fill indexData with indexes. true will take longer to create. 
	*/
	void loadObj(char * fileName, vector<float> * vertexData, vector<unsigned int> * indexData, bool useIndexed);
private:
	void loadFile(char * fileName);
	char* readValue();
	char * readAttributes(vector<float> * attributeIn, unsigned int dimensions, char * value, char * match);
	char * readFaces(vector<float> * vertexIn, vector<float> * textureIn, vector<float> * normalIn, vector<vector<Index *>*> * indexIn, vector<float> * vertexData, vector<unsigned int> * indexData, char * value, bool useIndexed);
	Index * sortIndex(Index * index, vector<vector<Index*>*> * indicies);
	unsigned int getNextIndex();
	char * loaded;
	unsigned long position;
	unsigned long size;
	unsigned int nextIndex;
};

// structure for storing the seperate index lists from the obj
struct Index 
{
	//stored seperate indexes
	unsigned int vertex;
	unsigned int texture;
	unsigned int normal;
	unsigned int index;
	
	Index(int v, int t, int n) 
	{
		vertex = v;
		texture = t;
		normal = n;
		index = 0;
	}
};