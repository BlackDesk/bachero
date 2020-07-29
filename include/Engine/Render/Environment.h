#ifndef BACHERO_ENGINE_RENDER_ENVIRONMENT_H

#include "SDL.h"

#include "Engine/Render/RuntimeException.h"

#include <iostream>

namespace Engine::Render {
    class Environment {
    public:
        Environment() {
            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
                throwRenderRuntimeException("Could not initialize sdl2: \n");
        }

        ~Environment() {
            SDL_Quit();
        }
    };
}


#endif