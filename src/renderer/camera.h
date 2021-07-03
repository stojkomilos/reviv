#pragma once

#include"core/mat.h"
#include"core/entity.h"
#include"core/basic_components.h"

class PerspectiveCameraComponent : public SpecificComponent<PerspectiveCameraComponent>
{
public:
	PerspectiveCameraComponent() = default;
	PerspectiveCameraComponent(const PerspectiveCameraComponent&) = default;

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
