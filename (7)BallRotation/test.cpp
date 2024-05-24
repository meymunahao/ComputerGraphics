#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = SCREEN_WIDTH + 300;
const int IMAGE_HEIGHT = SCREEN_HEIGHT + 300;

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

    // Set renderer color to pink
    SDL_SetRenderDrawColor(gRenderer, 255, 192, 203, 255);

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

    // Load textures
    SDL_Texture* ballTexture = loadTexture("./Assets/Pixel_Forest/ball2.png");

    // Initial position and velocity of the ball
    int ballPosX = 0, ballPosY = 0;
    int ballVelX = 3, ballVelY = 3; // Initial velocity

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

        // Move the ball
        ballPosX += ballVelX;
        ballPosY += ballVelY;

        // Check if the ball collided with the edges
        if ((ballPosX < 0) || (ballPosX + 100 > SCREEN_WIDTH)) {
            ballVelX = -ballVelX; // Reverse velocity on collision with horizontal edges
        }
        if ((ballPosY < 0) || (ballPosY + 100 > SCREEN_HEIGHT)) {
            ballVelY = -ballVelY; // Reverse velocity on collision with vertical edges
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        // Set renderer color to sky blue
        SDL_SetRenderDrawColor(gRenderer, 135, 206, 250, 255); // RGB values for sky blue

        // Clear screen with sky blue color
        SDL_RenderClear(gRenderer);

        // Render the bouncing ball
        SDL_Rect ballRect = { ballPosX, ballPosY, 100, 100 };
        SDL_RenderCopy(gRenderer, ballTexture, NULL, &ballRect);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Destroy texture
    SDL_DestroyTexture(ballTexture);

    // Close SDL
    closeSDL();

    return 0;
}