#ifndef BACHERO_ENGINE_PHYSICS_AABB_TREE
#define BACHERO_ENGINE_PHYSICS_AABB_TREE

#include "Engine/Math/Rect.h"
#include "Engine/Common/SmallVector.h"

#include <memory>
#include <vector>

namespace Engine::Physics {
    class AABBTreeVisualizer {
    public:
        virtual void render(Math::Rect_d rect) = 0;
    };

    class AABBTree {
    public:
        AABBTree();

        void insert(void *data, Math::Rect_d aabb) noexcept;

        void insert(std::pair<void *, Math::Rect_d> pair) noexcept;

        //it's better to locate by pointer rather than aabb due to floating point errors
        void erase(void *data, Math::Rect_d aabb) noexcept;

        void erase(std::pair<void *, Math::Rect_d> pair) noexcept;

        void update(void *data, Math::Rect_d rect) noexcept;

        void visualize(AABBTreeVisualizer& visualizer);

        SmallVector<void *> query(Math::Rect_d aabb) noexcept;

        void clear() noexcept;

        ~AABBTree();

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };
}

#endif //BACHERO_ENGINE_PHYSICS_AABB_TREE