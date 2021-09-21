#pragma once

#include"core/mat.h"
#include"scene/entity.h"
#include"renderer/window.h"

class Camera
{
public:
    Camera() = default;
    Camera(const Camera&) = default;
    Camera(float nearRenderDistance, float renderDistance, float horizontalFovInRadians);

    void setViewMatrix(const Vec3& position, const Vec3& rotation);
    void setPerspectiveProjection(float horizontalFov, float ratio);
    void setOrthographicProjection(float width, float ratio);

    void onEventMouseMoved(Event* event);

    Vec3 right, up, direction;

    Mat<4,4> viewMatrix, projectionMatrix;

    float nearRenderDistance = 0.1f;
    float renderDistance = 10000.f;
    float m_HorizontalFov, m_VerticalFov;
};

void log(const Camera& camera);