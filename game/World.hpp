#pragma once
#include "../core/GameObject.hpp"
#include "CircleBehavior.hpp"

class World: public GameObject
{
    GameObject* hero;
    SDL_Texture *texture = NULL;

    public:
        World(SDL_Renderer* renderer)
        {
            Type = GameObjectTypeEnum::DRAWABLE;

            snprintf(Tag, 100, "World");

            SDL_Point texture_size = {0};

            char* pngPath = NULL;

            SDL_asprintf(&pngPath, "%slanding-base.png", SDL_GetBasePath());

            SDL_Surface* surface = SDL_LoadPNG(pngPath);

            SDL_free(pngPath);

            texture_size.x = surface->w;
            texture_size.y = surface->h;

            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_DestroySurface(surface);

            hero = GameObject::CreateDrawable(texture);

            snprintf(hero->Tag, 100, "Hero");

            hero->Dimensions.x = -2.;

            auto circleBehavior = new CircleBehavior();

            hero->AddChild(circleBehavior);

            AddChild(hero);
        }

        ~World()
        {
            printf("destroying world\n");
        }
};
