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
#pragma once

#include "BudgetWindow.h"
#include "EvaluationWindow.h"
#include "FileIoDialog.h"
#include "GraphWindow.h"
#include "MiniMapWindow.h"
#include "OptionsWindow.h"
#include "QueryWindow.h"
#include "ToolPalette.h"
#include "WindowGroup.h"
#include "WindowStack.h"


#include <SDL2/SDL.h>


class InterfaceManager
{
public:
	InterfaceManager() = delete;
	InterfaceManager(const InterfaceManager&) = delete;
	InterfaceManager& operator=(const InterfaceManager&) = delete;

public:
	InterfaceManager(SDL_Renderer* renderer, SDL_Window* window, Budget& budget);

	void injectMouseMotion(const Vector<int>& delta);
	void injectMouseDown(const Point<int>& position);
	void injectMouseUp();

	bool pointInWindow(const Point<int>& position) const;

	void centerAllWindows();

	void showBudgetWindow();
	bool budgetWindowVisible() const;

	bool modalWindowVisible() const;

	void hideAllWindows();

	void draw();

private:
	SDL_Renderer* mRenderer{ nullptr };
	SDL_Window* mWindow{ nullptr };

	BudgetWindow mBudgetWindow;
	//GraphWindow mGraphWindow;
	//EvaluationWindow mEvaluationWindow;
	//MiniMapWindow mMiniMapWindow;
	//ToolPalette mToolPalette;
	//OptionsWindow mOptionsWindow;
	//QueryWindow mQueryWindow;
	//StringRender mStringRenderer;
	//FileIoDialog mFileIo;

	WindowStack mWindowStack;
	WindowGroup mModalWindows;
};
