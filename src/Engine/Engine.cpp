#include "Engine/Engine.h"

#include "Engine/Render/Environment.h"
#include "Engine/Render/Window.h"
#include "Engine/Render/SpriteComponent.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Input/InputMapper.h"
#include "Engine/ECS/ECS.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Game/Entities/Player.h"
#include "Game/Entities/Box.h"
#include "Game/Entities/Wall.h"

#include <iostream>

namespace Engine {
    class Engine::Impl {
    public:
        void init() {
            _environment = new Render::Environment();
            Math::Vector2ui winDimensions = {640, 480};
            _window = new Render::Window("Bachero!", winDimensions, false);

            Render::TextureManager::getInstance()->setDefaultRenderer(_window->getRenderer());
            Input::InputManager::getInstance()->init();
            Input::InputMapper::getInstance()->init();

            ECS::SystemManager::getInstance()->createSystem<Physics::PhysicsSystem>();

            ECS::EntityManager::getInstance()->createEntity<Player>()->init();
            ECS::EntityManager::getInstance()->createEntity<Box>(Math::Vector2d{300, 300})->init();
            ECS::EntityManager::getInstance()->createEntity<Box>(Math::Vector2d{350, 300})->init();
            ECS::EntityManager::getInstance()->createEntity<Box>(Math::Vector2d{400, 300})->init();
            ECS::EntityManager::getInstance()->createEntity<Wall>(Math::Rect_d{{0, 0}, {640, 10}})->init();
            ECS::EntityManager::getInstance()->createEntity<Wall>(Math::Rect_d{{0, 470}, {640, 10}})->init();
            ECS::EntityManager::getInstance()->createEntity<Wall>(Math::Rect_d{{0, 10}, {10, 460}})->init();
            ECS::EntityManager::getInstance()->createEntity<Wall>(Math::Rect_d{{630, 10}, {10, 460}})->init();
            ECS::EntityManager::getInstance()->createEntity<Wall>()->init();
        }

        void clean() {
            if (_cleaned)
                return;
            _cleaned = true;

            ECS::EntityManager::getInstance()->clean();
            ECS::SystemManager::getInstance()->clean();
            Input::InputMapper::getInstance()->clean();
            Input::InputManager::getInstance()->clean();
            Render::TextureManager::getInstance()->clean();

            delete _window;

            //must be deleted the last
            delete _environment;
        }

        bool isRunning() const {
            return _isRunning;
        }

        void handleEvents() {
            Input::InputManager::getInstance()->handleEvents();
            Input::InputMapper::getInstance()->handleEvents();
            ECS::SystemManager::getInstance()->handleEvents();
            ECS::EntityManager::getInstance()->handleEvents();

            if (Input::InputManager::getInstance()->onQuit())
                _isRunning = false;
        }

        void update() {
            ECS::SystemManager::getInstance()->update();
            ECS::EntityManager::getInstance()->update();
        }

        void render() {
            _window->clear();

            ECS::EntityManager::getInstance()->render();
            ECS::SystemManager::getInstance()->render();

            _window->present();
        }


    private:
        Render::Environment *_environment = nullptr;
        Render::Window *_window = nullptr;

        bool _isRunning = true;
        bool _cleaned = false;
    };

    Engine::Engine()
            : _impl(std::make_unique<Impl>()) {}

    void Engine::init() {
        _impl->init();
    }

    void Engine::clean() {
        _impl->clean();
    }

    void Engine::handleEvents() {
        _impl->handleEvents();
    }

    void Engine::update() {
        _impl->update();
    }

    void Engine::render() {
        _impl->render();
    }

    bool Engine::isRunning() {
        return _impl->isRunning();
    }

    Engine::~Engine() = default;
}