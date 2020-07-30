#ifndef BACHERO_ENGINE_INPUT_MAPPER
#define BACHERO_ENGINE_INPUT_MAPPER

#include "Engine/Input/InputManager.h"
#include "Engine/Math/Vector2.h"

namespace Engine::Input {
    class InputMapper : public Singleton<InputMapper> {
    public:
        void init() {}

        void clean() {}

        void handleEvents() {
            int W = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_W);
            int A = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_A);
            int S = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_S);
            int D = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_D);

            int ArrowUp = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_UP);
            int ArrowLeft = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_LEFT);
            int ArrowDown = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_DOWN);
            int ArrowRight = InputManager::getInstance()->isKeyDown(SDL_SCANCODE_RIGHT);

            _movementDir.x = (D - A) + (ArrowRight - ArrowLeft);
            _movementDir.y = (S - W) + (ArrowDown - ArrowUp);
            if (_movementDir.lengthSqr() > 0)
                _movementDir /= _movementDir.length();
        }

        Math::Vector2d getMovementDir() {
            return _movementDir;
        }

    private:
        Math::Vector2d _movementDir;
    };
}

#endif //BACHERO_ENGINE_INPUT_MAPPER