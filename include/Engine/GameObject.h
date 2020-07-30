#ifndef BACHERO_ENGINE_GAMEOBJECT
#define BACHERO_ENGINE_GAMEOBJECT

namespace Engine {
    class GameObject {
    public:
        virtual void handleEvents() {}

        virtual void update() {}

        virtual void render() {}

        virtual ~GameObject() = default;
    };
}

#endif //BACHERO_ENGINE_GAMEOBJECT