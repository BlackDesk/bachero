#ifndef BACHERO_ENGINE_RENDER_TEXT
#define BACHERO_ENGINE_RENDER_TEXT

#include "SDL.h"
#include "SDL_ttf.h"

#include "Engine/Render/RuntimeException.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Color.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Rect.h"

#include <string>

namespace Engine::Render {
    class Text {
    public:
        Text(std::string fontName,
             std::size_t fontSize,
             Color color,
             Renderer *renderer)
                : _fontName(std::move(fontName)),
                _fontSize(fontSize),
                _renderer(renderer),
                _color(color) {
            _reloadFont();
            _reloadTexture();
        }

        void setFont(std::string fontName) {
            _fontName = std::move(fontName);
            _reloadFont();
            _reloadTexture();
        }

        void setFontSize(std::size_t fontSize) {
            _fontSize = fontSize;
            _reloadFont();
            _reloadTexture();
        }

        void setText(std::string text) {
            _text = std::move(text);
            _reloadTexture();
        }

        void setColor(Color color) {
            _color = color;
            _reloadTexture();
        }

        Math::Vector2i position;

        void render() {
            if (!_texture)
                return;

            Math::Rect_i rect;
            rect.position = position;
            rect.size = _dimensions;
            auto textRect = (SDL_Rect)rect;
            SDL_RenderCopy(_renderer->get(), _texture, NULL, &textRect);
        }

        ~Text() {
            _unloadFont();
            _unloadTexture();
        }

    private:
        void _unloadFont() {
            if (_font) {
                TTF_CloseFont(_font);
                _font = nullptr;
            }
        }

        void _reloadFont() {
            _unloadFont();

            _font = TTF_OpenFont(_fontName.c_str(), _fontSize);
            if (!_font)
                throwRenderRuntimeException("Could not open font:\n" + TTF_GetError() + "\n");
        }

        void _unloadTexture() {
            if (_texture) {
                SDL_DestroyTexture(_texture);
                _texture = nullptr;
            }
        }

        void _reloadTexture() {
            _unloadTexture();

            //no sense in rendering empty strings
            if (_text.empty())
                return;

            SDL_Surface *surface = TTF_RenderText_Solid(_font, _text.c_str(), _color);
            if (!surface)
                throwRenderRuntimeExceptionSDL("Could not create a surface for text.\n");
            _texture = SDL_CreateTextureFromSurface(_renderer->get(), surface);
            _dimensions = {surface->w, surface->h};
            SDL_FreeSurface(surface);
        }

        TTF_Font *_font = nullptr;
        SDL_Texture *_texture = nullptr;
        std::string _fontName, _text;
        std::size_t _fontSize;
        Color _color;
        Renderer *_renderer = nullptr;
        Math::Vector2ui _dimensions;
    };
}

#endif //BACHERO_ENGINE_RENDER_TEXT