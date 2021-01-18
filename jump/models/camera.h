#ifndef OPENGL_TEMPLATE_CAMERA_H
#define OPENGL_TEMPLATE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw3.h>

class Camera {
    /**
     * Current and target look at positions for interpolation
     */
    glm::vec3 lookAtPosition;
    glm::vec3 targetPosition;

    /**
     * Power for interpolation of look at direction
     */
    glm::vec3 power = glm::vec3(0.0002, 0.02, 0.0002);

    /**
     * Mouse speed
     */
    float mouseSpeed = 0.0008f;

    /**
     * Current vertical angle
     */
    float verticalAngle = 0;

    /**
     * Current horizontal angle
     */
    float horizontalAngle = 3.14f;

    /**
     * Last x and y positions of the cursor on the screen
     */
    float lastX = 0;
    float lastY = 0;

    /**
     * True if mouse movement must be initialized
     */
    bool firstMouseMovement = true;

    /**
     * View and projection matrices
     */
    glm::mat4 V;
    glm::mat4 P;

public:
    /**
     * Constructor
     */
    Camera() : P(glm::perspective(glm::radians(70.0f), 16.f / 9.f, 0.1f, 100.0f)) {};

    /**
     * Update rotation of the camera
     *
     * @param x new x position of cursor
     * @param y new y position of cursor
     */
    void updateRotation(float x, float y);

    /**
     * Update look at position
     *
     * @param pos position of the player
     * @param deltaTime delta time of the game loop
     */
    void updateLookingPosition(glm::vec3 pos, float deltaTime);

    /**
     * Update projection matrix
     *
     * @param width new window width
     * @param height new window height
     */
    void updateProjectionMatrix(int width, int height);

    /**
     * Get view matrix from camera
     *
     * @return view matrix
     */
    glm::mat4 getViewMatrix();

    /**
     * Get projection matrix from camera
     *
     * @return projection matrix
     */
    glm::mat4 getProjectionMatrix();

    /**
     * Direction vectors for viewing direction and right direction
     */
    glm::vec3 right;
    glm::vec3 direction;
};


#endif //OPENGL_TEMPLATE_CAMERA_H
