/*
 * ray.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Jason
 */

#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"

/*
 * Ray class that holds a point and direction and is used to compute traits
 */
class ray{
public:
	ray() {}
	ray(const vec3 &a, const vec3 &b){ A = a; B = b; }
	vec3 origin() const { return A; }
	vec3 dir() const { return B; }
	vec3 point(float t) const { vec3 temp = t*B; return temp+A; }

	vec3 A;
	vec3 B;
};


#endif /* RAY_H_ */
