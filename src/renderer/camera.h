#pragma once

#include"core/mat.h"
#include"core/entity.h"
#include"core/basic_components.h"

class Cameraa : public SpecificComponent<Cameraa>
{
public:
	Cameraa() = default;
	Cameraa(const Cameraa&) = default;

	void setUp(float nearPlane, float farPlane, float fov, float ratio); // TODO: staviti ovo u constructor
	void alignWithEntity(const Entity& entity);

    void recalculateViewMatrix(const PositionComponent& position, const RotationComponent& rotation); // gRenderManager -> recalculateViewMatrix ili static deo ProjectionCamera klase
	void recalculateProjectionMatrix();

	Vec3f right, up, direction;

	Mat4 viewMatrix, projectionMatrix;

	float leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane;
	float ratio, fov;
};
