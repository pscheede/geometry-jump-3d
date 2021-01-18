#include "player.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Player::updatePlayer(GLFWwindow *window, glm::vec3 _direction, glm::vec3 _right, World const &world, float delta) {
    static double lastStart = glfwGetTime();

    direction = _direction;
    right = _right;

    speedFBtarget = 0;
    speedRLtarget = 0;

    angleFBtarget = 0;
    angleRLtarget = 0;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        speedFBtarget += 2.f;
        angleFBtarget += .2f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        speedFBtarget -= 2.f;
        angleFBtarget -= .2f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        speedRLtarget -= 2.f;
        angleRLtarget += .2f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        speedRLtarget += 2.f;
        angleRLtarget -= .2f;
    }

    speedFB = speedFB + (speedFBtarget - speedFB) * (1 - pow(speedPower, delta));
    speedRL = speedRL + (speedRLtarget - speedRL) * (1 - pow(speedPower, delta));

    if (speedFB != 0)
        pos += normalize(direction * glm::vec3(1, 0, 1)) * speedFB * delta;
    if (speedRL != 0)
        pos += normalize(right * glm::vec3(1, 0, 1)) * speedRL * delta;

    angleFB = angleFB + (angleFBtarget - angleFB) * (1 - pow(anglePower, delta));
    angleRL = angleRL + (angleRLtarget - angleRL) * (1 - pow(anglePower, delta));

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (canToggle) {
            isFalling = !isFalling;
            velocityUp = 2;
        }
        canToggle = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
        canToggle = true;
    }

    if (isFalling) {
        pos.y += velocityUp * delta;
        velocityUp -= 4 * delta;

        // Collision
        if (velocityUp < 0) {
            for (Platform p: world.platforms) {
                float upperX = p.pos.x + p.size.x;
                float lowerX = p.pos.x - p.size.x;
                float upperZ = p.pos.z + p.size.z;
                float lowerZ = p.pos.z - p.size.z;
                float upperY = p.pos.y + p.size.y;
                float lowerY = p.pos.y - p.size.y;
                if (pos.x - size.x < upperX && pos.x + size.x > lowerX && pos.z - size.z < upperZ &&
                    pos.z + size.z > lowerZ) {
                    if (pos.y - size.y < upperY && pos.y - size.y - velocityUp * delta > upperY) {
//                && pos.y - size.y / 2.f < upperY && pos.y + size.y / 2.f > lowerY) {
                        pos.y = upperY + size.y;
                        velocityUp = -velocityUp / 1.7f;
                        if (velocityUp < 2) {
                            velocityUp = 2;
                        }
                        numOfJumps++;
                        if (numOfJumps == 20) {
                            numOfJumps = 0;
                            savedPosition = pos;
                        }
                        break;
                    }
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            pos = glm::vec3(0, 0.25, 0);
            velocityUp = 0;
            numOfJumps = 0;
            savedPosition = pos;
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS || pos.y < -.2) {
            numOfJumps = 0;
            pos = savedPosition + glm::vec3(0, 0.25, 0);
            velocityUp = 0;
        }
    } else {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            pos.y += 2 * delta;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            pos.y -= 2 * delta;
        }
    }

    lastStart = glfwGetTime();
}

glm::mat4 Player::getModelMatrix() const {
    return glm::translate(glm::mat4(1.f), pos) *
           glm::rotate(glm::mat4(1.0f), -angleRL, normalize(direction * glm::vec3(1, 0, 1))) *
           glm::rotate(glm::mat4(1.0f), -angleFB, normalize(right * glm::vec3(1, 0, 1)));
}
