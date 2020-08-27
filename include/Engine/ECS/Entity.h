#ifndef BACHERO_ENGINE_ECS_ENTITY
#define BACHERO_ENGINE_ECS_ENTITY

#include "Engine/GameObject.h"
#include "Engine/ECS/Component.h"
#include "Engine/Common/Singleton.h"

#include <array>
#include <bitset>
#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>

namespace Engine::ECS {
    class Entity : public GameObject {
    public:
        template<class T>
        bool hasComponent() const {
            return _presence[getComponentTypeID<T>()];
        }

        template<class ...Ts>
        bool hasComponents() const {
            static auto mask = _genComponentsMask<Ts...>();

            return mask == (mask & _presence);
        }

        template<class T>
        T *getComponent() const {
            static auto id = getComponentTypeID<T>();

            if (!_presence[id])
                throw std::runtime_error("Entity doesn't have that component.");

            return _getComponent<T>(Int2Type<getComponentEnumType<T>()>());
        }

        template<class T, typename ...Args>
        T *addComponent(Args &&...args) {
            static auto id = getComponentTypeID<T>();

            if (_presence[id])
                throw std::runtime_error("Entity already has that component.");
            _presence.set(id);

            return _addComponent<T>(Int2Type<getComponentEnumType<T>()>(), std::forward<Args>(args)...);
        }

        template<class T>
        void removeComponent() {
            static auto id = getComponentTypeID<T>();
            if (!_presence[id])
                throw std::runtime_error("Entity doesn't have that component.");
            _presence.reset(id);
            _removeComponent<T>(Int2Type<getComponentEnumType<T>()>());
        }

        virtual void init() final {
            for (auto &component : _dataOnlyComponents)
                if (component)
                    component->init();
            for (auto &component : _components)
                if (component)
                    component->init();
        }

        void handleEvents() final {
            for (auto &component : _components)
                if (component)
                    component->handleEvents();
        }

        void update() final {
            for (auto &component : _components)
                if (component)
                    component->update();
        }

        void render() final {
            for (auto &component : _components)
                if (component)
                    component->render();
        }

        virtual void destroy() final {
            _isActive = false;
        }

        virtual bool isActive() const final {
            return _isActive;
        }

    private:
        template<int I>
        struct Int2Type {
            enum {
                value = I
            };
        };

        template<class T>
        T *_getComponent(Int2Type<ComponentEnumType::Behavioral>) const {
            static auto id = getComponentTypeID<T>();
            return static_cast<T *>(_components[id].get());
        }

        template<class T>
        T *_getComponent(Int2Type<ComponentEnumType::DataOnly>) const {
            static auto id = getComponentTypeID<T>();
            return static_cast<T *>(_dataOnlyComponents[id].get());
        }

        template<class T, typename ...Args>
        T *_addComponent(Int2Type<ComponentEnumType::Behavioral>, Args &&...args) {
            static auto id = getComponentTypeID<T>();

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            T *compPtr = component.get();

            _components[id] = std::move(component);
            _components[id]->owner = this;

            return compPtr;
        }

        template<class T, typename ...Args>
        T *_addComponent(Int2Type<ComponentEnumType::DataOnly>, Args &&...args) {
            static auto id = getComponentTypeID<T>();

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            T *compPtr = component.get();

            _dataOnlyComponents[id] = std::move(component);
            _dataOnlyComponents[id]->owner = this;

            return compPtr;
        }

        template<class T>
        void _removeComponent(Int2Type<ComponentEnumType::Behavioral>) {
            static auto id = getComponentTypeID<T>();
            //replacing with empty ptr, the previous will be destroyed
            _components[id] = std::unique_ptr<T>();
        }

        template<class T>
        void _removeComponent(Int2Type<ComponentEnumType::DataOnly>) {
            static auto id = getComponentTypeID<T>();
            //replacing with empty ptr, the previous will be destroyed
            _dataOnlyComponents[id] = std::unique_ptr<T>();
        }

