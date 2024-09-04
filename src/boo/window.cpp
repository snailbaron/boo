#include "window.hpp"

#include "config.hpp"

Window::Window()
    : _window(
        config.windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.screenWidth,
        config.screenHeight,
        0)
    , _renderer(
        _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
{ }

Size Window::size()
{
    auto size = _window.size();
    return {size.w, size.h};
}

sdl::Renderer& Window::renderer()
{
    return _renderer;
}
