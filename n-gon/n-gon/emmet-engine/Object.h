#pragma once

//core object everything will extend. only has a toString function and a name for identifiying
class Object
{
public:
	Object();
	~Object();
	//returns a string describing the object
	virtual char * toString();
	//returns the name of the object
	char * getName();
protected:
	//the name of the object
	char * name;
};

