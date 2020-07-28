#include "Engine/Render/Texture.h"

#include "Engine/Render/RuntimeException.h"

namespace Engine::Render {
    Texture *Texture::load(const std::filesystem::path &path, Renderer *renderer) {
        SDL_Surface *tempSurface = IMG_Load(path.c_str());
        if (tempSurface == nullptr)
            throwRenderRuntimeException("Could not load image: \n");
        SDL_Texture *SDL_texture = SDL_CreateTextureFromSurface(renderer->get(), tempSurface);
        SDL_FreeSurface(tempSurface);

        if (SDL_texture == nullptr)
            throwRenderRuntimeException("Could not create texture: \n");

        return new Texture(SDL_texture, renderer->get());
    }

    Math::Vector2ui Texture::size() {
        if (!_size.has_value()) {
            int width, height;
            SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
            _size = Math::Vector2ui(width, height);
        }
        return _size.value();
    }

    void Texture::draw(Math::Rect_ui src, Math::Rect_ui dst) {
        auto src_ = (SDL_Rect)src;
        auto dst_ = (SDL_Rect)dst;
        SDL_RenderCopy(_renderer, _texture, &src_, &dst_);
    }

    Texture::~Texture() {
        SDL_DestroyTexture(_texture);
    }
}