#pragma once
#include "../core/GameObject.hpp"
#include "CircleBehavior.hpp"
#include "KeyboardBehavior.hpp"
#include "Car.hpp"
#include "LandingBase.hpp"
#include "TileGround.hpp"
#include "Zombie.hpp"
#include <SDL3/SDL_render.h>

class World: public GameObject
{
    SDL_Texture *texture = NULL;

    public:
        World(SDL_Renderer* renderer)
        {
            Type = GameObjectTypeEnum::DRAWABLE;

            snprintf(Tag, 100, "World");

            auto landingBase = new LandingBase(renderer);
            auto car = new Car(renderer);
            car->AddChild(landingBase);
            // auto ground = new TileGround(renderer, car);
            // auto zombie = new Zombie(renderer);
            // AddChild(ground);
            AddChild(car);
            // AddChild(zombie);
        }

        ~World()
        {
            printf("destroying world\n");
        }

        void Draw(SDL_Renderer* renderer) override
        {
            GameObject::Draw(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
            SDL_RenderDebugText(renderer, 0, 0, "WASD to accelerate, break and turn.");
            SDL_RenderDebugText(renderer, 0, 20, "Esc to quit(on desktop)");
        }
};
