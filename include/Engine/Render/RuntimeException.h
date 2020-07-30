#ifndef BACHERO_ENGINE_RENDER_RUNTIME_EXCEPTION
#define BACHERO_ENGINE_RENDER_RUNTIME_EXCEPTION

#include <stdexcept>

namespace Engine::Render {
    typedef std::runtime_error RuntimeException;

#define throwRenderRuntimeExceptionSDL(s) \
        throw RuntimeException((std::string)s \
        + "SDL: " + SDL_GetError() + "\n" \
        + "File: " + __FILENAME__ + "\n" \
        + "Line: " + std::to_string(__LINE__) + "\n")


#define throwRenderRuntimeException(s) \
        throw RuntimeException((std::string)s \
        + "File: " + __FILENAME__ + "\n" \
        + "Line: " + std::to_string(__LINE__) + "\n")
}

#endif //BACHERO_ENGINE_RENDER_RUNTIME_EXCEPTION