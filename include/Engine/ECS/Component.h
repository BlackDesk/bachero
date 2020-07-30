#ifndef BACHERO_ENGINE_ECS_COMPONENT
#define BACHERO_ENGINE_ECS_COMPONENT

#include "Engine/GameObject.h"

#include <cstddef>
#include <cstdint>

namespace Engine::ECS {
    class Entity;

    typedef std::size_t ComponentTypeID;
    ComponentTypeID getComponentTypeID() {
        static ComponentTypeID id = 0;
        return id++;
    }

    template<class T>
    ComponentTypeID getComponentTypeID() {
        static ComponentTypeID id = getComponentTypeID();
        return id;
    }

    class Component : public GameObject {
    public:
        Entity *owner;
    };
}

#endif //BACHERO_ENGINE_ECS_COMPONENT