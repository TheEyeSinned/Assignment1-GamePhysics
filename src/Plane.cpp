#include "Plane.h"
#include "TextureManager.h"
#include "Util.h"

Plane::Plane()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	getRigidBody()->isColliding = false;
	setType(PLANE);

	
}

Plane::~Plane()
= default;

void Plane::draw()
{
	
	//run
	Util::DrawLine(glm::vec2(xPosition, yPosition), glm::vec2(run, yPosition), glm::vec4(1.0f, 0, 1.0f, 255));

	//rise
	Util::DrawLine(glm::vec2(xPosition, rise), glm::vec2(xPosition, yPosition), glm::vec4(0, 1.0f, 0, 255));

	//ramp
	Util::DrawLine(glm::vec2(xPosition, rise), glm::vec2(run, yPosition), glm::vec4(0, 0, 1.0f, 255));
}

void Plane::update()
{
}

void Plane::clean()
{
}

