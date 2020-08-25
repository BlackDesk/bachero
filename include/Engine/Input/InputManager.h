#ifndef BACHERO_ENGINE_INPUT_MANAGER
#define BACHERO_ENGINE_INPUT_MANAGER

#include "Engine/Common/Singleton.h"
#include "Engine/Math/Vector2.h"

#include "SDL.h"

#include <cstdint>

namespace Engine::Input {
    enum MouseButton
    {
        Left = 0,
        Middle = 1,
        Right = 2
    };

    class InputManager : public Singleton<InputManager> {
    public:
        InputManager();

        void init();
        void clean();

        void handleEvents();

        bool isKeyDown(int keyCode);
        bool onKeyDown(int keyCode);
        bool onKeyUp(int keyCode);

        bool isMouseButtonDown(MouseButton button);
        bool onMouseButtonDown(MouseButton button);
        bool onMouseButtonUp(MouseButton button);
        Math::Vector2i getMousePos();

        bool onQuit();

        ~InputManager();

    private:
        bool _quit = false;
        int _numKeys = 0;
        uint8_t *_keyState = nullptr;
        uint8_t *_onKeyUpState = nullptr;
        uint8_t *_onKeyDownState = nullptr;
        bool _mouseState[3];
        bool _onMouseDown[3];
        bool _onMouseUp[3];
        Math::Vector2i _mousePos;
    };
}

#endif //BACHERO_ENGINE_INPUT_MANAGER