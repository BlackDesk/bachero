#ifndef BACHERO_ENGINE_RENDER_RUNTIME_EXCEPTION
#define BACHERO_ENGINE_RENDER_RUNTIME_EXCEPTION

#include <stdexcept>

namespace Engine::Render {
    typedef std::runtime_error RuntimeException;

#define throwRenderRuntimeException(s) throw RuntimeException((std::string)s + SDL_GetError())
}

#endif //BACHERO_ENGINE_RENDER_RUNTIME_EXCEPTION