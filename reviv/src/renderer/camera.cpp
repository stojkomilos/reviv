#include"camera.h"
#include"scene/scene.h"

void log(const Camera& camera)
{
    cout << "viewMatrix: " << endl;
    log(camera.viewMatrix);
    cout << "projectionMatrix: " << endl;
    log(camera.projectionMatrix);
}

Camera::Camera(float nearRenderDistance, float renderDistance, float horizontalFov)
    : nearRenderDistance(nearRenderDistance), renderDistance(renderDistance), m_HorizontalFov(horizontalFov)
{ }

void Camera::recalculateViewMatrix(const Vec3f& position, const Rotation& rotation) // right handed coordinate system
{
    direction = getDirectionFromRotation(rotation);
    right = normalise(cross(direction, Vec3f(0, 0, 1)));
    up = normalise(cross(right, direction));

    viewMatrix.a.x = right.x;
    viewMatrix.b.x = right.y;
    viewMatrix.c.x = right.z;
    viewMatrix.d.x = -dot(position, right);

    viewMatrix.a.y = up.x;
    viewMatrix.b.y = up.y;
    viewMatrix.c.y = up.z;
    viewMatrix.d.y = -dot(position, up);

    viewMatrix.a.z = -direction.x;
    viewMatrix.b.z = -direction.y;
    viewMatrix.c.z = -direction.z;
    viewMatrix.d.z = dot(position, direction);

    viewMatrix.a.w = 0;
    viewMatrix.b.w = 0;
    viewMatrix.c.w = 0;
    viewMatrix.d.w = 1;
}

void Camera::recalculateProjectionMatrix(const WindowData& windowData)
{
    ratio = (float)windowData.width / (float)windowData.height;

    m_VerticalFov = 2 * atan(tan(m_HorizontalFov / 2.f) / ratio);

    float width = nearRenderDistance * tan(m_HorizontalFov / 2.f) * 2.f;
    float height = nearRenderDistance * tan(m_VerticalFov / 2.f) * 2.f;

    projectionMatrix.a.x = nearRenderDistance / width * 2.f;
    projectionMatrix.b.x = 0;
    projectionMatrix.c.x = 0;
    projectionMatrix.d.x = 0;

    projectionMatrix.a.y = 0;
    projectionMatrix.b.y = nearRenderDistance / height * 2.f;
    projectionMatrix.c.y = 0;
    projectionMatrix.d.y = 0;

    projectionMatrix.a.z = 0;
    projectionMatrix.b.z = 0;
    projectionMatrix.c.z = -(renderDistance + nearRenderDistance) / (renderDistance - nearRenderDistance);
    projectionMatrix.d.z = -2.f * renderDistance * nearRenderDistance / (renderDistance - nearRenderDistance);

    //projectionMatrix.a.z = 0;
    //projectionMatrix.b.z = 0;
    //projectionMatrix.c.z = 0;
    //projectionMatrix.d.z = 0;

    projectionMatrix.a.w = 0;
    projectionMatrix.b.w = 0;
    projectionMatrix.c.w = -1;
    projectionMatrix.d.w = 0;
}