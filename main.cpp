#include <memory>
#include <iostream>
#include <exception>

#include "Engine/Game.h"

int main(int argc, char* args[]) {
    auto game = std::make_unique<Engine::Game>();

    game->init();

    try {
        while (game->isRunning()) {
            game->handleEvents();
            game->update();
            game->render();
        }
    } catch (const std::exception &any) {
        std::cout << any.what() << std::endl;
    } catch (...) {
        //ignore
    }

    game->clean();
}