#pragma once
#include "../core/GameObject.hpp"
#include "Car.hpp"
#include "BackGround.hpp"
#include "ForeGround.hpp"
#include "GenericImage.hpp"
#include "MiddleLayer.hpp"
#include "Piggeon.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>

class World: public GameObject
{
    SDL_Texture *texture = NULL;
    Car* _car;
    BackGround* _backGround;
    MiddleLayer* _middleLayer;

    public:
        World(SDL_Renderer* renderer)
        {
            Type = GameObjectTypeEnum::DRAWABLE;

            Tag = "World";

            _backGround = new BackGround(renderer);
            Dimensions.x = -_backGround->Dimensions.w / 2.;
            Dimensions.y = -_backGround->Dimensions.h / 2.;
            AddChild(_backGround);

            _car = new Car(renderer);
            _car->Dimensions.x = _backGround->Dimensions.w / 2.;
            _car->Dimensions.y = _backGround->Dimensions.h / 2.;


            _middleLayer = new MiddleLayer(renderer, _car);

            AddChild(_middleLayer);

            auto treesFront = new ForeGround(renderer);
            AddChild(treesFront);
        }

        ~World()
        {
            printf("destroying world\n");
        }

        void Update(float deltaTime) override
        {
            GameObject::Update(deltaTime);
            auto carPosition = _car->GetWorldPositions();
            Dimensions.x = -_car->Dimensions.x + 412;
            Dimensions.y = -_car->Dimensions.y + 300;

            // ConstraintObjectsToMap(_backGround->Children);
            ConstraintObjectsToMap(_middleLayer->Children);
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

        void ConstraintObjectsToMap(std::vector<GameObject*> objectsToConstraint)
        {
            for(int i = 0; i < objectsToConstraint.size(); i++)
            {
                // if(objectsToConstraint[i]->Tag[0] != 'B' && objectsToConstraint[i]->Tag[0] != 'F')
                {
                    objectsToConstraint[i]->Dimensions.y = SDL_clamp(objectsToConstraint[i]->Dimensions.y, .5f * objectsToConstraint[i]->Dimensions.x - 1250, .5f * objectsToConstraint[i]->Dimensions.x + 1530);
                    objectsToConstraint[i]->Dimensions.y = SDL_clamp(objectsToConstraint[i]->Dimensions.y, -.5f * objectsToConstraint[i]->Dimensions.x + 1930, -.5f * objectsToConstraint[i]->Dimensions.x + 4670);
                    objectsToConstraint[i]->Dimensions.x = SDL_clamp(objectsToConstraint[i]->Dimensions.x, 400, 5912);
                }
            }
        }
};
