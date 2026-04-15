// This file is part of Micropolis-SDL2PP
// Micropolis-SDL2PP is based on Micropolis
//
// Copyright © 2022 - 2026 Leeor Dicker
//
// Portions Copyright © 1989-2007 Electronic Arts Inc.
//
// Micropolis-SDL2PP is free software; you can redistribute it and/or modify
// it under the terms of the GNU GPLv3, with additional terms. See the README
// file, included in this distribution, for details.
#include "RCI.h"

#include <algorithm>


void RCI::adjustResidentialDemand(int delta)
{
	if(delta == 0)
	{
		return;
	}

	mResidentialDemand = std::clamp(mResidentialDemand + delta, -2000, 2000); // magic numbers
}


void RCI::adjustCommercialDemand(int delta)
{
	if (delta == 0)
	{
		return;
	}

	mCommercialDemand = std::clamp(mCommercialDemand + delta, -1500, 1500); // magic numbers
}


void RCI::adjustIndustrialDemand(int delta)
{
	if (delta == 0)
	{
		return;
	}

	mIndustrialDemand = std::clamp(mIndustrialDemand + delta, -1500, 1500); // magic numbers
}
