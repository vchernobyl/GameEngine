#include "ZombieGame/ZombieGame.h"
#include "NinjaGame/NinjaGame.h"
#undef main

int main(int argc, char** argv) {
    NinjaGame game;
    game.Run();

    return 0;
}
