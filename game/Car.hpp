#pragma once

#include "../core/GameObject.hpp"


class Car: GameObject
{
    public:
        Car()
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

            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_DestroySurface(surface);
        }
}
