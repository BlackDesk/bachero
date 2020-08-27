#ifndef BACHERO_ENGINE_RENDER_RENDERER
#define BACHERO_ENGINE_RENDER_RENDERER

#include "SDL.h"

#include "Engine/Math/Vector2.h"

namespace Engine::Render {
    class Renderer {
    public:
        explicit Renderer(SDL_Renderer *renderer) : _renderer(renderer) {}

        SDL_Renderer *get() {
            return _renderer;
        }

        void setLogicalSize(Math::Vector2ui dimensions) {
            SDL_RenderSetLogicalSize(_renderer, dimensions.x, dimensions.y);
        }

        Math::Vector2ui getLogicalSize() const {
            int w, h;
            SDL_RenderGetLogicalSize(_renderer, &w, &h);
            return {w, h};
        }

        Math::Vector2ui getOutputSize() const {
            int w, h;
            SDL_GetRendererOutputSize(_renderer, &w, &h);
            return {w, h};
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