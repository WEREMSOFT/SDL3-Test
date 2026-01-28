#pragma once
#include "../core/GameObject.hpp"
#include "GenericImage.hpp"
#include "Piggeon.hpp"
#include <vector>

class MiddleLayer: public GameObject
{
    std::vector<Piggeon> piggeons;
    public:

    MiddleLayer(SDL_Renderer *renderer, Car* car)
    {
        Tag = "piggeonContainer";
        Type = GameObjectTypeEnum::DRAWABLE;

        AddChild(car);

        const int piggeonSideCount = 70000;
        piggeons.reserve(piggeonSideCount);

        for(int i = 0; i < piggeonSideCount; i++)
        {
                float piggeonX = 2045.f + SDL_randf() * (3900.f - 2045.f);
                float piggeonY = 906.f + SDL_randf() * (1991.f - 906.f);
                auto& piggeon = piggeons.emplace_back(renderer, car);
                // std::string piggeonShadowPath = "Assets/pigeonShadow.png";
                // auto piggeonShadow = new GenericImage(renderer, piggeonShadowPath);
                // piggeonShadow->Dimensions.x = 7;
                // piggeonShadow->Dimensions.y = 20;
                piggeon.Dimensions.x = piggeonX;
                piggeon.Dimensions.y = piggeonY;
                // piggeon.AddChild(piggeonShadow);
                piggeon.Parent = this;

                // AddChild(piggeon);
        }
    }

    void Update(float deltaTime) override
    {
        GameObject::Update(deltaTime);
        for (Piggeon& p : piggeons) {
            p.Update(deltaTime);
        }
    }

    void Draw(SDL_Renderer* renderer) override
    {
        GameObject::Draw(renderer);
        for (Piggeon& p : piggeons) {
            p.Draw(renderer);
        }
    }
};
