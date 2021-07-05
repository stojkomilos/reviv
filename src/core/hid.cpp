#include"hid.h"

extern Entity* gpPlayerEntity;
extern Entity* gpCameraEntity;

const float sensitivity = 0.002f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    static float lastX, lastY;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float difX = (float)xpos - lastX;
    float difY = (float)ypos - lastY;
    gpPlayerEntity->getComponent<RotationComponent>()->yaw -= difX * sensitivity;
    gpPlayerEntity->getComponent<RotationComponent>()->pitch -= difY * sensitivity;

    if (gpPlayerEntity->getComponent<RotationComponent>()->pitch >= 1.55f)
    {
        gpPlayerEntity->getComponent<RotationComponent>()->pitch = 1.55f;
    }
    else if (gpPlayerEntity->getComponent<RotationComponent>()->pitch <= -1.55f)
    {
        gpPlayerEntity->getComponent<RotationComponent>()->pitch = -1.55f;
    }
    lastX = (float) xpos;
    lastY = (float)ypos;

}


void processInput(GLFWwindow* window) {

   // gpPlayerEntity->getComponent<>().walking = false;
   // gpPlayerEntity->getComponent<>().holdingLeftClick = false;
   // gpPlayerEntity->getComponent<>().holdingFlyDownButton = false;
   // gpPlayerEntity->getComponent<>().holdingFlyUpButton = false;


/*
    bool movingForward, movingBack, movingRight, movingLeft;
    movingForward = movingBack = movingRight = movingLeft = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().holdingLeftClick = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_W)) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().walking = true;
        movingForward = true;
        gpPlayerEntity->getComponent<>().velocity.x = gpPlayerEntity->getComponent<>().forward.x * gpPlayerEntity->getComponent<>().speed;
        gpPlayerEntity->getComponent<>().velocity.z = gpPlayerEntity->getComponent<>().forward.z * gpPlayerEntity->getComponent<>().speed;
    }
    if ((glfwGetKey(window, GLFW_KEY_S)) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().walking = true;
        movingBack = true;
        gpPlayerEntity->getComponent<>().velocity.x = -gpPlayerEntity->getComponent<>().forward.x * gpPlayerEntity->getComponent<>().speed;
        gpPlayerEntity->getComponent<>().velocity.z = -gpPlayerEntity->getComponent<>().forward.z * gpPlayerEntity->getComponent<>().speed;
    }
    if ((glfwGetKey(window, GLFW_KEY_D)) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().walking = true;
        movingRight = true;
        gpPlayerEntity->getComponent<>().velocity.x = gpPlayerEntity->getComponent<>().right.x * gpPlayerEntity->getComponent<>().speed;
        gpPlayerEntity->getComponent<>().velocity.z = gpPlayerEntity->getComponent<>().right.z * gpPlayerEntity->getComponent<>().speed;
    }
    if ((glfwGetKey(window, GLFW_KEY_A)) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().walking = true;
        movingLeft = true;
        gpPlayerEntity->getComponent<>().velocity.x = -gpPlayerEntity->getComponent<>().right.x * gpPlayerEntity->getComponent<>().speed;
        gpPlayerEntity->getComponent<>().velocity.z = -gpPlayerEntity->getComponent<>().right.z * gpPlayerEntity->getComponent<>().speed;
    }
    if ((movingForward ^ movingBack) and (movingLeft ^ movingRight)) {
        gpPlayerEntity->getComponent<>().velocity = mat::add(mat::multiplyScalar(mat::normalise(mat::add(mat::multiplyScalar(gpPlayerEntity->getComponent<>().forward, movingForward * 2 - 1), mat::multiplyScalar(gpPlayerEntity->getComponent<>().right, movingRight * 2 - 1))), gpPlayerEntity->getComponent<>().speed), Vec3f(0.f, gpPlayerEntity->getComponent<>().velocity.y, 0.f));
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT)) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().acceleration.y = 0;
        gpPlayerEntity->getComponent<>().velocity.y = 0;
        //std::cout << "A" << std::endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS) {
        gpPlayerEntity->getComponent<>().velocity.y = -gpPlayerEntity->getComponent<>().flyUpSpeed;
        gpPlayerEntity->getComponent<>().acceleration.y = 0;
        // std::cout << "B" << std::endl;
        gpPlayerEntity->getComponent<>().holdingFlyDownButton = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!gpPlayerEntity->getComponent<>().creativeMode) {
            if (gpPlayerEntity->getComponent<>().standingOnSurface) {
                gpPlayerEntity->getComponent<>().velocity.y = 5;
                gpPlayerEntity->getComponent<>().position.y += 0.1f;


            }
        }
        else {
            gpPlayerEntity->getComponent<>().velocity.y = gpPlayerEntity->getComponent<>().flyUpSpeed;
            gpPlayerEntity->getComponent<>().holdingFlyUpButton = true; 
        }

    }
*/

}