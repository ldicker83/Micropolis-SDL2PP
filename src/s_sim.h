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

class Budget;
class CityProperties;

void SimFrame(CityProperties&, Budget&);
void FireZone(int Xloc, int Yloc, int ch);
void DoSimInit(CityProperties&, Budget&);
void DoSPZone(bool powered, const CityProperties&);
void RepairZone(int ZCent, int zsize);
