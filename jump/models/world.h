#ifndef OPENGL_TEMPLATE_WORLD_H
#define OPENGL_TEMPLATE_WORLD_H

#include <vector>
#include <glm/glm.hpp>

struct Platform {
    /**
     * Position of the platform
     */
    glm::vec3 pos;

    /**
     * Size of the platform
     */
    glm::vec3 size;
};

class World {
public:
    /**
     * Get the model matrix of the game world
     *
     * @return model matrix
     */
    static glm::mat4 getModelMatrix();

    /**
     * All platforms of the game world
     */
    std::vector<Platform> platforms;

    /**
     * Initialize the world with new platforms
     */
    void initialize();
};


#endif //OPENGL_TEMPLATE_WORLD_H
