#ifndef BACHERO_ENGINE_RENDER_SPRITE
#define BACHERO_ENGINE_RENDER_SPRITE

#include "Engine/Render/Texture.h"

#include "Engine/Render/RuntimeException.h"

namespace Engine::Render {
    class Sprite {
    public:
        explicit Sprite(Texture *texture) : _texture(texture) {
            if (!texture)
                throwRenderRuntimeException("Texture pointer can\'t be null.\n");
            _frameSize = texture->size();
        }

        Sprite(Texture *texture, Math::Vector2ui frameSize)
                : Sprite(texture) {
            _frameSize = frameSize;
        }

        void render() {
            Math::Rect_ui src(Math::Vector2ui(frame.x * _frameSize.x,
                                              frame.y * _frameSize.y),
                              _frameSize);
            Math::Rect_ui dst(position,
                              _frameSize);
            _texture->draw(src, dst, anchorPointLocal, rotation, flip);
        }

        Math::Vector2i position;
        Math::Vector2i anchorPointLocal;
        Math::Vector2ui frame;
        Texture::Flip flip = Texture::Flip::none;
        float rotation = 0.0f;

    private:
        Texture *_texture;
        Math::Vector2ui _frameSize;
    };
}

#endif //BACHERO_ENGINE_RENDER_SPRITE