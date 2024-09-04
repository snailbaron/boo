#include "collision.hpp"

#include <vector>

std::optional<Vector> intersection(const Line& lhs, const Line& rhs)
{
    // lhs: a1 * x + b1 * y = c1
    // rhs: a2 * x + b2 * y = c2

    float a1 = lhs.norm().x();
    float b1 = lhs.norm().y();
    float c1 = lhs.value();

    float a2 = rhs.norm().x();
    float b2 = rhs.norm().y();
    float c2 = rhs.value();

    float det = a1 * b2 - a2 * b1;
    if (det == 0) {
        return std::nullopt;
    }

    float dx = c1 * b2 - c2 * b1;
    float dy = a1 * c2 - a2 * c1;

    return Vector{dx / det, dy / det};
}

std::optional<Vector> intersection(const Ray& ray, const Segment& segment)
{
    auto x = intersection(ray.line(), segment.line());
    if (!x) {
        return std::nullopt;
    }

    if (ray.contains(*x) && segment.contains(*x)) {
        return x;
    }
    return std::nullopt;
}

Collision::operator bool() const
{
    return std::isfinite(time);
}

std::partial_ordering Collision::operator<=>(const Collision& other) const
{
    return time <=> other.time;
}

Collision collision(
    const Vector& point, const Vector& velocity, const Circle& circle)
{
    float r = circle.radius;
    Norm n = velocity.norm();
    Norm u = ccw(n);

    float d = dot(u, circle.center) - dot(u, point);
    if (d * d > r * r) {
        return {};
    }

    float lp = dot(n, circle.center) - dot(n, point);
    float lq = lp - std::sqrt(r * r - d * d);

    return Collision{
        .time = lq / velocity.len(),
        .norm = Norm{point + lq * n - circle.center},
    };
}

Collision collision(
    const Circle& circle, const Vector& velocity, const Vector& point)
{
    return collision(circle.center, velocity, Circle{point, circle.radius});
}

Collision collision(
    const Vector& point, const Vector& velocity, const Segment& segment)
{
    auto x = intersection(Ray{point, velocity}, segment);
    if (!x) {
        return {};
    }

    auto collisionNorm = segment.line().norm();
    if (dot(*x - point, collisionNorm) > 0) {
        collisionNorm = -collisionNorm;
    }

    return Collision{
        .time = (*x - point).len() / velocity.len(),
        .norm = collisionNorm,
    };
}

Collision collision(const Circle& circle, const Vector& velocity, const Rectangle& rectangle)
{
    auto angleCircles = std::vector{
        Circle{rectangle.topLeft(), circle.radius},
        Circle{rectangle.topRight(), circle.radius},
        Circle{rectangle.bottomRight(), circle.radius},
        Circle{rectangle.bottomLeft(), circle.radius},
    };

    auto sides = std::vector{
        shift(rectangle.top(), {0, circle.radius}),
        shift(rectangle.bottom(), {0, -circle.radius}),
        shift(rectangle.left(), {-circle.radius, 0}),
        shift(rectangle.right(), {circle.radius, 0}),
    };

    auto bestCollision = Collision{};
    for (const auto& angleCircle : angleCircles) {
        auto c = collision(circle.center, velocity, angleCircle);
        if (c < bestCollision) {
            bestCollision = c;
        }
    }
    for (const auto& side : sides) {
        auto c = collision(circle.center, velocity, side);
        if (c < bestCollision) {
            bestCollision = c;
        }
    }

    return bestCollision;
}
