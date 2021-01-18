#ifndef OPENGL_TEMPLATE_PLAYER_H
#define OPENGL_TEMPLATE_PLAYER_H

#include <glm/glm.hpp>
#include <glfw3.h>

#include "world.h"

class Player {
    /**
     * Velocity in upward direction
     */
    float velocityUp;

    /**
     * Current and target angles for interpolation of player rotation (front-back FB, right-left RL)
     */
    float angleRL;
    float angleFB;
    float angleRLtarget;
    float angleFBtarget;

    /**
     * Current and target speed for interpolation of player speed (front-back FB, right-left RL)
     */
    float speedFB;
    float speedRL;
    float speedFBtarget;
    float speedRLtarget;

    /**
     * Interpolation powers for angle and speed
     */
    float anglePower = 0.0001;
    float speedPower = 0.00001;

    /**
     * Vectors for forward and right direction
     */
    glm::vec3 direction;
    glm::vec3 right;

    /**
     * True if play is falling and not in "god" mode
     */
    bool isFalling = true;

    /**
     * Boolean for only accepting single key presses for toggling "god" mode
     */
    bool canToggle = true;

    /**
     * Number of jumps since the last savepoint
     */
    int numOfJumps = 0;

    /**
     * Last saved position
     */
    glm::vec3 savedPosition = glm::vec3(0, 0, 0);

public:
    /**
     * Update player state
     *
     * @param window OpenGL window
     * @param direction forward direction vector
     * @param right right direction vector
     * @param world game world object
     * @param delta delta time of last game loop iteration
     */
    void updatePlayer(GLFWwindow *window, glm::vec3 direction, glm::vec3 right, World const &world, float delta);

    /**
     * Current position
     */
    glm::vec3 pos = glm::vec3(0, 0.25, 0);

    /**
     * Size of player
     */
    glm::vec3 size = glm::vec3(.05f);

    /**
     * Get model matrix of player
     *
     * @return model matrix
     */
    glm::mat4 getModelMatrix() const;
};


#endif //OPENGL_TEMPLATE_PLAYER_H
