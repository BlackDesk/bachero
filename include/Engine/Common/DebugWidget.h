#ifndef BACHERO_ENGINE_COMMON_DEBUG_WIDGET
#define BACHERO_ENGINE_COMMON_DEBUG_WIDGET

#include "Engine/Common/DeltaTime.h"
#include "Engine/Render/Text.h"
#include "Engine/Render/TextureManager.h"

namespace Engine {
    class DebugWidget {
    public:
        DebugWidget() {
            _text = std::make_unique<Render::Text>("assets/Open_Sans/OpenSans-regular.ttf",
                                                    30,
                                                    Render::Color{0, 0, 0, 255},
                                                    Render::TextureManager::getInstance()->getDefaultRenderer());
            memset(_freqQueue, 0, sizeof(_freqQueue));
        }

        void update() {
            float dt = DeltaTime::get();
            float freq = (dt > 0.0f ? 1.0f / dt : 0.0f);

            _freqSum += freq;
            _freqSum -= _freqQueue[_freqQueuePnt];
            _freqQueue[_freqQueuePnt] = freq;
            _freqQueuePnt = (_freqQueuePnt + 1) % _freqQueueSize;

            int fps = (int)std::round(_freqSum / _freqQueueSize);

            if (fps != _pFPS) {
                _text->setText(std::to_string(fps));
                _pFPS = fps;
            }
        }

        void render() {
            _text->render();
        }

    private:
        static const std::size_t _freqQueueSize = 200;
        float _freqQueue[_freqQueueSize];
        std::size_t _freqQueuePnt = 0;
        float _freqSum = 0.0f;

        int _pFPS = -1;
        std::unique_ptr<Render::Text> _text;
    };
}

#endif //BACHERO_ENGINE_COMMON_DEBUG_WIDGET