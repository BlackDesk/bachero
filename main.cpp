#include <memory>
#include <iostream>
#include <exception>

#include "Engine/Game.h"
#include "Engine/Common/FramerateLimiter.h"
#include "Engine/Common/DeltaTime.h"

int main(int argc, char* args[]) {
    auto game = std::make_unique<Engine::Game>();
    Engine::FramerateLimiter limiter(60);

#if DEBUG_LEVEL == 0 || !defined(DEBUG_LEVEL)
    try {
#endif
        game->init();

        while (game->isRunning()) {
            Engine::DeltaTime::record();
            limiter.recordStart();

            game->handleEvents();
            game->update();
            game->render();

            limiter.recordStop();
            limiter.delay();
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