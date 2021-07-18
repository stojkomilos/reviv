#pragma once

#include"core/mat.h"
#include"scene/entity.h"
#include"renderer/window.h"

class Camera
{
public:
	Camera() = default;
	Camera(float nearPlane, float farPlane, float fov);
	Camera(const Camera&) = default;

    void recalculateViewMatrix(const Vec3f& position, const Rotation& rotation); // gRenderManager -> recalculateViewMatrix ili static deo ProjectionCamera klase
	void recalculateProjectionMatrix(const WindowData& windowData);

    void onEventMouseMoved(Event* event);

	Vec3f right, up, direction;

	Mat4 viewMatrix, projectionMatrix;

    float renderDistance = 1000;
	float leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane;
	float ratio, fov;
};

void log(const Camera& camera);