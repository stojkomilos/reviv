#include "player.h"

Player::Player() {
	pitch = yaw = speed = jumpSpeed = height = flyUpSpeed = destroyRange = 0;
		creativeMode = walking = holdingLeftClick = holdingFlyUpButton = holdingFlyDownButton = standingOnSurface = 0;;
}
void Player::updateUnitDirectionalVectors()
{
    direction.x = -cos(pitch) * cos(yaw);
    direction.y = -sin(pitch);
    direction.z = -cos(pitch) * sin(yaw);
    direction = mat::normalise(direction);

    right = mat::normalise(mat::cross(direction, mat::vec3(0, 1, 0)));
    up = mat::normalise(mat::cross(right, direction));
    
    forward.x = -direction.x;
    forward.y = 0;
    forward.z = -direction.z;
    forward = mat::normalise(forward);
}