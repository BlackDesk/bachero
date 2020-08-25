#ifndef BACHERO_ENGINE_COMMON_DEBUG_MODE
#define BACHERO_ENGINE_COMMON_DEBUG_MODE

namespace Engine {
    class DebugMode {
    public:
        static void enable() {
            _enabled = true;
        }

        static void disable() {
            _enabled = false;
        }

        static void toggle() {
            _enabled = !_enabled;
        }

        static bool isEnabled() {
            return _enabled;
        }

        bool operator()() const {
            return isEnabled();
        }

        operator auto () const {
            return &DebugMode::operator();
        }

    private:
        static bool _enabled;
    };

    bool DebugMode::_enabled = false;
}

#endif //BACHERO_ENGINE_COMMON_DEBUG_MODE