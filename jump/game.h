#ifndef OPENGL_TEMPLATE_GAME_H
#define OPENGL_TEMPLATE_GAME_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <glfw3.h>

#include "models/camera.h"
#include "models/player.h"
#include "models/world.h"

class Game {
private:
    /**
     * Buffer containing vertices and normals for world and player
     */
    GLuint vertexbuffer[4];

    /**
     * ID for the vertexbuffer
     */
    GLuint VertexArrayID;

    /**
     * IDs for shaders and matrices
     */
    GLuint programID, playerProgramID, matrixID, modelMatrixID, viewMatrixID, lightID,
            playerLightID, playerModelID, playerMatrixID, playerViewMatrixID;

    /**
     * Vertices and normals for the game world
     */
    std::vector<glm::vec3> world_vertices;
    std::vector<glm::vec3> world_normals;

    /**
     * Vertices and normals for the player
     */
    std::vector<glm::vec3> player_vertices;
    std::vector<glm::vec3> player_normals;

    /**
     * The window of the application
     */
    GLFWwindow *window;

    /**
     * Objects for camera, player, and world
     */
    Camera cam;
    Player player;
    World world;

    /**
     * Booleans to only accept single key presses
     */
    bool canGenerate = true;
    bool canChangeMouse = true;

    /**
     * True if the mouse is captured by the window
     */
    bool mouseCaptured = true;

    /**
     * Game loop
     */
    void updateAnimationLoop();

    /**
     * Clean up vertex buffer
     *
     * @return true if successful
     */
    bool cleanupVertexbuffer();

    /**
     * Close the windows
     *
     * @return true if successful
     */
    static bool closeWindow();

    /**
     * Load a cube to x y and z coordinates and apply a transformation matrix
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param z z coordinate
     * @param trans transformation matrix
     */
    void loadCube(float x, float y, float z, glm::mat4 trans);

    /**
     * Load a cube according to a platform object
     *
     * @param plat platform object
     */
    void loadCube(Platform plat);

    /**
     * Load the player cube
     */
    void loadPlayer();

    /**
     * Update the inner game state
     */
    void updateGameState();

    /**
     * Initialize the game world
     */
    void initializeWorld();

    /**
     * Initialize the window
     *
     * @return true if successful
     */
    bool initializeWindow();

    /**
     * Initialize the vertex buffer
     *
     * @return true if successful
     */
    bool initializeVertexbuffer();

    /**
     * Initialize the OpenGL IDs
     */
    void initializeIDs();

public:
    /**
     * Run the game
     */
    void run();

    /**
     * Initialize the game
     *
     * @return true if successful
     */
    bool initialize();

    /**
     * Overall delta time of the last game loop iteration
     */
    float deltaTime = 0;

    /**
     * Width and height of window
     */
    static int width, height;
};


#endif //OPENGL_TEMPLATE_GAME_H
