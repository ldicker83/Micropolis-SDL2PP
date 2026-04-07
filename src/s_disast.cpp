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
#include "CityProperties.h"
#include "Map.h"

#include "s_alloc.h"
#include "s_sim.h"

#include "s_msg.h"

#include "Scan.h"
#include "Sprite.h"

#include "Util.h"

#include <algorithm>


bool tileIsNuclear(const int tile)
{
    return ((tile & LowerMask) == NuclearPower);
}


bool tileIsArsonable(const int tile)
{
    return !(tile & ZonedBit) && (tile & BurnableBit);
}


bool tileIsRiverEdge(const int tile)
{
    const int masked = tile & LowerMask;
    return masked >= RiverEdgeFirst && masked <= RiverEdgeLast;
}


bool tileIsFloodable(const int tile)
{
    const int masked = tile & LowerMask;
    return masked == Dirt || ((tile & BulldozableBit) || (tile & BurnableBit));
}


bool canSpreadFloodTo(const int tile)
{
    const int masked = tile & LowerMask;
    return (masked == Dirt) || (tile & BurnableBit) || (masked >= Rubble && (masked <= RubbleLast));
}


bool tileIsVulnerable(const int tile)
{
    const unsigned int unmasked = tile & LowerMask;
    return !(unmasked < ResidentialBase) || (unmasked > ZoneLast) || (tile & ZonedBit);
}


int ShakeNow;
int FloodCount;
int FloodX, FloodY;


// forward declare from s_sim
void DoMeltdown(int, int);

// forward declare from w_tk
void DoEarthQuake();


void MakeMeltdown()
{
    for (int x = 0; x < (SimWidth - 1); x++)
    {
        for (int y = 0; y < (SimHeight - 1); y++)
        {
            if(tileIsNuclear(tileValue(x, y)))
            {
                DoMeltdown(x, y);
                return;
            }
        }
    }
}


void FireBomb()
{
    crashPosition({ randomRange(0, SimWidth - 1), randomRange(0, SimHeight - 1) });
    generateExplosion(crashPosition());
    ClearMes();
    SendMesAt(NotificationId::FirebombingReported, crashPosition().x, crashPosition().y);
}


void MakeEarthquake()
{
    DoEarthQuake();
    SendMesAt(NotificationId::EarthquakeReported, cityCenterOfMass().x, cityCenterOfMass().y);

    int time = randomRange(0, 700) + 300;

    for (int z = 0; z < time; z++)
    {
        int x = randomRange(0, SimWidth - 1);
        int y = randomRange(0, SimHeight - 1);

        if ((x < 0) || (x > (SimWidth - 1)) || (y < 0) || (y > (SimHeight - 1)))
        {
            continue;
        }

        if (tileIsVulnerable(Map[x][y]))
        {
            if (z & 0x3)
            {
                Map[x][y] = (Rubble + BulldozableBit) + (rand16() & 3);
            }
            else
            {
                Map[x][y] = (FireBase + AnimatedBit) + (rand16() & 7);
            }
        }
    }
}


void MakeFire()
{
    for (int t = 0; t < 40; t++)
    {
        const int x = randomRange(0, SimWidth - 1);
        const int y = randomRange(0, SimHeight - 1);
        const int cell = Map[x][y];

        if(tileIsArsonable(cell))
        {
            const int tile = maskedTileValue(x, y);
            if ((tile > RiverEdgeLast) && (tile < ZoneLast))
            {
                Map[x][y] = FireBase + randomRange(0, 7) | AnimatedBit;
                SendMesAt(NotificationId::FireReported, x, y);
                return;
            }
        }
    }
}


void MakeFlood()
{
    static const int Dx[4] = { 0, 1, 0,-1 };
    static const int Dy[4] = { -1, 0, 1, 0 };

    for (int iteration = 0; iteration < 300; ++iteration)
    {
        const int cellX = randomRange(0, SimWidth - 1);
        const int cellY = randomRange(0, SimHeight - 1);
        const int cell = tileValue(cellX, cellY);

        if (tileIsRiverEdge(cell))
        {
            for (int t = 0; t < 4; t++)
            {
                const int floodX = cellX + Dx[t];
                const int floodY = cellY + Dy[t];
                if (coordinatesValid({ floodX, floodY }))
                {
                    if(tileIsFloodable(cell))
                    {
                        Map[floodX][floodY] = Flood;
                        FloodCount = 30;
                        SendMesAt(NotificationId::FloodingReported, floodX, floodY);
                        FloodX = floodX;
                        FloodY = floodY;
                        return;
                    }
                }
            }
        }
    }
}


void DoFlood()
{
    static int Dx[4] = { 0, 1, 0,-1 };
    static int Dy[4] = { -1, 0, 1, 0 };

    if (FloodCount)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (randomRange(0, 7) == 0)
            {
                int x = SimulationTarget.x + Dx[i];
                int y = SimulationTarget.y + Dy[i];
                if (coordinatesValid({ x, y }))
                {
                    int cell = Map[x][y];

                    if(canSpreadFloodTo(cell))
                    {
                        if (cell & ZonedBit)
                        {
                            FireZone(x, y, cell);
                        }
                        Map[x][y] = Flood + randomRange(0, 2);
                    }
                }
            }
        }
    }
    else
    {
        if (randomRange(0, 15) == 0)
        {
            Map[SimulationTarget.x][SimulationTarget.y] = 0;
        }
    }
}


void DropFireBombs()
{
    /*
    proc FireBomb{}
    {
        sim FireBomb
    }
    */

    /*
    proc DropFireBombs{}
    {
        FireBomb
            after 300 FireBomb
            after 600 FireBomb
            after 900 FireBomb
    }
    */
}


void ScenarioDisaster()
{
    switch (DisasterEvent)
    {
    case 1:	// Dullsville
        break;

    case 2: // San Francisco
        if (DisasterWait == 1)
        {
            MakeEarthquake();
        }
        break;

    case 3: // Hamburg
        DropFireBombs();
        break;

    case 4: // Bern
        break;

    case 5: // Tokyo
        if (DisasterWait == 1)
        {
            generateMonster();
        }
        break;

    case 6:	// Detroit
        break;

    case 7: // Boston
        if (DisasterWait == 1)
        {
            MakeMeltdown();
        }
        break;

    case 8:	// Rio
        if ((DisasterWait % 24) == 0)
        {
            MakeFlood();
        }
        break;
    }

    if (DisasterWait)
    {
        DisasterWait--;
    }
    else
    {
        DisasterEvent = 0;
    }
}


void DoDisasters(CityProperties& properties)
{
    // Chance of disasters at lev 0 1 2
    static int DisChance[3] = { 10 * 48, 5 * 48, 60 };

    if (FloodCount)
    {
        FloodCount--;
    }

    if (DisasterEvent)
    {
        ScenarioDisaster();
    }

    if (!disastersEnabled())
    {
        return;
    }
    
    int disasterChance = randomRange(0, DisChance[properties.GameLevel()]);
    if (disasterChance == 0)
    {
        int disasterType = randomRange(0, 8);
        switch (disasterType)
        {
        case 0:
        case 1:
            MakeFire();
            break;

        case 2:
        case 3:
            MakeFlood();
            break;

        case 4:
            break;

        case 5:
            generateTornado();
            break;

        case 6:
            MakeEarthquake();
            break;

        case 7:
        case 8:
            if (PolluteAverage > /* 80 */ 60)
            {
                generateMonster();
            }
            break;
        }
    }
}
