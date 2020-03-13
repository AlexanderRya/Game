#include <game/core/Game.hpp>

int main() {
    using namespace game::core;
    Game game;
    game.load();
    game.run();
    return 0;
}