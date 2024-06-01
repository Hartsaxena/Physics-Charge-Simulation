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
    this->ay -= ay; // Subtracting since y increases downwards in the simulation
}

void MovingCharge::updateVelocity(double timestep) {
    /*
    Updates the velocity of the charge based on its acceleration.
    */
    this->vx += this->ax * timestep;
    this->vy += this->ay * timestep;
}
void MovingCharge::updatePosition(double timestep) {
    /*
    Updates the position of the charge based on its velocity.
    timestep is the amount of time that has passed since the last update. (default is 1 second)
    */
    this->x += this->vx * timestep;
    this->y += this->vy * timestep;
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
Force Force::operator+=(const Force& other) {
    *this = *this + other;
    return *this;
}

std::ostream& operator<<(std::ostream& os, Force const& obj) {
    double angleInDegrees = obj.angle * (180.0 / M_PI);
    return os << "Force(magnitude: " << obj.magnitude << ", angle (degrees): " << angleInDegrees << ")";
}


double ChargeSim::distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
double ChargeSim::distance(Charge a, double x, double y) {
    return ChargeSim::distance(a.x, a.y, x, y);
}
double ChargeSim::distance(Charge a, Charge b) {
    return ChargeSim::distance(a.x, a.y, b.x, b.y);
}

double ChargeSim::angle(double x1, double y1, double x2, double y2) {
    double dX = x2 - x1;
    double dY = -y2 - -y1;
    return atan(dY / dX);
}
double ChargeSim::angle(Charge a, Charge b) {
    return ChargeSim::angle(a.x, a.y, b.x, b.y);
}

Force ChargeSim::electricForce(Charge source, Charge test) {
    // Find magnitude of force using Coulomb's Law
    double q1 = fabs(test.q);
    double q2 = fabs(source.q);
    double r = distance(source, test);
    double mag = k * (q1 * q2) / pow(r, 2);

    // Convert coordinates to normal coordinate system (y increases upwards)
    double dX = source.x - test.x;
    double dY = -source.y - -test.y;

    // Find angle of force using atan
    double angle = atan(dY / dX);
    Force f = {mag, angle};

    return f;
}


void ChargeSim::addCharge(Charge* newCharge) {
    if (chargeCount < maxCharges) {
        this->fieldCharges[chargeCount] = newCharge;
        if (newCharge->type == ChargeType::MOVING) {
            MovingCharge* movingCharge = static_cast<MovingCharge*>(newCharge);
            this->movingCharges[movingChargeCount++] = movingCharge;
        }
        chargeCount++;
    } else {
        std::cout<<"Max charges reached. Cannot add more charges.\n";
    }
}
void ChargeSim::addCharge(double x, double y, double q, ChargeType type) {
    if (type == ChargeType::MOVING) {
        MovingCharge* newCharge = new MovingCharge(x, y, q);
        addCharge(newCharge);
    } else {
        StationaryCharge* newCharge = new StationaryCharge(x, y, q);
        addCharge(newCharge);
    }
}
void ChargeSim::removeCharge(int index) {
    Charge removedCharge = *fieldCharges[index];
    if (removedCharge.type == ChargeType::MOVING) {
        for (int i = index; i < movingChargeCount - 1; i++) {
            movingCharges[i] = movingCharges[i + 1];
        }
        movingChargeCount--;
    }
    if (index < chargeCount) {
        for (int i = index; i < chargeCount - 1; i++) {
            fieldCharges[i] = fieldCharges[i + 1];
        }
        chargeCount--;
    }
}
void ChargeSim::resetCharges() {
    this->chargeCount = 0;
    this->movingChargeCount = 0;
}

int ChargeSim::findClosestCharge(double x, double y) {
    int closestIndex = -1;
    double minDistance = 1000000; // Arbitrarily large number

    for (int i = 0; i < chargeCount; i++) {
        if (fieldCharges[i]->x == x && fieldCharges[i]->y == y) {
            return i;
        }

        double distance = ChargeSim::distance(*fieldCharges[i], x, y);

        if (distance < minDistance) {
            closestIndex = i;
            minDistance = distance;
        }
    }

    return closestIndex;
}


void ChargeSim::update() {
    if (paused) return;
    this->updateForces();
    this->applyAcceleration();
}


void ChargeSim::updateForces() {
    /*
    Update all the forces on the charges in the simulation.
    First, we find the forces between all the charges and change their accelerations.
    Then, we update the speeds and positions of the moving charges.
    */

    for (int i = 0; i < this->movingChargeCount; i++) {
        MovingCharge* a = movingCharges[i];

        Force finalForce = Force(0.0, 0.0); // Initialize force to 0 N

        // Now compare to all other charges and find the forces between them.
        for (int j = i; j < chargeCount; j++) {
            if (j == i) continue;

            // Find the electric force
            Charge b = *fieldCharges[j];

            // Apply force
            if (a->type == ChargeType::MOVING) {
                Force f = ChargeSim::electricForce(b, *a); // Force of b on a.
                finalForce += f;
            }
        }

        // Apply the total force on the charge
        a->applyForce(finalForce);
    }
}

void ChargeSim::applyAcceleration() {
    for (int i = 0; i < this->movingChargeCount; i++) {
        MovingCharge* charge = movingCharges[i];
        charge->updateVelocity();
        charge->updatePosition();
    }
}

void ChargeSim::togglePause() {
    this->paused = !this->paused;
}