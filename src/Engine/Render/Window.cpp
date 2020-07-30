#include "Engine/Render/Window.h"

#include "Engine/Render/RuntimeException.h"

namespace Engine::Render {
    Window::Window(const std::string &title, Math::Vector2ui dimensions, bool fullscreen) {
        uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
        if (fullscreen)
            flags |= SDL_WINDOW_FULLSCREEN;

        _window = SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                dimensions.x, dimensions.y,
                flags
        );

        if (_window == nullptr)
            throwRenderRuntimeExceptionSDL("could not create window: %s\n");

        auto *SDL_renderer = SDL_CreateRenderer(_window, -1, 0);
        SDL_SetRenderDrawColor(SDL_renderer, 255,255,255,255);

        if (SDL_renderer == nullptr)
            throwRenderRuntimeExceptionSDL("could not create renderer: %s\n");

        _renderer = new Renderer(SDL_renderer);
    }

    Renderer *Window::getRenderer() {
        return _renderer;
    }

    void Window::clear() {
        _renderer->clear();
    }

    void Window::present() {
        _renderer->present();
    }

    Window::~Window() {
        SDL_DestroyWindow(_window);
        delete _renderer;
    }
}