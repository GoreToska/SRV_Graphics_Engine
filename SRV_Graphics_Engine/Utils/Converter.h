#pragma once


class Converter
{
public:
	static int GetNormalizedInt(int value)
	{
		if (value > 0)
			return 1;

		if (value < 0)
			return -1;

		return 0;
	}

	static float GetNormalizedFloat(float value)
	{
		if (value > 0.0f)
			return 1.0f;

		if (value < 0.0f)
			return -1.0f;

		return 0.0f;
	}

	static double GetNormalizedDouble(double value)
	{
		if (value > 0.0)
			return 1.0;

		if (value < 0.0)
			return -1.0;

		return 0.0;
	}
};