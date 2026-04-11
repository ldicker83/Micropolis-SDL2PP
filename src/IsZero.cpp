// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2026 Leeor Dicker
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "IsZero.h"

#include <cmath>
#include <stdexcept>


bool isZero(float value)
{
	return std::fpclassify(value) == FP_ZERO;
}


bool isZero(double value)
{
	return std::fpclassify(value) == FP_ZERO;
}


bool isZero(long double value)
{
	return std::fpclassify(value) == FP_ZERO;
}


// Defined in source file to limit include of `<stdexcept>`
void throwDomainError(const char* errorMessage)
{
	throw std::domain_error(errorMessage);
}
