
class Time;
class State;
class Entity;
class GraphicsManager;
class Controls;
class StateMachine;

struct UpdatePackage
{
	Time * time;
	State * state;
	Entity * entity;
	GraphicsManager * graphics;
	Controls * controls;
	StateMachine * stateMachine;
};