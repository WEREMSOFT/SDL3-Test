/* primitives.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some lines,
 * rectangles and points to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#include "core/game_object.hpp"
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
#include <math.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static SDL_Point texture_size = {0};

GameObject *gameObject;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    int i;

    SDL_SetAppMetadata("SDL3 Test", "1.0", "none");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/primitives", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    char* pngPath = NULL;

    SDL_asprintf(&pngPath, "%slanding-base.png", SDL_GetBasePath());

    SDL_Surface* surface = SDL_LoadPNG(pngPath);

    SDL_free(pngPath);

    texture_size.x = surface->w;
    texture_size.y = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    gameObject = GameObject::CreateDrawable(texture);

    gameObject->Dimensions.x = -2.;
    // gameObject->Dimensions.w *= 2.0;
    // gameObject->Dimensions.h *= 2.0;

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

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect rect;


    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0XFF, 33, 33, SDL_ALPHA_OPAQUE);  /* dark gray, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    const Uint64 now = SDL_GetTicks();
    const float phase = (((float) ((int) (now % 2000))) / 2000.0f) * SDL_PI_F * 2;

    /* draw a filled rectangle in the middle of the canvas. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = rect.y = 100;
    rect.w = 440 + sin(phase) * 50;
    rect.h = 280 + cos(phase) * 50;
    SDL_RenderFillRect(renderer, &rect);

    SDL_FRect destination_rect = {0};

    destination_rect.x = 100;
    destination_rect.y = 100;
    destination_rect.w = texture_size.x;
    destination_rect.h = texture_size.y;

    // SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);

    // SDL_RenderTexture(renderer, texture, NULL, NULL);

    // gameObject->Dimensions.x = 100 + sin(phase) * 10;
    // gameObject->Dimensions.y = 100 + cos(phase) * 10;
    // gameObject->Dimensions.w = 200 + cos(phase) * 10;


    gameObject->Draw(renderer);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
