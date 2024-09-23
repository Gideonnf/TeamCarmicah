#pragma once


class InputListener
{
public:
	InputListener() //ctor
	{

	}
	~InputListener() // dtor
	{

	}

	// not sure if this is necessary but i'll follow the tutorial and copy him
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;
};