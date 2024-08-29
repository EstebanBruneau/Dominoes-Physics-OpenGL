#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"
#include <vector>


class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);
const Color PURPLE(0.5f, 0.0f, 0.5f);


// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
public:
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();
};

class Matrix3x3
{
private:
    double elements[3][3];

public:
    // Default constructor initializes to identity matrix
    Matrix3x3()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                elements[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }

    // Constructor that takes a 2D array to initialize the matrix
    Matrix3x3(double init[3][3])
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                elements[i][j] = init[i][j];
            }
        }
    }

    // Getter for the elements of the matrix
    double get(int i, int j) const
    {
        return elements[i][j];
    }

    // Setter for the elements of the matrix
    void set(int i, int j, double value)
    {
        elements[i][j] = value;
    }

    // Matrix inversion
    Matrix3x3 inverse() const
    {
        double det = elements[0][0] * (elements[1][1] * elements[2][2] - elements[1][2] * elements[2][1])
                   - elements[0][1] * (elements[1][0] * elements[2][2] - elements[1][2] * elements[2][0])
                   + elements[0][2] * (elements[1][0] * elements[2][1] - elements[1][1] * elements[2][0]);

        double invdet = 1.0 / det;

        double inv[3][3];
        inv[0][0] = (elements[1][1] * elements[2][2] - elements[1][2] * elements[2][1]) * invdet;
        inv[0][1] = (elements[0][2] * elements[2][1] - elements[0][1] * elements[2][2]) * invdet;
        inv[0][2] = (elements[0][1] * elements[1][2] - elements[0][2] * elements[1][1]) * invdet;
        inv[1][0] = (elements[1][2] * elements[2][0] - elements[1][0] * elements[2][2]) * invdet;
        inv[1][1] = (elements[0][0] * elements[2][2] - elements[0][2] * elements[2][0]) * invdet;
        inv[1][2] = (elements[1][0] * elements[0][2] - elements[0][0] * elements[1][2]) * invdet;
        inv[2][0] = (elements[1][0] * elements[2][1] - elements[2][0] * elements[1][1]) * invdet;
        inv[2][1] = (elements[2][0] * elements[0][1] - elements[0][0] * elements[2][1]) * invdet;
        inv[2][2] = (elements[0][0] * elements[1][1] - elements[1][0] * elements[0][1]) * invdet;
    }

    // Matrix multiplication by vector
    Vector operator*(const Vector &v) const
    {
        double x = elements[0][0] * v.x + elements[0][1] * v.y + elements[0][2] * v.z;
        double y = elements[1][0] * v.x + elements[1][1] * v.y + elements[1][2] * v.z;
        double z = elements[2][0] * v.x + elements[2][1] * v.y + elements[2][2] * v.z;

        return Vector(x, y, z);
    }
};

class Segment : public Form
{
private:
    Point p1, p2;
    Vector direction;
public:
    Segment(Point pt1 = Point(), Point pt2 = Point(), Color cl = Color());
    Point getP1() const {return p1;}
    Point getP2() const {return p2;}
    Vector getDirection() const {return direction;}
    void update(double delta_t);
    void render();
};


// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
public:
    Sphere(double r = 1.0, Color cl = Color());
    double getRadius() const {return radius;}
    void setRadius(double r) {radius = r;}
    void update(double delta_t);
    void render();
};


// A face of a cube
class Cube_face : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
    Point org;
    GLuint texture_id;
public:
    Cube_face(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,1,0),
              Point org = Point(), double l = 1.0, double w = 1.0, Color cl = Color());
    Point checkForCollision(Segment s);
    double getLength() const {return length;}
    double getWidth() const {return width;}
    Point getCenter();
    void update(double delta_t);
    void render();
    Vector getNormal();
    void setTexture(GLuint textureid) {texture_id = textureid;}
};

