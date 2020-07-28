#ifndef BACHERO_ENGINE_MATH_RECT
#define BACHERO_ENGINE_MATH_RECT

#include "Engine/Math/Vector2.h"

#include "SDL.h"

namespace Engine::Math {
    template<typename T>
    struct Rect {
        Vector2<T> position;
        Vector2<T> size;

        explicit operator SDL_Rect() {
            SDL_Rect rect;

            rect.x = position.x;
            rect.y = position.y;
            rect.w = size.x;
            rect.h = size.y;

            return rect;
        }
    };

    typedef Rect<double> Rect_d;
    typedef Rect<int> Rect_i;
    typedef Rect<unsigned int> Rect_ui;
}

#endif //BACHERO_ENGINE_MATH_RECT