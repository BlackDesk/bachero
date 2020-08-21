#include "Engine/Render/TextureManager.h"

#include "Engine/Render/RuntimeException.h"

namespace Engine::Render {
    Texture *TextureManager::loadTexture(const std::filesystem::path &path,
                                         const std::string &id,
                                         Renderer *renderer) {
        auto *texture = Texture::load(path, renderer);

        // everything went ok, add the texture to our list
        _textureMap[id] = texture;
        return texture;
    }

    Texture *TextureManager::loadTexture(const std::filesystem::path &path,
                                         const std::string &id) {
        return loadTexture(path, id, _defaultRenderer);
    }

    Texture *TextureManager::getTexture(const std::string &id) {
        //throws exception if texture with that id is not presented
        return _textureMap.at(id);
    }

    void TextureManager::unloadTexture(const std::string &id) {
        delete _textureMap[id];
        _textureMap.erase(id);
    }

    void TextureManager::clean() {
        for (auto&[id, texture] : _textureMap)
            delete texture;
        _textureMap.clear();
    }

    void TextureManager::setDefaultRenderer(Renderer *renderer) {
        _defaultRenderer = renderer;
    }

    Renderer *TextureManager::getDefaultRenderer() {
        return _defaultRenderer;
    }
}