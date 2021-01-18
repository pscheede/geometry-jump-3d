#include "game.h"

#include <cstdio>

#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <iostream>
#include "common/objloader.hpp"

int Game::width = 1600;
int Game::height = 900;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // Adjust window scaling
    glViewport(0, 0, width, height);
    Game::height = height;
    Game::width = width;
}

bool Game::initialize() {
    if (!initializeWindow()) return false;

    initializeWorld();

    if (!initializeVertexbuffer()) return false;

    initializeIDs();

    return true;
}

void Game::run() {
    updateGameState();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float frameEnd = glfwGetTime();

    //start animation loop until escape key is pressed
    do {
        float start = glfwGetTime();
        deltaTime = start - frameEnd;
        frameEnd = start;

        updateAnimationLoop();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);


    //Cleanup and close window
    cleanupVertexbuffer();
    glDeleteProgram(programID);
    closeWindow();
}

bool Game::initializeWindow() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "Geometry Jump 3D", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Mouse capturing
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(0);

    // Dark blue background
    glClearColor(0.043f, 0.145f, 0.271f, 0.0f);
    return true;
}

bool Game::initializeVertexbuffer() {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(4, vertexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, world_vertices.size() * sizeof(glm::vec3), &world_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, world_normals.size() * sizeof(glm::vec3), &world_normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
    glBufferData(GL_ARRAY_BUFFER, player_vertices.size() * sizeof(glm::vec3), &player_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
    glBufferData(GL_ARRAY_BUFFER, player_normals.size() * sizeof(glm::vec3), &player_normals[0], GL_STATIC_DRAW);

    return true;
}

void Game::initializeIDs() {
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("WorldShader.vertexshader", "WorldShader.fragmentshader");
    playerProgramID = LoadShaders("PlayerShader.vertexshader", "PlayerShader.fragmentshader");

    matrixID = glGetUniformLocation(programID, "MVP");
    modelMatrixID = glGetUniformLocation(programID, "M");
    viewMatrixID = glGetUniformLocation(programID, "V");
    playerViewMatrixID = glGetUniformLocation(playerProgramID, "V");
    lightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    playerLightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    playerModelID = glGetUniformLocation(playerProgramID, "Mp");
    playerMatrixID = glGetUniformLocation(playerProgramID, "MpVP");
}

void Game::updateAnimationLoop() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    updateGameState();

    cam.updateProjectionMatrix(width, height);

    glm::mat4 M = World::getModelMatrix();
    glm::mat4 V = cam.getViewMatrix();
    glm::mat4 P = cam.getProjectionMatrix();

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &(P * V * M)[0][0]);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &V[0][0]);

//    glm::vec3 lightPos = glm::vec3(4, player.pos.y + 7, 2);
    glm::vec3 lightPos = glm::vec3(player.pos.x + 4, player.pos.y + 8, player.pos.z + 2);
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) 0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, world_vertices.size() * sizeof(glm::vec3)); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glUseProgram(playerProgramID);

    glm::mat4 Mp = player.getModelMatrix();

    glUniformMatrix4fv(playerMatrixID, 1, GL_FALSE, &(P * V * Mp)[0][0]);
    glUniformMatrix4fv(playerModelID, 1, GL_FALSE, &Mp[0][0]);
    glUniformMatrix4fv(playerViewMatrixID, 1, GL_FALSE, &V[0][0]);

    glUniform3f(playerLightID, lightPos.x, lightPos.y, lightPos.z);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
    glVertexAttribPointer(
            2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
    glVertexAttribPointer(
            3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) 0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, player_vertices.size() * sizeof(glm::vec3)); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::updateGameState() {
    // todo global delta time

    GLdouble xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (canGenerate) {
            initializeWorld();
            initializeVertexbuffer();
        }
        canGenerate = false;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        canGenerate = true;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        if (canChangeMouse) {
            if (mouseCaptured) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                mouseCaptured = false;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                mouseCaptured = true;
            }
        }
        canChangeMouse = false;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) {
        canChangeMouse = true;
    }


    cam.updateRotation(xPos, yPos);

    player.updatePlayer(window, cam.direction, cam.right, world, deltaTime);

    cam.updateLookingPosition(player.pos, deltaTime);
}

bool Game::cleanupVertexbuffer() {
    // Cleanup VBO
    glDeleteBuffers(1, vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    return true;
}

bool Game::closeWindow() {
    glfwTerminate();
    return true;
}

void Game::loadPlayer() {
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    loadOBJ("cube.obj", temp_vertices, temp_uvs, temp_normals);

    auto trans = glm::scale(glm::mat4(1.f), player.size);
    // processing
    assert(temp_vertices.size() == temp_normals.size());
    for (int i = 0; i < temp_vertices.size(); i++) {
        auto &vert = temp_vertices.at(i);

        auto vert4 = glm::vec4(vert, 1);
        vert4 = trans * vert4;
        temp_vertices.at(i) = glm::vec3{vert4};

//        temp_vertices.at(i)[1] += player.size.y;
    }

    player_vertices.insert(player_vertices.end(), temp_vertices.begin(), temp_vertices.end());
    player_normals.insert(player_normals.end(), temp_normals.begin(), temp_normals.end());
}

void Game::loadCube(float x, float y, float z, glm::mat4 trans) {
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    loadOBJ("cube.obj", temp_vertices, temp_uvs, temp_normals);

    // processing
    assert(temp_vertices.size() == temp_normals.size());
    for (int i = 0; i < temp_vertices.size(); i++) {
        auto &vert = temp_vertices.at(i);

        auto vert4 = glm::vec4(vert, 1);
        vert4 = trans * vert4;
        temp_vertices.at(i) = glm::vec3{vert4};

        temp_vertices.at(i)[0] += x;
        temp_vertices.at(i)[1] += y;
        temp_vertices.at(i)[2] += z;
    }

    world_vertices.insert(world_vertices.end(), temp_vertices.begin(), temp_vertices.end());
    world_normals.insert(world_normals.end(), temp_normals.begin(), temp_normals.end());
}

void Game::loadCube(Platform plat) {
    glm::mat4 scale = glm::scale(glm::mat4(1.f), plat.size);
    loadCube(plat.pos.x, plat.pos.y, plat.pos.z, scale);
}

void Game::initializeWorld() {
    world_vertices.clear();
    world_normals.clear();

    world.initialize();

    for (Platform p: world.platforms) {
        loadCube(p);
    }

    loadPlayer();
}


