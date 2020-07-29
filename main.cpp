#include <memory>
#include <iostream>
#include <exception>

#include "Engine/Game.h"

int main(int argc, char* args[]) {
    auto game = std::make_unique<Engine::Game>();

#if DEBUG_LEVEL == 0 || !defined(DEBUG_LEVEL)
    try {
#endif
        game->init();

        while (game->isRunning()) {
            game->handleEvents();
            game->update();
            game->render();
        }

#if DEBUG_LEVEL == 0 || !defined(DEBUG_LEVEL)
    } catch (const std::exception &any) {
        std::cout << any.what() << std::endl;
    } catch (...) {
        //ignore
    }
#endif

    game->clean();
}