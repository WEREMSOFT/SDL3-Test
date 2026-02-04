#pragma once
#include "../core/GameObject.hpp"
#include <SDL3/SDL_rect.h>
#include "../core/Vector2.hpp"
#include <SDL3/SDL_render.h>
#include <cstdlib>
#include "Car.hpp"
#include "Animal.hpp"
#include "Piggeon.hpp"
#include "Squirrel.hpp"

#define ENTITY_COUNT 100001

class PalomaSystem: public GameObject
{
    float elapsedFrametime = 0;
    Car* _car;
    SDL_FRect shadowSource = {0};
    AnimationDefinition squirrelAnimations[(int)SquirrelAnimationEnum::COUNT] = {0};

    public:
    Animal* Palomas;
    SDL_Texture* Texture;
    PalomaSystem(SDL_Texture* texture, Car* car)
    {
        Palomas = (Animal*) calloc(ENTITY_COUNT, sizeof(Animal));
        Texture = texture;
        Type = GameObjectTypeEnum::DRAWABLE;
        _car = car;

        for(int i = 0; i < ENTITY_COUNT - 1; i++)
        {
            Piggeon::Init(&Palomas[i]);
        }

        Squirrel::Init(&Palomas[ENTITY_COUNT-1], _car);

        shadowSource.x = 0;
        shadowSource.y = 128;
        shadowSource.w = 32;
        shadowSource.h = 32;

        qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);

        squirrelAnimations[(int)SquirrelAnimationEnum::IDLE_1].frames = 5;
        squirrelAnimations[(int)SquirrelAnimationEnum::IDLE_2].frames = 5;
        squirrelAnimations[(int)SquirrelAnimationEnum::IDLE_3].frames = 1;
        squirrelAnimations[(int)SquirrelAnimationEnum::FLYING].frames = 7;
    }

    ~PalomaSystem()
    {
        free(Palomas);
    }

    // // Custom Sort Algorithm
    // int partition(Animal arr[], int low, int high)
    // {
    //     // Initilize pivot to the first element
    //     auto p = arr[low];
    //     int i = low;
    //     int j = high;

    //     while(i < j)
    //     {
    //         while(arr[i].Dimensions.y <= p.Dimensions.y && i <= high - 1)
    //         {
    //             i++;
    //         }
    //         while(arr[j].Dimensions.y > p.Dimensions.y && j >= low + 1)
    //         {
    //             j--;
    //         }
    //         if(i < j)
    //         {
    //             auto value = arr[i];
    //             arr[i] = arr[j];
    //             arr[j] = value;
    //         }
    //     }
    //     auto value = arr[low];
    //     arr[low] = arr[j];
    //     arr[j] = value;
    //     return j;
    // }

    // void QuickSort(Animal arr[], int low, int high)
    // {
    //     if(low < high)
    //     {
    //         int pi = partition(arr, low, high);

    //         QuickSort(arr, low, pi - 1);
    //         QuickSort(arr, pi + 1, high);
    //     }
    // }

    int partition(Animal arr[], int low, int high)
    {
        // Initilize pivot to the first element
        auto p = arr[low];
        int i = low;
        int j = high;

        while(i < j)
        {
            while(arr[i].Angle <= p.Angle && i <= high - 1)
            {
                i++;
            }
            while(arr[j].Angle > p.Angle && j >= low + 1)
            {
                j--;
            }
            if(i < j)
            {
                auto value = arr[i];
                arr[i] = arr[j];
                arr[j] = value;
            }
        }
        auto value = arr[low];
        arr[low] = arr[j];
        arr[j] = value;
        return j;
    }

    void QuickSort(Animal arr[], int low, int high)
    {
        if(low < high)
        {
            int pi = partition(arr, low, high);

            QuickSort(arr, low, pi - 1);
            QuickSort(arr, pi + 1, high);
        }
    }

    void Update(float deltaTime) override
    {
        elapsedFrametime += deltaTime;

        for(int i = 0; i < ENTITY_COUNT; i++)
        {
            switch(Palomas[i].Type)
            {
                case AnimalTypeEnum::Piggeon:
                    Piggeon::Update(&Palomas[i], deltaTime, _car, elapsedFrametime);
                    break;
                case AnimalTypeEnum::Squirrel:
                    Squirrel::Update(&Palomas[i], deltaTime, _car, elapsedFrametime, squirrelAnimations);
                    break;
            }
        }

        if(elapsedFrametime > 0.1)
        {
            elapsedFrametime = 0;
            // qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);
            QuickSort(Palomas, 0, ENTITY_COUNT - 1);
        }
    }

    void Draw(SDL_Renderer* renderer) override
    {
        for(int i = 0; i < ENTITY_COUNT; i++)
        {
            DrawPaloma(&Palomas[i], renderer);
        }
    }

    static int comparePaloma(const void* a, const void* b)
    {
        const Animal* pa = (const Animal*)a;
        const Animal* pb = (const Animal*)b;

        if (pa->Dimensions.y < pb->Dimensions.y) return -1;
        if (pa->Dimensions.y > pb->Dimensions.y) return 1;
        return 0;
    }

    void DrawPaloma(Animal* paloma, SDL_Renderer *renderer)
    {
        SDL_FRect worldPosition = GetWorldPositionsPaloma(paloma);

        worldPosition.y += paloma->baseDifferenceY;

        SDL_RenderTexture(renderer, Texture, &shadowSource, &worldPosition);

        worldPosition.y -= paloma->baseDifferenceY;

        if(paloma->direction.x < 0)
        {
            SDL_RenderTextureRotated(
                renderer,
                Texture,
                &paloma->SourceRect,          // src rect (NULL = textura completa)
                &worldPosition,
                0.0,           // ángulo
                NULL,          // centro (NULL = centro del dst)
                SDL_FLIP_HORIZONTAL
            );
        } else {
            SDL_RenderTexture(renderer, Texture, &paloma->SourceRect, &worldPosition);
        }
    }

    SDL_FRect GetWorldPositionsPaloma(Animal* paloma)
    {
        auto tempParent = Parent;
        SDL_FRect worldPosition = { paloma->Dimensions.x, paloma->Dimensions.y, paloma->Dimensions.w, paloma->Dimensions.h};
        while(tempParent != nullptr)
        {
            worldPosition.x += tempParent->Dimensions.x;
            worldPosition.y += tempParent->Dimensions.y;
            tempParent = tempParent->Parent;
        }
        return worldPosition;
    }

    void ConstraintObjectsToMap()
    {
        for (int i = 0; i < ENTITY_COUNT; i++) {
            Palomas[i].Dimensions.y = SDL_clamp(Palomas[i].Dimensions.y, .5f * Palomas[i].Dimensions.x - 1250, .5f * Palomas[i].Dimensions.x + 1530);
            Palomas[i].Dimensions.y = SDL_clamp(Palomas[i].Dimensions.y, -.5f * Palomas[i].Dimensions.x + 1930, -.5f * Palomas[i].Dimensions.x + 4670);
            Palomas[i].Dimensions.x = SDL_clamp(Palomas[i].Dimensions.x, 400, 5912);
        }
    }
};
