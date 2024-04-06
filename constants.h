#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "maths.h"

namespace bitmasks
{
	inline constexpr std::uint8_t addition{ 0b0000'0001 };
	inline constexpr std::uint8_t soustraction{ 0b0000'0010 };
	inline constexpr std::uint8_t multiplication{ 0b0000'0100 };
	inline constexpr std::uint8_t division{ 0b0000'1000 };
}

#endif