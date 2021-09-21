#include"camera.h"
#include"scene/scene.h"
#include"core/application.h"

void log(const Camera& camera)
{
    cout << "viewMatrix: " << endl;
    log(camera.viewMatrix);
    cout << "projectionMatrix: " << endl;
    log(camera.projectionMatrix);
}

Camera::Camera(float nearRenderDistance, float renderDistance, float horizontalFovInRadians)
    : nearRenderDistance(nearRenderDistance), renderDistance(renderDistance), m_HorizontalFov(horizontalFovInRadians)
{ }

void Camera::setViewMatrix(const Vec3& position, const Vec3& rotation)
{
    direction = getDirectionFromRotation(rotation);

    Vec3 tempRight = cross(direction, Vec3(0, 0, 1));
    right = tempRight / (module(tempRight));
    Vec3 tempUp = cross(right, direction);
    up = tempUp / (module(tempUp));

    //cout << "tempRight: " << endl;
    //log(tempRight);
    //cout << "right: " << endl;
    //log(right);
    //cout << "tempUp: " << endl;
    //log(tempUp);
    //cout << "up: " << endl;
    //log(up);
    //cout << "----------" << endl;

    //cout << "old viewMatrix: " << endl;
    //log(viewMatrix);

    *viewMatrix.getPtr(0, 0) = right.get(0, 0);
    *viewMatrix.getPtr(0, 1) = right.get(1, 0);
    *viewMatrix.getPtr(0, 2) = right.get(2, 0);
    *viewMatrix.getPtr(0, 3) = -dot(position, right);

    *viewMatrix.getPtr(1, 0) = up.get(0, 0);
    *viewMatrix.getPtr(1, 1) = up.get(1, 0);
    *viewMatrix.getPtr(1, 2) = up.get(2, 0);
    *viewMatrix.getPtr(1, 3) = -dot(position, up);

    *viewMatrix.getPtr(2, 0) = -direction.get(0, 0);
    *viewMatrix.getPtr(2, 1) = -direction.get(1, 0);
    *viewMatrix.getPtr(2, 2) = -direction.get(2, 0);
    *viewMatrix.getPtr(2, 3) = dot(position, direction);

    *viewMatrix.getPtr(3, 0) = 0;
    *viewMatrix.getPtr(3, 1) = 0;
    *viewMatrix.getPtr(3, 2) = 0;
    *viewMatrix.getPtr(3, 3) = 1;
}

void Camera::setPerspectiveProjection(float horizontalFov, float ratio)
{
    RV_ASSERT(nearRenderDistance != 0 && renderDistance != 0 && nearRenderDistance < renderDistance, "camera incorrectly or incompletly configured");
    m_HorizontalFov = horizontalFov;

    m_VerticalFov = 2 * atan(tan(m_HorizontalFov / 2.f) / ratio);

    float width = nearRenderDistance * tan(m_HorizontalFov / 2.f) * 2.f;
    float height = nearRenderDistance * tan(m_VerticalFov / 2.f) * 2.f;


    *projectionMatrix.getPtr(0, 0) = nearRenderDistance / width * 2.f;
    *projectionMatrix.getPtr(0, 1) = 0;
    *projectionMatrix.getPtr(0, 2) = 0;
    *projectionMatrix.getPtr(0, 3) = 0;

    *projectionMatrix.getPtr(1, 0) = 0;
    *projectionMatrix.getPtr(1, 1) = nearRenderDistance / height * 2.f;
    *projectionMatrix.getPtr(1, 2) = 0;
    *projectionMatrix.getPtr(1, 3) = 0;

    *projectionMatrix.getPtr(2, 0) = 0;
    *projectionMatrix.getPtr(2, 1) = 0;
    *projectionMatrix.getPtr(2, 2) = -(renderDistance + nearRenderDistance) / (renderDistance - nearRenderDistance);
    *projectionMatrix.getPtr(2, 3) = -2.f * renderDistance * nearRenderDistance / (renderDistance - nearRenderDistance);

    *projectionMatrix.getPtr(3, 0) = 0;
    *projectionMatrix.getPtr(3, 1) = 0;
    *projectionMatrix.getPtr(3, 2) = -1;
    *projectionMatrix.getPtr(3, 3) = 0;
}

void Camera::setOrthographicProjection(float width, float ratio)
{
    float height = width / ratio;
    RV_ASSERT(nearRenderDistance != 0 && renderDistance != 0 && nearRenderDistance < renderDistance, "camera incorrectly or incompletly configured");

    *projectionMatrix.getPtr(0, 0) = 2.f / width;
    *projectionMatrix.getPtr(0, 1) = 0;
    *projectionMatrix.getPtr(0, 2) = 0;
    *projectionMatrix.getPtr(0, 3) = 0;

    *projectionMatrix.getPtr(1, 0) = 0;
    *projectionMatrix.getPtr(1, 1) = 2.f / height;
    *projectionMatrix.getPtr(1, 2) = 0;
    *projectionMatrix.getPtr(1, 3) = 0;

    *projectionMatrix.getPtr(2, 0) = 0;
    *projectionMatrix.getPtr(2, 1) = 0;
    *projectionMatrix.getPtr(2, 2) = -2 / (renderDistance - nearRenderDistance);
    *projectionMatrix.getPtr(2, 3) = -(renderDistance + nearRenderDistance) / (renderDistance - nearRenderDistance);
    
    *projectionMatrix.getPtr(3, 0) = 0;
    *projectionMatrix.getPtr(3, 1) = 0;
    *projectionMatrix.getPtr(3, 2) = 0;
    *projectionMatrix.getPtr(3, 3) = 1;
}