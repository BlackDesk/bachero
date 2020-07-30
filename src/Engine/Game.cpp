#include "Engine/Game.h"

#include "Engine/Render/Environment.h"
#include "Engine/Render/Window.h"
#include "Engine/Render/SpriteComponent.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/ECS/Entity.h"

#include <iostream>

namespace Engine {
    class Game::Impl {
    public:
        Impl() {

        }

        void init() {
            _environment = new Render::Environment();
            Math::Vector2ui winDimensions = {640, 480};
            _window = new Render::Window("Bachero!", winDimensions, false);

            auto *entity = ECS::EntityManager::getInstance()->createEntity<ECS::Entity>();
            auto *texture = Render::TextureManager::getInstance()->
                    loadTexture("assets/sprite.png",
                                "player",
                                _window->getRenderer());
            entity->addComponent<TransformComponent>();
            entity->addComponent<Render::SpriteComponent>(texture, Math::Vector2ui(144, 192));
            entity->getComponent<Render::SpriteComponent>()->frame = {1, 0};
            entity->init();
        }

        void clean() {
            if (_cleaned)
                return;
            _cleaned = true;

            delete _window;

            //must be deleted the last
            delete _environment;
        }

        bool isRunning() const {
            return _isRunning;
        }

        void handleEvents() {
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        _isRunning = false;
                        break;
                    default:
                        break;
                }
            }
            ECS::EntityManager::getInstance()->handleEvents();
        }

        void update() {
            ECS::EntityManager::getInstance()->update();
        }

        void render() {
            _window->clear();

            ECS::EntityManager::getInstance()->render();

            _window->present();
        }


    private:
        Render::Environment *_environment;
        Render::Window *_window;

        bool _isRunning = true;
        bool _cleaned = false;
    };

    Game::Game()
            : _impl(std::make_unique<Impl>()) {}

    void Game::init() {
        _impl->init();
    }

    void Game::clean() {
        _impl->clean();
    }

    void Game::handleEvents() {
        _impl->handleEvents();
    }

    void Game::update() {
        _impl->update();
    }

    void Game::render() {
        _impl->render();
    }

    bool Game::isRunning() {
        return _impl->isRunning();
    }

    Game::~Game() = default;
}