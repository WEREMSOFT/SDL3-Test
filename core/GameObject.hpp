#pragma once
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <vector>

#include <SDL3/SDL.h>

#include "GameObjectTypeEnum.hpp"

class GameObject {
    std::vector<GameObject*> _children;

    public :
        float Angle = 0;
        char Tag[50] = {0};
        SDL_Texture* Texture;
        GameObjectTypeEnum Type;
        GameObject* Parent = nullptr;
        SDL_FRect Dimensions = {0};
        SDL_FRect SourceRect = {0};

        GameObject(GameObjectTypeEnum type = GameObjectTypeEnum::UNKNOWN)
        {
            Type = type;
            _children.reserve(2);
        }

        virtual ~GameObject()
        {
            if(strlen(Tag) == 0)
            {
                printf("destroying game object\n");
            }
            else
            {
                printf("Destroying gameobject with tag: %s\n", Tag);
            }

            for(int i = 0; i < _children.size(); i++)
            {
                delete _children[i];
            }
        }

        virtual void Update(float deltaTime)
        {
            for(int i = 0; i < _children.size(); i++)
            {
                _children[i]->Update(deltaTime);
            }
        }

        virtual void Draw(SDL_Renderer* renderer)
        {
            if(Texture != NULL)
            {
                auto tempParent = Parent;
                SDL_FRect tempDimensions = { Dimensions.x, Dimensions.y, Dimensions.w, Dimensions.h};
                while(tempParent != nullptr)
                {
                    tempDimensions.x += tempParent->Dimensions.x;
                    tempDimensions.y += tempParent->Dimensions.y;
                    tempParent = tempParent->Parent;
                }

                if(SourceRect.h == 0 || SourceRect.w == 0)
                    SDL_RenderTexture(renderer, Texture, NULL, &tempDimensions);
                else
                {
                    SDL_RenderTexture(renderer, Texture, &SourceRect, &tempDimensions);
                }
            }

            for(int i = 0; i < _children.size(); i++)
            {
                if(_children[i]->Type == GameObjectTypeEnum::DRAWABLE)
                {
                    _children[i]->Draw(renderer);
                }
            }
        }

        virtual void AddChild(GameObject* child)
        {
            child->Parent = this;
            _children.push_back(child);
        }

        static GameObject* CreateDrawable(SDL_Texture* texture)
        {
            GameObject *gameObject = new GameObject(GameObjectTypeEnum::DRAWABLE);
            gameObject->Texture = texture;
            gameObject->Dimensions.w = texture->w;
            gameObject->Dimensions.h = texture->h;

            return gameObject;
        }
};
