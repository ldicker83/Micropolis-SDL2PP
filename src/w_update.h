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

#include <string>

class Budget;

enum class Month
{
	Jan,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec
};


bool newMonth();

void updateFunds(Budget&);
void updateDate();

extern const std::string dateStr[12];

int lastCityTime();
void lastCityTime(int time);

int lastCityMonth();
void lastCityMonth(int month);

int lastCityYear();
void lastCityYear(int year);

const std::string& monthString(Month month);
