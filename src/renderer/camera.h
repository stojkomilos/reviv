#pragma once

#include"core/mat.h"
#include"scene/entity.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = default;

	void setUp(float nearPlane, float farPlane, float fov, float ratio); // TODO: staviti ovo u constructor
	void alignWithEntity(const Entity& entity);

    void recalculateViewMatrix(const Vec3f& position, const Rotation& rotation); // gRenderManager -> recalculateViewMatrix ili static deo ProjectionCamera klase
	void recalculateProjectionMatrix();

	Vec3f right, up, direction;

	Mat4 viewMatrix, projectionMatrix;

	float leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane;
	float ratio, fov;
};

void log(const Camera& camera);