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

#include <SDL2/SDL.h>

class Button
{
public:
    using ClickedCallback = std::function<void(void)>;

public:
    Button() = delete;

    Button(ClickedCallback clicked, const SDL_Rect& area) :
        mClicked{ clicked }, mArea(area)
    {}


    const SDL_Rect& area() const
    {
        return mArea;
    }


    void click() const
    {
        mClicked();
    }

private:
    ClickedCallback mClicked;
    SDL_Rect mArea;
};
