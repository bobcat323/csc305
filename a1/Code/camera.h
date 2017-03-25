/*
 * camera.h
 *
 *  Created on: Jan 25, 2017
 *      Author: Jason
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "ray.h"

vec3 randDiskUnit();

class camera {
public:
	/*
	 * Parameters explained:
	 * camera position, object position, normal to the camera, degrees from top to bottom, aspect ratio,
	 * aperture value, distance from camera to object
	*/
	camera(vec3 lookFrom, vec3 lookAt, vec3 normUp, float vertFOV, float aspect, float aperture, float focusDist){
		lensRadius = aperture / 2;
		float theta = vertFOV * M_PI/180;
		float halfHeight = tan(theta/2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(normUp, w));
		v = cross(w, u);

		bottomLeft = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
		horz = 2 * halfWidth * focusDist * u;
		vert = 2 * halfHeight * focusDist * v;

	}
	/*
	 * Create a ray sent from the lens' position to project onto the image dimensions
	 */
	ray getr(float r, float g){
		vec3 rd = lensRadius * randDiskUnit();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, bottomLeft + r * horz + g * vert - origin - offset);
	}
	vec3 origin, bottomLeft, horz, vert, u, v, w;
	float lensRadius;
};

/*
 * Create a random point within the lens to send rays onto an object
 */
vec3 randDiskUnit(){
	vec3 point;
	do{
		float random1 = (float) rand() / (float)RAND_MAX;
		float random2 = (float) rand() / (float)RAND_MAX;
		point = 2.0 * vec3(random1, random2, 0.0) - vec3(1, 1, 0);
	}while(dot(point, point) >= 1.0);
	return point;
}

#endif /* CAMERA_H_ */
