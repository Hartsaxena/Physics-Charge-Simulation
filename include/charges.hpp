#pragma once


const double k = 8.99e9; // Coulomb's constant
const double chargeMass = 1e-6; // Mass of a charge in kg

typedef enum ChargeType {STATIONARY, MOVING} ChargeType;


class Force; // Forward declaration

class Charge
{
public:
    double x, y;
    double q; // Charge in Coulombs
    double mass = chargeMass; // Mass of charge in kg
    ChargeType type;

    Charge(): x(0), y(0), q(0), type(MOVING) {} // Default constructor

    Charge(double x, double y, double q, ChargeType type=MOVING):
        x(x),
        y(y),
        q(q),
        type(type)
        {}
};

class StationaryCharge : public Charge
{
public:
    StationaryCharge(): // Default constructor
        Charge()
        {}
    StationaryCharge(double x, double y, double q):
        Charge(x, y, q, STATIONARY)
        {}
};

class MovingCharge : public Charge
{
public:
    double vx, vy; // Velocity in m/
    double ax = 0, ay = 0; // Acceleration in m/s^2

    MovingCharge(): // Default constructor
        Charge(),
        vx(0), vy(0)
        {}

    MovingCharge(double x, double y, double q):
        Charge(x, y, q, MOVING),
        vx(0), vy(0)
        {}

    void applyForce(Force f);
    void updateVelocity();
    void updatePosition();
};

class Force
{
public:
    double magnitude; // Newtons
    double angle; // Radians

    Force(double magnitude, double angle):
        magnitude(magnitude),
        angle(angle)
        {}

    Force operator+(const Force& other);
    void echo(); // For debugging purposes

};


class ChargeSim
{
public:
    static const int maxCharges = 100;
    Charge fieldCharges[maxCharges];
    int chargeCount;
    MovingCharge* movingCharges[maxCharges];
    int movingChargeCount;

    ChargeSim():
        chargeCount(0), movingChargeCount(0),
        fieldCharges(),
        movingCharges()
        {}
    void addCharge(Charge newCharge);
    void addCharge(double x, double y, double q, ChargeType type=MOVING); // Second way to add a charge. Default is moving charge.
    void removeCharge(int index);
    int findClosestCharge(double x, double y);

    void updateForces();
    void applyAcceleration();

    static double distance(Charge a, Charge b);
    static double distance(Charge a, double x, double y);
    static double distance(double x1, double y1, double x2, double y2);
    static Force electricalForce(Charge a, Charge b);
};
