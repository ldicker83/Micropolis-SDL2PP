// This file is part of Micropolis-SDL2PP
// Micropolis-SDL2PP is based on Micropolis
//
// Copyright © 2022 - 2024 Leeor Dicker
//
// Portions Copyright © 1989-2007 Electronic Arts Inc.
//
// Micropolis-SDL2PP is free software; you can redistribute it and/or modify
// it under the terms of the GNU GPLv3, with additional terms. See the README
// file, included in this distribution, for details.
#include "Map.h"

#include "s_alloc.h"

#include "Point.h"

#include "Texture.h"
#include "Sprite.h"

#include "Util.h"

#include <vector>

#include <SDL2/SDL.h>

extern SDL_Renderer* MainWindowRenderer;

extern Texture BigTileset;
extern Texture MainMapTexture;


std::vector<int> MapBuffer;


namespace
{
	SDL_Rect tileRect{ 0, 0, 16, 16 };
	bool flagBlink{ false };
};


void toggleBlinkFlag()
{
	flagBlink = !flagBlink;
}


void ResetMap()
{
	MapBuffer.resize(SimWidth * SimHeight);
	std::fill(MapBuffer.begin(), MapBuffer.end(), Dirt);
}


int& tileValue(const Point<int>& location)
{
	return tileValue(location.x, location.y);
}


int& tileValue(const int x, const int y)
{
	return MapBuffer[static_cast<size_t>(x) * SimHeight + static_cast<size_t>(y)];
}


unsigned int maskedTileValue(const Point<int>& location)
{
	return maskedTileValue(location.x, location.y);
}


unsigned int maskedTileValue(const int x, const int y)
{
	return tileValue(x, y) & LowerMask;
}


unsigned int maskedTileValue(unsigned int tile)
{
	return tile & LowerMask;
}


bool tileIsPowered(const unsigned int tile)
{
	return tile & PowerBit;
}


bool tileIsZoned(const unsigned int tile)
{
	return tile & ZonedBit;
}


bool tileIsRoad(const Point<int> coordinates)
{
	if (!coordinatesValid(coordinates))
	{
		return false;
	}

	const auto tile = maskedTileValue(coordinates);

	if (tile < BridgeBase || tile > RailLast)
	{
		return false;
	}

	if ((tile >= PowerBase) && (tile < RailHorizontalPowerVertical))
	{
		return false;
	}

	return true;
}


bool tileCanBeBulldozed(const Point<int> coordinates)
{
	return tileValue(coordinates) & BulldozableBit;
}


bool blink()
{
	return flagBlink;
}


MapData getMapData()
{
	return MapData
	{
		reinterpret_cast<const char*>(MapBuffer.data()),
		static_cast<unsigned int>(MapBuffer.size() * sizeof(int))
	};
}

/**
 * Assumes \c begin and \c end are in a valid range
 */
void drawBigMapSegment(const Point<int>& begin, const Point<int>& end)
{
	SDL_SetRenderTarget(MainWindowRenderer, MainMapTexture.texture);

	SDL_Rect drawRect{ 0, 0, 16, 16 };
	unsigned int tile = 0;

	for (int row = begin.x; row < end.x; row++)
	{
		for (int col = begin.y; col < end.y; col++)
		{
			tile = tileValue(row, col);
			// Blink lightning bolt in unpowered zone center
			if (blink() && tileIsZoned(tile) && !tileIsPowered(tile))
			{
				tile = LightningBolt;
			}

			drawRect = { row * drawRect.w, col * drawRect.h, drawRect.w, drawRect.h };

			const unsigned int masked = maskedTileValue(tile);
			tileRect =
			{
				(static_cast<int>(masked) % 32) * 16,
				(static_cast<int>(masked) / 32) * 16,
				16, 16
			};

			SDL_RenderCopy(MainWindowRenderer, BigTileset.texture, &tileRect, &drawRect);
		}
	}

	SDL_RenderPresent(MainWindowRenderer);
	SDL_SetRenderTarget(MainWindowRenderer, nullptr);
}


void drawBigMap()
{
	drawBigMapSegment(Point<int>{0, 0}, Point<int>{SimWidth, SimHeight});
}
