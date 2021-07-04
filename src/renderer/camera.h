#pragma once

#include"core/mat.h"
#include"core/entity.h"
#include"core/basic_components.h"

class PerspectiveCamera : public SpecificComponent<PerspectiveCamera>
{
public:
	PerspectiveCamera() = default;
	PerspectiveCamera(const PerspectiveCamera&) = default;

	void setUp(float nearPlane, float farPlane, float fov, float ratio); // TODO: staviti ovo u constructor
	void recalculateViewMatrix();
	void recalculateProjectionMatrix();
	void alignWithEntity(const Entity& entity);

	Vec3f position;
	float pitch, yaw, roll;
	Vec3f right, up, direction;

	Mat4 viewMatrix, projectionMatrix;

	float leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane;
	float ratio, fov;
};
