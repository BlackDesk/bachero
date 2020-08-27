#ifndef BACHERO_GAME_LOOP
#define BACHERO_GAME_LOOP

#include <memory>
#include <iostream>
#include <exception>

#include "Engine/Engine.h"
#include "Engine/Common/FramerateLimiter.h"
#include "Engine/Common/DeltaTime.h"
#include "Engine/Render/Camera.h"

#include "Game/Entities/Player.h"
#include "Game/Entities/Box.h"
#include "Game/Entities/Wall.h"

class GameLoop {
public:
    void run() {
        auto *engine = Engine::Engine::getInstance();
        Engine::FramerateLimiter limiter(60);

#if DEBUG_LEVEL == 0 || !defined(DEBUG_LEVEL)
        try {
#endif
        engine->init("Bachero!",
                     {640, 480},
                     false);
        initEntities();

        while (engine->isRunning()) {
            Engine::DeltaTime::record();
            limiter.recordStart();

            engine->loop();

            limiter.recordStop();
            limiter.delay();
        }

#if DEBUG_LEVEL == 0 || !defined(DEBUG_LEVEL)
        } catch (const std::exception &any) {
        std::cout << any.what() << std::endl;
    } catch (...) {
        //ignore
    }
#endif

        engine->clean();
    }

    void initEntities() {
        auto *entityManager = Engine::ECS::EntityManager::getInstance();
        entityManager->createEntity<Player>()->init();
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                entityManager->createEntity<Box>(Engine::Math::Vector2d{50 + i * 15, 10 + j * 15})->init();
        entityManager->createEntity<Wall>(Engine::Math::Rect_d{{0, 0}, {640, 10}})->init();
        entityManager->createEntity<Wall>(Engine::Math::Rect_d{{0, 470}, {640, 10}})->init();
        entityManager->createEntity<Wall>(Engine::Math::Rect_d{{0, 10}, {10, 460}})->init();
        entityManager->createEntity<Wall>(Engine::Math::Rect_d{{630, 10}, {10, 460}})->init();
        entityManager->createEntity<Wall>()->init();
        entityManager->createEntity<Engine::Render::Camera>(Engine::Math::Vector2ui(186, 140))->init();
    }
};

#endif //BACHERO_GAME_LOOP