#ifndef BACHERO_ENGINE_RENDER_UTILS
#define BACHERO_ENGINE_RENDER_UTILS

#include "SDL.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Render/Color.h"
#include "Engine/Render/Renderer.h"

namespace Engine::Render {
    inline void drawQuadrangle(Renderer *renderer,
                               Color color,
                               Math::Vector2f p1,
                               Math::Vector2f p2,
                               Math::Vector2f p3,
                               Math::Vector2f p4) {

        auto *sdlRenderer = renderer->get();
        SDL_SetRenderDrawColor(sdlRenderer,
                               color.r,
                               color.g,
                               color.b,
                               color.a);
        SDL_RenderDrawLine(sdlRenderer,
                           p1.x, p1.y,
                           p2.x, p2.y);
        SDL_RenderDrawLine(sdlRenderer,
                           p2.x, p2.y,
                           p3.x, p3.y);
        SDL_RenderDrawLine(sdlRenderer,
                           p3.x, p3.y,
                           p4.x, p4.y);
        SDL_RenderDrawLine(sdlRenderer,
                           p4.x, p4.y,
                           p1.x, p1.y);
    }

    //https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
    inline void drawCircle(Renderer *renderer,
                           Color color,
                           Math::Vector2f center,
                           float radius) {
        auto *sdlRenderer = renderer->get();
        SDL_SetRenderDrawColor(sdlRenderer,
                               color.r,
                               color.g,
                               color.b,
                               color.a);

        const int32_t diameter = (radius * 2);

        int32_t x = (radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        while (x >= y) {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(sdlRenderer, center.x + x, center.y - y);
            SDL_RenderDrawPoint(sdlRenderer, center.x + x, center.y + y);
            SDL_RenderDrawPoint(sdlRenderer, center.x - x, center.y - y);
            SDL_RenderDrawPoint(sdlRenderer, center.x - x, center.y + y);
            SDL_RenderDrawPoint(sdlRenderer, center.x + y, center.y - x);
            SDL_RenderDrawPoint(sdlRenderer, center.x + y, center.y + x);
            SDL_RenderDrawPoint(sdlRenderer, center.x - y, center.y - x);
            SDL_RenderDrawPoint(sdlRenderer, center.x - y, center.y + x);

            if (error <= 0) {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0) {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }
}

#endif //BACHERO_ENGINE_RENDER_UTILS