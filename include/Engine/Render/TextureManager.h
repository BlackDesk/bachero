#ifndef BACHERO_ENGINE_RENDER_TEXTURE_MANAGER_H
#define BACHERO_ENGINE_RENDER_TEXTURE_MANAGER_H

#include "Engine/Render/Texture.h"
#include "Engine/Common/Singleton.h"
#include "Engine/Render/Renderer.h"

#include <map>
#include <string>
#include <filesystem>

namespace Engine::Render {
    class TextureManager : public Common::Singleton<TextureManager> {
    public:
        Texture *loadTexture(const std::filesystem::path &path, const std::string &id, Renderer *renderer);

        Texture *getTexture(const std::string &id);

        void unloadTexture(const std::string &id);

        void clean();

    private:
        std::map<std::string, Texture *> _textureMap;
    };
}

#endif //BACHERO_ENGINE_RENDER_TEXTURE_MANAGER_H
