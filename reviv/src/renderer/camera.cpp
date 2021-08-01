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

Camera::Camera(float nearRenderDistance, float renderDistance, float horizontalFov)
    : nearRenderDistance(nearRenderDistance), renderDistance(renderDistance), m_HorizontalFov(horizontalFov)
{ }

void Camera::setViewMatrix(const Vec3f& position, const Rotation& rotation)
{
    direction = getDirectionFromRotation(rotation);

    Vec3f tempRight = cross(direction, Vec3f(0, 0, 1));
    right = tempRight / (module(tempRight));
    Vec3f tempUp = cross(right, direction);
    up = tempUp / (module(tempUp));

    viewMatrix.a[0][0] = right.a[0];
    viewMatrix.a[0][1] = right.a[1];
    viewMatrix.a[0][2] = right.a[2];
    viewMatrix.a[0][3] = -dot(position, right);

    viewMatrix.a[1][0] = up.a[0];
    viewMatrix.a[1][1] = up.a[1];
    viewMatrix.a[1][2] = up.a[2];
    viewMatrix.a[1][3] = -dot(position, up);

    viewMatrix.a[2][0] = -direction.a[0];
    viewMatrix.a[2][1] = -direction.a[1];
    viewMatrix.a[2][2] = -direction.a[2];
    viewMatrix.a[2][3] = dot(position, direction);

    viewMatrix.a[3][0] = 0;
    viewMatrix.a[3][1] = 0;
    viewMatrix.a[3][2] = 0;
    viewMatrix.a[3][3] = 1;
}

void Camera::setPerspectiveProjection()
{
    float ratio = (Application::get()->getWindow()->m_Data.width * 1.f)
                 / (Application::get()->getWindow()->m_Data.height * 1.f);

    m_VerticalFov = 2 * atan(tan(m_HorizontalFov / 2.f) / ratio);

    float width = nearRenderDistance * tan(m_HorizontalFov / 2.f) * 2.f;
    float height = nearRenderDistance * tan(m_VerticalFov / 2.f) * 2.f;


    projectionMatrix.a[0][0] = nearRenderDistance / width * 2.f;
    projectionMatrix.a[0][1] = 0;
    projectionMatrix.a[0][2] = 0;
    projectionMatrix.a[0][3] = 0;

    projectionMatrix.a[1][0] = 0;
    projectionMatrix.a[1][1] = nearRenderDistance / height * 2.f;
    projectionMatrix.a[1][2] = 0;
    projectionMatrix.a[1][3] = 0;

    projectionMatrix.a[2][0] = 0;
    projectionMatrix.a[2][1] = 0;
    projectionMatrix.a[2][2] = -(renderDistance + nearRenderDistance) / (renderDistance - nearRenderDistance);
    projectionMatrix.a[2][3] = -2.f * renderDistance * nearRenderDistance / (renderDistance - nearRenderDistance);

    projectionMatrix.a[3][0] = 0;
    projectionMatrix.a[3][1] = 0;
    projectionMatrix.a[3][2] = -1;
    projectionMatrix.a[3][3] = 0;
}

void Camera::setOrthographicProjection(float width)
{
    float ratio = (Application::get()->getWindow()->m_Data.width * 1.f)
                 / (Application::get()->getWindow()->m_Data.height * 1.f);

    float height = width / ratio;

    projectionMatrix.a[0][0] = 2.f / width;
    projectionMatrix.a[0][1] = 0;
    projectionMatrix.a[0][2] = 0;
    projectionMatrix.a[0][3] = 0;

    projectionMatrix.a[1][0] = 0;
    projectionMatrix.a[1][1] = 2.f / height;
    projectionMatrix.a[1][2] = 0;
    projectionMatrix.a[1][3] = 0;

    //projectionMatrix.a[2][0] = 0;
    //projectionMatrix.a[2][1] = 0;
    //projectionMatrix.a[2][2] = 0;
    //projectionMatrix.a[2][3] = 0;

    projectionMatrix.a[2][0] = 0;
    projectionMatrix.a[2][1] = 0;
    projectionMatrix.a[2][2] = -2 / (renderDistance - nearRenderDistance);
    projectionMatrix.a[2][3] = -(renderDistance + nearRenderDistance) / (renderDistance - nearRenderDistance);
    
    projectionMatrix.a[3][0] = 0;
    projectionMatrix.a[3][1] = 0;
    projectionMatrix.a[3][2] = 0;
    projectionMatrix.a[3][3] = 1;
}