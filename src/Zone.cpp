// This file is part of Micropolis-SDL2PP
// Micropolis-SDL2PP is based on Micropolis
//
// Copyright � 2022 Leeor Dicker
//
// Portions Copyright � 1989-2007 Electronic Arts Inc.
//
// Micropolis-SDL2PP is free software; you can redistribute it and/or modify
// it under the terms of the GNU GPLv3, with additional terms. See the README
// file, included in this distribution, for details.
#include "main.h"

#include "CityProperties.h"

#include "Map.h"
#include "Power.h"

#include "s_alloc.h"
#include "s_sim.h"
#include "s_traf.h"

#include "w_util.h"
#include "Zone.h"


/*
 * set bit in MapWord depending on powermap
 */
bool setZonePower()
{
    if (testPowerBit(SimulationTarget))
    {
        tileValue(SimulationTarget) = CurrentTile | PWRBIT;
        return true;
    }

    tileValue(SimulationTarget) = CurrentTile & (~PWRBIT);
    return false;
}


void zonePlop(const int base)
{
    std::array<Vector<int>, 9> adjacentTile =
    { {
        { -1, -1 },
        {  0, -1 },
        {  1, -1 },
        { -1,  0 },
        {  0,  0 },
        {  1,  0 },
        { -1,  1 },
        {  0,  1 },
        {  1,  1 }
    } };

    /* check for fire  */
    for (int i{}; i < 9; ++i)
    {
        const Point<int> coordinates = SimulationTarget + adjacentTile[i];

        if (CoordinatesValid(coordinates))
        {
            int tile = maskedTileValue(coordinates);
            if ((tile >= FLOOD) && (tile < ROADBASE))
            {
                return;
            }
        }
    }

    int tileBase{ base };
    for (int i{}; i < 9; ++i)
    {
        const Point<int> coordinates = SimulationTarget + adjacentTile[i];

        if (CoordinatesValid(coordinates))
        {
            tileValue(coordinates) = tileBase + BNCNBIT;
        }

        ++tileBase;
    }

    CurrentTile = tileValue(SimulationTarget);
    setZonePower();
    tileValue(SimulationTarget) |= ZONEBIT + BULLBIT;
}


void residentialPlop(int density, int value)
{
    const int base{ (((value * 4) + density) * 9) + RZB - 4 };
    zonePlop(base);
}


void commercialPlop(int density, int value)
{
    const int base{ (((value * 5) + density) * 9) + CZB - 4 };
    zonePlop(base);
}


void industrialPlop(int density, int value)
{
    const int base{ (((value * 4) + density) * 9) + (IZB - 4) };
    zonePlop(base);
}


int residentialZonePopulation(int tile)
{
    const int density{ (((tile - RZB) / 9) % 4) };
    return ((density * 8) + 16);
}


int commercialZonePopulation(int tile)
{
    const int density{ (((tile - CZB) / 9) % 5) + 1 };
    return (tile == COMCLR) ? 0 : density;
}


int industrialZonePopulation(int tile)
{
    const int density{ (((tile - IZB) / 9) % 4) + 1 };
    return (tile == INDCLR) ? 0 : density;
}


void doHospital()
{
    if (CurrentTileMasked == HOSPITAL)
    {
        HospPop++;

        if (!(CityTime & 15))/*post*/
        {
            RepairZone(HOSPITAL, 3);
        }

        if (NeedHosp == -1)
        {
            if (!RandomRange(0, 20))
            {
                zonePlop(RESBASE);
            }
        }
    }
}


void doChurch()
{
    if (CurrentTileMasked == CHURCH)
    {
        ChurchPop++;

        if (!(CityTime & 15))/*post*/
        {
            RepairZone(CHURCH, 3);
        }

        if (NeedChurch == -1)
        {
            if (!RandomRange(0, 20))
            {
                zonePlop(RESBASE);
            }
        }
    }
}


#define T 1
#define F 0
#define ASCBIT (ANIMBIT | CONDBIT | BURNBIT)
#define REGBIT (CONDBIT | BURNBIT)


