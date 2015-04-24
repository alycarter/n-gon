#include "InputLayoutReader.h"
#include <fstream>
#include <iostream>
#include <D3D11.h>

#define VSHADER "VShader"

#define UNSIGNED_INT_TYPE "uint"
#define INT_TYPE "int"
#define FLOAT_TYPE "float"
#define FLOAT2_TYPE "float2"
#define FLOAT3_TYPE "float3"
#define FLOAT4_TYPE "float4"
#define MATRIX_TYPE "matrix"

InputLayoutReader::InputLayoutReader(void)
{
	//add the characters that are not part of relevent words (only up to the bit that we care about in the shader)
	addStoppingChar(' ');
	addStoppingChar(',');
	addStoppingChar(';');
	addStoppingChar('(');
	addStoppingChar('\n');
	addStoppingChar('\t');
	addStoppingChar(':');
	addStoppingChar(')');
	addStoppingChar('{');
	addStoppingChar('}');
	currentInputs = new stack<D3D11_INPUT_ELEMENT_DESC>();
}

InputLayoutReader::~InputLayoutReader(void)
{
	delete currentInputs;
}

void InputLayoutReader::parseInputLayout(char * fileName, vector<D3D11_INPUT_ELEMENT_DESC> * output)
{
	//open the shader file
	ifstream file(fileName, ios::in | ios::binary);
	//char to read data into
	char read = 0;
	//clear any left over stuff from the last read
	initialize();
	//while we havnt reached the end of the file or until we finished reading relevant data
	while(!file.eof() && !finishedReadingInput)
	{
		//read the next character
		file.read(&read, 1);
		//handle the char we read
		handleCharacter(read, output);
	}
	//close the file again
	file.close();
}

void InputLayoutReader::initialize()
{
	//clear any left over input slot markers
	inputSlotMarkers.clear();
	//clear anything left in the word
	word.clear();
	//we arnt reading relivant data yet
	readingInput = false;
	//we havnt finished reading relivant data
	finishedReadingInput = false;
	//we are at the first part of the variable declaration
	declarationPosition = 0;
	//remove any left over inputs
	while(!currentInputs->empty())
	{
		currentInputs->pop();
	}
}

void InputLayoutReader::handleCharacter(char read, vector<D3D11_INPUT_ELEMENT_DESC> * output)
{
	//if the char is part of a word
	if(!isStoppingChar(read))
	{
		//add the char to the current word we are building
		word.push_back(read);
	}else
	{
		//if its not part of a word and we were building a word
		if(word.size() > 0)
		{
			//add the null termination char
			word.push_back('\0');
			//if we are reading relivant data
			if(readingInput)
			{
				//handle the word as input data
				handleWord(&word.at(0), output);
			}
			//if we read the vertex shader main function identifier
			if(!strcmp(&word.at(0),VSHADER))
			{
				//we will now start reading relivant data to the input layout 
				readingInput = true;
			}
			//clear the word
			word.clear();
		}
		//if we are reading relavant input data and we found the end of the input parameters 
		if(readingInput && read == ')')
		{
			//we finished reading the data we need for the input layout
			finishedReadingInput = true;	
		}
	}
}
	

void InputLayoutReader::handleWord(char * word, vector<D3D11_INPUT_ELEMENT_DESC> * output)
{
	//handle it differently depending on what part of the variable declaration we are on
	switch(declarationPosition)
	{
	//if its the data type
	case 0:
		//create any input layouts from the data type
		handleDataType(word);
		//move onto the next part (variable name)
		declarationPosition++;
		break;
	case 1:
		//we dont care about the variable name so move onto the semantic
		declarationPosition++;
		break;
	case 2:
		handleSemantic(word, output);
		//we are done with this variable now
		declarationPosition = 0;
		break;
	default:
		//this should never run
		cout << "invalid declaration position" <<endl;
		//move back to the start of a variable
		declarationPosition = 0;
		break;
	}
}

