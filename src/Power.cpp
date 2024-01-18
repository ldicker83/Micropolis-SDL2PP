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
#include "main.h"

#include "Map.h"

#include "s_alloc.h"
#include "s_msg.h"

#include <stack>

/* Power Scan */

namespace
{
    constexpr int CoalPowerProvided{ 700 };
    constexpr int NuclearPowerProvided{ 2000 };

    constexpr auto PowerMapRow = ((SimWidth + 15) / 16);
    constexpr auto PowerMapSize = (PowerMapRow * SimHeight);
    constexpr auto PowerStackSize = ((SimWidth * SimHeight) / 4);

    std::stack<Point<int>> PowerStack;
    std::array<int, PowerMapSize> PowerMap{};

    const Point<int>& topPowerStack()
    {
        return PowerStack.top();
    }

    void popPowerStack()
    {
        if (!PowerStack.empty())
        {
            PowerStack.pop();
        }
    }

};


void pushPowerStack(const Point<int>& location)
{

    if (PowerStack.size() < PowerStackSize)
    {
        PowerStack.push(location);
    }
}


void resetPowerStack()
{
    while (!PowerStack.empty())
    {
        PowerStack.pop();
    }
}


void resetPowerMap()
{
    PowerMap.fill(0);
}


void setPowerBit(const Point<int>& location)
{
    /* XXX: assumes 120x100 */
    const auto powerWrd = (location.x / 16) + (location.y * 8);
    PowerMap[powerWrd] |= 1 << (location.x & 15);
}


bool powerBitSet(const Point<int>& location)
{
    const auto powerWord = (location.x / 16) + (location.y * 8);
    return (PowerMap[powerWord] & (1 << (location.x & 15))) != 0;
}


bool testPowerBit(const Point<int>& location)
{
    const auto tile = maskedTileValue(location.x, location.y);
    if ((tile == NUCLEAR) || (tile == POWERPLANT))
    {
        return true;
    }

    /* XXX: assumes 120x100 */
    int PowerWrd = (location.x / 16) + (location.y * 8);
    if (PowerWrd >= PowerMapSize)
    {
        return false;
    }

    return powerBitSet(location);
}


bool isTileConductive(SearchDirection direction)
{
    const auto saved = SimulationTarget;

    if (moveSimulationTarget(direction))
    {
        if ((tileValue(SimulationTarget) & CONDBIT) && !testPowerBit(SimulationTarget))
        {
            SimulationTarget = saved;
            return true;
        }
    }

    SimulationTarget = saved;

    return false;
}


void powerScan()
{
    resetPowerMap();

    int powerAvailable = (CoalPop * CoalPowerProvided) + (NuclearPop * NuclearPowerProvided);
    int powerConsumed = 0;

    int conductiveTileCount{};
    while (!PowerStack.empty())
    {
        SimulationTarget = topPowerStack();
        popPowerStack();

        int ADir{ 4 };
        do
        {
            if (++powerConsumed > powerAvailable)
            {
                SendMes(NotificationId::BrownoutsReported);
                return;
            }

            moveSimulationTarget(static_cast<SearchDirection>(ADir));
            setPowerBit(SimulationTarget);

            conductiveTileCount = 0;
            int searchDirection{ 0 };
            while ((searchDirection < 4) && (conductiveTileCount < 2))
            {
                if (isTileConductive(static_cast<SearchDirection>(searchDirection)))
                {
                    conductiveTileCount++;
                    ADir = searchDirection;
                }
                searchDirection++;
            }
            if (conductiveTileCount > 1)
            {
                pushPowerStack(SimulationTarget);
            }
        } while (conductiveTileCount);
    }
}
