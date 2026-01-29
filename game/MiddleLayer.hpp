#pragma once
#include "../core/GameObject.hpp"
#include "GenericImage.hpp"
#include "Piggeon.hpp"
#include <vector>

class MiddleLayer: public GameObject
{
    public:
    std::vector<Piggeon> Piggeons;
    const int piggeonSideCount = 100000;

    MiddleLayer(SDL_Renderer *renderer, Car* car)
    {
        Tag = "piggeonContainer";
        Type = GameObjectTypeEnum::DRAWABLE;

        AddChild(car);

        Piggeons.reserve(piggeonSideCount);

        for(int i = 0; i < piggeonSideCount; i++)
        {
                float piggeonX = 2045.f + SDL_randf() * (3900.f - 2045.f);
                float piggeonY = 906.f + SDL_randf() * (1991.f - 906.f);
                auto& piggeon = Piggeons.emplace_back(renderer, car);
                piggeon.Dimensions.x = piggeonX;
                piggeon.Dimensions.y = piggeonY;
                piggeon.Parent = this;
        }
    }

    void Update(float deltaTime) override
    {
        GameObject::Update(deltaTime);
        for (Piggeon& p : Piggeons) {
            p.Update(deltaTime);
        }
    }

    void Draw(SDL_Renderer* renderer) override
    {
        GameObject::Draw(renderer);
        for (Piggeon& p : Piggeons) {
            p.Draw(renderer);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        char fpsText[100] = {0};
        snprintf(fpsText, 100, "number of piggeons: %d", piggeonSideCount);
        SDL_RenderDebugText(renderer, 300, 20, fpsText);
    }
};
