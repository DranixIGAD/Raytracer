#pragma once
#include <cstdint>

class color
{
public:
	uint8_t R;
	uint8_t G;
	uint8_t B;

	color()
		: R(0)
		, G(0)
		, B(0)
	{
	}

	color(const uint8_t& value)
		: R(value)
		, G(value)
		, B(value)
	{
	}
	
	color(const uint8_t& r, const uint8_t& g, const uint8_t& b)
		: R(r)
		, G(g)
		, B(b)
	{
	}
	
	/// Returns the value of the given color added to this.
	color operator+(const color& v) const
	{
		color tempColor;
		
		tempColor.R = R + v.R;
		tempColor.G = G + v.G;
		tempColor.B = B + v.B;
		
		return tempColor;
	}

	/// Returns a copy of this color scaled by the given value.
	color operator*(float value) const
	{
		color tempColor;

		tempColor.R = static_cast<uint8_t>(static_cast<float>(R) * value);
		tempColor.G = static_cast<uint8_t>(static_cast<float>(G) * value);
		tempColor.B = static_cast<uint8_t>(static_cast<float>(B) * value);

		return tempColor;
	}

	/// Returns a copy of this color scaled by the given color.
	color operator*(const color& v) const
	{
		color tempColor;

		tempColor.R = R * v.R;
		tempColor.G = G * v.G;
		tempColor.B = B * v.B;

		return tempColor;
	}

	/// Adds the given color to this.
	void operator+=(const color& v)
	{
		R += v.R;
		G += v.G;
		B += v.B;
	}
};