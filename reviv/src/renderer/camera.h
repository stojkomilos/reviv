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

    void setViewMatrix(const Vec3f& position, const Rotation& rotation);
    void setPerspectiveProjection(float horizontalFov, float ratio);
    void setOrthographicProjection(float width, float ratio);

    void onEventMouseMoved(Event* event);

    Vec3f right, up, direction;

    Mat4 viewMatrix, projectionMatrix;

    float nearRenderDistance = 0.1f;
    float renderDistance = 10000.f;
    //float ratio;
    float m_HorizontalFov, m_VerticalFov;

    //float newProjectionMatrix[16];
    //float newViewMatrix[16];
};

void log(const Camera& camera);