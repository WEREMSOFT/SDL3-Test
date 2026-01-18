#pragma once

#include "../core/GameObject.hpp"
#include "KeyboardBehavior.hpp"
#include <SDL3/SDL_render.h>


class Car: public GameObject
{
    public:
        Car(SDL_Renderer* renderer)
        {
            snprintf(Tag, 100, "Car");
            Type = GameObjectTypeEnum::DRAWABLE;

            SDL_Point texture_size = {0};

            char* pngPath = NULL;

            SDL_asprintf(&pngPath, "%scar.png", SDL_GetBasePath());

            SDL_Surface* surface = SDL_LoadPNG(pngPath);

            SDL_free(pngPath);

            texture_size.x = surface->w;
            texture_size.y = surface->h;

            Dimensions.w = surface->w;
            Dimensions.h = surface->h;

            Texture = SDL_CreateTextureFromSurface(renderer, surface);

            AddChild(new KeyboardBehavior());

            SDL_DestroySurface(surface);
        }
};
