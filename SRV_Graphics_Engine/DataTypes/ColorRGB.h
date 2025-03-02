#pragma once

#ifndef Colors
#define BLACK       ColorRGB(0.0f, 0.0f, 0.0f)
#define WHITE       ColorRGB(1.0f, 1.0f, 1.0f)
#define RED         ColorRGB(1.0f, 0.0f, 0.0f)
#define GREEN       ColorRGB(0.0f, 1.0f, 0.0f)
#define BLUE        ColorRGB(0.0f, 0.0f, 1.0f)
#define YELLOW      ColorRGB(1.0f, 1.0f, 0.0f)
#define CYAN        ColorRGB(0.0f, 1.0f, 1.0f)
#define MAGENTA     ColorRGB(1.0f, 0.0f, 1.0f)
#define ORANGE      ColorRGB(1.0f, 0.5f, 0.0f)
#define PURPLE      ColorRGB(0.5f, 0.0f, 0.5f)
#define GRAY        ColorRGB(0.5f, 0.5f, 0.5f)
#define LIGHT_GRAY  ColorRGB(0.75f, 0.75f, 0.75f)
#define DARK_GRAY   ColorRGB(0.25f, 0.25f, 0.25f)
#define NAVY        ColorRGB(0.0f, 0.0f, 0.5f)
#define OLIVE       ColorRGB(0.5f, 0.5f, 0.0f)
#define TEAL        ColorRGB(0.0f, 0.5f, 0.5f)
#endif // Colors


struct ColorRGB
{
public:
	ColorRGB() : r(0.0), g(0.0), b(0.0) {};
	ColorRGB(float r, float g, float b) : r(r), g(g), b(b) {};

	float r;
	float g;
	float b;
};

