#pragma once

#include "../core/GameObject.hpp"
#include <SDL3/SDL_render.h>

class BackGround: public GameObject
{
    public:
    BackGround(SDL_Renderer* renderer)
    {
        snprintf(Tag, 100, "BackGround");
        Type = GameObjectTypeEnum::DRAWABLE;

        SDL_Point texture_size = {0};

        char* pngPath = NULL;

        SDL_asprintf(&pngPath, "%sAssets/MapBase.png", SDL_GetBasePath());

        SDL_Surface* surface = SDL_LoadPNG(pngPath);

        SDL_free(pngPath);

        texture_size.x = surface->w;
        texture_size.y = surface->h;

        Dimensions.w = SourceRect.w = surface->w;
        Dimensions.h = SourceRect.h = surface->h;
        SourceRect.x = 0;
        SourceRect.y = 0;

        Dimensions.y = Dimensions.x = 100;

        Texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_DestroySurface(surface);
    }

    void Update(float deltaTime) override
    {
        GameObject::Update(deltaTime);

        for(int i = 0; i < _children.size(); i++)
        {
            if(_children[i]->Tag[0] != 'F')
            {
                _children[i]->Dimensions.y = SDL_clamp(_children[i]->Dimensions.y, .5f * _children[i]->Dimensions.x - 1250, .5f * _children[i]->Dimensions.x + 1530);
                _children[i]->Dimensions.y = SDL_clamp(_children[i]->Dimensions.y, -.5f * _children[i]->Dimensions.x + 1930, -.5f * _children[i]->Dimensions.x + 4670);
                _children[i]->Dimensions.x = SDL_clamp(_children[i]->Dimensions.x, 400, 5912);
            }
        }
    }
};
