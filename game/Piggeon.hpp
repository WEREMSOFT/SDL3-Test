#pragma once
#include "MovingGameObject.hpp"
#include "Car.hpp"
#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_stdinc.h>
#include <cmath>
#include <cstdlib>

class Piggeon: public MovingGameObject
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

    const int NUM_FRAMES = 8;
    SDL_FRect _prevDimensions = {0};
    Car *_car;

    float elapsedFrametime = 0;
    float elapsedIddleTime = 0;
    float velocityY = 0;
    float baseY = 0;
    Vector2f direction = { 0, 1. };

    public:
    int Animation = (int)AnimationEnum::IDLE_2;

    int State = (int)State::IDLE;

    Piggeon(SDL_Renderer* renderer, Car* car)
    {
        _car = car;
        Tag = "Piggeon";
        Type = GameObjectTypeEnum::DRAWABLE;

        Velocity = 100.;
        AngularVelocity = 2.;

        SDL_Point texture_size = {0};

        char* pngPath = NULL;

        SDL_asprintf(&pngPath, "%sAssets/Pigeon.png", SDL_GetBasePath());

        SDL_Surface* surface = SDL_LoadPNG(pngPath);

        SDL_free(pngPath);

        texture_size.x = surface->w;
        texture_size.y = surface->h;

        Dimensions.h = Dimensions.w = SourceRect.h = SourceRect.w = 32;
        SourceRect.x = 0;
        SourceRect.y = 0;

        Texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_DestroySurface(surface);
    }

    void Update(float deltaTime) override
    {
        elapsedFrametime += deltaTime;
        elapsedIddleTime += deltaTime;

        GameObject::Update(deltaTime);

        SourceRect.y = 32 * Animation;

        Vector2f distanceV;
        float distance;

        if(baseY == 0)
        {
            baseY = Dimensions.y;
        }

        distanceV = (Vector2f){ Dimensions.x - (_car->Dimensions.x + _car->Dimensions.w * .5f), Dimensions.y - (_car->Dimensions.y + _car->Dimensions.h * .5f) };
        distance = Length(distanceV);

        switch (State) {
            case int(State::IDLE):
                if(elapsedIddleTime > 2.)
                {
                    elapsedIddleTime = 0;
                    Animation = (int)(random() % 2 == 0 ? AnimationEnum::IDLE_1 : AnimationEnum::IDLE_2);
                }

                if(distance < 100.)
                {
                    State = (int)State::FLYING;
                    Animation = (int)AnimationEnum::FLYING;
                    baseY = Dimensions.y;
                    velocityY = -100.;
                    Angle = SDL_randf() * (M_PI * 2.);
                    direction = Rotate(direction, Angle);
                }

                break;
            case (int)State::FLYING:


                const float gravity = 50.8;

                velocityY += gravity * deltaTime;
                Dimensions.y += velocityY * deltaTime ;

                // printf("Velocity: %.2f\n", velocityY);

                Vector2f vecIncrement = Scale(direction, Velocity * deltaTime);
                Dimensions.x += vecIncrement.x;
                Dimensions.y += vecIncrement.y;
                baseY += vecIncrement.y;

                if(Dimensions.y > baseY)
                {
                    velocityY = -15.;
                    Dimensions.y = baseY;
                    State = (int)State::IDLE;
                }
        }

        if(elapsedFrametime > 0.1)
        {
            elapsedFrametime = 0;
            SourceRect.x += 32;
            if(SourceRect.x > (32 * 3))
            {
                SourceRect.x = 0;
            }
        }

        _prevDimensions = Dimensions;
    }

    void Draw(SDL_Renderer* renderer) override
    {
        auto dimensionTemp = Dimensions;
        Dimensions.y = baseY;
        for(int i = 0; i < Children.size(); i++)
        {
            if(Children[i]->Type == GameObjectTypeEnum::DRAWABLE)
            {
                Children[i]->Draw(renderer);
            }
        }

        Dimensions = dimensionTemp;

        if(Texture != NULL)
        {
            auto tempParent = Parent;
            SDL_FRect worldPosition = GetWorldPositions();

            if(SourceRect.h == 0 || SourceRect.w == 0)
                SDL_RenderTexture(renderer, Texture, NULL, &worldPosition);
            else
            {
                if(direction.x < 0)
                {
                    SDL_RenderTextureRotated(
                        renderer,
                        Texture,
                        &SourceRect,          // src rect (NULL = textura completa)
                        &worldPosition,
                        0.0,           // ángulo
                        NULL,          // centro (NULL = centro del dst)
                        SDL_FLIP_HORIZONTAL
                    );
                } else {
                    SDL_RenderTexture(renderer, Texture, &SourceRect, &worldPosition);
                }
            }
        }
    }
};
