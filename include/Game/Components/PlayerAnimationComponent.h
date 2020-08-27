#ifndef BACHERO_GAME_PLAYER_ANIMATION_COMPONENT
#define BACHERO_GAME_PLAYER_ANIMATION_COMPONENT

#include "Engine/ECS/ECS.h"
#include "Engine/Render/SpriteComponent.h"
#include "Engine/Common/DeltaTime.h"

class PlayerAnimationComponent : public Engine::ECS::Component {
public:
    void init() override {
        _sprite = owner->getComponent<Engine::Render::SpriteComponent>();
        rowLen = _sprite->getSprite()->texture->size().x /
                    _sprite->getSprite()->frameSize.x;
    }

    void render() override {
        animationSpeed = 25 * playerSpeed;
        if (state == State::idle) {
            _sprite->frame = {direction, 0};
        } else if (state == State::walking) {
            _sprite->frame = {currentFrame, 1 + direction};
            t += Engine::DeltaTime::get();
            currentFrame = (int)(t * animationSpeed) % rowLen;
        }
    }

    enum State {
        idle = 0,
        walking = 1
    } state = State::idle;

    enum Direction {
        up = 0,
        down = 1,
        left = 2,
        right = 3
    } direction = Direction::down;

    float playerSpeed = 0;

    std::size_t currentFrame = 0;
    float animationSpeed = 0; //frames per second

private:
    float t = 0;
    std::size_t rowLen = 0;
    Engine::Render::SpriteComponent *_sprite = nullptr;
};

#endif //BACHERO_GAME_PLAYER_ANIMATION_COMPONENT