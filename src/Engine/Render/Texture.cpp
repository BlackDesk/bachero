#include "Engine/Render/Texture.h"

#include "Engine/Render/RuntimeException.h"

namespace Engine::Render {
    Texture *Texture::load(const std::filesystem::path &path, Renderer *renderer) {
        SDL_Surface *tempSurface = IMG_Load(path.c_str());
        if (tempSurface == nullptr)
            throwRenderRuntimeExceptionSDL("Could not load image: \n");
        SDL_Texture *SDL_texture = SDL_CreateTextureFromSurface(renderer->get(), tempSurface);
        SDL_FreeSurface(tempSurface);

        if (SDL_texture == nullptr)
            throwRenderRuntimeExceptionSDL("Could not create texture: \n");

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

    void Texture::draw(Math::Rect_ui src, Math::Rect_i dst) {
        auto src_ = (SDL_Rect)src;
        auto dst_ = (SDL_Rect)dst;
        SDL_RenderCopy(_renderer, _texture, &src_, &dst_);
    }

    void Texture::draw(Math::Rect_ui src, Math::Rect_i dst, Flip flip) {
        auto src_ = (SDL_Rect)src;
        auto dst_ = (SDL_Rect)dst;
        SDL_RenderCopyEx(_renderer, _texture, &src_, &dst_, 0, nullptr, (SDL_RendererFlip)flip);
    }

    void Texture::draw(Math::Rect_ui src, Math::Rect_i dst,
                       Math::Vector2f rotPnt, double angle, Flip flip) {
        auto src_ = (SDL_Rect)src;
        auto dst_ = (SDL_Rect)dst;
        auto pnt_ = SDL_Point{(int)rotPnt.x, (int)rotPnt.y};

        constexpr double toDeg = 180.0 / M_PI;

        SDL_RenderCopyEx(_renderer, _texture, &src_, &dst_, angle * toDeg, &pnt_, (SDL_RendererFlip)flip);
    }

    Texture::~Texture() {
        SDL_DestroyTexture(_texture);
    }
}