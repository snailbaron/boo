#pragma once

#include "sdl.hpp"

struct Size {
    int w = 0;
    int h = 0;
};

class Window {
public:
    Window();

    Size size();

    sdl::Renderer& renderer();

private:
    sdl::Window _window;
    sdl::Renderer _renderer;
};