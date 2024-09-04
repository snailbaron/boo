#include "world.hpp"

#include "collision.hpp"

#include <algorithm>

void World::setupTestLevel()
{
    _bricks = {
        Rectangle{{-10, 15}, 2, 1},
        Rectangle{{-8, 13}, 2, 1},
        Rectangle{{0, 13}, 2, 1},
        Rectangle{{5, 16}, 2, 1},
    };
}

void World::update(float delta)
{
    auto bestCollision = Collision{};

    for (const auto& brick : _bricks) {
        auto c = collision(_ball, _ballVelocity, brick);
        if (c < bestCollision) {
            bestCollision = c;
        }
    }
}

void World::setPadPosition(float pos)
{
    pos = std::clamp(pos, 0.f, 1.f);
    float padMinX = _minx + _pad.w() / 2;
    float padMaxX = _maxx - _pad.w() / 2;
    _pad.moveTo({padMinX * (1 - pos) + padMaxX * pos, _pad.center().y});
}

const std::vector<Rectangle>& World::bricks() const
{
    return _bricks;
}

const Rectangle& World::pad() const
{
    return _pad;
}

const Circle& World::ball() const
{
    return _ball;
}
