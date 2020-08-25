#ifndef BACHERO_ENGINE_MATH_MAT2X2
#define BACHERO_ENGINE_MATH_MAT2X2

#include "Engine/Math/Vector2.h"

#include <cmath>

namespace Engine::Math {
    struct Mat2x2 {
        union {
            struct {
                float m00, m01;
                float m10, m11;
            };
            float m[2][2];
        };

        Math::Vector2f operator*(Math::Vector2f vec) const {
            return Math::Vector2f(vec.x * m00 + vec.y * m01,
                                  vec.x * m10 + vec.y * m11);
        }
    };

    inline Mat2x2 getRotationalMat(float rotation) {
        float cos = std::cos(rotation), sin = std::sin(rotation);
        return Mat2x2{cos, -sin,
                      sin, cos};
    }
}

#endif //BACHERO_ENGINE_MATH_MAT2X2