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


namespace
{
    const SDL_Rect BgRect{ 0, 0, 256, 256 };
};


QueryWindow::QueryWindow(SDL_Renderer* renderer) :
    mFont{ new Font("res/raleway-medium.ttf", 13) },
    mFontBold{ new Font("res/raleway-bold.ttf", 13) },
    mTexture(loadTexture(renderer, "images/QueryWindow.png")),
    mTextTexture(newTexture(renderer, mTexture.dimensions)),
    mRenderer{ renderer },
    mStringRenderer{ renderer }
{
    size({ BgRect.w, BgRect.h });

    SDL_SetTextureColorMod(mFont->texture(), 0, 0, 0);
    SDL_SetTextureColorMod(mFontBold->texture(), 0, 0, 0);

    SDL_SetTextureBlendMode(mTextTexture.texture, SDL_BLENDMODE_BLEND);

    closeButtonActive(true);
}


void QueryWindow::setQueryResult(const ZoneStats& stats)
{
}


void QueryWindow::draw()
{
    const SDL_Rect rect{ area().x, area().y, area().width, area().height };
    SDL_RenderCopy(mRenderer, mTexture.texture, &BgRect, &rect);
    SDL_RenderCopy(mRenderer, mTextTexture.texture, &BgRect, &rect);
}
