#include "WindowGroup.h"

#include "WindowBase.h"


bool WindowGroup::windowVisible() const
{
    for (const auto* window : mWindows)
    {
        if (window->visible())
        {
            return true;
        }
    }

    return false;
}


void WindowGroup::addWindow(WindowBase* window)
{
    mWindows.push_back(window);
}


void WindowGroup::hide()
{
    for (auto* window : mWindows)
    {
        window->hide();
    }
}


void WindowGroup::draw()
{
    for (auto* window : mWindows)
    {
        if (window->visible())
        {
            window->draw();
        }
    }
}


void WindowGroup::update()
{
    for (auto* window : mWindows)
    {
        if (window->visible())
        {
            window->update();
        }
    }
}
