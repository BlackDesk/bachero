#ifndef BACHERO_ENGINE_RENDER_COLOR
#define BACHERO_ENGINE_RENDER_COLOR

#include "SDL.h"

#include <cstdint>

namespace Engine::Render {
    struct Color {
        uint8_t r, g, b, a;

        operator SDL_Color() {
            return SDL_Color{r, g, b, a};
        }
    };
}

#endif