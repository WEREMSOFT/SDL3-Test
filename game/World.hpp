#pragma once
#include "../core/GameObject.hpp"
#include "Car.hpp"
#include "BackGround.hpp"
#include "ForeGround.hpp"
#include "GenericImage.hpp"
#include "Piggeon.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>

class World: public GameObject
{
    SDL_Texture *texture = NULL;
    Car* _car;
    BackGround* _backGround;

    public:
        World(SDL_Renderer* renderer)
        {
            Type = GameObjectTypeEnum::DRAWABLE;

            snprintf(Tag, 100, "World");

            _backGround = new BackGround(renderer);
            _backGround->Dimensions.x = -_backGround->Dimensions.w / 2.;
            _backGround->Dimensions.y = -_backGround->Dimensions.h / 2.;
            _car = new Car(renderer);

            // _car->Dimensions.y = 384;
            // _car->Dimensions.x = 512;

            _car->Dimensions.x = _backGround->Dimensions.w / 2.;
            _car->Dimensions.y = _backGround->Dimensions.h / 2.;

            // auto landingPlatform = new LandingBase(renderer);
            // auto ground = new TileGround(renderer, car);
            // auto zombie = new Zombie(renderer);
            // AddChild(ground);
            AddChild(_backGround);
            _backGround->AddChild(_car);

            const int piggeonSideCount = 50;

            for(int i = 0; i < piggeonSideCount; i++)
            {
                    float piggeonX = 2045.f + SDL_randf() * (3900.f - 2045.f);
                    float piggeonY = 906.f + SDL_randf() * (1991.f - 906.f);
                    auto piggeon = new Piggeon(renderer, _car);
                    auto piggeonShadow = new GenericImage(renderer, "Assets/pigeonShadow.png");
                    piggeonShadow->Dimensions.x = 7;
                    piggeonShadow->Dimensions.y = 20;
                    piggeon->Dimensions.x = piggeonX;
                    piggeon->Dimensions.y = piggeonY;
                    piggeon->AddChild(piggeonShadow);
                    _backGround->AddChild(piggeon);
            }


            // AddChild(_car);
            // _car->AddChild(landingPlatform);
            // AddChild(zombie);
            auto treesFront = new ForeGround(renderer);

            _backGround->AddChild(treesFront);
        }

        ~World()
        {
            printf("destroying world\n");
        }

        void Update(float deltaTime) override
        {
            GameObject::Update(deltaTime);
            auto carPosition = _car->GetWorldPositions();
            _backGround->Dimensions.x = -_car->Dimensions.x + 412;
            _backGround->Dimensions.y = -_car->Dimensions.y + 300;

            for(int i = 0; i < _children.size(); i++)
            {
                if(_children[i]->Tag[0] != 'B' && _children[i]->Tag[0] != 'F')
                {
                    _children[i]->Dimensions.y = SDL_clamp(_children[i]->Dimensions.y, .5f * _children[i]->Dimensions.x - 1250, .5f * _children[i]->Dimensions.x + 1530);
                    _children[i]->Dimensions.y = SDL_clamp(_children[i]->Dimensions.y, -.5f * _children[i]->Dimensions.x + 1930, -.5f * _children[i]->Dimensions.x + 4670);
                    _children[i]->Dimensions.x = SDL_clamp(_children[i]->Dimensions.x, 400, 5912);
                }
            }
        }

        void Draw(SDL_Renderer* renderer) override
        {
            GameObject::Draw(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
            SDL_RenderDebugText(renderer, 0, 0, "WASD to accelerate, break and turn.");
            SDL_RenderDebugText(renderer, 0, 20, "Esc to quit(on desktop)");

            char carPositionText[300] = {0};
            snprintf(carPositionText, 300, "Car Position: %.2f, %.2f", _car->Dimensions.x, _car->Dimensions.y);

            SDL_RenderDebugText(renderer, 0, 40, carPositionText);
        }
};
