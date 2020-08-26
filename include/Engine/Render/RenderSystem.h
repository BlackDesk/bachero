#ifndef BACHERO_ENGINE_RENDER_SYSTEM
#define BACHERO_ENGINE_RENDER_SYSTEM

#include "Engine/ECS/ECS.h"
#include "Engine/Render/SpriteComponent.h"

namespace Engine::Render {
    class RenderSystem : public ECS::System {
    public:
        void render() override {
            ECS::EntityManager::getInstance()->
                    getEntitiesThatHaveComponent<SpriteComponent>(_drawables);
            for (auto *entity : _drawables) {
                auto *sprite = entity->getComponent<SpriteComponent>();
                sprite->render();
            }
        }

    private:
        std::vector<ECS::Entity *> _drawables;
    };
}

#endif //BACHERO_ENGINE_RENDER_SYSTEM