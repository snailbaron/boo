#include "view.hpp"

void Camera::screenSize(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
    recalculateCorner();
}

float Camera::projectX(float worldX) const
{
    return (worldX - _worldCorner.x) * _pixelsPerUnit * _zoom;
}

float Camera::projectY(float worldY) const
{
    return (_worldCorner.y - worldY) * _pixelsPerUnit * _zoom;
}

Vector Camera::project(const Vector& worldPoint) const
{
    return {projectX(worldPoint.x), projectY(worldPoint.y)};
}

SDL_FRect Camera::project(const Rectangle& worldRectangle) const
{
    return SDL_FRect{
        .x = projectX(worldRectangle.xmin()),
        .y = projectY(worldRectangle.ymax()),
        .w = worldRectangle.w() * _pixelsPerUnit * _zoom,
        .h = worldRectangle.h() * _pixelsPerUnit * _zoom,
    };
}

SDL_FRect Camera::project(const Circle& worldCircle) const
{
    return SDL_FRect{
        .x = projectX(worldCircle.center.x - worldCircle.radius),
        .y = projectY(worldCircle.center.y + worldCircle.radius),
        .w = worldCircle.radius * 2 * _pixelsPerUnit * _zoom,
        .h = worldCircle.radius * 2 * _pixelsPerUnit * _zoom,
    };
}

void Camera::recalculateCorner()
{
    _worldCorner = {
        -1.f * _screenWidth / (2 * _pixelsPerUnit * _zoom),
        1.f * _screenHeight / (_pixelsPerUnit * _zoom)
    };
}

View::View(Window& window, Resources& resources)
    : _window(window)
    , _resources(resources)
{
    auto [w, h] = _window.size();
    _camera.screenSize(w, h);
}

void View::render(const World& world)
{
    auto& renderer = _window.renderer();

    const auto& brickSprite = _resources[r::Sprite::Brick];
    const auto& padSprite = _resources[r::Sprite::Platform];
    const auto& ballSprite = _resources[r::Sprite::Ball];

    renderer.clear();

    for (const auto& brick : world.bricks()) {
        renderer.copy(
            *brickSprite.texture,
            brickSprite.frames.front().rect,
            _camera.project(brick));
    }

    renderer.copy(
        *padSprite.texture,
        padSprite.frames.front().rect,
        _camera.project(world.pad()));
    renderer.copy(
        *ballSprite.texture,
        ballSprite.frames.front().rect,
        _camera.project(world.ball()));

    renderer.present();
}