// A cuboid (for dominoes)
class Cuboid : public Form
{
private:
    Vector vdir1, vdir2, vdir3;
    double length, width, height;
    Point position;
public:
    Cuboid(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1), Vector v3 = Vector(0,1,0),
          Point position = Point(), double l = 1.0, double w = 1.0, double h = 1.0,
          Color cl = Color());
    void update(double delta_t);
    void render();
    Point checkForCollision(Cuboid c, int &ri, int &rj);
    void handleCollision(Cuboid &c, const Point &collisionPoint, const Vector &collisionNormal);
    Point checkForCollision(Cube_face f, int &ri);

    Vector getVdir1() const {return vdir1;}
    Vector getVdir2() const {return vdir2;}
    Vector getVdir3() const {return vdir3;}
    Point getPosition() const {return position;}
    double getLength() const {return length;}
    double getWidth() const {return width;}
    double getHeight() const {return height;}
    Cube_face getFace(int i); 
    Segment getSegment(int i);
    Color getColor() const { return col; }

    void setVdir1(Vector v) {vdir1 = v;}
    void setVdir2(Vector v) {vdir2 = v;}
    void setVdir3(Vector v) {vdir3 = v;}
    void setPosition(Point p) {position = p;}
    void setLength(double l) {length = l;}
    void setWidth(double w) {width = w;}
    void setHeight(double h) {height = h;}
    // void setFace(int i, Cube_face f);
    // void setSegment(int i, Segment s);  

};

class Domino : public Cuboid
{
private:
    double mass;
    Vector velocity;
    Vector angularVelocity;
    Vector angularAcceleration;
    Matrix3x3 momentOfInertia;
    double theta;
    double phi;
    double psi;
    double frictionCoefficient;
    Vector acceleration;
    static std::vector<Domino> allDominoes;
    GLuint texture_id;
public:
    Domino(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1), Vector v3 = Vector(0,1,0),
           Point position = Point(0,0,0), double l = 1.0, double w = 1.0, double h = 1.0,
           Color cl = Color(), double m = 1.0, Vector v = Vector(), Vector omega = Vector(),
           Matrix3x3 I = Matrix3x3(), double t = 0.0, double p = 0.0,
           double mu = 0.0)
        : Cuboid(v1, v2, v3, position, l, w, h, cl), mass(m), velocity(v), angularVelocity(omega),
          momentOfInertia(I), theta(t), phi(p), psi(0.0), frictionCoefficient(mu), acceleration(Vector()) {}

    double getMass() const { return mass; }
    Vector getVelocity() const { return velocity; }
    Vector getAngularVelocity() const { return angularVelocity; }
    Matrix3x3 getMomentOfInertia() const { return momentOfInertia; }
    double getTheta() const { return theta; }
    double getPhi() const { return phi; }
    double getPsi() const { return psi; }
    double getFrictionCoefficient() const { return frictionCoefficient; }
    Vector getAcceleration() const { return acceleration; }
    std::vector<Domino> getAllDominoes() { return allDominoes; }
    void handleCollisionGround(Cube_face, Point, Vector);
    Vector getAngularAcceleration() const { return angularAcceleration; }
    void setTexture(GLuint textureid) {texture_id = textureid;}


    void setMass(double m) { mass = m; }
    void setVelocity(Vector v) { velocity = v; }
    void setAngularVelocity(Vector omega) { angularVelocity = omega; }
    void setMomentOfInertia(Matrix3x3 I) { momentOfInertia = I; }
    void setTheta(double t) { theta = t; }
    void setPhi(double p) { phi = p; }
    void setPsi(double ps) { psi = ps; }
    void setFrictionCoefficient(double mu) { frictionCoefficient = mu; }
    void setAcceleration(Vector a) { acceleration = a; }
    void setAllDominoes(std::vector<Domino> dominoes) { allDominoes = dominoes; }
    void setAngularAcceleration(Vector a) { angularAcceleration = a; }

    Point checkForCollision(Domino d, int &ri, int &rj);
    Point checkForCollision(Cube_face f, int &ri);
    void handleCollision(Domino &d, const Point &collisionPoint, const Vector &collisionNormal);
    void update(double delta_t);
    void checkCollisions(std::vector<Domino> allDominoes);
    Point updatePosition(const Point &r, const Vector &v1, double delta_t, const Vector &animAccel);
    void render();
    void startFalling();
};





#endif
// FORMS_H_INCLUDED
