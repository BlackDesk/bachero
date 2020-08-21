#ifndef BACHERO_ENGINE_RENDER_WINDOW_H
#define BACHERO_ENGINE_RENDER_WINDOW_H

#include "SDL.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Render/Renderer.h"

#include <stdexcept>
#include <string>

namespace Engine::Render {
    class Window {
    public:
        Window(const std::string &title, Math::Vector2ui dimensions, bool fullscreen = false);

        Renderer *getRenderer();

        void clear();

        void present();

        ~Window();

    private:
        SDL_Window* _window = nullptr;
        Renderer* _renderer = nullptr;
        SDL_Rect _wholeWindowRect;
    };
}

#endif //BACHERO_ENGINE_RENDER_WINDOW_H
