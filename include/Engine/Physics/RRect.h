#ifndef BACHERO_ENGINE_PHYSICS_RRECT
#define BACHERO_ENGINE_PHYSICS_RRECT

#include "Engine/Math/Vector2.h"

namespace Engine::Physics {
    struct RRect {
        Math::Vector2f position;
        Math::Vector2f size;
        double rotation = 0;

        RRect(Math::Vector2f pos, Math::Vector2f sz, double rot)
                : position(pos), size(sz), rotation(rot) {}
    };
}

#endif //BACHERO_ENGINE_MATH_RRECT