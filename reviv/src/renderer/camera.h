#pragma once

#include"core/mat.h"
#include"scene/entity.h"
#include"renderer/window.h"

class Camera
{
public:
    Camera() = default;
    Camera(const Camera&) = default;
    Camera(float renderDistance, float nearRenderDistance, float horizontalFov);

    void recalculateViewMatrix(const Vec3f& position, const Rotation& rotation);
    void recalculateProjectionMatrix(const WindowData& windowData);

    void onEventMouseMoved(Event* event);

    Vec3f right, up, direction;

    Mat4 viewMatrix, projectionMatrix;

    float renderDistance = 10000.f;
    float nearRenderDistance = 0.1f; //TODO, bolje vrednosti, z fighting? FOV=\aprrox 90-100
    float ratio;
    float m_HorizontalFov, m_VerticalFov;
};

void log(const Camera& camera);