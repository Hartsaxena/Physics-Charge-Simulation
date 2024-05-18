#include <cmath>
#include <iostream>
#include "charges.hpp"


void MovingCharge::applyForce(Force f) {
    /*
    Applies a force to the charge and updates the x and y components of its acceleration.
    */

    // Find x and y components of force
    double fx = f.magnitude * cos(f.angle);
    double fy = f.magnitude * sin(f.angle);

    // Find acceleration of charge in x and y directions
    double ax = fx / this->mass;
    double ay = fy / this->mass;

    this->ax += ax;
    this->ay += ay;
}

void MovingCharge::updateVelocity() {
    /*
    Updates the velocity of the charge based on its acceleration.
    */
    this->vx += this->ax;
    this->vy += this->ay;
}
void MovingCharge::updatePosition() {
    /*
    Updates the position of the charge based on its velocity.
    */
    this->x += this->vx;
    this->y += this->vy;
}


Force Force::operator+(const Force& other) {
    // get x and y components of forces
    double ax = this->magnitude * cos(this->angle);
    double ay = this->magnitude * sin(this->angle);
    double bx = other.magnitude * cos(other.angle);
    double by = other.magnitude * sin(other.angle);

    // Add components together and find magnitude of final force
    double forceX = ax + bx;
    double forceY = ay + by;
    double magnitude = sqrt(pow(forceX, 2) + pow(forceY, 2));

    // Find angle of force using atan
    double angle = atan(forceY / forceX);

    Force resultForce = Force(magnitude, angle);
    return resultForce;
}

void Force::echo() {
    double angleInDegrees = this->angle * (180.0 / M_PI);
    std::cout << "Force(magnitude: " << this->magnitude << ", angle (degrees): " << angleInDegrees << ")\n";
}


double ChargeSim::distance(Charge a, Charge b) {
    return ChargeSim::distance(a.x, a.y, b.x, b.y);
}
double ChargeSim::distance(Charge a, double x, double y) {
    return ChargeSim::distance(a.x, a.y, x, y);
}
double ChargeSim::distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

Force ChargeSim::electricalForce(Charge source, Charge test) {
    double mag = k * (source.q * test.q) / pow(distance(source, test), 2); // Pythagorean's Theorem
    double angle = atan((test.y - source.y) / (test.x - source.x)); // Find angle of force using atan
    Force f = {mag, angle};

    return f;
}


void ChargeSim::addCharge(Charge newCharge) {
    if (chargeCount < maxCharges) {
        fieldCharges[chargeCount++] = newCharge;
        if (newCharge.type == ChargeType::MOVING) {
            this->movingCharges[movingChargeCount++] = static_cast<MovingCharge*>(&newCharge);
        }
    } else {
        std::cout<<"Max charges reached. Cannot add more charges.\n";
    }
}
void ChargeSim::addCharge(double x, double y, double q, ChargeType type) {
    Charge newCharge = {x, y, q, type};
    addCharge(newCharge);
}
void ChargeSim::removeCharge(int index) {
    if (index < chargeCount) {
        for (int i = index; i < chargeCount - 1; i++) {
            fieldCharges[i] = fieldCharges[i + 1];
        }
        chargeCount--;
    }
}

int ChargeSim::findClosestCharge(double x, double y) {
    int closestIndex = -1;
    double minDistance = 1000000; // Arbitrarily large number

    for (int i = 0; i < chargeCount; i++) {
        if (fieldCharges[i].x == x && fieldCharges[i].y == y) {
            return i;
        }

        double distance = ChargeSim::distance(fieldCharges[i], x, y);

        if (distance < minDistance) {
            closestIndex = i;
            minDistance = distance;
        }
    }

    return closestIndex;
}


void ChargeSim::updateForces() {
    /*
    Update all the forces on the charges in the simulation.
    First, we find the forces between all the charges and change their accelerations.
    Then, we update the speeds and positions of the moving charges.
    */

    // Array to store moving charges so we can update speeds / positions at the same time.
    MovingCharge* movingCharges[maxCharges];
    int movingChargeCount = 0;

    for (int i = 0; i < chargeCount; i++) {
        for (int j = i; j < chargeCount; j++) {
            if (j == i) continue;

            // Find the electric force
            Charge a = fieldCharges[i];
            Charge b = fieldCharges[j];

            // Apply force
            if (a.type == ChargeType::MOVING) {
                Force f = electricalForce(a, b);
                MovingCharge* movingA = static_cast<MovingCharge*>(&a); // We can assume the type cast will work.
                movingA->applyForce(f);
            }
        }
    }
}

void ChargeSim::applyAcceleration() {
    for (int i = 0; i < movingChargeCount; i++) {
        MovingCharge* charge = movingCharges[i];
        charge->updateVelocity();
        charge->updatePosition();
    }
}