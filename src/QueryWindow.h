// This file is part of Micropolis-SDL2PP
// Micropolis-SDL2PP is based on Micropolis
//
// Copyright © 2022 Leeor Dicker
//
// Portions Copyright © 1989-2007 Electronic Arts Inc.
//
// Micropolis-SDL2PP is free software; you can redistribute it and/or modify
// it under the terms of the GNU GPLv3, with additional terms. See the README
// file, included in this distribution, for details.
#pragma once

#include "WindowBase.h"

#include "Font.h"
#include "StringRender.h"
#include "Texture.h"

#include <memory>
#include <SDL2/SDL.h>


struct ZoneStats;


class QueryWindow : public WindowBase
{
public:
    QueryWindow() = delete;
    QueryWindow(const QueryWindow&) = delete;
    const QueryWindow operator=(const QueryWindow&) = delete;

    QueryWindow(SDL_Renderer* renderer);

    void setQueryResult(const ZoneStats& stats);

    void draw() override;
    void update() override {}

private:
    void onMouseDown(const Point<int>&) override;

private:
    Font* mFont{ nullptr };
    Font* mFontBold{ nullptr };
    Font* mFontBoldItalic{ nullptr };

    Texture mTexture;
    Texture mTextTexture;

    SDL_Renderer* mRenderer{ nullptr };

    StringRender mStringRenderer;
};
