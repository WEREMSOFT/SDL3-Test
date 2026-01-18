#pragma once
#include <math.h>
#include <stdio.h>
#include "../core/GameObject.hpp"
#include <SDL3/SDL.h>

class KeyboardBehavior: public GameObject
{
    const float _velocity = 100.;
    public:
        KeyboardBehavior()
        {
            Type = GameObjectTypeEnum::BEHAVIOR;
            snprintf(Tag, 100, "Keyboard Behavior");
        }

        void Update(float deltaTime) override
        {
            GameObject::Update(deltaTime);

            static float phase = 0;

            phase += 2. * deltaTime;

            const bool* keys = SDL_GetKeyboardState(NULL);

            if (keys[SDL_SCANCODE_W]) {
                Parent->Dimensions.y -= _velocity * deltaTime;
            }

            if (keys[SDL_SCANCODE_S]) {
                Parent->Dimensions.y += _velocity * deltaTime;
            }

            if (keys[SDL_SCANCODE_D]) {
                Parent->Dimensions.x += _velocity * deltaTime;
            }

            if (keys[SDL_SCANCODE_A]) {
                Parent->Dimensions.x -= _velocity * deltaTime;
            }
        }
};
