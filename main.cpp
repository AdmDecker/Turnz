#include "Headers/Global.h"
#include "Headers/GameSettings.h"
#include "Headers/States.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include <iostream>


int main(int, char **)
{
    read_Settings();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) //Initialize SDL
    {
        LogSDLError("SDL Initialization");
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) //Initialize SDL_Image
    {
        LogSDLError("IMG_Init");
        return 1;
    }

    if (0 != TTF_Init())
    {
        LogSDLError("Could not initialize TTF");
    }

    if (GameLoop() != 0)
    {
        LogSDLError("MainMenu returned !0");
    }



    return 0;
}

