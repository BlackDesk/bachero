#ifndef BACHERO_ENGINE_RENDER_SYSTEM
#define BACHERO_ENGINE_RENDER_SYSTEM

#include "Engine/ECS/ECS.h"
#include "Engine/Render/SpriteComponent.h"
#include "Engine/Render/Camera.h"

namespace Engine::Render {
    class RenderSystem : public ECS::System {
    public:
        explicit RenderSystem(Renderer *renderer)
                : _renderer(renderer) {
        }

        void init() override {

        }

        void render() override {
            CameraComponent *camera = getActiveCamera();

            if (camera) {
                Math::Rect_i cameraRect = camera->getPositionedRect();

                if (_renderer->getLogicalSize() != camera->getSize())
                    _renderer->setLogicalSize(camera->getSize());

                ECS::EntityManager::getInstance()->
                        getEntitiesThatHaveComponent<SpriteComponent>(_drawables);

                std::sort(_drawables.begin(), _drawables.end(),
                          [](const auto *a, const auto *b) -> bool {
                                auto *transformA = a->template getComponent<TransformComponent>();
                                auto *transformB = b->template getComponent<TransformComponent>();
                                return transformA->position.y < transformB->position.y;
                          });

                for (auto *entity : _drawables) {
                    auto *sprite = entity->getComponent<SpriteComponent>();
                    sprite->draw(cameraRect.position);
                }
            }
        }

    private:
        CameraComponent *getActiveCamera() {
            ECS::EntityManager::getInstance()->
                    getEntitiesThatHaveComponent<CameraComponent>(_cameras);

            CameraComponent *activeCamera = nullptr;
            for (auto *camera : _cameras) {
                auto component = camera->getComponent<CameraComponent>();
                if (!activeCamera ||
                    component->getActivityMarker() >
                    activeCamera->getActivityMarker())
                    activeCamera = component;
            }

            return activeCamera;
        }

        std::vector<ECS::Entity *> _drawables;
        std::vector<ECS::Entity *> _cameras;
        Renderer *_renderer;
    };
}

#endif //BACHERO_ENGINE_RENDER_SYSTEM