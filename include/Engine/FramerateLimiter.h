#ifndef BACHERO_ENGINE_FRAMERATE_LIMITER
#define BACHERO_ENGINE_FRAMERATE_LIMITER

#include "SDL.h"
#include <chrono>

namespace Engine {
    class FramerateLimiter {
    public:
        explicit FramerateLimiter(size_t framerate) {
            if (framerate > 0) {
                _limit = true;
                _period = 1000 / framerate;
            } else {
                _limit = false;
            }
        }

        explicit FramerateLimiter() {
            _limit = false;
        }

        void recordStart() {
            begin = std::chrono::steady_clock::now();
        }

        void recordStop() {
            end = std::chrono::steady_clock::now();
        }

        void delay() {
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
            int delay = (int)_period - delta.count();
            if (_limit && delay > 0)
                SDL_Delay(delay);
        }

    private:
        bool _limit = true;
        size_t _period = 0;
        std::chrono::steady_clock::time_point begin, end;
    };
}

#endif //BACHERO_ENGINE_FRAMERATE_LIMITER