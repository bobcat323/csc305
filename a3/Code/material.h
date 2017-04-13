/*
 * material.h
 *
 *  Created on: Jan 27, 2017
 *      Author: Jason
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cstdlib>	//rand_max
#include "ray.h"
#include "hitable.h"
#include "texture.h"

vec3 randUnit();
vec3 reflect(const vec3 &vec, const vec3 &norm);
bool refract(const vec3 &vec, const vec3 &norm, float non, vec3 &refracted);
float schlick(float cos, float refl);

/*
 * Pure abstract class, defines a scatter function that confirms whether rays should be reflected/refracted
 */
class material {
public:
	virtual bool scatter(const ray &r, const hit_record &record, vec3 &diminish, ray &scattered) const = 0;
	virtual vec3 emitted(float u, float v, const vec3& p) const {return vec3(0, 0, 0);}
};

/*
 * Lambertian material that behaves as a matte image that absorbs rays(light), little to no reflection
 */
class lambertian : public material {
public:
	lambertian(texture* a) : albedo(a) {}
	virtual bool scatter(const ray &r, const hit_record &record, vec3 &diminish, ray &scattered) const {
		vec3 target = record.p + record.norm + randUnit();
		scattered = ray(record.p, target - record.p, r.time());
		diminish = albedo->value(record.u, record.v, record.p);
		return true;
	}
	texture* albedo;
};

/*
 * Metal material that behaves as a reflecting agent when rays are sent onto it, lots of reflection
 */
class metal : public material {
public:
	metal(const vec3 &a, float f) : albedo(a) {
		if(f < 1){		//ray reflects based on sphere's radius
			fuzz = f;
		}else{
			fuzz = 1;	//ray is hitting the surface, no fuzziness
		}
	}
	virtual bool scatter(const ray &r, const hit_record &record, vec3 &diminish, ray &scattered) const {
		vec3 reflected = reflect(unit_vector(r.dir()), record.norm);
		scattered = ray(record.p, reflected + fuzz * randUnit());
		diminish = albedo;
		return (dot(scattered.dir(), record.norm) > 0);
	}
	vec3 albedo;
	float fuzz;
};

/*
 * Dielectric material that behaves as a refracting agent when rays are sent onto it, lots of refraction
 */
class dielectric : public material {
public:
	dielectric(float ri) : refl(ri) {}
	virtual bool scatter(const ray &r, const hit_record &record, vec3 &diminish, ray &scattered) const {
		vec3 outNorm, refracted;
		float non, reflProb, cos;
		vec3 reflected = reflect(r.dir(), record.norm);
		diminish = vec3(1.0, 1.0, 1.0);	//diminish is always 1,1,1 since glass doesn't absorb any rays

		if(dot(r.dir(), record.norm) > 0){
			outNorm = -record.norm;
			non = refl;
			cos = refl * dot(r.dir(), record.norm) / r.dir().length();
		}else{
			outNorm = record.norm;
			non = 1.0 / refl;
			cos = -dot(r.dir(), record.norm) / r.dir().length();
		}
		if(refract(r.dir(), outNorm, non, refracted)){
			reflProb = schlick(cos, refl);
		}else{
			reflProb = 1.0;
		}
		if(((float) rand() / (float)RAND_MAX) < reflProb){
			scattered = ray(record.p, reflected);
		}else{
			scattered = ray(record.p, refracted);
		}
		return true;
	}
	float refl;
};

class diffuse_light : public material{
public:
	diffuse_light(texture* a) : emit(a) {}
	virtual bool scatter(const ray& r, const hit_record& rec, vec3& diminish, ray& scattered) const {return false;}
	virtual vec3 emitted(float u, float v, const vec3& p) const {return emit->value(u, v, p); }
	texture* emit;
};

class isotropic:public material{
public:
	isotropic(texture* a):albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& diminish, ray& scattered) const{
		scattered = ray(rec.p, randUnit());
		diminish = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
	texture* albedo;
};

/*
 * Create a random point on the object
 */
vec3 randUnit(){
	vec3 point;
	do{
		float random1 = ((float) rand() / (float)RAND_MAX);
		float random2 = ((float) rand() / (float)RAND_MAX);
		float random3 = ((float) rand() / (float)RAND_MAX);
		point = 2.0 * vec3(random1, random2, random3) - vec3(1,1,1);
	}while(point.squared_length() >= 1.0);
	return point;
}

/*
 * Reflect function:
 * Computes the vector 'B' when a vector 'vec' is projected onto an object
 * 'B' is found by bouncing 'vec' off the object and out another direction
 */
vec3 reflect(const vec3 &vec, const vec3 &unitVec){
	return vec - 2 * dot(vec, unitVec) * unitVec;
}

/*
 * Refract function:
 * Determines whether or not the ray pierces through or bounces off
 */
bool refract(const vec3 &vec, const vec3 &norm, float non, vec3 &refracted){
	vec3 unitVec = unit_vector(vec);
	float dotProd = dot(unitVec, norm);
	float disc = 1.0 - non * non * (1 - dotProd * dotProd);
	if(disc > 0){
		refracted = non * (unitVec - norm * dotProd) - norm * sqrt(disc);
		return true;
	}else{
		return false;
	}
}

/*
 * The appearance when the depth of field is low at certain angles
 */
float schlick(float cos, float refl){
	float r0 = (1 - refl) / (1 + refl);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cos), 5);
}

#endif /* MATERIAL_H_ */
