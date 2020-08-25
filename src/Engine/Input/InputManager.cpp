#include "Engine/Input/InputManager.h"

#include <iostream>

namespace Engine::Input {
    InputManager::InputManager() {
        memset(_mouseState, 0, sizeof(_mouseState));
        memset(_onMouseDown, 0, sizeof(_onMouseDown));
        memset(_onMouseUp, 0, sizeof(_onMouseUp));
    }

    void InputManager::init() {}

    void InputManager::clean() {}

    void InputManager::handleEvents() {
        memset(_onMouseDown, 0, sizeof(_onMouseDown));
        memset(_onMouseUp, 0, sizeof(_onMouseUp));

        auto *sdlKeyState = SDL_GetKeyboardState(&_numKeys);
        if (!_keyState)
            _keyState = new uint8_t[_numKeys];
        memcpy(_keyState, sdlKeyState, _numKeys);
        if (!_onKeyUpState)
            _onKeyUpState = new uint8_t[_numKeys];
        if (!_onKeyDownState)
            _onKeyDownState = new uint8_t[_numKeys];
        memset(_onKeyUpState, 0, _numKeys);
        memset(_onKeyDownState, 0, _numKeys);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    _quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        _onMouseDown[MouseButton::Left] = true;
                        _mouseState[MouseButton::Left] = true;
                    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                        _onMouseDown[MouseButton::Middle] = true;
                        _mouseState[MouseButton::Middle] = true;
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        _onMouseDown[MouseButton::Right] = true;
                        _mouseState[MouseButton::Right] = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        _onMouseUp[MouseButton::Left] = true;
                        _mouseState[MouseButton::Left] = false;
                    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                        _onMouseUp[MouseButton::Middle] = true;
                        _mouseState[MouseButton::Middle] = false;
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        _onMouseUp[MouseButton::Right] = true;
                        _mouseState[MouseButton::Right] = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    _mousePos.x = event.motion.x;
                    _mousePos.y = event.motion.y;
                    break;
                case SDL_KEYUP:
                    _onKeyUpState[event.key.keysym.scancode] = true;
                    break;
                case SDL_KEYDOWN:
                    _onKeyDownState[event.key.keysym.scancode] = true;
                    break;
                default:
                    break;
            }
        }
    }

    bool InputManager::isKeyDown(int keyCode) {
        return _keyState && _keyState[keyCode];
    }

    bool InputManager::onKeyDown(int keyCode) {
        return _onKeyDownState && _onKeyDownState[keyCode];
    }

    bool InputManager::onKeyUp(int keyCode) {
        return _onKeyUpState && _onKeyUpState[keyCode];
    }

    bool InputManager::onQuit() {
        return _quit;
    }

    bool InputManager::isMouseButtonDown(MouseButton button) {
        return _mouseState[button];
    }

    bool InputManager::onMouseButtonDown(MouseButton button) {
        return _onMouseDown[button];
    }

    bool InputManager::onMouseButtonUp(MouseButton button) {
        return _onMouseUp[button];
    }

    Math::Vector2i InputManager::getMousePos() {
        return _mousePos;
    }

    InputManager::~InputManager() {
        delete _onKeyDownState;
        delete _onKeyUpState;
        delete _keyState;
    }
}