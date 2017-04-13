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
	ray(const vec3 &a, const vec3 &b, float ti = 0.0){ A = a; B = b; _time = ti;}
	vec3 origin() const { return A; }
	vec3 dir() const { return B; }
	float time() const {return _time;}
	vec3 point(float t) const {return A + t*B; }

	vec3 A;
	vec3 B;
	float _time;
};


#endif /* RAY_H_ */
