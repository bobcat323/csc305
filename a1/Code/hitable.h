/*
 * hitable.h
 *
 *  Created on: Jan 23, 2017
 *      Author: Jason
 */

#ifndef HITABLE_H_
#define HITABLE_H_

#include "ray.h"

class material;

/*
 * A struct that holds record of rays that successfully hit an object
 */
struct hit_record{
	float t;
	vec3 p;
	vec3 norm;
	material *matPtr;
};

/*
 * Pure abstract class that contains the hit function to determine if the ray has hit an object or not
 */
class hitable {
public:
	virtual bool hit(const ray &r, float min, float max, hit_record &record) const = 0;
};

#endif /* HITABLE_H_ */
