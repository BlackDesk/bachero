#ifndef BACHERO_ENGINE_RENDER_SPRITE_COMPONENT_H
#define BACHERO_ENGINE_RENDER_SPRITE_COMPONENT_H

#include "Engine/ECS/Entity.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Render/Sprite.h"

namespace Engine::Render {
    class SpriteComponent : public ECS::DataOnlyComponent {
    public:
        explicit SpriteComponent(Texture *texture) {
            _sprite = std::make_unique<Sprite>(texture);
        }

        SpriteComponent(Texture *texture, Math::Vector2i frameSize) {
            _sprite = std::make_unique<Sprite>(texture, frameSize);
        }

        Math::Vector2ui frame;
        Math::Vector2f position; //local space

        void init() override {
            if (!owner->hasComponent<TransformComponent>())
                owner->addComponent<TransformComponent>();
            _transform = owner->getComponent<TransformComponent>();
        }

        //called internally by the render system
        void draw(Math::Vector2f cameraPos) {
            if (_transform)
                _sprite->position = _transform->position + position;
            _sprite->frame = frame;
            _sprite->rotation = _transform->rotation;
            _sprite->anchorPointLocal = -position;

            _sprite->draw(cameraPos);
        }

        Sprite *getSprite() const {
            return _sprite.get();
        }

    private:
        std::unique_ptr<Sprite> _sprite;
        TransformComponent *_transform = nullptr;
    };
}

#endif //BACHERO_ENGINE_RENDER_SPRITE_COMPONENT_H
