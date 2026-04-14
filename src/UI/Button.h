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
#pragma once

#include <functional>

#include <SDL3/SDL.h>

class Button
{
public:
    static constexpr auto DefaultSize = 10;

    using ClickedCallback = std::function<void(void)>;

public:
    Button():
        mClicked{ []() {} }, mArea{ 0, 0, DefaultSize, DefaultSize }
    {}


    Button(ClickedCallback clicked, const SDL_Rect& area, int userValue = 0) :
        mClicked{ clicked }, mArea(area), mUserValue(userValue)
    {}


	void area(const SDL_Rect& area)
    {
        mArea = area;
    }


    const SDL_Rect& area() const
    {
        return mArea;
    }


    void areaF(const SDL_FRect& areaF)
    {
        area({ static_cast<int>(areaF.x), static_cast<int>(areaF.y), static_cast<int>(areaF.w), static_cast<int>(areaF.h) });
    }


    const SDL_FRect areaF() const
    {
        return { static_cast<float>(mArea.x), static_cast<float>(mArea.y), static_cast<float>(mArea.w), static_cast<float>(mArea.h) };
	}


    void click() const
    {
        mClicked();
    }


    void userValue(int value)
    {
        mUserValue = value;
	}


    int userValue() const
    {
        return mUserValue;
	}

private:
    ClickedCallback mClicked;
    SDL_Rect mArea;

	int mUserValue{ 0 };
};
