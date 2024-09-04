#pragma once

#include <cmath>
#include <concepts>
#include <ostream>

class Norm;

struct Vector {
    Vector() = default;
    Vector(float x, float y);

    Vector(const Norm& norm);

    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(float scalar);
    Vector& operator/=(float scalar);

    float sqLen() const;
    float len() const;
    Norm norm() const;

    float x = 0.f;
    float y = 0.f;
};

Vector operator-(const Vector& vector);
Vector operator+(Vector lhs, const Vector& rhs);
Vector operator-(Vector lhs, const Vector& rhs);
Vector operator*(Vector vector, float scalar);
Vector operator*(float scalar, Vector vector);
Vector operator/(Vector vector, float scalar);

float dot(const Vector& lhs, const Vector& rhs);
Vector ccw(const Vector& vector);

std::ostream& operator<<(std::ostream& output, const Vector& vector);

class Norm {
public:
    Norm() = default;
    explicit Norm(float x, float y);
    explicit Norm(const Vector& vector);

    float x() const;
    float y() const;

private:
    struct NoCheck {};
    explicit Norm(NoCheck, float x, float y);

    friend Norm operator-(const Norm& norm);

    friend Norm cw(const Norm& norm);
    friend Norm ccw(const Norm& norm);

    float _x = 0.f;
    float _y = 0.f;
};


struct Circle {
    Vector center;
    float radius = 0.f;
};

class Line {
public:
    Line(const Norm& norm, float value);

    static Line fromPointAndDirection(
        const Vector& point, const Vector& direction);
    static Line betweenPoints(const Vector& u, const Vector& v);

    const Norm& norm() const;
    Norm direction() const;
    float value() const;
    float coordinate(const Vector& v) const;
    Vector pointAtCoordinate(float coordinate) const;

private:
    Norm _norm;
    float _value = 0.f;
};

class Ray {
public:
    Ray(Line line, float startValue);
    Ray(const Vector& point, const Vector& direction);

    const Line& line() const;
    bool contains(const Vector& point) const;

private:
    Line _line;
    float _startValue = 0.f;
};

class Segment {
public:
    Segment(Line line, float startValue, float endValue);
    Segment(const Vector& u, const Vector& v);

    const Line& line() const;
    bool contains(const Vector& point) const;

    Vector start() const;
    Vector end() const;

private:
    Line _line;
    float _startValue = 0.f;
    float _endValue = 0.f;
};

Segment shift(const Segment& segment, const Vector& offset);

class Rectangle {
public:
    Rectangle(const Vector& center, float w, float h);

    Vector topLeft() const;
    Vector topRight() const;
    Vector bottomLeft() const;
    Vector bottomRight() const;

    Segment top() const;
    Segment bottom() const;
    Segment left() const;
    Segment right() const;

    float xmin() const;
    float xmax() const;
    float ymin() const;
    float ymax() const;

    float w() const;
    float h() const;
    Vector center() const;

    void moveTo(const Vector& newCenter);

private:
    float _xmin = 0.f;
    float _xmax = 0.f;
    float _ymin = 0.f;
    float _ymax = 0.f;
};