void SetSmoke(int ZonePower)
{
    static int AniThis[8] = { T,    F,    T,    T,    F,    F,    T,    T };
    static int DX1[8] = { -1,    0,    1,    0,    0,    0,    0,    1 };
    static int DY1[8] = { -1,    0,   -1,   -1,    0,    0,   -1,   -1 };
    static int DX2[8] = { -1,    0,    1,    1,    0,    0,    1,    1 };
    static int DY2[8] = { -1,    0,    0,   -1,    0,    0,   -1,    0 };
    static int AniTabA[8] = { 0,    0,   32,   40,    0,    0,   48,   56 };
    static int AniTabB[8] = { 0,    0,   36,   44,    0,    0,   52,   60 };
    static int AniTabC[8] = { IND1,    0, IND2, IND4,    0,    0, IND6, IND8 };
    static int AniTabD[8] = { IND1,    0, IND3, IND5,    0,    0, IND7, IND9 };
    
    int z;

    if (CurrentTileMasked < IZB)
    {
        return;
    }

    z = (CurrentTileMasked - IZB) >> 3;
    z = z & 7;

    if (AniThis[z])
    {
        int xx = SimulationTarget.x + DX1[z];
        int yy = SimulationTarget.y + DY1[z];
        if (CoordinatesValid({ xx, yy }))
        {
            if (ZonePower)
            {
                if ((Map[xx][yy] & LOMASK) == AniTabC[z])
                {
                    Map[xx][yy] = ASCBIT | (SMOKEBASE + AniTabA[z]);
                    Map[xx][yy] = ASCBIT | (SMOKEBASE + AniTabB[z]);
                }
            }
            else
            {
                if ((Map[xx][yy] & LOMASK) > AniTabC[z])
                {
                    Map[xx][yy] = REGBIT | AniTabC[z];
                    Map[xx][yy] = REGBIT | AniTabD[z];
                }
            }
        }
    }
}


void MakeHosp()
{
    if (NeedHosp > 0)
    {
        zonePlop(HOSPITAL - 4);
        NeedHosp = false;
        return;
    }

    if (NeedChurch > 0)
    {
        zonePlop(CHURCH - 4);
        NeedChurch = false;
        return;
    }
}


int GetCRVal()
{
    int LVal;

    LVal = LandValueMap.value(SimulationTarget.skewInverseBy({ 2, 2 }));
    LVal -= PollutionMap.value(SimulationTarget.skewInverseBy({ 2, 2 }));

    if (LVal < 30) 
    {
        return 0;
    }

    if (LVal < 80) 
    {
        return 1;
    }

    if (LVal < 150) 
    {
        return 2;
    }

    return 3;
}


int EvalLot(int x, int y)
{
    int z, score;
    static int DX[4] = { 0, 1, 0,-1 };
    static int DY[4] = { -1, 0, 1, 0 };

    /* test for clear lot */
    z = Map[x][y] & LOMASK;
    if (z && ((z < RESBASE) || (z > RESBASE + 8)))
    {
        return -1;
    }

    score = 1;

    for (z = 0; z < 4; z++)
    {
        int xx = x + DX[z];
        int yy = y + DY[z];

        /* look for road */
        if (CoordinatesValid({ xx, yy }) && Map[xx][yy] && ((Map[xx][yy] & LOMASK) <= LASTROAD))
        {
            score++;
        }
    }

    return score;
}


int EvalRes(int traf)
{
    int Value;

    if (traf < 0)
    {
        return -3000;
    }

    Value = LandValueMap.value(SimulationTarget.skewInverseBy({ 2, 2 }));
    Value -= PollutionMap.value(SimulationTarget.skewInverseBy({ 2, 2 }));

    if (Value < 0)/* Cap at 0 */
    {
        Value = 0;
    }
    else
    {
        Value = Value << 5;
    }

    if (Value > 6000) /* Cap at 6000 */
    {
        Value = 6000;
    }

    Value = Value - 3000;
    return Value;
}


int EvalCom(int traf)
{
    if (traf < 0)
    {
        return -3000;
    }

    return ComRate.value(SimulationTarget.skewInverseBy({ 8, 8 }));
}


int EvalInd (int traf)
{
  if (traf < 0) 
  {
      return -1000;
  }

  return 0;
}


void BuildHouse(int value)
{
    int z, score, hscore, BestLoc;
    static int ZeX[9] = { 0,-1, 0, 1,-1, 1,-1, 0, 1 };
    static int ZeY[9] = { 0,-1,-1,-1, 0, 0, 1, 1, 1 };

    BestLoc = 0;
    hscore = 0;
    for (z = 1; z < 9; z++)
    {
        int xx = SimulationTarget.x + ZeX[z];
        int yy = SimulationTarget.y + ZeY[z];
        if (CoordinatesValid({ xx, yy }))
        {
            score = EvalLot(xx, yy);
            if (score != 0)
            {
                if (score > hscore)
                {
                    hscore = score;
                    BestLoc = z;
                }

                if ((score == hscore) && !(Rand16() & 7))
                {
                    BestLoc = z;
                }
            }
        }
    }

    if (BestLoc)
    {
        int xx = SimulationTarget.x + ZeX[BestLoc];
        int yy = SimulationTarget.y + ZeY[BestLoc];

        if (CoordinatesValid({ xx, yy }))
        {
            Map[xx][yy] = HOUSE + BLBNCNBIT + RandomRange(0, 2) + (value * 3);
        }
    }
}


