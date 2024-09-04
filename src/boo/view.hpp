#pragma once

#include "resources.hpp"
#include "window.hpp"
#include "world.hpp"

class Camera {
public:
    void screenSize(int width, int height);

    float projectX(float worldX) const;
    float projectY(float worldY) const;

    Vector project(const Vector& worldPoint) const;
    SDL_FRect project(const Rectangle& worldRectangle) const;
    SDL_FRect project(const Circle& worldCircle) const;

private:
    void recalculateCorner();

    Vector _worldCorner;
    int _screenWidth = 0;
    int _screenHeight = 0;
    int _pixelsPerUnit = 32;
    int _zoom = 1;
};

class View {
public:
    View(Window& window, Resources& resources);

    void render(const World& world);

private:
    Window& _window;
    Resources& _resources;
    Camera _camera;
};