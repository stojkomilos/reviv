#include "camera.h"
void PerspectiveCamera::setUp(float nearPlane1, float farPlane1, float fov1, float ratio1)
{
    nearPlane = nearPlane1; 
    farPlane = farPlane1;
    fov = fov1;
    ratio = ratio1;
}
void PerspectiveCamera::recalculateViewMatrix()
{

    direction.x = -cos(pitch) * cos(yaw);
    direction.y = -sin(pitch);
    direction.z = -cos(pitch) * sin(yaw);
    direction = normalise(direction);

    right = normalise(cross(direction, Vec3f(0, 1, 0)));
    up = normalise(cross(right, direction));

    viewMatrix.a.x = right.x;
    viewMatrix.b.x = right.y;
    viewMatrix.c.x = right.z;
    viewMatrix.d.x = -dot(position, right);

    viewMatrix.a.y = up.x;
    viewMatrix.b.y = up.y;
    viewMatrix.c.y = up.z;
    viewMatrix.d.y = -dot(position, up);

    viewMatrix.a.z = direction.x;
    viewMatrix.b.z = direction.y;
    viewMatrix.c.z = direction.z;
    viewMatrix.d.z = -dot(position, direction);

    viewMatrix.a.w = 0;
    viewMatrix.b.w = 0;
    viewMatrix.c.w = 0;
    viewMatrix.d.w = 1;
}
void PerspectiveCamera::recalculateProjectionMatrix()
{

    float L = 2.0f * tan(fov / 2) * nearPlane;

    float H = L / ratio;

    projectionMatrix.a.x = nearPlane * 2.0f / L;
    projectionMatrix.b.y = nearPlane * 2.0f / H;
    projectionMatrix.c.z = -(farPlane + nearPlane) / (farPlane - nearPlane);
    projectionMatrix.d.z = 2.0f * farPlane * nearPlane / (nearPlane - farPlane);

    projectionMatrix.c.w = -1;

}