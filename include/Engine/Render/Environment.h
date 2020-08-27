#ifndef BACHERO_ENGINE_RENDER_ENVIRONMENT_H

#include "SDL.h"
#include "SDL_ttf.h"

#include "Engine/Render/RuntimeException.h"

#include <iostream>

namespace Engine::Render {
    class Environment {
    public:
        Environment() {
            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
                throwRenderRuntimeExceptionSDL("Could not initialize sdl2: \n");
            if (TTF_Init() < 0)
                throwRenderRuntimeException("Could not initialize SDL TTF: \n" + TTF_GetError() + "\n");
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, nullptr);
        }

        ~Environment() {
            TTF_Quit();
            SDL_Quit();
        }
    };
}


#endif