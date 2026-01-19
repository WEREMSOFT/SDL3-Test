#pragma once
#include <SDL3/SDL_scancode.h>
#include <math.h>
#include <stdio.h>
#include "../core/GameObject.hpp"
#include "../core/Vector2.hpp"

#include <SDL3/SDL.h>

class KeyboardBehavior: public GameObject
{
    const float _velocity = 150.;
    const float _angularVelocity = 2.;

    public:
        KeyboardBehavior()
        {
            Type = GameObjectTypeEnum::BEHAVIOR;
            snprintf(Tag, 100, "Keyboard Behavior");
        }

        void Update(float deltaTime) override
        {
            Vector2f direction = { 0, 1. };
            GameObject::Update(deltaTime);

            static float phase = 0;

            phase += 2. * deltaTime;

            const bool* keys = SDL_GetKeyboardState(NULL);

            if(keys[SDL_SCANCODE_SPACE])
            {
                Parent->Angle = M_PI;
            }

            if (keys[SDL_SCANCODE_D]) {
                Parent->Angle += _angularVelocity * deltaTime;
            }

            if (keys[SDL_SCANCODE_A]) {
                Parent->Angle -= _angularVelocity * deltaTime;
            }

            direction = Rotate(direction, Parent->Angle);

            if (keys[SDL_SCANCODE_W]) {
                Vector2f vecIncrement = Scale(direction, -_velocity * deltaTime);
                Parent->Dimensions.x += vecIncrement.x;
                Parent->Dimensions.y += vecIncrement.y;
            }

            if (keys[SDL_SCANCODE_S]) {
                Vector2f vecIncrement = Scale(direction, _velocity * deltaTime);
                Parent->Dimensions.x += vecIncrement.x;
                Parent->Dimensions.y += vecIncrement.y;
            }
        }
};
