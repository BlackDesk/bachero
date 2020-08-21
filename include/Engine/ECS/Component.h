#ifndef BACHERO_ENGINE_ECS_COMPONENT
#define BACHERO_ENGINE_ECS_COMPONENT

#include "Engine/GameObject.h"
#include "Engine/Common/Singleton.h"

#include <cstddef>
#include <cstdint>
#include <array>
#include <memory>

namespace Engine::ECS {
    class Entity;

    typedef std::size_t ComponentTypeID;

    inline ComponentTypeID getComponentTypeID() {
        static ComponentTypeID id = 0;
        return id++;
    }

    template<class T>
    ComponentTypeID getComponentTypeID() {
        static ComponentTypeID id = getComponentTypeID();
        return id;
    }

    enum ComponentEnumType {
        DataOnly = 0,
        Behavioral = 1
    };

    //no virtual methods for handling events, updating, rendering and so on
    struct DataOnlyComponent {
        Entity *owner = nullptr;

        virtual void init() {}
    };

    class Component : public DataOnlyComponent, public GameObject {
    public:
        Entity *owner = nullptr;
    };

    template<class T>
    constexpr ComponentEnumType getComponentEnumType() {
        return std::is_base_of_v<Component, T> ?
               ComponentEnumType::Behavioral :
               ComponentEnumType::DataOnly;
    }
}

#endif //BACHERO_ENGINE_ECS_COMPONENT