#include "camera.h"
#include <qgl.h>
#include <iostream>
#include "stdio.h"

using namespace std;

void OrbitCamera::mouseMove(const Vector2 &delta)
{
    // Rotate the eye vector around the origin
    theta += delta.x * 0.01f;
    phi += delta.y * 0.01f;

    // Keep theta in [0, 2pi] and phi in [-pi/2, pi/2]
    theta -= floorf(theta / M_2PI) * M_2PI;
    phi = max(0.01f - M_PI / 2, min(M_PI / 2 - 0.01f, phi));
}

void OrbitCamera::mouseWheel(float delta, Vector3 * target)
{
	float ndelta = delta/19.0;
	Vector3 newCenter = *target - Vector3::fromAngles(theta, phi) * ndelta;
	if(newCenter.x*newCenter.x + newCenter.z*newCenter.z < 14000 && newCenter.y < 150  && newCenter.y >=2) {
		*target = newCenter;
	}
}

void OrbitCamera::lookAt(const Vector3 &point)
{
    /**
     * Spherical coordinate! http://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
     * note, we are using y as the up vector as opposed to z which changes the equation slightly
     */
    Vector3 dir = point.unit();
    phi = asinf(dir.y);
    theta = atanf(dir.z/dir.x);
}

