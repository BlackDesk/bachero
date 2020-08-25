#ifndef BACHERO_ENGINE_MATH_RECT
#define BACHERO_ENGINE_MATH_RECT

#include "Engine/Math/Vector2.h"

#include "SDL.h"

namespace Engine::Math {
    template<typename T, typename Q>
    struct Rect {
        //minimal point
        Vector2<T> position;
        Vector2<Q> size;

        Rect() = default;

        Rect(Vector2<T> pos, Vector2<Q> sz)
                : position(pos), size(sz) {}

        decltype(auto) midPoint() const {
            return position + size / 2;
        }

        decltype(auto) minPoint() const {
            return position;
        }

        decltype(auto) maxPoint() const {
            return position + size;
        }

        //clockwise
        template<typename R>
        void getCorners(Math::Vector2<R> &p1, Math::Vector2<R> &p2,
                        Math::Vector2<R> &p3, Math::Vector2<R> &p4) const {
            p1.x = position.x;
            p1.y = position.y;

            p2.x = position.x + size.x;
            p2.y = position.y;

            p3.x = position.x + size.x;
            p3.y = position.y + size.y;

            p4.x = position.x;
            p4.y = position.y + size.y;
        }

        template<typename T1, typename Q1>
        operator Rect<T1, Q1>() {
            return Rect<T1, Q1>{Vector2<T1>(position), Vector2<Q1>(size)};
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
    typedef Rect<float, float> Rect_f;
    typedef Rect<int, int> Rect_i;
    typedef Rect<unsigned int, unsigned int> Rect_ui;
}

#endif //BACHERO_ENGINE_MATH_RECT