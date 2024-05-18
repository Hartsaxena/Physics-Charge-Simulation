#include <SDL2/SDL.h>
#include <iostream>
#include "front.hpp"


FrontEndManager::FrontEndManager(int windowWidth, int windowHeight,
                                bool fullscreen,
                                const char* windowTitle,
                                Color backgroundColor)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        this->running = false;
    }
    std::cout << "SDL initialized!\n";

    // Create Screen
    Uint32 windowFlags = SDL_WINDOW_SHOWN; // Default Flags
    if (fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    this->window = SDL_CreateWindow(windowTitle,
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    windowWidth, windowHeight,
                    windowFlags);

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    
    updateMousePosition();

    this->backgroundColor = backgroundColor;

    std::fill_n(this->firstPressedKeys, SDL_NUM_SCANCODES, false);

    std::cout << "Created FrontEndManager Instance\n";
}

void FrontEndManager::quit() {
    std::cout<<"Quitting Program\n";
    SDL_Quit();
}

int FrontEndManager::manageEvents() {
    SDL_Event event;
    std::fill_n(this->firstPressedKeys, SDL_NUM_SCANCODES, false);
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            this->running = false;
            return 1;
        }

        if (event.type == SDL_KEYDOWN) {
            updateFirstPressedKeys(&event);
            if (this->firstPressedKey(SDL_SCANCODE_ESCAPE)) {
                this->running = false;
                return 1;
            }
        }
    }
    return 0;
}
void FrontEndManager::updateFirstPressedKeys(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        // Get SDL_Scancode from SDL_Keycode
        SDL_Scancode scancode = SDL_GetScancodeFromKey(event->key.keysym.sym);
        firstPressedKeys[scancode] = true;
    }
}

void FrontEndManager::refreshScreen() {
    SDL_RenderPresent(this->renderer);
    this->fillScreen(this->backgroundColor);
}

void FrontEndManager::fillScreen(Color color) {
    this->fillScreen(color.r, color.g, color.b, color.a);
}
void FrontEndManager::fillScreen(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    SDL_RenderClear(this->renderer);
}


void FrontEndManager::updateMousePosition() {
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
}

bool FrontEndManager::pressingKey(SDL_Scancode key) {
    return this->keyboardState[key];
}
bool FrontEndManager::pressingKey(SDL_Keycode key) {
    return this->pressingKey(SDL_GetScancodeFromKey(key));
}
bool FrontEndManager::firstPressedKey(SDL_Scancode key) {
    return this->firstPressedKeys[key];
}
bool FrontEndManager::firstPressedKey(SDL_Keycode key) {
    return this->firstPressedKey(SDL_GetScancodeFromKey(key));
}


void FrontEndManager::drawCircle(Position center, int radius, Color color) {
    /*
    Mid-point circle drawing algorithm.
    */
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
    while (x >= y)
    {
        SDL_RenderDrawPoint(this->renderer, center.x + x, center.y + y);
        SDL_RenderDrawPoint(this->renderer, center.x + y, center.y + x);
        SDL_RenderDrawPoint(this->renderer, center.x - x, center.y + y);
        SDL_RenderDrawPoint(this->renderer, center.x - y, center.y + x);
        SDL_RenderDrawPoint(this->renderer, center.x - x, center.y - y);
        SDL_RenderDrawPoint(this->renderer, center.x - y, center.y - x);
        SDL_RenderDrawPoint(this->renderer, center.x + x, center.y - y);
        SDL_RenderDrawPoint(this->renderer, center.x + y, center.y - x);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        } else {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}