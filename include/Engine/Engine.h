#ifndef BACHERO_ENGINE_H
#define BACHERO_ENGINE_H

#include "Engine/Common/Singleton.h"
#include "Engine/Math/Vector2.h"

#include <memory>

namespace Engine {
    class Engine : public Singleton<Engine> {
    public:
        Engine();

        void init(const std::string& title,
                  Math::Vector2ui winDimensions,
                  bool fullscreen);
        bool isRunning();

        void handleEvents();
        void update();
        void render();
        void refresh();

        void loop();

        void clean();

        ~Engine();

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };
}

#endif //BACHERO_ENGINE_H
