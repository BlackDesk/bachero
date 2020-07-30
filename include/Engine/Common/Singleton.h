#ifndef BACHERO_ENGINE_COMMON_SINGLETON
#define BACHERO_ENGINE_COMMON_SINGLETON

namespace Engine::Common {
    template<class T>
    class Singleton {
    public:
        Singleton(Singleton const&)=delete;
        Singleton(Singleton&&)=delete;
        auto& operator=(Singleton const&)=delete;
        auto& operator=(Singleton &&)=delete;

        static T *getInstance();

    protected:
        explicit Singleton() = default;
    private:
        static T *_instance;
    };

    template<class T>
    T *Singleton<T>::getInstance() {
        if (!_instance)
            _instance = new T();
        return _instance;
    }

    template<class T>
    T *Singleton<T>::_instance = nullptr;
}

#endif //BACHERO_ENGINE_COMMON_SINGLETON