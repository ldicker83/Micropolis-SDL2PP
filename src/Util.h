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

#include "Point.h"

#include <SDL2/SDL.h>
#include <string>

class Budget;
class CityProperties;

enum class SimulationSpeed
{
	Paused,
	Slow,
	Normal,
	Fast,
	AfricanSwallow
};

const std::string& speedString(SimulationSpeed speed);

std::string numberToDollarDecimal(int value);

void simSpeed(SimulationSpeed speed);
SimulationSpeed simSpeed();

void pause();
void resume();

bool paused();

int currentYear();
void setYear(int year);
void setGameLevelFunds(int level, CityProperties& properties, Budget&);
bool coordinatesValid(const Point<int>& position);
Point<int> positionToCell(const Point<int>& position, const Point<int>& offset);
const Vector<int> vectorFromPoints(const Point<int>& start, const Point<int>& end);
bool pointInRect(const Point<int>& point, const SDL_Rect& rect);

int randomRange(int min, int max);
int random();
int rand16();
