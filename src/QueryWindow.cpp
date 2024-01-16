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

#include "QueryWindow.h"

#include "w_resrc.h"

#include <array>

namespace
{
    constexpr SDL_Rect BgRect{ 0, 0, 256, 256 };
    constexpr SDL_Rect CloseButtonRect{ 61, 221, 142, 20 };
};


QueryWindow::QueryWindow(SDL_Renderer* renderer) :
    mFont{ new Font("res/raleway-medium.ttf", 13) },
    mFontBold{ new Font("res/raleway-bold.ttf", 13) },
    mFontBoldItalic{ new Font("res/raleway-bolditalic.ttf", 13) },
    mTexture(loadTexture(renderer, "images/QueryWindow.png")),
    mTextTexture(newTexture(renderer, mTexture.dimensions)),
    mRenderer{ renderer },
    mStringRenderer{ renderer }
{
    size({ BgRect.w, BgRect.h });

    SDL_SetTextureColorMod(mFont->texture(), 0, 0, 0);
    SDL_SetTextureColorMod(mFontBold->texture(), 0, 0, 0);
    SDL_SetTextureColorMod(mFontBoldItalic->texture(), 0, 0, 0);

    SDL_SetTextureBlendMode(mTextTexture.texture, SDL_BLENDMODE_BLEND);

    closeButtonActive(false);
}


void QueryWindow::setQueryResult(const ZoneStats& stats)
{
    flushTexture(mRenderer, mTextTexture);

    const int titleX = size().x / 2 - mFontBold->width(stats.title) / 2;

    mStringRenderer.drawString(*mFontBold, stats.title, { titleX, 40 });

    const std::array<std::tuple<std::string, std::string>, 5> queryStats =
    {
        std::tuple{ "Density", stats.density },
        std::tuple{ "Land Value", stats.landValue },
        std::tuple{ "Pollution Level", stats.pollution },
        std::tuple{ "Crime", stats.crime },
        std::tuple{ "Growth", stats.populationGrowth },
    };

    constexpr Point<int> startPoint = { 10, 75 };

    int row = 0;
    for (const auto& [title, value] : queryStats)
    {
        const Point<int> titleDrawOrigin = startPoint + Vector<int>{ 0, mFontBoldItalic->height()* row + 10 };
        mStringRenderer.drawString(*mFontBoldItalic, title, titleDrawOrigin);

        const Point<int> valueDrawOrigin = { size().y - 10 - mFont->width(value), titleDrawOrigin.y };
        mStringRenderer.drawString(*mFont, value, valueDrawOrigin);

        ++row;
    }

    SDL_SetRenderTarget(mRenderer, nullptr);
}


void QueryWindow::draw()
{
    const SDL_Rect rect{ area().x, area().y, area().width, area().height };
    SDL_RenderCopy(mRenderer, mTexture.texture, &BgRect, &rect);
    SDL_RenderCopy(mRenderer, mTextTexture.texture, &BgRect, &rect);
}


void QueryWindow::onMouseDown(const Point<int>& pt)
{
    const Rectangle<int> closeButtonRect =
    {
        area().x + CloseButtonRect.x,
        area().y + CloseButtonRect.y,
        CloseButtonRect.w,
        CloseButtonRect.h
    };
    
    if (closeButtonRect.contains(pt))
    {
        hide();
    }
}
