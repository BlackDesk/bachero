#ifndef BACHERO_ENGINE_COMMON_DEBUG_WIDGET
#define BACHERO_ENGINE_COMMON_DEBUG_WIDGET

#include "Engine/Common/DeltaTime.h"
#include "Engine/Render/Text.h"
#include "Engine/Render/TextureManager.h"

namespace Engine {
    class DebugWidget {
    public:
        DebugWidget() {
            _text1 = std::make_unique<Render::Text>("assets/Open_Sans/OpenSans-regular.ttf",
                                                    30,
                                                    Render::Color{0, 0, 0, 255},
                                                    Render::TextureManager::getInstance()->getDefaultRenderer());
            _text2 = std::make_unique<Render::Text>("assets/Open_Sans/OpenSans-regular.ttf",
                                                    30,
                                                    Render::Color{0, 0, 0, 255},
                                                    Render::TextureManager::getInstance()->getDefaultRenderer());
            _text2->position.y = 35;
        }

        void update() {
            double dt = DeltaTime::get();
            pdt1 = pdt1 * (1 - k1) + dt * k1;
            if (pdt1 > 0)
                _text1->setText(std::to_string((int) (1.0 / pdt1)));
            pdt2 = pdt2 * (1 - k2) + dt * k2;
            if (pdt1 > 0)
                _text2->setText(std::to_string((int) (1.0 / pdt2)));
        }

        void render() {
            _text1->render();
            _text2->render();
        }

    private:
        const double k1 = 0.6, k2 = 0.01;
        double pdt1 = 0, pdt2 = 0;
        std::unique_ptr<Render::Text> _text1;
        std::unique_ptr<Render::Text> _text2;
    };
}

#endif //BACHERO_ENGINE_COMMON_DEBUG_WIDGET