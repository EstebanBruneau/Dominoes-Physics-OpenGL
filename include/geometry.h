#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <iostream>


class Coordinates
{
public:
    double x, y, z;
    Coordinates(double xx=0, double yy=0, double zz=0) {x=xx; y=yy; z=zz;}
};


// Declaration in order to use it within Point methods
class Vector;

class Point : public Coordinates
{
public:
    // Point constructor calls the base class constructor and do nothing more
    Point(double xx=0, double yy=0, double zz=0) : Coordinates(xx, yy, zz) {}
    void translate(const Vector &);
    Point translate2(const Vector &v) const;
    Point operator+(const Vector &v);
};


class Vector : public Coordinates
{
public:
    // Instantiates a Vector from its coordinates
    Vector(double xx=0, double yy=0, double zz=0) : Coordinates(xx, yy, zz) {}
    Vector(Point, Point);

    double norm();
    Vector integral(double delta_t);
    Vector cross(const Vector &v);
    void operator+=(const Vector &v);
    float dot(const Vector& v) const {return x * v.x + y * v.y + z * v.z;}
};


// Compute the distance between two points
double distance(Point p1, Point p2);

// Overloaded standard operators
std::ostream& operator<<(std::ostream& os, const Coordinates& coord);
Vector operator+(const Vector &v1, const Vector &v2);
Vector operator-(const Vector &v);
Vector operator-(const Vector &v1, const Vector &v2);
Vector operator*(const double &k, const Vector &v);
// Scalar product
double operator*(const Vector &v1, const Vector &v2);
// Vector product
Vector operator^(const Vector &v1, const Vector &v2);

Point operator+(const Point &p, const Vector &v);

Vector operator/(const Vector &v, const double &k);

Point operator-(const Point &p, const Vector &v);

#endif // GEOMETRY_H_INCLUDED
