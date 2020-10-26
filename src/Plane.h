#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"

class Plane final : public Sprite
{
public:
	Plane();
	~Plane();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	int getRise() { return rise; }
	int getRun() { return run; }

	int getXPos() { return xPosition; }
	int getYPos() { return yPosition; }

	int rise = 150;
	int run = 350;
	int xPosition = 100;
	int yPosition = 300;

	
private:
	
	
	

};

#endif /* defined (__PLANE__) */