        static const size_t _maxComponents = 32;
        typedef std::array<std::unique_ptr<Component>, _maxComponents> ComponentsArray;
        typedef std::array<std::unique_ptr<DataOnlyComponent>, _maxComponents> DataOnlyComponentsArray;
        typedef std::bitset<_maxComponents> ComponentsBitset;

        template<typename ...Ts>
        static ComponentsBitset _genComponentsMask() {
            static std::vector<ComponentTypeID> ids{
                    getComponentTypeID<Ts>()...
            };

            ComponentsBitset bitset;
            for (auto id : ids)
                bitset.set(id);

            return bitset;
        }

        ComponentsArray _components;
        DataOnlyComponentsArray _dataOnlyComponents;
        ComponentsBitset _presence;

        bool _isActive = true;
    };

    class EntityFactory {
    public:
        virtual Entity *create() = 0;
    };

    class EntityManager : public Singleton<EntityManager> {
    public:
        template<class T, typename ...Args>
        T *createEntity(Args &&...args) {
            auto *entity = new T(std::forward<Args>(args)...);
            _entities.emplace_back(std::unique_ptr<T>(entity));
            return entity;
        }

        template<class T>
        T *createEntityWithFactory() {
            static auto id = std::type_index(typeid(T));
            auto *entity = const_cast<T *>(_factories[id]->create());
            _entities.emplace_back(std::unique_ptr<T>(entity));
            return entity;
        }

        template<class T>
        void registerFactory(EntityFactory *factory) {
            static auto id = std::type_index(typeid(T));
            _factories[id] = factory;
        }

        template<typename ...Ts>
        void getEntitiesThatHaveComponents(std::vector<Entity *> &result, bool includeNonActive = false) const {
            result.clear();
            for (auto &entity : _entities)
                if ((includeNonActive || entity->isActive()) && entity->hasComponents<Ts...>())
                    result.emplace_back(entity.get());
            if (includeNonActive)
                for (auto &entity : _entitiesToBeRemoved)
                    if (entity->hasComponents<Ts...>())
                        result.emplace_back(entity.get());
        }

        template<typename T>
        void getEntitiesThatHaveComponent(std::vector<Entity *> &result, bool includeNonActive = false) const {
            result.clear();
            for (auto &entity : _entities)
                if ((includeNonActive || entity->isActive()) && entity->hasComponent<T>())
                    result.emplace_back(entity.get());
            if (includeNonActive)
                for (auto &entity : _entitiesToBeRemoved)
                    if (entity->hasComponent<T>())
                        result.emplace_back(entity.get());
        }

        template<typename ...Ts>
        [[nodiscard]] std::vector<Entity *> getEntitiesThatHaveComponents(bool includeNonActive = false) const {
            std::vector<Entity *> result;
            getEntitiesThatHaveComponents<Ts...>(result, includeNonActive);
            return result;
        }

        //more optimized way to retrieve one component
        template<typename T>
        [[nodiscard]] std::vector<Entity *> getEntitiesThatHaveComponent(bool includeNonActive = false) const {
            std::vector<Entity *> result;
            getEntitiesThatHaveComponent<T>(result, includeNonActive);
            return result;
        }

        //O(n)
        void refresh() {
            //two step removal system
            // to ensure that all the systems updated their structures regardless the moment of destroy
            _entitiesToBeRemoved.clear();
            std::vector<std::unique_ptr<Entity>> _entitiesBuf;
            std::partition_copy(std::make_move_iterator(_entities.begin()),
                                std::make_move_iterator(_entities.end()),
                                std::back_inserter(_entitiesToBeRemoved), std::back_inserter(_entitiesBuf),
                                [](const auto &entity) -> bool {
                                    return !entity->isActive();
                                }
            );
            _entities = std::move(_entitiesBuf);
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

        void clean() {
            _entities.clear();
        }

    private:
        std::vector<std::unique_ptr<Entity>> _entities;
        std::vector<std::unique_ptr<Entity>> _entitiesToBeRemoved;

        std::unordered_map<std::type_index, EntityFactory *> _factories;
    };
}

#endif //BACHERO_ENGINE_ECS_ENTITY