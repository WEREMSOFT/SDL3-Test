/* primitives.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some lines,
 * rectangles and points to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
 #include <SDL3/SDL_events.h>
 #include <SDL3/SDL_filesystem.h>
 #include <SDL3/SDL_init.h>
 #include <SDL3/SDL_rect.h>
 #include <SDL3/SDL_render.h>
 #include <SDL3/SDL_scancode.h>
 #include <SDL3/SDL_stdinc.h>
 #include <SDL3/SDL_surface.h>
 #include <SDL3/SDL_timer.h>
 #define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
 #include <SDL3/SDL.h>
 #include <SDL3/SDL_main.h>
SDL_COMPILE_TIME_ASSERT(SDL_Version, SDL_VERSION_ATLEAST(3, 4, 0));
#include "game/World.hpp"

#include <math.h>


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

Uint64 last = SDL_GetPerformanceCounter();

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    const int compiled = SDL_VERSION;  /* hardcoded number from SDL headers */
    const int linked = SDL_GetVersion();  /* reported by linked SDL library */

    SDL_Log("We compiled against SDL version %d.%d.%d ...\n",
            SDL_VERSIONNUM_MAJOR(compiled),
            SDL_VERSIONNUM_MINOR(compiled),
            SDL_VERSIONNUM_MICRO(compiled));

    SDL_Log("But we are linking against SDL version %d.%d.%d.\n",
            SDL_VERSIONNUM_MAJOR(linked),
            SDL_VERSIONNUM_MINOR(linked),
            SDL_VERSIONNUM_MICRO(linked));

    int i;

    SDL_SetAppMetadata("SDL3 Test", "1.0", "none");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Zombie Car Survival!!", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 1024, 768, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    World* world = new World(renderer);

    *appstate = world;

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if(event->type == SDL_EVENT_KEY_DOWN)
    {
        if(event->key.scancode == SDL_SCANCODE_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 now = SDL_GetPerformanceCounter();
    double deltaTime =
        (double)(now - last) / (double)SDL_GetPerformanceFrequency();
    last = now;

    SDL_SetRenderDrawColor(renderer, 15, 50, 90, SDL_ALPHA_OPAQUE);  /* dark gray, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    World* world = (World *)appstate;

    world->Update(deltaTime);
    world->Draw(renderer);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    delete (World*)appstate;
}
