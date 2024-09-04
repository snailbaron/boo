#pragma once

#include "geometry.hpp"

#include <vector>

class World {
public:
    void setupTestLevel();

    void update(float delta);
    void setPadPosition(float pos);

    const std::vector<Rectangle>& bricks() const;
    const Rectangle& pad() const;
    const Circle& ball() const;

private:
    float _minx = -10;
    float _maxx = 10;
    float _miny = 0;
    float _maxy = 8;

    std::vector<Rectangle> _bricks;
    Rectangle _pad{{0, 2}, 5, 1};
    Circle _ball;
    Vector _ballVelocity;
};