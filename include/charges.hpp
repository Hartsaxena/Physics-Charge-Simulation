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
    void updateVelocity(double timestep = 1.0);
    void updatePosition(double timestep = 1.0);
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

    Force operator+(const Force& other); // For adding forces together
    Force operator+=(const Force& other); // For adding forces together
    friend std::ostream& operator<<(std::ostream& os, Force const& m); // For debugging (std::cout)
};


class ChargeSim
{
public:
    const double physicsTimestep = 0.01; // 10ms
    static const int maxCharges = 100;
    Charge* fieldCharges[maxCharges];
    int chargeCount;
    MovingCharge* movingCharges[maxCharges];
    int movingChargeCount;
    bool paused = false;

    ChargeSim():
        chargeCount(0), movingChargeCount(0),
        fieldCharges(),
        movingCharges()
        {}
    void addCharge(Charge* newCharge);
    void addCharge(double x, double y, double q, ChargeType type=MOVING); // Second way to add a charge. Default is moving charge.
    void removeCharge(int index);
    void resetCharges();
    int findClosestCharge(double x, double y);

    void update();
    void updateForces();
    void applyAcceleration();
    void togglePause();

    static double distance(double x1, double y1, double x2, double y2);
    static double distance(Charge a, double x, double y);
    static double distance(Charge a, Charge b);
    static double angle(double x1, double y1, double x2, double y2);
    static double angle(Charge a, Charge b);
    static Force electricForce(Charge a, Charge b);
};
