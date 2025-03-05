#pragma once

#ifndef Colors
#define BLACK			ColorRGB(0.0f, 0.0f, 0.0f)
#define WHITE			ColorRGB(1.0f, 1.0f, 1.0f)
#define RED				ColorRGB(1.0f, 0.0f, 0.0f)
#define SCARLET_RED		ColorRGB(184.0/255.0, 43.0/255.0, 20.0/255.0)
#define GREEN			ColorRGB(0.0f, 1.0f, 0.0f)
#define GRASS_GREEN		ColorRGB(62.0/255.0, 117.0/255.0, 59.0/255.0)
#define BLUE			ColorRGB(0.0f, 0.0f, 1.0f)
#define YELLOW			ColorRGB(1.0f, 1.0f, 0.0f)
#define LIGHT_YELLOW     ColorRGB(252.0/255.0, 232.0/255.0, 131.0/255.0)
#define LIGHT_YELLOW_2    ColorRGB(255.0/255.0, 236.0/255.0, 139.0/255.0)
#define CYAN			ColorRGB(0.0f, 1.0f, 1.0f)
#define MAGENTA			ColorRGB(1.0f, 0.0f, 1.0f)
#define ORANGE			ColorRGB(1.0f, 0.5f, 0.0f)
#define LIGHT_ORANGE		ColorRGB(255.0/255.0, 202.0/255.0, 134.0/255.0)
#define PURPLE			ColorRGB(0.5f, 0.0f, 0.5f)
#define GRAY			ColorRGB(0.5f, 0.5f, 0.5f)
#define LIGHT_GRAY		ColorRGB(0.75f, 0.75f, 0.75f)
#define DARK_GRAY		ColorRGB(0.25f, 0.25f, 0.25f)
#define NAVY			ColorRGB(0.0f, 0.0f, 0.5f)
#define PERSID_BlUE		ColorRGB(0.0f, 103.0/255.0, 165.0/255.0)
#define BlUE_GREEN		ColorRGB(0.0f, 171.0/255.0, 179.0/255.0)
#define OLIVE			ColorRGB(0.5f, 0.5f, 0.0f)
#define TEAL			ColorRGB(0.0f, 0.5f, 0.5f)
#define LIGHT_BROWN		ColorRGB(0.0f, 171.0/255.0, 179.0/255.0)
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

