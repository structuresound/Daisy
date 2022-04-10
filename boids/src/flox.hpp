#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

#ifndef PI
#define PI 3.141592635
#endif

#ifndef PVECTOR_H_
#define PVECTOR_H_

// The Pvector class implements Euclidian vectors -- that is, each vector has
// both a magnitude and a direction. We use Pvectors for implementing movement
// and the three Boid rules -- cohesion, separation, and matching velocity
// through the use of acceleration, force, and velocity vectors.

class Pvector
{

public:
    float x;
    float y;

    // Constructors
    Pvector() {}

    Pvector(float xComp, float yComp)
    {
        x = xComp;
        y = yComp;
    }

    // Sets values of x and y for Pvector
    void set(float i, float o)
    {
        x = i;
        y = o;
    }

    void addVector(const Pvector &v)
    {
        x += v.x;
        y += v.y;
    }

    // Adds to a Pvector by a constant number
    void addScalar(float s)
    {
        x += s;
        y += s;
    }

    // Subtracts 2 vectors
    void subVector(const Pvector &v)
    {
        x -= v.x;
        y -= v.y;
    }

    // Subtracts two vectors and returns the difference as a vector
    Pvector subTwoVector(const Pvector &v, const Pvector &v2)
    {
        Pvector tmp(v.x - v2.x, v.y - v2.y);
        return std::move(tmp);
    }

    // Adds to a Pvector by a constant number
    void subScalar(float s)
    {
        x -= s;
        y -= s;
    }

    // Multiplies 2 vectors
    void mulVector(const Pvector &v)
    {
        x *= v.x;
        y *= v.y;
    }

    // Adds to a Pvector by a constant number
    void mulScalar(float s)
    {
        if (s == 0)
        {
            x = 0;
            y = 0;
        }
        x *= s;
        y *= s;
    }

    // Divides 2 vectors
    void divVector(const Pvector &v)
    {
        x /= v.x;
        y /= v.y;
    }

    // Adds to a Pvector by a constant number
    void divScalar(float s)
    {
        // x /= s;
        // y /= s;
        float inv = 1 / s;
        x *= inv;
        y *= inv;
    }

    void limit(double max)
    {
        double size = magnitude();

        if (size > max)
        {
            set(x / size, y / size);
        }
    }

    // Calculates the distance between the first Pvector and second Pvector
    float distance(const Pvector &v)
    {
        float dx = x - v.x;
        float dy = y - v.y;
        float dist = sqrt(dx * dx + dy * dy);
        return dist;
    }

    // Calculates the dot product of a vector
    float dotProduct(const Pvector &v)
    {
        float dot = x * v.x + y * v.y;
        return dot;
    }

    // Calculates magnitude of referenced object
    float magnitude()
    {
        return sqrt(x * x + y * y);
    }

    void setMagnitude(float x)
    {
        normalize();
        mulScalar(x);
    }

    // Calculate the angle between Pvector 1 and Pvector 2
    float angleBetween(const Pvector &v)
    {
        if (x == 0 && y == 0)
            return 0.0f;
        if (v.x == 0 && v.y == 0)
            return 0.0f;

        double dot = x * v.x + y * v.y;
        double v1mag = sqrt(x * x + y * y);
        double v2mag = sqrt(v.x * v.x + v.y * v.y);
        double amt = dot / (v1mag * v2mag); // Based of definition of dot product
        // dot product / product of magnitudes gives amt
        if (amt <= -1)
        {
            return PI;
        }
        else if (amt >= 1)
        {
            return 0;
        }
        float tmp = acos(amt);
        return tmp;
    }

    // normalize divides x and y by magnitude if it has a magnitude.
    void normalize()
    {
        float m = magnitude();

        if (m > 0)
        {
            set(x / m, y / m);
        }
        else
        {
            set(x, y);
        }
    }

    // Creates and returns a copy of the Pvector used as a parameter
    Pvector copy(const Pvector &v)
    {
        Pvector copy(v.x, v.y);
        return copy;
    }
};

#endif

#ifndef BOID_H_
#define BOID_H_

// The Boid Class
//
// Attributes
//  bool predator: flag that specifies whether a given boid is a predator.
//  Pvector location: Vector that specifies a boid's location.
//  Pvector velocity: Vector that specifies a boid's current velocity.
//  Pvector acceleration: Vector that specifies a boid's current acceleration.
//  float maxSpeed: Limits magnitude of velocity vector.
//  float maxForce: Limits magnitude of acceleration vector. (F = m*a!)
//
// Methods
//  applyForce(Pvector force): Adds the given vector to acceleration
//
//  Pvector Separation(vector<Boid> Boids): If any other boids are within a
//      given distance, Separation computes a a vector that distances the
//      current boid from the boids that are too close.
//
//  Pvector Alignment(vector<Boid> Boids): Computes a vector that causes the
//      velocity of the current boid to match that of boids that are nearby.
//
//  Pvector Cohesion(vector<Boid> Boids): Computes a vector that causes the
//      current boid to seek the center of mass of nearby boids.