void IncROG(int amount)
{
    const int rogVal = RateOfGrowthMap.value({ SimulationTarget.x >> 3, SimulationTarget.y >> 3 });
    RateOfGrowthMap.value({ SimulationTarget.x >> 3, SimulationTarget.y >> 3 }) = rogVal + (amount << 2);
}


void DoResIn(int pop, int value)
{
    int z;

    z = PollutionMap.value(SimulationTarget.skewInverseBy({ 2, 2 }));

    if (z > 128)
    {
        return;
    }

    if (CurrentTileMasked == FREEZ)
    {
        if (pop < 8)
        {
            BuildHouse(value);
            IncROG(1);
            return;
        }

        if (PopulationDensityMap.value(SimulationTarget.skewInverseBy({ 2, 2 })) > 64)
        {
            residentialPlop(0, value);
            IncROG(8);
            return;
        }

        return;
    }

    if (pop < 40)
    {
        residentialPlop((pop / 8) - 1, value);
        IncROG(8);
    }
}


void DoComIn(int pop, int value)
{
    int z;

    z = LandValueMap.value(SimulationTarget.skewInverseBy({ 2, 2 }));
    z = z >> 5;

    if (pop > z)
    {
        return;
    }

    if (pop < 5)
    {
        commercialPlop(pop, value);
        IncROG(8);
    }
}


void DoIndIn(int pop, int value)
{
    if (pop < 4)
    {
        industrialPlop(pop, value);
        IncROG(8);
    }
}


void DoResOut(int pop, int value)
{
    static int Brdr[9] = { 0,3,6,1,4,7,2,5,8 };
    int x, y, loc, z;

    if (!pop)
    {
        return;
    }

    if (pop > 16)
    {
        residentialPlop(((pop - 24) / 8), value);
        IncROG(-8);
        return;
    }

    if (pop == 16)
    {
        IncROG(-8);
        Map[SimulationTarget.x][SimulationTarget.y] = (FREEZ | BLBNCNBIT | ZONEBIT);

        for (x = SimulationTarget.x - 1; x <= SimulationTarget.x + 1; x++)
        {
            for (y = SimulationTarget.y - 1; y <= SimulationTarget.y + 1; y++)
            {
                if (x >= 0 && x < SimWidth && y >= 0 && y < SimHeight)
                {
                    if ((Map[x][y] & LOMASK) != FREEZ)
                    {
                        Map[x][y] = LHTHR + value + RandomRange(0, 2) + BLBNCNBIT;
                    }
                }
            }
        }
    }

    if (pop < 16)
    {
        IncROG(-1);
        z = 0;

        for (x = SimulationTarget.x - 1; x <= SimulationTarget.x + 1; x++)
        {
            for (y = SimulationTarget.y - 1; y <= SimulationTarget.y + 1; y++)
            {
                if (x >= 0 && x < SimWidth && y >= 0 && y < SimHeight)
                {
                    loc = Map[x][y] & LOMASK;
                    if ((loc >= LHTHR) && (loc <= HHTHR))
                    {
                        Map[x][y] = Brdr[z] + BLBNCNBIT + FREEZ - 4;
                        return;
                    }
                }
                z++;
            }
        }
    }
}


void DoComOut(int pop, int value)
{
    if (pop > 1)
    {
        commercialPlop(pop - 2, value);
        IncROG(-8);
        return;
    }

    if (pop == 1)
    {
        zonePlop(COMBASE);
        IncROG(-8);
    }
}


void DoIndOut(int pop, int value)
{
    if (pop > 1)
    {
        industrialPlop(pop - 2, value);
        IncROG(-8);
        return;
    }

    if (pop == 1)
    {
        zonePlop(INDCLR - 4);
        IncROG(-8);
    }
}


int DoFreePop()
{
    int count;
    int loc, x, y;

    count = 0;
    for (x = SimulationTarget.x - 1; x <= SimulationTarget.x + 1; x++)
    {
        for (y = SimulationTarget.y - 1; y <= SimulationTarget.y + 1; y++)
        {
            if (x >= 0 && x < SimWidth && y >= 0 && y < SimHeight)
            {
                loc = Map[x][y] & LOMASK;
                if ((loc >= LHTHR) && (loc <= HHTHR))
                {
                    count++;
                }
            }
        }
    }

    return count;
}


