#ifndef BACHERO_ENGINE_RENDER_SPRITE
#define BACHERO_ENGINE_RENDER_SPRITE

#include "Engine/Render/Texture.h"

#include "Engine/Render/RuntimeException.h"

namespace Engine::Render {
    class Sprite {
    public:
        explicit Sprite(Texture *texture_) : texture(texture_) {
            if (!texture)
                throwRenderRuntimeException("Texture pointer can\'t be null.\n");
            frameSize = texture->size();
        }

        Sprite(Texture *texture, Math::Vector2ui frameSize_)
                : Sprite(texture) {
            frameSize = frameSize_;
        }

        void draw(Math::Vector2f cameraPos) {
            Math::Rect_ui src(Math::Vector2ui(frame.x * frameSize.x,
                                              frame.y * frameSize.y),
                              frameSize);
            Math::Rect_ui dst(position - cameraPos,
                              frameSize);
            texture->draw(src, dst, anchorPointLocal, rotation, flip);
        }

        Texture *texture;

        Math::Vector2i position;
        Math::Vector2i anchorPointLocal;
        Math::Vector2ui frameSize;
        Math::Vector2ui frame;
        Texture::Flip flip = Texture::Flip::none;
        float rotation = 0.0f;
    };
}

#endif //BACHERO_ENGINE_RENDER_SPRITE