#pragma once
#include "../core/GameObject.hpp"
#include <SDL3/SDL_rect.h>
#include "../core/Vector2.hpp"
#include <SDL3/SDL_render.h>
#include <cstdlib>
#include "Car.hpp"

struct Animal
{
    SDL_FRect Dimensions;
    SDL_FRect SourceRect;
    Vector2f direction;
    int Animation;
    int State;
    int Type;
    float elapsedIddleTime;
    float baseY;
    float Angle;
    float velocityY;
    float baseDifferenceY;
    float Velocity;
};

#define ENTITY_COUNT 100000

class PalomaSystem: public GameObject
{
    enum class AnimationEnum
    {
        IDLE_1,
        WALKING,
        IDLE_2,
        FLYING,
        COUNT
    };

    enum class State{
        IDLE,
        FLYING,
        COUNT
    };

    float elapsedFrametime = 0;
    Car* _car;
    SDL_FRect shadowSource = {0};

    public:

    Animal* Palomas;
    SDL_Texture* Texture;
    PalomaSystem(SDL_Texture* texture, Car* car)
    {
        Palomas = (Animal*) calloc(ENTITY_COUNT, sizeof(Animal));
        Texture = texture;
        Type = GameObjectTypeEnum::DRAWABLE;
        _car = car;

        for(int i = 0; i < ENTITY_COUNT; i++)
        {
            initPaloma(&Palomas[i]);
        }

        shadowSource.x = 0;
        shadowSource.y = 128;
        shadowSource.w = 32;
        shadowSource.h = 32;

        qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);
    }

    ~PalomaSystem()
    {
        free(Palomas);
    }

    void Update(float deltaTime) override
    {
        elapsedFrametime += deltaTime;

        for(int i = 0; i < ENTITY_COUNT; i++)
        {
            UpdatePaloma(&Palomas[i], deltaTime);
        }

        if(elapsedFrametime > 0.1)
        {
            elapsedFrametime = 0;
            qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);
        }
    }

    void UpdatePaloma(Animal* paloma, float deltaTime)
    {
        paloma->elapsedIddleTime += deltaTime;
        paloma->SourceRect.y = 32 * paloma->Animation;

        Vector2f distanceV;
        float distance;

        if(paloma->baseY == 0)
        {
            paloma->baseY = paloma->Dimensions.y;
        }

        distanceV = (Vector2f){ paloma->Dimensions.x - (_car->Dimensions.x + _car->Dimensions.w * .5f), paloma->Dimensions.y - (_car->Dimensions.y + _car->Dimensions.h * .5f) };
        distance = Length(distanceV);

        switch (paloma->State) {
            case int(State::IDLE):
                if(paloma->elapsedIddleTime > 2.)
                {
                    paloma->elapsedIddleTime = 0;
                    paloma->Animation = (int)(random() % 2 == 0 ? AnimationEnum::IDLE_1 : AnimationEnum::IDLE_2);
                }

                if(distance < 100.)
                {
                	distanceV = Normalize(distanceV);
                    paloma->State = (int)State::FLYING;
                    paloma->Animation = (int)AnimationEnum::FLYING;
                    paloma->baseY = paloma->Dimensions.y;
                    paloma->velocityY = -100.;
                    paloma->Angle = SDL_randf() * M_PI * 0.5 - M_PI * 0.25;
                    paloma->direction = Rotate(distanceV, paloma->Angle);
                }

                break;
            case (int)State::FLYING:
                const float gravity = 50.8;

                paloma->velocityY += gravity * deltaTime;
                paloma->Dimensions.y += paloma->velocityY * deltaTime ;

                Vector2f vecIncrement = Scale(paloma->direction, paloma->Velocity * deltaTime);
                paloma->Dimensions.x += vecIncrement.x;
                paloma->Dimensions.y += vecIncrement.y;
                paloma->baseY += vecIncrement.y;
                paloma->baseDifferenceY = paloma->baseY - paloma->Dimensions.y;
                if(paloma->Dimensions.y > paloma->baseY)
                {
                    paloma->velocityY = -15.;
                    paloma->Dimensions.y = paloma->baseY;
                    paloma->State = (int)State::IDLE;
                }
        }

        paloma->Dimensions.y = SDL_clamp(paloma->Dimensions.y, .5f * paloma->Dimensions.x - 1250, .5f * paloma->Dimensions.x + 1530);
        paloma->Dimensions.y = SDL_clamp(paloma->Dimensions.y, -.5f * paloma->Dimensions.x + 1930, -.5f * paloma->Dimensions.x + 4670);
        paloma->Dimensions.x = SDL_clamp(paloma->Dimensions.x, 400, 5912);

        if(elapsedFrametime > 0.1)
        {
            paloma->SourceRect.x += 32;
            if(paloma->SourceRect.x > (32 * 3))
            {
                paloma->SourceRect.x = 0;
            }
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

    void initPaloma(Animal* paloma)
    {
        paloma->Velocity = 100.;
        paloma->Dimensions.h = paloma->Dimensions.w = paloma->SourceRect.h = paloma->SourceRect.w = 32;
        paloma->SourceRect.x = 0;
        paloma->SourceRect.y = 0;

        float piggeonX = 2045.f + SDL_randf() * (3900.f - 2045.f);
        float piggeonY = 906.f + SDL_randf() * (1991.f - 906.f);

        paloma->Dimensions.x = piggeonX;
        paloma->Dimensions.y = piggeonY;
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