void DoIndustrial(int ZonePwrFlg)
{
    int tpop, zscore, TrfGood;

    IndZPop++;

    SetSmoke(ZonePwrFlg);

    tpop = industrialZonePopulation(CurrentTileMasked);
    IndPop += tpop;

    if (tpop > RandomRange(0, 5))
    {
        TrfGood = MakeTraf(2);
    }
    else
    {
        TrfGood = true;
    }

    if (TrfGood == -1)
    {
        DoIndOut(tpop, Rand16() & 1);
        return;
    }

    if (!(Rand16() & 7))
    {
        zscore = IValve + EvalInd(TrfGood);

        if (!ZonePwrFlg)
        {
            zscore = -500;
        }

        if ((zscore > -350) && (zscore - 26380) > Rand16())
        {
            DoIndIn(tpop, Rand16() & 1);
            return;
        }

        if ((zscore < 350) && (zscore + 26380) < Rand16())
        {
            DoIndOut(tpop, Rand16() & 1);
        }
    }
}


void DoCommercial(int ZonePwrFlg)
{
    int TrfGood;
    int zscore, locvalve, value;

    ComZPop++;

    int tpop = commercialZonePopulation(CurrentTileMasked);

    ComPop += tpop;

    if (tpop > RandomRange(0, 5))
    {
        TrfGood = MakeTraf(1);
    }
    else
    {
        TrfGood = 1;
    }

    if (TrfGood == -1)
    {
        value = GetCRVal();
        DoComOut(tpop, value);
        return;
    }

    if (!(Rand16() & 7))
    {
        locvalve = EvalCom(TrfGood);
        zscore = CValve + locvalve;

        if (!ZonePwrFlg)
        {
            zscore = -500;
        }

        if (TrfGood && (zscore > -350) && zscore - 26380 > Rand16())
        {
            value = GetCRVal();
            DoComIn(tpop, value);
            return;
        }

        if (zscore < 350 && zscore + 26380 < Rand16())
        {
            value = GetCRVal();
            DoComOut(tpop, value);
        }
    }
}


void DoResidential(int ZonePwrFlg)
{
    int tpop, value, TrfGood;

    ResZPop++;
    if (CurrentTileMasked == FREEZ)
    {
        tpop = DoFreePop();
    }
    else
    {
        tpop = residentialZonePopulation(CurrentTileMasked);
    }

    ResPop += tpop;
    if (tpop > RandomRange(0, 35))
    {
        TrfGood = MakeTraf(0);
    }
    else
    {
        TrfGood = true;
    }

    if (TrfGood == -1)
    {
        value = GetCRVal();
        DoResOut(tpop, value);
        return;
    }

    if ((CurrentTileMasked == FREEZ) || (!(Rand16() & 7)))
    {
        int locvalve = EvalRes(TrfGood);
        int zscore = RValve + locvalve;
        if (!ZonePwrFlg)
        {
            zscore = -500;
        }

        if (zscore > -350 && zscore - 26380 > -Rand16())
        {
            if ((!tpop) && (!(Rand16() & 3)))
            {
                MakeHosp();
                return;
            }

            value = GetCRVal();
            DoResIn(tpop, value);

            return;
        }

        if ((zscore < 350) && zscore + 26380 < Rand16())
        {
            value = GetCRVal();
            DoResOut(tpop, value);
        }
    }
}


void DoZone(const CityProperties& properties)
{
    int ZonePwrFlg;

    ZonePwrFlg = setZonePower();	/* Set Power Bit in Map from PowerMap */

    if (ZonePwrFlg)
    {
        PwrdZCnt++;
    }
    else
    {
        unPwrdZCnt++;
    }

    if (CurrentTileMasked > PORTBASE) /* do Special Zones  */
    {
        DoSPZone(ZonePwrFlg, properties);
        return;
    }

    if (CurrentTileMasked < HOSPITAL)
    {
        DoResidential(ZonePwrFlg);
        return;
    }

    if (CurrentTileMasked < COMBASE)
    {
        doHospital();
        doChurch();
        return;
    }

    if (CurrentTileMasked < INDBASE)
    {
        DoCommercial(ZonePwrFlg);
        return;
    }

    DoIndustrial(ZonePwrFlg);
    return;
}