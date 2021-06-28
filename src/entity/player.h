#pragma once

#include "entity/entity.h"

class Player : public Entity {
public:
	Player();
	void updateUnitDirectionalVectors();
	mat::vec3 velocity, acceleration;

	mat::vec3 right, direction, up;
	mat::vec3 forward;
	mat::mat4 modelMatrix;
	int standingOnSurface;
	float speed, jumpSpeed, height, flyUpSpeed;
	float destroyRange;
	bool creativeMode, walking, holdingLeftClick;
	bool holdingFlyUpButton, holdingFlyDownButton;

	mat::vec3 previousPosition;
};
