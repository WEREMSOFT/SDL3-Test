#pragma once
#include "../core/GameObject.hpp"
#include <SDL3/SDL_rect.h>
#include "../core/Vector2.hpp"
#include <SDL3/SDL_render.h>
#include <cstdlib>
#include "Car.hpp"
#include "Animal.hpp"
#include "Piggeon.hpp"
#include "Squirrel.hpp"

#define ENTITY_COUNT 100001

#define VERTS_PER_QUAD 4
#define INDICES_PER_QUAD 6

#define MAX_QUADS ENTITY_COUNT
#define MAX_VERTS (MAX_QUADS * VERTS_PER_QUAD)
#define MAX_IDXS (MAX_QUADS * INDICES_PER_QUAD)

class PalomaSystem : public GameObject
{
	float elapsedFrametime = 0;
	Car *_car;
	SDL_FRect shadowSource = {0};
	AnimationDefinition squirrelAnimations[(int)SquirrelAnimationEnum::COUNT] = {0};

	static float positions[ENTITY_COUNT * 2];

	float g_xy[MAX_VERTS * 2];
	float g_uv[MAX_VERTS * 2];
	SDL_FColor g_col[MAX_VERTS];
	Uint32 g_idx[MAX_IDXS];

public:
	Animal *Palomas;
	SDL_Texture *Texture;
	PalomaSystem(SDL_Texture *texture, Car *car)
	{
		Palomas = (Animal *)calloc(ENTITY_COUNT, sizeof(Animal));
		Texture = texture;
		Type = GameObjectTypeEnum::DRAWABLE;
		_car = car;

		for (int i = 0; i < ENTITY_COUNT - 1; i++)
		{
			Piggeon::Init(&Palomas[i]);
		}

		Squirrel::Init(&Palomas[ENTITY_COUNT - 1], _car);

		shadowSource.x = 0;
		shadowSource.y = 128;
		shadowSource.w = 32;
		shadowSource.h = 32;

		qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);

		squirrelAnimations[(int)SquirrelAnimationEnum::IDLE_1].frames = 5;
		squirrelAnimations[(int)SquirrelAnimationEnum::IDLE_2].frames = 5;
		squirrelAnimations[(int)SquirrelAnimationEnum::IDLE_3].frames = 1;
		squirrelAnimations[(int)SquirrelAnimationEnum::FLYING].frames = 7;

