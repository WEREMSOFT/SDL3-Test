#pragma once
#include "MovingGameObject.hpp"
#include "Car.hpp"
#include "Piggeon.hpp"
#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_stdinc.h>
#include <cmath>
#include <cstdlib>

class Squirrel: public Piggeon
{
    enum class AnimationEnum
    {
        IDLE_1 = 0,
        IDLE_2,
        FLYING,
        WALKING,
        IDLE_3,
        COUNT
    };

    enum class State{
        IDLE,
        FLYING,
        COUNT
    };

    public:
    int FrameStart = 128 + 32;
    AnimationDefinition animations[(int)AnimationEnum::COUNT] = {0};
    int State = (int)State::IDLE;

    Squirrel(SDL_Renderer* renderer, Car* car): Piggeon(renderer, car)
    {
        Tag = "Squirrel";
        Animation = (int)AnimationEnum::IDLE_3;


        animations[(int)AnimationEnum::IDLE_1].frames = 5;
        animations[(int)AnimationEnum::IDLE_2].frames = 5;
        animations[(int)AnimationEnum::IDLE_3].frames = 1;
        animations[(int)AnimationEnum::FLYING].frames = 7;
    }

    void Update(float deltaTime) override
    {
        elapsedFrametime += deltaTime;
        elapsedIddleTime += deltaTime;

        GameObject::Update(deltaTime);

        SourceRect.y = 32 * Animation + FrameStart;

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
                    Animation = (int)(random() % 2 == 0 ? AnimationEnum::IDLE_1 : AnimationEnum::IDLE_3);
                }

  	            if(distance < 100.)
                {
                	distanceV = Normalize(distanceV);
                    State = (int)State::FLYING;
                    Animation = (int)AnimationEnum::FLYING;
                    baseY = Dimensions.y;
                    velocityY = -100.;
                    Angle = SDL_randf() * M_PI * 0.5 - M_PI * 0.25;
                    direction = Rotate(distanceV, Angle);
                }


                break;
            case (int)State::FLYING:
                const float gravity = 50.8;

                velocityY += gravity * deltaTime;
                Dimensions.y += velocityY * deltaTime ;

                Vector2f vecIncrement = Scale(direction, Velocity * deltaTime);
                Dimensions.x += vecIncrement.x;
                Dimensions.y += vecIncrement.y;
                baseY += vecIncrement.y;
                baseDifferenceY = baseY - Dimensions.y;
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
            if(SourceRect.x > (32 * animations[Animation].frames))
            {
                SourceRect.x = 0;
            }
        }

        _prevDimensions = Dimensions;
    }

};
