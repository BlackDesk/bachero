#ifndef BACHERO_ENGINE_ECS_ENTITY
#define BACHERO_ENGINE_ECS_ENTITY

#include "Engine/GameObject.h"
#include "Engine/ECS/Component.h"
#include "Engine/Common/Singleton.h"

#include <array>
#include <bitset>
#include <memory>
#include <vector>

namespace Engine::ECS {
    class Entity : public GameObject {
    public:
        template<class T>
        bool hasComponent() {
            return _presence[getComponentTypeID<T>()];
        }

        template<class ...Ts>
        bool hasComponents() {
            static std::vector<ComponentTypeID> ids{
                    getComponentTypeID<Ts>()...
            };

            bool res = true;
            for (auto &id : ids)
                res &= _presence[id];

            return res;
        }

        template<class T>
        T *getComponent() {
            auto id = getComponentTypeID<T>();
            if (!_presence[id])
                throw std::runtime_error("Entity doesn't have that component.");
            return static_cast<T *>(_components[id].get());
        }

        template<class T, typename ...Args>
        void addComponent(Args &&...args) {
            auto id = getComponentTypeID<T>();
            if (_presence[id])
                throw std::runtime_error("Entity already has that component.");
            _presence.set(id);
            _components[id] = std::make_unique<T>(std::forward<Args>(args)...);
            _components[id]->owner = this;
        }

        template<class T>
        void removeComponent() {
            auto id = getComponentTypeID<T>();
            if (!_presence[id])
                throw std::runtime_error("Entity doesn't have that component.");
            _presence.reset(id);
            //replacing with empty ptr, the previous will be destroyed
            _components[id] = std::unique_ptr<T>();
        }

        void handleEvents() override {
            for (auto &component : _components)
                if (component)
                    component->handleEvents();
        }

        void update() override {
            for (auto &component : _components)
                if (component)
                    component->update();
        }

        void render() override {
            for (auto &component : _components)
                if (component)
                    component->render();
        }

        virtual void destroy() {
            _isActive = false;
        }

        virtual bool isActive() {
            return _isActive;
        }

    private:
        static const size_t _maxComponents = 32;
        typedef std::array<std::unique_ptr<Component>, _maxComponents> ComponentsArray;
        typedef std::bitset<_maxComponents> ComponentsBitset;
        ComponentsArray _components;
        ComponentsBitset _presence;

        bool _isActive = true;
    };

    class EntityManager : public Common::Singleton<EntityManager> {
    public:
        template<class T, typename ...Args>
        Entity *createEntity(Args &&...args) {
            auto *entity = new T(std::forward<Args>(args)...);
            _entities.emplace_back(std::unique_ptr<T>(entity));
            return entity;
        }

        //O(n) sadly
        void refresh() {
            _entities.erase(std::remove_if(_entities.begin(), _entities.end(),
                                           [](auto &ptr) {
                                               return !ptr->isActive();
                                           }), _entities.end());
        }

        void handleEvents() {
            for (auto &entity : _entities)
                entity->handleEvents();
        }

        void update() {
            for (auto &entity : _entities)
                entity->update();
        }

        void render() {
            for (auto &entity : _entities)
                entity->render();
        }

    private:
        std::vector<std::unique_ptr<Entity>> _entities;
    };
}

#endif //BACHERO_ENGINE_ECS_ENTITY