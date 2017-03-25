/*
 * sphere.h
 *
 *  Created on: Jan 23, 2017
 *      Author: Jason
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "hitable.h"

/*
 * Sphere class that holds properties of itself
 */
class sphere : public hitable {
public:
	sphere() { }
	sphere(vec3 cent, float val, material *mat) : center(cent), radius(val), matPtr(mat) { };
	virtual bool hit(const ray &r, float min, float max, hit_record &record) const;
	vec3 center;
	float radius;
	material *matPtr;
};

/*
 * Overloading function from 'hitable' class applying to spheres
 */
bool sphere::hit(const ray &r,float min, float max, hit_record &record) const {
	float a, b, c;
	vec3 centToOrigin = r.origin() - center;
	a = dot(r.dir(), r.dir());
	b = dot(centToOrigin, r.dir());
	c = dot(centToOrigin, centToOrigin) - radius * radius;

	if((b * b - (a * c)) > 0) {	//discriminant: return false if ray doesn't hit sphere
		float tempDisc = (-b - sqrt(b * b - (a * c))) / a;
		if(tempDisc < max && tempDisc > min){
			record.t = tempDisc;
			record.p = r.point(record.t);
			record.norm = (record.p - center) / radius;
			record.matPtr = matPtr; 	//newly added
			return true;
		}
		tempDisc = (-b + sqrt(b * b - (a * c))) / a;
		if(tempDisc < max && tempDisc > min){
			record.t = tempDisc;
			record.p = r.point(record.t);
			record.norm = (record.p - center) / radius;
			record.matPtr = matPtr;		//newly added
			return true;
		}
	}
	return false;
}

#endif /* SPHERE_H_ */
