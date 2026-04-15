// This file is part of Micropolis-SDL2PP
// Micropolis-SDL2PP is based on Micropolis
//
// Copyright © 2022 - 2026 Leeor Dicker
//
// Portions Copyright © 1989-2007 Electronic Arts Inc.
//
// Micropolis-SDL2PP is free software; you can redistribute it and/or modify
// it under the terms of the GNU GPLv3, with additional terms. See the README
// file, included in this distribution, for details.
#include "DashboardWindow.h"


#include "../Colors.h"
#include "../Util.h"


namespace
{
    const SDL_FRect BackgroundArtRect{ 0.0f, 0.0f, 512.0f, 82.0f };
    constexpr auto RciValveHeight = 20;
}


DashboardWindow::DashboardWindow(SDL_Renderer* renderer, const RCI& rci) :
	mRenderer{ renderer },
	mTexture(loadTexture(renderer, "images/DashboardWindow.png")),
	mRci{ rci }
{
	size({ 512, 82 });
	alwaysVisible(true);
	show();
}


void DashboardWindow::draw()
{
	const auto rect = fRectFromRect({ area().position.x, area().position.y, area().size.x, area().size.y });
	SDL_RenderTexture(mRenderer, mTexture.texture, &BackgroundArtRect, &rect);
    drawValve();
}


void DashboardWindow::update()
{}


void DashboardWindow::drawValve()
{
    const auto residentialPercent = static_cast<float>(mRci.residentialDemand()) / 2000.0f;
    const auto commercialPercent = static_cast<float>(mRci.commercialDemand()) / 1500.0f;
    const auto industrialPercent = static_cast<float>(mRci.industrialDemand()) / 1500.0f;

    mResidentialValveRect.h = -(RciValveHeight * residentialPercent);
    mCommercialValveRect.h = -(RciValveHeight * commercialPercent);
    mIndustrialValveRect.h = -(RciValveHeight * industrialPercent);

    SDL_SetRenderDrawColor(mRenderer, Colors::Green.r, Colors::Green.g, Colors::Green.b, 255);
    SDL_RenderFillRect(mRenderer, &mResidentialValveRect);

    SDL_SetRenderDrawColor(mRenderer, Colors::MediumBlue.r, Colors::MediumBlue.g, Colors::MediumBlue.b, 255);
    SDL_RenderFillRect(mRenderer, &mCommercialValveRect);

    SDL_SetRenderDrawColor(mRenderer, Colors::Gold.r, Colors::Gold.g, Colors::Gold.b, 255);
    SDL_RenderFillRect(mRenderer, &mIndustrialValveRect);

    // not a huge fan of this
	const SDL_FPoint rciSrcPoint{ area().position.x + 9.0f + 4.0f, area().position.y + 44.0f };
    SDL_FRect rciSrc{ 0.0f, 83.0f, 32.0f, 11.0f };
    SDL_FRect rciDst{ rciSrcPoint.x, rciSrcPoint.y, 32.0f, 11.0f };
    SDL_RenderTexture(mRenderer, mTexture.texture, &rciSrc, &rciDst);
}


void DashboardWindow::onMoved(const Vector<int>&)
{
    onPositionChanged(area().position);
}


void DashboardWindow::onPositionChanged(const Point<int>& pos)
{
    mResidentialValveRect.x = static_cast<float>(pos.x + 18);
    mResidentialValveRect.y = static_cast<float>(pos.y + 10 + 39);
	mCommercialValveRect.x = static_cast<float>(pos.x + 27);
	mCommercialValveRect.y = static_cast<float>(pos.y + 10 + 39);
	mIndustrialValveRect.x = static_cast<float>(pos.x + 36);
	mIndustrialValveRect.y = static_cast<float>(pos.y + 10 + 39);
}
