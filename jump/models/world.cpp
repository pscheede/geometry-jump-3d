#include "world.h"

#include <cstdlib>
#include <ctime>

glm::mat4 World::getModelMatrix() {
    return glm::mat4(1.f);
}

void World::initialize() {
    srand((unsigned) time(nullptr));
    float x = 0, y = 0, z = 0;
    float sx = .5, sy = .02, sz = .5;

    platforms = std::vector<Platform>{
            Platform{glm::vec3(x, y, z), glm::vec3(sx, sy, sz)},
//            Platform{glm::vec3(1, 0.25, 0), glm::vec3(.5, 0.02, .5)},
//            Platform{glm::vec3(1, 0.5, 1), glm::vec3(.5, 0.02, .5)},
//            Platform{glm::vec3(0, 0.75, 1), glm::vec3(.5, 0.02, .5)},
//            Platform{glm::vec3(0, 1.1, 2.5), glm::vec3(.5, 0.02, .5)},
//            Platform{glm::vec3(0, 1.6, 3.8), glm::vec3(.5, 0.02, .5)},
    };


    size_t numOfPlatforms = 200;

    for (auto i = 0; i < numOfPlatforms; i++) {
        auto y_offset = (rand() % 100) / 100. * 0.4 + .1;

//        if (y - y_offset < 0) {
//            y_offset = 0.5;
//        }

        auto new_xs = (rand() % 100) / 100. * .7 + .1;
        auto new_zs = (rand() % 100) / 100. * .7 + .1;

        double max_offs_x = .8 + sx + new_xs;
        double max_offs_z = .8 + sz + new_zs;

        auto x_offset = (rand() % 100) / 100. * 2 * max_offs_x - max_offs_x;
//        if (x_offset != 0)
//            x_offset += (sx + new_xs) * (x_offset / abs(x_offset));
        auto z_offset = (rand() % 100) / 100. * 2 * max_offs_z - max_offs_z;
//        if (z_offset != 0)
//            z_offset += (sz + new_zs) * (z_offset / abs(z_offset));

        x += x_offset;
        y += y_offset;
        z += z_offset;

        sx = new_xs;
        sz = new_zs;

        platforms.push_back(Platform{glm::vec3(x, y, z), glm::vec3(sx, sy, sz)});
    }
}
