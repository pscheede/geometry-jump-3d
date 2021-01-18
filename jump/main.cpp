#include "game.h"

int main() {
    Game game{};

    //Initialize game
    if (!game.initialize()) return -1;

    game.run();

    return 0;
}