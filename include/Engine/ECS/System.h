#ifndef BACHERO_ENGINE_ECS_SYSTEM
#define BACHERO_ENGINE_ECS_SYSTEM

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Component.h"

namespace Engine::ECS {
    class System : public GameObject {
    public:
        virtual void init() {}
    };

    class SystemManager : public Singleton<SystemManager> {
    public:
        SystemManager() = default;

        template<class T, typename ...Args>
        System *createSystem(Args &&...args) {
            auto *system = new T(std::forward<Args>(args)...);
            _systems.emplace_back(std::unique_ptr<T>(system));
            return system;
        }

        void handleEvents() {
            for (auto &system : _systems)
                system->handleEvents();
        }

        void update() {
            for (auto &system : _systems)
                system->update();
        }

        void render() {
            for (auto &system : _systems)
                system->render();
        }

        void clean() {
            _systems.clear();
        }

    private:
        std::vector<std::unique_ptr<System>> _systems;
    };
}

#endif //BACHERO_ENGINE_ECS_SYSTEM