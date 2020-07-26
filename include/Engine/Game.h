#ifndef BACHERO_GAME_H
#define BACHERO_GAME_H

#include <memory>

namespace Engine {
    class Game {
    public:
        Game();

        void init();
        bool isRunning();

        void handleEvents();
        void update();
        void render();

        void clean();

        ~Game();

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };
}

#endif //BACHERO_GAME_H
