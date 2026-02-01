#pragma once
#include "../core/GameObject.hpp"
#include <SDL3/SDL_rect.h>
#include "../core/Vector2.hpp"
#include <SDL3/SDL_render.h>
#include <cstdlib>
#include "Car.hpp"

struct AnimationDefinition
{
    int frames;
};

enum class SquirrelAnimationEnum
{
    IDLE_1 = 0,
    IDLE_2,
    FLYING,
    WALKING,
    IDLE_3,
    COUNT
};

enum class PiggeonAnimationEnum
{
    IDLE_1,
    WALKING,
    IDLE_2,
    FLYING,
    COUNT
};


enum AnimalTypeEnum
{
    Piggeon,
    Squirrel
};

struct Animal
{
    SDL_FRect Dimensions;
    SDL_FRect SourceRect;
    Vector2f direction;
    int Animation;
    int State;
    AnimalTypeEnum Type;
    float elapsedIddleTime;
    float baseY;
    float Angle;
    float velocityY;
    float baseDifferenceY;
    float Velocity;
};

#define ENTITY_COUNT 100001

class PalomaSystem: public GameObject
{

    enum class State{
        IDLE,
        FLYING,
        COUNT
    };

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
            InitPaloma(&Palomas[i]);
        }

        InitSquirrel(&Palomas[ENTITY_COUNT-1]);

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

    void Update(float deltaTime) override
    {
        elapsedFrametime += deltaTime;

        for(int i = 0; i < ENTITY_COUNT; i++)
        {
            switch(Palomas[i].Type)
            {
                case AnimalTypeEnum::Piggeon:
                    UpdatePaloma(&Palomas[i], deltaTime);
                    break;
                case AnimalTypeEnum::Squirrel:
                    UpdateSquirrel(&Palomas[i], deltaTime);
                    break;
            }
        }

        if(elapsedFrametime > 0.1)
        {
            elapsedFrametime = 0;
            qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);
        }
    }

    void UpdateSquirrel(Animal* squirrel, float deltaTime)
    {
        static const int FrameStart = 128 + 32;
        squirrel->elapsedIddleTime += deltaTime;
        squirrel->SourceRect.y = 32 * squirrel->Animation + FrameStart;

        Vector2f distanceV;
        float distance;

        if(squirrel->baseY == 0)
        {
            squirrel->baseY = squirrel->Dimensions.y;
        }

        distanceV = (Vector2f){ squirrel->Dimensions.x - (_car->Dimensions.x + _car->Dimensions.w * .5f), squirrel->Dimensions.y - (_car->Dimensions.y + _car->Dimensions.h * .5f) };
        distance = Length(distanceV);

        switch (squirrel->State) {
            case int(State::IDLE):
                if(squirrel->elapsedIddleTime > 2.)
                {
                    squirrel->elapsedIddleTime = 0;
                    squirrel->Animation = (int)(random() % 2 == 0 ? SquirrelAnimationEnum::IDLE_1 : SquirrelAnimationEnum::IDLE_3);
                }

  	            if(distance < 100.)
                {
                	distanceV = Normalize(distanceV);
                    squirrel->State = (int)State::FLYING;
                    squirrel->Animation = (int)SquirrelAnimationEnum::FLYING;
                    squirrel->baseY = squirrel->Dimensions.y;
                    squirrel->velocityY = -100.;
                    squirrel->Angle = SDL_randf() * M_PI * 0.5 - M_PI * 0.25;
                    squirrel->direction = Rotate(distanceV, squirrel->Angle);
                }


                break;
            case (int)State::FLYING:
                const float gravity = 50.8;

                squirrel->velocityY += gravity * deltaTime;
                squirrel->Dimensions.y += squirrel->velocityY * deltaTime ;

                Vector2f vecIncrement = Scale(squirrel->direction, squirrel->Velocity * deltaTime);
                squirrel->Dimensions.x += vecIncrement.x;
                squirrel->Dimensions.y += vecIncrement.y;
                squirrel->baseY += vecIncrement.y;
                squirrel->baseDifferenceY = squirrel->baseY - squirrel->Dimensions.y;
                if(squirrel->Dimensions.y > squirrel->baseY)
                {
                    squirrel->velocityY = -15.;
                    squirrel->Dimensions.y = squirrel->baseY;
                    squirrel->State = (int)State::IDLE;
                }
        }


        if(elapsedFrametime > 0.1)
        {
            squirrel->SourceRect.x += 32;
            if(squirrel->SourceRect.x > (32 * squirrelAnimations[squirrel->Animation].frames))
            {
                squirrel->SourceRect.x = 0;
            }
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
                    paloma->Animation = (int)(random() % 2 == 0 ? PiggeonAnimationEnum::IDLE_1 : PiggeonAnimationEnum::IDLE_2);
                }

                if(distance < 100.)
                {
                	distanceV = Normalize(distanceV);
                    paloma->State = (int)State::FLYING;
                    paloma->Animation = (int)PiggeonAnimationEnum::FLYING;
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

    void InitSquirrel(Animal* squirrel)
    {
        squirrel->Type = AnimalTypeEnum::Squirrel;
        squirrel->Animation = (int)SquirrelAnimationEnum::IDLE_3;

        squirrel->Velocity = 100.;
        squirrel->Dimensions.h = squirrel->Dimensions.w = squirrel->SourceRect.h = squirrel->SourceRect.w = 32;
        squirrel->SourceRect.x = 0;
        squirrel->SourceRect.y = 0;

        squirrel->Dimensions.x = _car->Dimensions.x - 100;
        squirrel->Dimensions.y = _car->Dimensions.y;
    }

    void InitPaloma(Animal* paloma)
    {
        paloma->Type = AnimalTypeEnum::Piggeon;
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
