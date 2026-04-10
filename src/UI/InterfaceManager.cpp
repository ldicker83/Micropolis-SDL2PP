#include "InterfaceManager.h"

#include <unordered_map>

namespace
{

	std::unordered_map<InterfaceManager::Window, WindowBase*> WindowTable;


	void CenterWindow(SDL_Window* sdlWindow, WindowBase& window)
	{
		int windowWidth = 0, windowHeight = 0;
		SDL_GetWindowSize(sdlWindow, &windowWidth, &windowHeight);
		window.position({ windowWidth / 2 - window.area().width / 2, windowHeight / 2 - window.area().height / 2 });
	}


	void BringWindowToFront(WindowStack& stack, WindowBase& window)
	{
		stack.bringToFront(&window);
		window.toggleVisible();
		
		if(window.visible())
		{
			window.update();
		}
	}
}

InterfaceManager::InterfaceManager(SDL_Renderer* renderer, SDL_Window* window, Budget& budget) :
	mRenderer{ renderer },
	mWindow{ window },
	mBudgetWindow{ renderer, budget },
	mOptionsWindow{ renderer }
{
	mWindowStack.addWindow(&mBudgetWindow);
	mWindowStack.addWindow(&mOptionsWindow);

	mModalWindows.addWindow(&mBudgetWindow);
	mModalWindows.addWindow(&mOptionsWindow);

	WindowTable[InterfaceManager::Window::Budget] = &mBudgetWindow;
	WindowTable[InterfaceManager::Window::Options] = &mOptionsWindow;
}


void InterfaceManager::injectMouseMotion(const Vector<int>& delta)
{
	mWindowStack.injectMouseMotion(delta);
}


void InterfaceManager::injectMouseDown(const Point<int>& position)
{
	if (mWindowStack.pointInWindow(position))
	{
		mWindowStack.updateStack(position);
		mWindowStack.front()->injectMouseDown(position);
		return;
	}
}


void InterfaceManager::injectMouseUp()
{
	mWindowStack.injectMouseUp();
}


bool InterfaceManager::pointInWindow(const Point<int>& position) const
{
	return mWindowStack.pointInWindow(position);
}


void InterfaceManager::centerAllWindows()
{
	CenterWindow(mWindow, mBudgetWindow);
	CenterWindow(mWindow, mOptionsWindow);
}


void InterfaceManager::centerWindow(Window window)
{
	CenterWindow(mWindow, *WindowTable.at(window));
}


void InterfaceManager::showBudgetWindow()
{
	BringWindowToFront(mWindowStack, mBudgetWindow);
}


bool InterfaceManager::budgetWindowVisible() const
{
	return mBudgetWindow.visible();
}


bool InterfaceManager::modalWindowVisible() const
{
	return mModalWindows.windowVisible();
}


void InterfaceManager::hideAllWindows()
{
	mWindowStack.hide();
	mModalWindows.hide();
}


void InterfaceManager::draw()
{
	if (mModalWindows.windowVisible())
	{
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 175);
		SDL_RenderFillRect(mRenderer, nullptr);

		mModalWindows.draw();
	}
	else
	{
		mWindowStack.draw();
	}
}
