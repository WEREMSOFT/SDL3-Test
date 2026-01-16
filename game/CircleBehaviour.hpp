#pragma once
#include <math.h>
#include <stdio.h>
#include "../core/GameObject.hpp"

class CircleBehaviour: public GameObject
{
    public:
        CircleBehaviour()
        {
            Type = GameObjectTypeEnum::BEHAVIOR;
        }

        ~CircleBehaviour()
        {
            printf("destroying behavior\n");
        }

        void Update(float deltaTime) override
        {
            GameObject::Update(deltaTime);

            static float phase = 0;

            phase += 2. * deltaTime;

            Parent->Dimensions.x = 100.f + sin(phase) * 100;
            Parent->Dimensions.y = 100.f + cos(phase) * 100;
        }
};
