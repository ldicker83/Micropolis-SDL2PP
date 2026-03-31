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
#include "Texture.h"

#include <string>

#include <SDL2/SDL.h>

 /* Constants */

constexpr auto SimWidth = 120;
constexpr auto SimHeight = 100;
constexpr SDL_Rect ValidMapCoordinates{ 0, 0, SimWidth - 1, SimHeight - 1 };

constexpr auto HalfWorldWidth = SimWidth / 2;
constexpr auto HalfWorldHeight = SimHeight / 2;

constexpr auto QuarterWorldWidth = SimWidth / 4;
constexpr auto QuarterWorldHeight = SimHeight / 4;

constexpr auto EighthWorldWidth = SimWidth / 8;
constexpr auto EighthWorldHeight = (SimHeight + 7) / 8;

constexpr auto HistoryLength = 120;
constexpr auto MiscHistoryLength = 240;


/* Status Bits */
constexpr int PowerBit = 0x8000; /* 20 bit 15 */
constexpr int ConductiveBit = 0x4000; /* 10 bit 14 */
constexpr int BurnableBit = 0x2000; /* 8 bit 13 */
constexpr int BulldozableBit = 0x1000; /* 4 bit 12 */
constexpr int AnimatedBit = 0x0800;
constexpr int ZonedBit = 0x0400; /* 1 bit 10 */
constexpr int UpperMask = 0xFC00; /* mask for upper 6 bits	*/
constexpr int LowerMask = 0x03FF; /* mask for low 10 bits	*/

constexpr int BLBNBIT = BulldozableBit + BurnableBit;
constexpr int BLBNCNBIT = BulldozableBit + BurnableBit + ConductiveBit;
constexpr int BNCNBIT = BurnableBit + ConductiveBit;

extern int CurrentTile; // unmasked tile value
extern int CurrentTileMasked; // masked tile value

extern int RoadTotal, RailTotal, FirePop;
extern int ResPop, ComPop, IndPop, TotalPop, LastTotalPop;
extern int ResZPop, ComZPop, IndZPop, TotalZPop;
extern int HospPop, ChurchPop, StadiumPop;
extern int PolicePop, FireStPop;
extern int CoalPop, NuclearPop, PortPop, APortPop;
extern int NeedHosp, NeedChurch;
extern int CrimeAverage, PolluteAverage, LVAverage;

extern int StartingYear;
extern int CityTime;
extern int ScenarioID;
extern int ShakeNow;

extern int RoadEffect, PoliceEffect, FireEffect;

extern int DisasterEvent;
extern int DisasterWait;

extern int ResCap, ComCap, IndCap;
extern int RValve, CValve, IValve;
extern int PoweredZoneCount;
extern int UnpoweredZoneCount;

extern int ScoreType;
extern int ScoreWait;

extern int InitSimLoad;
extern int DoInitialEval;

extern SDL_Renderer* MainWindowRenderer;

void showBudgetWindow();

void initWillStuff();

bool autoBudget();
void autoBudget(const bool b);

bool autoGoto();
void autoGoto(const bool b);

bool autoBulldoze();
void autoBulldoze(const bool b);

bool disastersEnabled();
void disastersEnabled(const bool b);

bool animationEnabled();
void animationEnabled(bool b);

void simExit();
const Point<int>& viewOffset();
