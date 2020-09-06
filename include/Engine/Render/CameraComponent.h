#ifndef BACHERO_ENGINE_RENDER_CAMERA_COMPONENT
#define BACHERO_ENGINE_RENDER_CAMERA_COMPONENT

#include "Engine/ECS/ECS.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Math/Rect.h"

namespace Engine::Render {
    class CameraComponent : public ECS::DataOnlyComponent {
    public:
        typedef uint64_t ActivityMarker;

        explicit CameraComponent(Math::Vector2ui size = {100, 100}) {
            setSize(size);
        }

        void init() override {
            if (!owner->hasComponent<TransformComponent>())
                owner->addComponent<TransformComponent>();
            _transform = owner->getComponent<TransformComponent>();
        }

        void setActive() {
            _marker = genMarker();
        }

        ActivityMarker getActivityMarker() const {
            return _marker;
        }

        void setSize(Math::Vector2ui size) {
            _size = size;
            hwRatio = (float) _size.y / _size.x;
            whRatio = (float) _size.x / _size.y;
        }

        void setWidth(uint32_t width) {
            _size = {width, width * hwRatio};
        }

        void setHeight(uint32_t height) {
            _size = {height * whRatio, height};
        }

        Math::Vector2ui getSize() const {
            return _size;
        }

        Math::Rect_i getPositionedRect() const {
            Math::Rect_i rect;
            rect.position = _transform->position;
            rect.size = (Math::Vector2ui) _size;
            return rect;
        }

    private:
        TransformComponent *_transform = nullptr;

        ActivityMarker _marker = 0;
        Math::Vector2ui _size;
        float hwRatio, whRatio;

        static ActivityMarker genMarker() {
            static ActivityMarker marker = 0;
            return ++marker;
        }
    };

    inline ECS::Entity *getActiveCamera() {
        static std::vector<ECS::Entity *> cameras;
        ECS::EntityManager::getInstance()->
                getEntitiesThatHaveComponent<CameraComponent>(cameras);

        ECS::Entity *activeCamera = nullptr;
        CameraComponent::ActivityMarker marker = 0;
        for (auto *camera : cameras) {
            auto *component = camera->getComponent<CameraComponent>();
            if (!activeCamera ||
                component->getActivityMarker() > marker) {
                activeCamera = camera;
                marker = component->getActivityMarker();
            }
        }

        return activeCamera;
    }
}

#endif //BACHERO_ENGINE_RENDER_CAMERA_COMPONENT