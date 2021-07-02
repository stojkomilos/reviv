#pragma once

#include"core/mat.h"
#include"core/entity.h"

class PerspectiveCamera
{
public:
	PerspectiveCamera();
	void setUp(float nearPlane, float farPlane, float fov, float ratio);
	void recalculateViewMatrix();
	void recalculateProjectionMatrix();
	void alignWithEntity(Entity& entity);

	mat::vec3 position;
	float pitch, yaw, roll;
	mat::vec3 right, up, direction;

	mat::mat4 viewMatrix, projectionMatrix;

	float leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane;
	float ratio, fov;
};
