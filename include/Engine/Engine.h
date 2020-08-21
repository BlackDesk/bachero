#ifndef BACHERO_ENGINE_H
#define BACHERO_ENGINE_H

#include "Engine/Common/Singleton.h"
#include <memory>

namespace Engine {
    class Engine : public Singleton<Engine> {
    public:
        Engine();

        void init();
        bool isRunning();

        void handleEvents();
        void update();
        void render();

        void clean();

        ~Engine();

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };
}

#endif //BACHERO_ENGINE_H
