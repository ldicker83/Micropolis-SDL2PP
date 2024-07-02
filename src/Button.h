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
