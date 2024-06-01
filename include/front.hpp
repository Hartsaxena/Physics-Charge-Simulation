#pragma once
#include <SDL2/SDL.h>

typedef struct Color {
    int r, g, b, a;

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    Color operator+(const Color& other) const {
        // Color blending algorithm - not perceptive color mixing
        int newR = (r + other.r) / 2;
        int newG = (g + other.g) / 2;
        int newB = (b + other.b) / 2; 

        return (Color) {newR, newG, newB, this->a};
    }
} Color;

const Color WHITE = {255, 255, 255, 255};
const Color BLACK = {0, 0, 0, 255};
const Color RED = {255, 0, 0, 255};
const Color GREEN = {0, 255, 0, 255};
const Color BLUE = {0, 0, 255, 255};

typedef struct Position {
    int x, y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
} Position;


class FrontEndManager {
public:
    bool running = true;
    Color backgroundColor;
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    bool firstPressedKeys[SDL_NUM_SCANCODES];
    Position mousePosition;

    explicit FrontEndManager(int windowWidth = 640, int windowHeight = 480,
                    bool fullscreen = false,
                    const char* windowTitle = "Charge Simulator",
                    Color backgroundColor = BLACK
                    );
    void quit();
    int manageEvents();
    void refreshScreen();

    void fillScreen(Color color);
    void fillScreen(int r, int g, int b, int a);
    void updateMousePosition();
    bool pressingKey(SDL_Scancode scancode);
    bool pressingKey(SDL_Keycode keycode);
    bool firstPressedKey(SDL_Scancode scancode);
    bool firstPressedKey(SDL_Keycode keycode);

    void drawCircle(Position center, int radius, Color color, bool fill);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    void updateFirstPressedKeys(SDL_Event* event);
};