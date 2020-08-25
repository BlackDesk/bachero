#include "Engine/Engine.h"

#include "Engine/ECS/ECS.h"
#include "Engine/Render/Environment.h"
#include "Engine/Render/Window.h"
#include "Engine/Render/TextureManager.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Input/InputMapper.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/Common/DebugWidget.h"
#include "Engine/Common/DebugMode.h"

#include <iostream>

namespace Engine {
    class Engine::Impl {
    public:
        void init(const std::string& title,
                  Math::Vector2d winDimensions,
                  bool fullscreen) {
            _environment = new Render::Environment();
            _window = new Render::Window(title, winDimensions, false);

            Render::TextureManager::getInstance()->setDefaultRenderer(_window->getRenderer());
            Input::InputManager::getInstance()->init();
            Input::InputMapper::getInstance()->init();

            _debugWidget = new DebugWidget();

            ECS::SystemManager::getInstance()->createSystem<Physics::PhysicsSystem>();

            std::cout << "Engine init success." << std::endl;
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

            delete _debugWidget;
            delete _window;

            //must be deleted the last
            delete _environment;

            std::cout << "Engine clean success." << std::endl;
        }

        bool isRunning() const {
            return _isRunning;
        }

        void handleEvents() {
            //Retrieving events
            Input::InputManager::getInstance()->handleEvents();
            Input::InputMapper::getInstance()->handleEvents();

            //updating systems
            ECS::SystemManager::getInstance()->handleEvents();
            ECS::EntityManager::getInstance()->handleEvents();

            if (Input::InputManager::getInstance()->onKeyUp(SDL_SCANCODE_LEFTBRACKET))
                _debugWidgetEnabled = !_debugWidgetEnabled;
            if (Input::InputManager::getInstance()->onKeyUp(SDL_SCANCODE_RIGHTBRACKET))
                DebugMode::toggle();

            if (Input::InputManager::getInstance()->onQuit())
                _isRunning = false;
        }

        void update() {
            _debugWidget->update();
            ECS::EntityManager::getInstance()->update();

            ECS::SystemManager::getInstance()->update();

            //refresh should happen only after all entities and systems were updated
            // otherwise this can lead to dangling pointers of removed objects without
            // any opportunity for systems to check for it beforehand
            ECS::EntityManager::getInstance()->refresh();
        }

        void render() {
            _window->clear();

            ECS::EntityManager::getInstance()->render();
            ECS::SystemManager::getInstance()->render();

            if (_debugWidgetEnabled)
                _debugWidget->render();

            _window->present();
        }


    private:
        DebugWidget *_debugWidget = nullptr;
        Render::Environment *_environment = nullptr;
        Render::Window *_window = nullptr;

        bool _debugWidgetEnabled = false;
        bool _isRunning = true;
        bool _cleaned = false;
    };

    Engine::Engine()
            : _impl(std::make_unique<Impl>()) {}

    void Engine::init(const std::string& title,
                      Math::Vector2d winDimensions,
                      bool fullscreen) {
        _impl->init(title, winDimensions, fullscreen);
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