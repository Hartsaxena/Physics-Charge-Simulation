#include <SDL2/SDL.h>
#include <iostream>
#include "front.hpp"
#include "charges.hpp"


int main(int argc, char* argv[]) {
    FrontEndManager manager = FrontEndManager(1000, 1000);
    if (!manager.running) {
        return 1;
    }

    ChargeSim sim = ChargeSim();

    const Position* mousePosition = &manager.mousePosition;

    std::cout<<manager.backgroundColor.r<<", "<<manager.backgroundColor.g<<", "<<manager.backgroundColor.b<<", "<<manager.backgroundColor.a<<"\n";

    while (true) {
        manager.manageEvents();
        if (manager.running == false) break;

        manager.updateMousePosition();
        if (manager.firstPressedKey(SDL_SCANCODE_N)) {
            sim.addCharge(mousePosition->x, mousePosition->y, 1, ChargeType::STATIONARY);
            std::cout<<"Pressed N\n";
            std::cout<<"Added stationary charge at "<<mousePosition->x<<", "<<mousePosition->y<<"\n";
        }
        if (manager.firstPressedKey(SDL_SCANCODE_M)) {
            sim.addCharge(mousePosition->x, mousePosition->y, -1, ChargeType::MOVING);
            std::cout<<"Added moving charge at "<<mousePosition->x<<", "<<mousePosition->y<<"\n";
        }
        if (manager.firstPressedKey(SDL_SCANCODE_R)) {
            sim = ChargeSim();
            std::cout<<"Reset charges\n";
        }

        if (manager.firstPressedKey(SDL_SCANCODE_Q)) {
            int chargeToDestroy = sim.findClosestCharge(mousePosition->x, mousePosition->y);
            if (chargeToDestroy != -1) {
                std::cout<<"Destroying charge at "<<sim.fieldCharges[chargeToDestroy].x<<", "<<sim.fieldCharges[chargeToDestroy].y<<"\n";
                sim.removeCharge(chargeToDestroy);
            } else {
                std::cout<<"No charge to destroy\n";
            }
        }
        for (int i = 0; i < sim.chargeCount; i++) {
            Charge c = sim.fieldCharges[i];
            Color chargeColor = c.q > 0 ? RED : BLUE; // Positive charges are red, negative charges are blue.
            manager.drawCircle({(int)c.x, (int)c.y}, 10, chargeColor);
        }

        manager.refreshScreen();
    }

    manager.quit();
    return 0;
}