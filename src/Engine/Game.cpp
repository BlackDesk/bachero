#include "Engine/Game.h"

#include "Engine/Render/Environment.h"
#include "Engine/Render/Window.h"
#include "Engine/Render/Texture.h"

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

            texture = Render::Texture::load("assets/sprite.bmp", _window->getRenderer());
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
        }

        void update() {

        }

        void render() {
            _window->clear();

            auto size = texture->size();
            Math::Rect_ui src{Math::Vector2ui(0, 0), size};
            Math::Rect_ui dst{Math::Vector2ui(0, 0), size};

            texture->draw(src, dst);

            _window->present();
        }


    private:
        Render::Environment *_environment;
        Render::Window *_window;

        Render::Texture *texture;

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