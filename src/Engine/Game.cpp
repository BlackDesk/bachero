#include "Engine/Game.h"

#include "Engine/Render/Environment.h"
#include "Engine/Render/Window.h"

#include <iostream>

namespace Engine {
    class Game::Impl {
    public:
        Impl() {

        }

        void init() {
            environment = std::make_unique<Render::Environment>();
            environment->init();

            window = std::make_unique<Render::Window>();
        }

        void clean() {
            if (_cleaned)
                return;
            _cleaned = true;

            environment->clean();
        }

        bool isRunning() const {
            return _isRunning;
        }

        void handleEvents() {

        }

        void update() {

        }

        void render() {

        }


    private:
        std::unique_ptr<Render::Environment> environment;
        std::unique_ptr<Render::Window> window;

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