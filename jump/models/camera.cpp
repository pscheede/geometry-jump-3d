//
// Created by philipp on 1/14/21.
//

#include "camera.h"

#include <cmath>

void Camera::updateRotation(float x, float y) {
    if (firstMouseMovement) {
        lastX = x;
        lastY = y;
        firstMouseMovement = false;
        return;
    }

    horizontalAngle += mouseSpeed * float(lastX - x);
    verticalAngle += mouseSpeed * float(lastY - y);

    lastX = x;
    lastY = y;

    if (verticalAngle > 3.1f / 2.f) {
        verticalAngle = 3.1f / 2.f;
    } else if (verticalAngle < -3.1f / 2.f) {
        verticalAngle = -3.1f / 2.f;
    }

    direction = glm::normalize(glm::vec3(
            std::cos(verticalAngle) * std::sin(horizontalAngle),
            std::sin(verticalAngle),
            std::cos(verticalAngle) * std::cos(horizontalAngle)
    ));

    right = normalize(cross(direction, glm::vec3(0, 1, 0)));
}

void Camera::updateLookingPosition(glm::vec3 pos, float deltaTime) {
    targetPosition = pos;
    lookAtPosition = lookAtPosition + (targetPosition - lookAtPosition) *
                                      glm::vec3(1 - pow(power.x, deltaTime), 1 - pow(power.y, deltaTime),
                                                1 - pow(power.z, deltaTime));
}

glm::mat4 Camera::getProjectionMatrix() {
    return P;
}

glm::mat4 Camera::getViewMatrix() {
    // Camera matrix
    V = glm::lookAt(
            lookAtPosition - 1.5f * direction, // Camera is at (4,3,3), in World Space
            lookAtPosition, // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    return V;
}

void Camera::updateProjectionMatrix(int width, int height) {
    P = glm::perspective(glm::radians(70.0f), (float) width / height, 0.1f, 100.0f);
}
