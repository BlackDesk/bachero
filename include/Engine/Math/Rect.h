#ifndef BACHERO_ENGINE_MATH_RECT
#define BACHERO_ENGINE_MATH_RECT

#include "Engine/Math/Vector2.h"

#include "SDL.h"

namespace Engine::Math {
    template<typename T, typename Q>
    struct Rect {
        Vector2<T> position;
        Vector2<Q> size;

        Rect() = default;

        Rect(Vector2<T> pos, Vector2<Q> sz)
                : position(pos), size(sz) {}

        template<typename T1, typename Q1>
        operator Rect<T1, Q1>() {
            return Rect<T1, Q1> {Vector2<T1>(position), Vector2<Q1>(size)};
        }

        explicit operator SDL_Rect() {
            SDL_Rect rect;

            rect.x = position.x;
            rect.y = position.y;
            rect.w = size.x;
            rect.h = size.y;

            return rect;
        }
    };

    typedef Rect<double, double> Rect_d;
    typedef Rect<int, unsigned int> Rect_i;
    typedef Rect<unsigned int, unsigned int> Rect_ui;
}

#endif //BACHERO_ENGINE_MATH_RECT