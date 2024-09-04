#include "geometry.hpp"

Norm::Norm(float x, float y)
{
    auto l = std::sqrt(x * x + y * y);
    _x = x / l;
    _y = y / l;
}

Norm::Norm(const Vector& vector)
    : Norm(vector.x, vector.y)
{ }

Norm::Norm(NoCheck, float x, float y)
    : _x(x)
    , _y(y)
{ }

float Norm::x() const
{
    return _x;
}

float Norm::y() const
{
    return _y;
}

Norm cw(const Norm& norm)
{
    return Norm{Norm::NoCheck{}, norm.y(), -norm.x()};
}

Norm ccw(const Norm& norm)
{
    return Norm{Norm::NoCheck{}, -norm.y(), norm.x()};
}

Norm operator-(const Norm& norm)
{
    return Norm{Norm::NoCheck{}, -norm.x(), -norm.y()};
}

Segment shift(const Segment& segment, const Vector& offset)
{
    return Segment{segment.start() + offset, segment.end() + offset};
}

Vector::Vector(float x, float y)
    : x(x)
    , y(y)
{ }

Vector::Vector(const Norm& norm)
    : x(norm.x())
    , y(norm.y())
{ }

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector& Vector::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector& Vector::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

float Vector::sqLen() const
{
    return x * x + y * y;
}

float Vector::len() const
{
    return std::sqrt(sqLen());
}

Norm Vector::norm() const
{
    return Norm{x, y};
}

Vector operator-(const Vector& vector)
{
    return {-vector.x, -vector.y};
}

Vector operator+(Vector lhs, const Vector& rhs)
{
    lhs += rhs;
    return lhs;
}

Vector operator-(Vector lhs, const Vector& rhs)
{
    lhs -= rhs;
    return lhs;
}

Vector operator*(Vector vector, float scalar)
{
    vector *= scalar;
    return vector;
}

Vector operator*(float scalar, Vector vector)
{
    vector *= scalar;
    return vector;
}

Vector operator/(Vector vector, float scalar)
{
    vector /= scalar;
    return vector;
}

float dot(const Vector& lhs, const Vector& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Vector ccw(const Vector& vector)
{
    return Vector{-vector.y, vector.x};
}

std::ostream& operator<<(std::ostream& output, const Vector& vector)
{
    return output << "(" << vector.x << ", " << vector.y << ")";
}

Rectangle::Rectangle(const Vector& center, float w, float h)
    : _xmin(center.x - w / 2)
    , _xmax(center.x + w / 2)
    , _ymin(center.y - h / 2)
    , _ymax(center.y + h / 2)
{ }

Vector Rectangle::topLeft() const
{
    return {_xmin, _ymax};
}

Vector Rectangle::topRight() const
{
    return {_xmax, _ymax};
}

Vector Rectangle::bottomLeft() const
{
    return {_xmin, _ymin};
}

Vector Rectangle::bottomRight() const
{
    return {_xmax, _ymin};
}

Segment Rectangle::top() const
{
    return Segment{topLeft(), topRight()};
}

Segment Rectangle::bottom() const
{
    return Segment{bottomLeft(), bottomRight()};
}

Segment Rectangle::left() const
{
    return Segment{topLeft(), bottomLeft()};
}

Segment Rectangle::right() const
{
    return Segment{topRight(), bottomRight()};
}

float Rectangle::xmin() const
{
    return _xmin;
}

float Rectangle::xmax() const
{
    return _xmax;
}

float Rectangle::ymin() const
{
    return _ymin;
}

float Rectangle::ymax() const
{
    return _ymax;
}

float Rectangle::w() const
{
    return _xmax - _xmin;
}

float Rectangle::h() const
{
    return _ymax - _ymin;
}

Vector Rectangle::center() const
{
    return {(_xmin + _xmax) / 2, (_ymin + _ymax) / 2};
}

void Rectangle::moveTo(const Vector& newCenter)
{
    auto offset = newCenter - center();
    _xmin += offset.x;
    _xmax += offset.x;
    _ymin += offset.y;
    _ymax += offset.y;
}

Line::Line(const Norm& norm, float value)
    : _norm(norm)
    , _value(value)
{ }

Line Line::fromPointAndDirection(const Vector& point, const Vector& direction)
{
    Norm norm = ccw(Norm{direction});
    return Line{norm, dot(norm, point)};
}

Line Line::betweenPoints(const Vector& u, const Vector& v)
{
    return Line::fromPointAndDirection(u, v - u);
}

const Norm& Line::norm() const
{
    return _norm;
}

Norm Line::direction() const
{
    return cw(_norm);
}

float Line::value() const
{
    return _value;
}

float Line::coordinate(const Vector& v) const
{
    return dot(direction(), v);
}

Vector Line::pointAtCoordinate(float coordinate) const
{
    auto origin = norm() * _value;
    return origin + direction() * coordinate;
}

Ray::Ray(Line line, float startValue)
    : _line(line)
    , _startValue(startValue)
{ }

Ray::Ray(const Vector& point, const Vector& direction)
    : _line(Line::fromPointAndDirection(point, direction))
    , _startValue(_line.coordinate(point))
{ }

const Line& Ray::line() const
{
    return _line;
}

bool Ray::contains(const Vector& point) const
{
    auto c = _line.coordinate(point);
    return c >= _startValue;
}

Segment::Segment(Line line, float startValue, float endValue)
    : _line(line)
    , _startValue(startValue)
    , _endValue(endValue)
{ }

Segment::Segment(const Vector& u, const Vector& v)
    : _line(Line::betweenPoints(u, v))
    , _startValue(_line.coordinate(u))
    , _endValue(_line.coordinate(v))
{ }

const Line& Segment::line() const
{
    return _line;
}

bool Segment::contains(const Vector& point) const
{
    auto c = _line.coordinate(point);
    return c >= _startValue && c <= _endValue;
}

Vector Segment::start() const
{
    return _line.pointAtCoordinate(_startValue);
}

Vector Segment::end() const
{
    return _line.pointAtCoordinate(_endValue);
}
