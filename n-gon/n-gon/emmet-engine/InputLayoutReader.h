#pragma once
#include <stack>
#include <vector>
using namespace std;

struct D3D11_INPUT_ELEMENT_DESC;

//class used for reading the input layout description from the vertex shader
class InputLayoutReader
{
public:
	/*sets up the reader for use */
	InputLayoutReader(void);
	~InputLayoutReader(void);
	/*will output information about the shader input 
	parameters: 
	char * fileName : c-string for the where to find the shader file
	vector<D3D11_INPUT_ELEMENT_DESC> * output : a vector to fill with the input decriptions*/
	void parseInputLayout(char * fileName, vector<D3D11_INPUT_ELEMENT_DESC> * output);
private:
	/*will clear any data left over form the last read*/
	void initialize();
	/*adds a char to the list of chars that shouldnt be added to words
	parameters: 
	char stop : char to add to the list*/
	void addStoppingChar(char stop);
	/*tests if the char is a stopping char
	parameters: 
	char test: the char to test if its a stopping char
	returns:
	true - the char shouldnt be added to words
	false - the char should be added to words*/
	bool isStoppingChar(char test);
	/*handles the word as a vertex shader input parameter
	parameters: 
	char * word : c-string of the word to process
	vector<D3D11_INPUT_ELEMENT_DESC> * output : a vector to fill with the input decriptions*/
	void handleWord(char * word, vector<D3D11_INPUT_ELEMENT_DESC> * output);
	/*handles the character thats been read from the file
	parameters: 
	char read : char to handle
	vector<D3D11_INPUT_ELEMENT_DESC> * output : a vector to fill with the input decriptions*/
	void handleCharacter(char read, vector<D3D11_INPUT_ELEMENT_DESC> * output);
	/*returns the input slot assigned to the parameter marker
	parameters: 
	char marker : char to add as a marker and retieve the input slot for*/
	unsigned int getInputSlot(char marker);
	/*creates new input elements based on the data type
	parameters: 
	char * dataType : c-string  for the data type name*/
	void handleDataType(char * dataType);
	/*completes in progress inputs with the semantic name
	parameters: 
	char * semantic : c-string  for the semantic name
	vector<D3D11_INPUT_ELEMENT_DESC> * output : a vector to fill with the input decriptions*/
	void handleSemantic(char * semantic, vector<D3D11_INPUT_ELEMENT_DESC> * output);
	/*tests if the input with that maker should be alligned or not
	parameters: 
	char marker: the marker to test
	returns:
	true - the marker should be automaticly alligned
	false - the marker should be alligned to 0*/
	bool shouldAllignInput(char marker);
	//vector of chars that are notpart of a word
	vector<char> stoppingChars;
	//list of markers that have been read in and alligned to 0
	vector<char> inputSlotMarkers;
	//what part of the variable declaration we are on
	char declarationPosition;
	//word we are currently reading in
	vector<char> word;
	//if we have hit the input parameters
	bool readingInput;
	//if we have finished reading the input parameters
	bool finishedReadingInput;
	//stack to hold the incomplete input layouts before putting them into the output vector
	stack<D3D11_INPUT_ELEMENT_DESC> * currentInputs;
};