void InputLayoutReader::handleDataType(char * dataType)
{
	//horrible make shift switch statement
	//create a new input element and set the format for each data type
	if(!strcmp(&word.at(0),INT_TYPE))
	{
		currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
		currentInputs->top().Format = DXGI_FORMAT_R32_SINT;
		return;
	}
	if(!strcmp(&word.at(0),UNSIGNED_INT_TYPE))
	{
		currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
		currentInputs->top().Format = DXGI_FORMAT_R32_UINT;
		return;
	}
	if(!strcmp(&word.at(0),FLOAT_TYPE))
	{
		currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
		currentInputs->top().Format = DXGI_FORMAT_R32_FLOAT;
		return;
	}
	if(!strcmp(&word.at(0),FLOAT2_TYPE))
	{
		currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
		currentInputs->top().Format = DXGI_FORMAT_R32G32_FLOAT;
		return;
	}
	if(!strcmp(&word.at(0),FLOAT3_TYPE))
	{
		currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
		currentInputs->top().Format = DXGI_FORMAT_R32G32B32_FLOAT;
		return;
	}
	if(!strcmp(&word.at(0),FLOAT4_TYPE))
	{
		currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
		currentInputs->top().Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		return;
	}
	//matrix is a 4x4 matrix so we need to create 4 float4s
	if(!strcmp(&word.at(0),MATRIX_TYPE))
	{
		for(int i = 0; i < 4; i++)
		{
			currentInputs->push(D3D11_INPUT_ELEMENT_DESC());
			currentInputs->top().Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		return;
	}
	//if it reaches here i missed out a data type and should add a new if statement for it
	cout << "missing conversion for data type : " << dataType << endl;
}

void InputLayoutReader::handleSemantic(char * semantic, vector<D3D11_INPUT_ELEMENT_DESC> * output)
{
	//start at semantic index 0
	unsigned int semanticIndex = 0;
	//copy the semantic string to a new string
	unsigned int length = strlen(semantic) + 1;
	char * semanticOut = new char[length];
	strcpy_s(semanticOut, length, semantic);
	//while there are still inputs to compleate in the stack
	while(!currentInputs->empty())
	{
		//get the description from the stack
		D3D11_INPUT_ELEMENT_DESC desc = currentInputs->top();
		currentInputs->pop();
		//set the byte allignment
		if(shouldAllignInput(semantic[0]))
		{
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		}else
		{
			desc.AlignedByteOffset = 0;
		}
		//set the index to the number of loops we've compleated
		desc.SemanticIndex = semanticIndex;
		//set the name to the copy we made
		desc.SemanticName = semanticOut;
		//set the input slot the slot assigned to tmarker
		desc.InputSlot = getInputSlot(semantic[0]);
		//i and I will be used for markers for per instance data (possibly be able to add other markers to a vector later)
		if(semantic[0] == 'i' || semantic[0] == 'I')
		{
			desc.InstanceDataStepRate = 1;
			desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		}else
		{
			desc.InstanceDataStepRate = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		}
		//add to the output layouts
		output->push_back(desc);
		//move on to the next semantic index
		semanticIndex++;
	}
}

void InputLayoutReader::addStoppingChar(char stop)
{
	//adds the stopping char to the list (sligntly redundant function)
	stoppingChars.push_back(stop);
}

bool InputLayoutReader::isStoppingChar(char test)
{
	//for each stopping char in the vector
	for(unsigned int i = 0; i < stoppingChars.size(); i++)
	{
		//if it matches our test char
		if(test == stoppingChars.at(i))
		{
			//we need to stop reading the word
			return true;
		}
	}
	//its a valid char to add to a word
	return false;
}

unsigned int InputLayoutReader::getInputSlot(char marker)
{
	//for each marker in the input slot vector
	for(unsigned int i = 0; i < inputSlotMarkers.size(); i++)
	{
		//if the marker maches the marker in the vector
		if(inputSlotMarkers.at(i) == marker)
		{
			//return the input slot ID
			return i;
		}
	}
	//it wasnt in the vector already so add it to the input slot markers
	inputSlotMarkers.push_back(marker);
	//return the input slot ID
	return inputSlotMarkers.size() - 1;
}

bool InputLayoutReader::shouldAllignInput(char marker)
{
	//input maker for the last marker added
	char lastInputSlot = 0;
	//if weve added a marker already
	if(inputSlotMarkers.size() > 0){
		//get the last marker
		lastInputSlot = inputSlotMarkers.at(inputSlotMarkers.size() - 1);
	}
	//return if the last marker we read is the same as the new one
	return lastInputSlot == marker;
}