class Boid
{
public:
    bool predator;
    Pvector location;
    Pvector velocity;
    Pvector acceleration;
    Pvector *wrapSize;
    float maxSpeed;
    float maxForce;
    float *simRate;

    Boid(Pvector &wrapRef, float x, float y)
    {
        wrapSize = &wrapRef;
        acceleration = Pvector(0, 0);
        velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2);
        location = Pvector(x, y);
        maxSpeed = 3.5;
        maxForce = 0.5;
    }

    Boid(Pvector &wrapRef, float x, float y, bool predCheck)
    {
        wrapSize = &wrapRef;
        predator = predCheck;
        if (predCheck == true)
        {
            maxSpeed = 7.5;
            maxForce = 0.5;
            velocity = Pvector(rand() % 3 - 1, rand() % 3 - 1);
        }
        else
        {
            maxSpeed = 3.5;
            maxForce = 0.5;
            velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2);
        }
        acceleration = Pvector(0, 0);
        location = Pvector(x, y);
    }

    // void applyForce(const Pvector &force);
    // // Three Laws that boids follow
    // Pvector Separation(const vector<Boid> &Boids);
    // Pvector Alignment(const vector<Boid> &Boids);
    // Pvector Cohesion(const vector<Boid> &Boids);
    // // Functions involving SFML and visualisation linking
    // Pvector seek(const Pvector &v);
    // void run(const vector<Boid> &v);
    // void update();
    // void flock(const vector<Boid> &v);
    // void borders();
    // float angle(const Pvector &v);

    // Adds force Pvector to current force Pvector
    void applyForce(const Pvector &force)
    {
        acceleration.addVector(force);
    }

    // Separation
    // Keeps boids from getting too close to one another
    Pvector Separation(const vector<Boid> &boids)
    {
        // Distance of field of vision for separation between boids
        float desiredseparation = 20;
        Pvector steer(0, 0);
        int count = 0;
        // For every boid in the system, check if it's too close
        for (int i = 0; i < boids.size(); i++)
        {
            // Calculate distance from current boid to boid we're looking at
            float d = location.distance(boids[i].location);
            // If this is a fellow boid and it's too close, move away from it
            if ((d > 0) && (d < desiredseparation))
            {
                Pvector diff(0, 0);
                diff = diff.subTwoVector(location, boids[i].location);
                diff.normalize();
                diff.divScalar(d); // Weight by distance
                steer.addVector(diff);
                count++;
            }
            // If current boid is a predator and the boid we're looking at is also
            // a predator, then separate only slightly
            if ((d > 0) && (d < desiredseparation) && predator == true && boids[i].predator == true)
            {
                Pvector pred2pred(0, 0);
                pred2pred = pred2pred.subTwoVector(location, boids[i].location);
                pred2pred.normalize();
                pred2pred.divScalar(d);
                steer.addVector(pred2pred);
                count++;
            }
            // If current boid is not a predator, but the boid we're looking at is
            // a predator, then create a large separation Pvector
            else if ((d > 0) && (d < desiredseparation + 70) && boids[i].predator == true)
            {
                Pvector pred(0, 0);
                pred = pred.subTwoVector(location, boids[i].location);
                pred.mulScalar(900);
                steer.addVector(pred);
                count++;
            }
        }
        // Adds average difference of location to acceleration
        if (count > 0)
            steer.divScalar((float)count);
        if (steer.magnitude() > 0)
        {
            // Steering = Desired - Velocity
            steer.normalize();
            steer.mulScalar(maxSpeed);
            steer.subVector(velocity);
            steer.limit(maxForce);
        }
        return steer;
    }

    // Alignment
    // Calculates the average velocity of boids in the field of vision and
    // manipulates the velocity of the current boid in order to match it
    Pvector Alignment(const vector<Boid> &Boids)
    {
        float neighbordist = 50; // Field of vision

        Pvector sum(0, 0);
        int count = 0;
        for (int i = 0; i < Boids.size(); i++)
        {
            float d = location.distance(Boids[i].location);
            if ((d > 0) && (d < neighbordist))
            { // 0 < d < 50
                sum.addVector(Boids[i].velocity);
                count++;
            }
        }
        // If there are boids close enough for alignment...
        if (count > 0)
        {
            sum.divScalar((float)count); // Divide sum by the number of close boids (average of velocity)
            sum.normalize();             // Turn sum into a unit vector, and
            sum.mulScalar(maxSpeed);     // Multiply by maxSpeed
            // Steer = Desired - Velocity
            Pvector steer;
            steer = steer.subTwoVector(sum, velocity); // sum = desired(average)
            steer.limit(maxForce);
            return steer;
        }
        else
        {
            Pvector temp(0, 0);
            return temp;
        }
    }

    // Cohesion
    // Finds the average location of nearby boids and manipulates the
    // steering force to move in that direction.
    Pvector Cohesion(const vector<Boid> &Boids)
    {
        float neighbordist = 50;
        Pvector sum(0, 0);
        int count = 0;
        for (int i = 0; i < Boids.size(); i++)
        {
            float d = location.distance(Boids[i].location);
            if ((d > 0) && (d < neighbordist))
            {
                sum.addVector(Boids[i].location);
                count++;
            }
        }
        if (count > 0)
        {
            sum.divScalar(count);
            return seek(sum);
        }
        else
        {
            Pvector temp(0, 0);
            return temp;
        }
    }

    // Limits the maxSpeed, finds necessary steering force and
    // normalizes vectors
    Pvector seek(const Pvector &v)
    {
        Pvector desired;
        desired.subVector(v); // A vector pointing from the location to the target
        // Normalize desired and scale to maximum speed
        desired.normalize();
        desired.mulScalar(maxSpeed * *simRate);
        // Steering = Desired minus Velocity
        acceleration.subTwoVector(desired, velocity);
        acceleration.limit(maxForce * *simRate); // Limit to maximum steering force
        return acceleration;
    }

    // Modifies velocity, location, and resets acceleration with values that
    // are given by the three laws.
    void update()
    {
        // To make the slow down not as abrupt
        acceleration.mulScalar(.4);
        // Update velocity
        velocity.addVector(acceleration);
        // Limit speed
        velocity.limit(maxSpeed * *simRate);
        location.addVector(velocity);
        // Reset accelertion to 0 each cycle
        acceleration.mulScalar(0);
    }

    // Run flock() on the flock of boids.
    // This applies the three rules, modifies velocities accordingly, updates data,
    // and corrects boids which are sitting outside of the SFML window
    void run(const vector<Boid> &boids)
    {
        flock(boids);
        update();
        borders();
    }

    // Applies the three laws to the flock of boids
    void flock(const vector<Boid> &v)
    {
        Pvector sep = Separation(v);
        Pvector ali = Alignment(v);
        Pvector coh = Cohesion(v);
        // Arbitrarily weight these forces
        float currentRate = 1.0; // *simRate;
        sep.mulScalar(1.5 * currentRate);
        ali.mulScalar(currentRate); // Might need to alter weights for different characteristics
        coh.mulScalar(currentRate);
        // Add the force vectors to acceleration
        applyForce(sep);
        applyForce(ali);
        applyForce(coh);
    }

    // Checks if boids go out of the window and if so, wraps them around to
    // the other side.
    void borders()
    {
        if (location.x < 0)
            location.x += wrapSize->x;
        if (location.y < 0)
            location.y += wrapSize->y;
        if (location.x > wrapSize->x)
            location.x -= wrapSize->x;
        if (location.y > wrapSize->y)
            location.y -= wrapSize->y;
    }
    // Calculates the angle for the velocity of a boid which allows the visual
    // image to rotate in the direction that it is going in.
    float angle(const Pvector &v)
    {
        // From the definition of the dot product
        float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
        return angle;
    }
};

#endif

#ifndef FLOCK_H_
#define FLOCK_H_

// Brief description of Flock Class:
// This file contains the class needed to create a flock of boids. It utilizes
// the boids class and initializes boid flocks with parameters that can be
// specified. This class will be utilized in main.

class Flock
{
public:
    // Constructors
    Flock() {}
    int getSize()
    {
        return flock.size();
    }

    // Read/write method that returns a reference of the Boid.
    Boid &getBoid(int i)
    {
        return flock[i];
    }

    void addBoid(const Boid &b)
    {
        flock.push_back(std::move(b));
    }

    void addPredator(const Boid &b)
    {
        flock.push_back(std::move(b));
    }

    int removePredator()
    {
        for (int i = 0; i < flock.size(); i++)
        {
            if (flock[i].predator)
            {
                flock.erase(flock.begin() + i);
                return i;
            }
        }
        return -1;
    }

    // Runs the run function for every boid in the flock checking against the flock
    // itself. Which in turn applies all the rules to the flock.
    void flocking()
    {
        for (int i = 0; i < flock.size(); i++)
            flock[i].run(flock);
    }

private:
    vector<Boid> flock;
};

#endif
