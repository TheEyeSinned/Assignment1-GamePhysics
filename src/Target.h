#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	bool isGravityEnabled = false;

	glm::vec2 throwSpeed;
	glm::vec2 throwPosition;
	

	void doThrow();

	void m_reset();

	float pixelPerMeter = 50;
private:
	void m_move();
	void m_checkBounds();
	
};


#endif /* defined (__TARGET__) */