#pragma once


struct ColorRGB
{
public:
	ColorRGB() : r(0.0), g(0.0), b(0.0) {};
	ColorRGB(float r, float g, float b) : r(r), g(g), b(b) {};

	float r;
	float g;
	float b;
};