		InitQuadIndices();
	}

	~PalomaSystem()
	{
		free(Palomas);
	}

	void Update(float deltaTime) override
	{
		elapsedFrametime += deltaTime;

		for (int i = 0; i < ENTITY_COUNT; i++)
		{
			switch (Palomas[i].Type)
			{
			case AnimalTypeEnum::Piggeon:
				Piggeon::Update(&Palomas[i], deltaTime, _car, elapsedFrametime);
				break;
			case AnimalTypeEnum::Squirrel:
				Squirrel::Update(&Palomas[i], deltaTime, _car, elapsedFrametime, squirrelAnimations);
				break;
			}
		}

		if (elapsedFrametime > 0.1)
		{
			elapsedFrametime = 0;
			qsort(Palomas, ENTITY_COUNT, sizeof(Animal), comparePaloma);
		}
	}

	void Draw(SDL_Renderer *renderer) override
	{

		DrawPalomasBatch(
			Palomas, // array de Animal
			renderer,
			Texture,
			Texture->w,
			Texture->h);

		// for (int i = 0; i < ENTITY_COUNT; i++)
		// {
		// 	DrawPaloma(&Palomas[i], renderer);
		// }
	}
	// #########################################
	void InitQuadIndices(void)
	{
		for (int i = 0; i < MAX_QUADS; i++)
		{
			Uint32 base = i * 4;
			int o = i * 6;

			g_idx[o + 0] = base + 0;
			g_idx[o + 1] = base + 1;
			g_idx[o + 2] = base + 2;
			g_idx[o + 3] = base + 2;
			g_idx[o + 4] = base + 3;
			g_idx[o + 5] = base + 0;
		}
	}

	inline void PushQuad(
		int quad,
		const SDL_FRect *dst,
		const SDL_FRect *src,
		int flip_x,
		int tex_w,
		int tex_h)
	{
		int v = quad * 4;
		int p = v * 2;

		/* posiciones */
		g_xy[p + 0] = dst->x;
		g_xy[p + 1] = dst->y;

		g_xy[p + 2] = dst->x + dst->w;
		g_xy[p + 3] = dst->y;

		g_xy[p + 4] = dst->x + dst->w;
		g_xy[p + 5] = dst->y + dst->h;

		g_xy[p + 6] = dst->x;
		g_xy[p + 7] = dst->y + dst->h;

		/* UVs */
		float u0 = src->x / tex_w;
		float v0 = src->y / tex_h;
		float u1 = (src->x + src->w) / tex_w;
		float v1 = (src->y + src->h) / tex_h;

		if (flip_x)
		{
			float t = u0;
			u0 = u1;
			u1 = t;
		}

		g_uv[p + 0] = u0;
		g_uv[p + 1] = v0;
		g_uv[p + 2] = u1;
		g_uv[p + 3] = v0;
		g_uv[p + 4] = u1;
		g_uv[p + 5] = v1;
		g_uv[p + 6] = u0;
		g_uv[p + 7] = v1;

		/* color */
		for (int i = 0; i < 4; i++)
			g_col[v + i] = (SDL_FColor){1.0, 1.0, 1.0, 1.0};
	}

	void DrawPalomasBatch(
		Animal *palomas,
		SDL_Renderer *renderer,
		SDL_Texture *texture,
		int tex_w,
		int tex_h)
	{
		// static bool firstFrame = true;
		int quad_count = 0;

		// if(!firstFrame)
		// 	quad_count = ENTITY_COUNT;

		/* --- sombras --- */
		// if (firstFrame)
			for (int i = 0; i < ENTITY_COUNT; i++)
			{
				Animal *p = &palomas[i];

				SDL_FRect pos = GetWorldPositionsPaloma(p);
				pos.y += p->baseDifferenceY;

				if (pos.x < -15 || pos.y < -15 || pos.x > 780 || pos.y > 680)
					continue;

				PushQuad(
					quad_count++,
					&pos,
					&shadowSource,
					0,
					tex_w,
					tex_h);
			}

		if (quad_count > 0)
		{
			SDL_RenderGeometryRaw(
				renderer, texture,
				g_xy, sizeof(float) * 2,
				g_col, sizeof(SDL_FColor),
				g_uv, sizeof(float) * 2,
				quad_count * 4,
				g_idx, quad_count * 6, sizeof(Uint32));
		}

		/* --- palomas --- */
		quad_count = 0;
		// if (firstFrame)
			for (int i = 0; i < ENTITY_COUNT; i++)
			{
				Animal *p = &palomas[i];

				SDL_FRect pos = GetWorldPositionsPaloma(p);
				if (pos.x < -15 || pos.y < -15 || pos.x > 780 || pos.y > 680)
					continue;

				int flip = (p->direction.x < 0);

				PushQuad(
					quad_count++,
					&pos,
					&p->SourceRect,
					flip,
					tex_w,
					tex_h);
			}

		if (quad_count > 0)
		{
			SDL_RenderGeometryRaw(
				renderer, texture,
				g_xy, sizeof(float) * 2,
				g_col, sizeof(SDL_FColor),
				g_uv, sizeof(float) * 2,
				quad_count * 4,
				g_idx, quad_count * 6, sizeof(Uint32));
		}
		// firstFrame = false;
	}

	// #########################################
	static int comparePaloma(const void *a, const void *b)
	{
		const Animal *pa = (const Animal *)a;
		const Animal *pb = (const Animal *)b;

		if (pa->Dimensions.y < pb->Dimensions.y)
			return -1;
		if (pa->Dimensions.y > pb->Dimensions.y)
			return 1;
		return 0;
	}

	void DrawPaloma(Animal *paloma, SDL_Renderer *renderer)
	{
		SDL_FRect worldPosition = GetWorldPositionsPaloma(paloma);

		worldPosition.y += paloma->baseDifferenceY;

		if (worldPosition.x < -15 || worldPosition.y < -15 || worldPosition.x > 780 || worldPosition.y > 680)
			return;

		SDL_RenderTexture(renderer, Texture, &shadowSource, &worldPosition);

		worldPosition.y -= paloma->baseDifferenceY;

		if (paloma->direction.x < 0)
		{
			SDL_RenderTextureRotated(
				renderer,
				Texture,
				&paloma->SourceRect, // src rect (NULL = textura completa)
				&worldPosition,
				0.0,  // ángulo
				NULL, // centro (NULL = centro del dst)
				SDL_FLIP_HORIZONTAL);
		}
		else
		{
			SDL_RenderTexture(renderer, Texture, &paloma->SourceRect, &worldPosition);
		}
	}

	SDL_FRect GetWorldPositionsPaloma(Animal *paloma)
	{
		auto tempParent = Parent;
		SDL_FRect worldPosition = {paloma->Dimensions.x, paloma->Dimensions.y, paloma->Dimensions.w, paloma->Dimensions.h};
		while (tempParent != nullptr)
		{
			worldPosition.x += tempParent->Dimensions.x;
			worldPosition.y += tempParent->Dimensions.y;
			tempParent = tempParent->Parent;
		}
		return worldPosition;
	}

	void ConstraintObjectsToMap()
	{
		for (int i = 0; i < ENTITY_COUNT; i++)
		{
			Palomas[i].Dimensions.y = SDL_clamp(Palomas[i].Dimensions.y, .5f * Palomas[i].Dimensions.x - 1250, .5f * Palomas[i].Dimensions.x + 1530);
			Palomas[i].Dimensions.y = SDL_clamp(Palomas[i].Dimensions.y, -.5f * Palomas[i].Dimensions.x + 1930, -.5f * Palomas[i].Dimensions.x + 4670);
			Palomas[i].Dimensions.x = SDL_clamp(Palomas[i].Dimensions.x, 400, 5912);
		}
	}
};
