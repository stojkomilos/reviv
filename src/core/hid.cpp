#include"hid.h"

/*
//extern PhysicsManager gPhysicsManager;
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
    gPhysicsManager.player.yaw -= difX * sensitivity;
    gPhysicsManager.player.pitch -= difY * sensitivity;

    if (gPhysicsManager.player.pitch >= 1.55f)
        gPhysicsManager.player.pitch = 1.55f;
    else if (gPhysicsManager.player.pitch <= -1.55f)
        gPhysicsManager.player.pitch = -1.55f;
    lastX = (float) xpos;
    lastY = (float)ypos;

}


void processInput(GLFWwindow* window) {

    gPhysicsManager.player.walking = false;
    gPhysicsManager.player.holdingLeftClick = false;
    gPhysicsManager.player.holdingFlyDownButton = false;
    gPhysicsManager.player.holdingFlyUpButton = false;


    bool movingForward, movingBack, movingRight, movingLeft;
    movingForward = movingBack = movingRight = movingLeft = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        gPhysicsManager.player.holdingLeftClick = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_W)) == GLFW_PRESS) {
        gPhysicsManager.player.walking = true;
        movingForward = true;
        gPhysicsManager.player.velocity.x = gPhysicsManager.player.forward.x * gPhysicsManager.player.speed;
        gPhysicsManager.player.velocity.z = gPhysicsManager.player.forward.z * gPhysicsManager.player.speed;
    }
    if ((glfwGetKey(window, GLFW_KEY_S)) == GLFW_PRESS) {
        gPhysicsManager.player.walking = true;
        movingBack = true;
        gPhysicsManager.player.velocity.x = -gPhysicsManager.player.forward.x * gPhysicsManager.player.speed;
        gPhysicsManager.player.velocity.z = -gPhysicsManager.player.forward.z * gPhysicsManager.player.speed;
    }
    if ((glfwGetKey(window, GLFW_KEY_D)) == GLFW_PRESS) {
        gPhysicsManager.player.walking = true;
        movingRight = true;
        gPhysicsManager.player.velocity.x = gPhysicsManager.player.right.x * gPhysicsManager.player.speed;
        gPhysicsManager.player.velocity.z = gPhysicsManager.player.right.z * gPhysicsManager.player.speed;
    }
    if ((glfwGetKey(window, GLFW_KEY_A)) == GLFW_PRESS) {
        gPhysicsManager.player.walking = true;
        movingLeft = true;
        gPhysicsManager.player.velocity.x = -gPhysicsManager.player.right.x * gPhysicsManager.player.speed;
        gPhysicsManager.player.velocity.z = -gPhysicsManager.player.right.z * gPhysicsManager.player.speed;
    }
    if ((movingForward ^ movingBack) and (movingLeft ^ movingRight)) {
        gPhysicsManager.player.velocity = mat::add(mat::multiplyScalar(mat::normalise(mat::add(mat::multiplyScalar(gPhysicsManager.player.forward, movingForward * 2 - 1), mat::multiplyScalar(gPhysicsManager.player.right, movingRight * 2 - 1))), gPhysicsManager.player.speed), Vec3f(0.f, gPhysicsManager.player.velocity.y, 0.f));
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT)) == GLFW_PRESS) {
        gPhysicsManager.player.acceleration.y = 0;
        gPhysicsManager.player.velocity.y = 0;
        //std::cout << "A" << std::endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS) {
        gPhysicsManager.player.velocity.y = -gPhysicsManager.player.flyUpSpeed;
        gPhysicsManager.player.acceleration.y = 0;
        // std::cout << "B" << std::endl;
        gPhysicsManager.player.holdingFlyDownButton = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!gPhysicsManager.player.creativeMode) {
            if (gPhysicsManager.player.standingOnSurface) {
                gPhysicsManager.player.velocity.y = 5;
                gPhysicsManager.player.position.y += 0.1f; ///TODOOOOOOOOOOOOOO smanji


            }
        }
        else {
            gPhysicsManager.player.velocity.y = gPhysicsManager.player.flyUpSpeed;
            gPhysicsManager.player.holdingFlyUpButton = true; 
        }

    }


}

*/