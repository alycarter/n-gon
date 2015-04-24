#include "Object.h"


Object::Object()
{
	//set the name to a default value
	name = "default object name";
}

Object::~Object()
{

}

//by default make toString return the name
char * Object::toString()
{
	return getName();
}

//returns the name of the object
char * Object::getName()
{
	return name;
}