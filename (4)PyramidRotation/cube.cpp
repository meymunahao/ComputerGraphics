#include "cube.h"
#include "Renderer3D.h"
#include <iostream>
#include <vector>
#include <SDL.h>

//Creates an array of points
std::vector<Point3D> points{
         Point3D{ -1, -1, -1 }, Point3D{ -1, -1, 1 },
         Point3D{ 1,  -1, -1 }, Point3D{ -1,  1, -1 },
         Point3D{ -1, 1, 1 }, Point3D{ 1,  -1, 1 },
         Point3D{ 1,  1, -1 }, Point3D{ 1,  1, 1 }
};

//creates an array of edges (This hold indices referencing to the points array)
std::vector<Edge> edges{
         Edge{ 0, 1 },
         Edge{ 0, 2 },
         Edge{ 0, 3 },
         Edge{ 2, 5 },
         Edge{ 3, 6 },
         Edge{ 3, 4 },
         Edge{ 4, 7 },
         Edge{ 6, 7 },
         Edge{ 7, 5 },
         Edge{ 5, 1 },
         Edge{ 4, 1 },
         Edge{ 2, 6 },
};

//default C++ function
int main(int argc, char* argv[])
{
    //creating a window and a renderer
    SDL_Window* window;
    SDL_Renderer* renderer;

    //creates a centered window with 960 width and 540 height
    window = SDL_CreateWindow("Cube Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;

    //Creating an instance of the the Renderer3D class
    Renderer3D render3D(window, renderer, points, edges);

    while (running)
    {
        //if the quit button is pressed, the loop breaks
        if (SDL_QuitRequested()) { running = false; break; }

        //Calls the function created in the Renderer3D class (render 3D is an instance of the class defined above)
        render3D.render();
    }

    /*
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
    }
    else
    {
        std::cout << "SDL initialization succeeded!";
    }

    std::cin.get();
    return 0;
    */


    return 0;
}