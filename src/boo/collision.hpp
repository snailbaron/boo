#pragma once

#include "geometry.hpp"

#include <limits>
#include <optional>

std::optional<Vector> intersection(const Line& lhs, const Line& rhs);
std::optional<Vector> intersection(const Ray& ray, const Segment& segment);

struct Collision {
    explicit operator bool() const;
    std::partial_ordering operator<=>(const Collision& other) const;

    float time = std::numeric_limits<float>::infinity();
    Norm norm;
};

Collision collision(
    const Vector& point, const Vector& velocity, const Circle& circle);
Collision collision(
    const Circle& circle, const Vector& velocity, const Vector& point);

Collision collision(
    const Vector& point, const Vector& velocity, const Segment& segment);

Collision collision(
    const Circle& circle, const Vector& velocity, const Rectangle& rectangle);