#ifndef BACHERO_ENGINE_RENDER_ENVIRONMENT_H

#include "SDL.h"

#include <iostream>

namespace Engine::Render {
    class Environment {
    public:
        Environment() {
            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
                throw std::runtime_error(
                        (std::string) "could not initialize sdl2: \n" + SDL_GetError());
        }

        ~Environment() {
            SDL_Quit();
        }
    };
}


#endif