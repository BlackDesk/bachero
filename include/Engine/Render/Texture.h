#ifndef BACHERO_ENGINE_RENDER_TEXTURE_H
#define BACHERO_ENGINE_RENDER_TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"

#include "Engine/Render/Renderer.h"
#include "Engine/Math/Rect.h"

#include <filesystem>
#include <optional>

namespace Engine::Render {
    class Texture {
    public:
        static Texture *load(const std::filesystem::path &path, Renderer *renderer);

        void draw(Math::Rect_ui src, Math::Rect_i dst);

        Math::Vector2ui size();

        ~Texture();

    private:
        Texture(SDL_Texture *texture, SDL_Renderer *renderer)
                : _texture(texture), _renderer(renderer) {}

        std::optional<Math::Vector2ui> _size;

        SDL_Renderer *_renderer = nullptr;
        SDL_Texture *_texture = nullptr;
    };
}

#endif //BACHERO_ENGINE_RENDER_TEXTURE_H