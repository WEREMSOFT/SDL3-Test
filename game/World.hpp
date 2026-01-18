#pragma once
#include "../core/GameObject.hpp"
#include "CircleBehavior.hpp"
#include "KeyboardBehavior.hpp"
#include "Car.hpp"

class World: public GameObject
{
    SDL_Texture *texture = NULL;

    public:
        World(SDL_Renderer* renderer)
        {
            Type = GameObjectTypeEnum::DRAWABLE;

            snprintf(Tag, 100, "World");

            Car* car = new Car(renderer);
            AddChild(car);
        }

        ~World()
        {
            printf("destroying world\n");
        }
};
