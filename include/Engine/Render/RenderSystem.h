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
            ECS::Entity *camera = getActiveCamera();

            if (camera) {
                auto *cameraComponent = camera->getComponent<CameraComponent>();
                Math::Rect_i cameraRect = cameraComponent->getPositionedRect();

                if (_renderer->getLogicalSize() != cameraComponent->getSize())
                    _renderer->setLogicalSize(cameraComponent->getSize());

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
        std::vector<ECS::Entity *> _drawables;
        Renderer *_renderer;
    };
}

#endif //BACHERO_ENGINE_RENDER_SYSTEM