#ifndef BACHERO_ENGINE_RENDER_WINDOW_H
#define BACHERO_ENGINE_RENDER_WINDOW_H

#include "SDL.h"

#include <stdexcept>
#include <string>

namespace Engine::Render {
    class Window {
    public:
        Window(size_t width = 640, size_t height = 480, bool fullscreen = false) {
            uint32_t flags = SDL_WINDOW_SHOWN;
            if (fullscreen)
                flags |= SDL_WINDOW_FULLSCREEN;

            window = SDL_CreateWindow(
                    "hello_sdl2",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    width, height,
                    flags
            );

            if (window == nullptr)
                throw std::runtime_error((std::string)"could not create window: %s\n" + SDL_GetError());

            screenSurface = SDL_GetWindowSurface(window);
        }

        ~Window() {
            delete window;
            delete screenSurface;
        }

    private:
        SDL_Window* window = nullptr;
        SDL_Surface* screenSurface = nullptr;
    };
}

#endif //BACHERO_ENGINE_RENDER_WINDOW_H
