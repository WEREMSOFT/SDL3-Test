#pragma once
#include "../core/GameObject.hpp"
#include "GenericImage.hpp"
#include "Piggeon.hpp"
#include <algorithm>
#include <vector>
#include <algorithm>

class MiddleLayer: public GameObject
{
    public:
    std::vector<GameObject*> Piggeons;
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

                auto piggeon = new Piggeon(renderer, car);
                Piggeons.emplace_back(piggeon);
                piggeon->Dimensions.x = piggeonX;
                piggeon->Dimensions.y = piggeonY;
                piggeon->Parent = this;
        }
    }

    ~MiddleLayer()
    {
        for (auto p : Piggeons) {
            delete p;
        }
    }

    void Update(float deltaTime) override
    {
        GameObject::Update(deltaTime);
        for (auto p : Piggeons) {
            p->Update(deltaTime);
        }

        std::sort(Piggeons.begin(), Piggeons.end(), [](const GameObject* a, const GameObject* b) {
            return a->Dimensions.y < b->Dimensions.y;
        });
    }

    void Draw(SDL_Renderer* renderer) override
    {
        GameObject::Draw(renderer);
        for (GameObject* p : Piggeons) {
            p->Draw(renderer);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        char fpsText[100] = {0};
        snprintf(fpsText, 100, "number of piggeons: %d", piggeonSideCount);
        SDL_RenderDebugText(renderer, 300, 20, fpsText);
    }
};
