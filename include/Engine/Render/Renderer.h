#ifndef BACHERO_ENGINE_RENDER_RENDERER
#define BACHERO_ENGINE_RENDER_RENDERER

#include "SDL.h"

namespace Engine::Render {
    class Renderer {
    public:
        Renderer(SDL_Renderer *renderer) : _renderer(renderer) {}

        SDL_Renderer *get() {
            return _renderer;
        }

        void clear() {
            SDL_RenderClear(_renderer);
        }

        void present() {
            SDL_RenderPresent(_renderer);
        }

        ~Renderer() {
            SDL_DestroyRenderer(_renderer);
        }

    private:

        SDL_Renderer *_renderer = nullptr;
    };
}

#endif //BACHERO_ENGINE_RENDER_RENDERER