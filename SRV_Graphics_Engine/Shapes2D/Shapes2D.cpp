#include "Shapes2D.h"

#include <DirectXMath.h>
#include <math.h>

#define M_PI 3.14159265358979323846   // pi




std::vector<Vertex3D> Shapes2D::GetCircleShape(float radius, int tesselation)
{
	std::vector<Vertex3D> AllPoints;
	AllPoints.resize(tesselation);

	for (int i = 0; i < tesselation; i++) // running the loop with integer guarantees we run it exactly (tesselation) times
	{
		float angle =
			float(i) / float(tesselation) // this goes from 0 to almost 1
			* M_PI * 2; // 2PI is 360 degrees, so multiply by that to get angles between 0 and almost 2PI

		AllPoints[(int)i] = Vertex3D(Vector3D(radius * cos(angle), radius * sin(angle), 0.0f), ColorRGB(1.0f, 1.0f, 1.0f));
	}

	return AllPoints;
}
