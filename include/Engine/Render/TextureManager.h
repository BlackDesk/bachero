#ifndef BACHERO_ENGINE_RENDER_TEXTURE_MANAGER_H
#define BACHERO_ENGINE_RENDER_TEXTURE_MANAGER_H

#include "Engine/Render/Texture.h"
#include "Engine/Common/Singleton.h"
#include "Engine/Render/Renderer.h"

#include <map>
#include <string>
#include <filesystem>

namespace Engine::Render {
    class TextureManager : public Singleton<TextureManager> {
    public:
        void setDefaultRenderer(Renderer *renderer);
        Renderer *getDefaultRenderer();

        Texture *loadTexture(const std::filesystem::path &path, const std::string &id, Renderer *renderer);
        Texture *loadTexture(const std::filesystem::path &path, const std::string &id);

        Texture *getTexture(const std::string &id);

        void unloadTexture(const std::string &id);

        void clean();

    private:
        std::map<std::string, Texture *> _textureMap;
        Renderer *_defaultRenderer;
    };
}

#endif //BACHERO_ENGINE_RENDER_TEXTURE_MANAGER_H
