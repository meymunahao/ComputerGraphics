#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    SDL_Texture* eclipseTexture = loadTexture("./Assets/solar_eclipse.png");
    SDL_Texture* groundTexture = loadTexture("./Assets/ocean.png");

    // Define variables for ground animation
    double groundOffset = 0.0;
    double groundSpeed = 0.05;

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        // Render ground with animation
        SDL_Rect groundDest = { 0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100 };
        SDL_Rect groundSrc = { 0, 0, SCREEN_WIDTH, 100 };
        SDL_RenderCopy(gRenderer, groundTexture, &groundSrc, &groundDest);

        // Update ground animation
        groundOffset += groundSpeed;
        if (groundOffset >= SCREEN_WIDTH)
            groundOffset = 0;

        groundSrc.x = static_cast<int>(groundOffset);
        SDL_RenderCopy(gRenderer, groundTexture, &groundSrc, &groundDest);

        // Render solar eclipse
        SDL_Rect eclipseDest = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 200, 50, 50 };
        SDL_RenderCopy(gRenderer, eclipseTexture, NULL, &eclipseDest);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Destroy textures
    SDL_DestroyTexture(eclipseTexture);
    SDL_DestroyTexture(groundTexture);

    // Close SDL
    closeSDL();

    return 0;
}
