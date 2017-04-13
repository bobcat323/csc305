/*
 * hitable.h
 *
 *  Created on: Jan 23, 2017
 *      Author: Jason
 */

#ifndef HITABLE_H_
#define HITABLE_H_

#include "ray.h"
#include "aabb.h"
#include <float.h>

class material;

/*
 * A struct that holds record of rays that successfully hit an object
 */
struct hit_record{
	float t;
	float u;
	float v;
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
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

class xy_rect : public hitable{
public:
	xy_rect(){}
	xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material* mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
	virtual bool hit(const ray& r, float t0, float t1, hit_record& record) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const{
		box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
		return true;
	}
	material* mp;
	float x0, x1, y0, y1, k;
};

class flip_normals : public hitable {
public:
	flip_normals(hitable* p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& record) const{
		if(ptr->hit(r, t_min, t_max, record)){
			record.norm = -record.norm;
			return true;
		}else{
			return false;
		}
	}
	virtual bool bounding_box(float t0, float t1, aabb& box) const{
			return ptr->bounding_box(t0, t1, box);
		}
	hitable* ptr;
};

class xz_rect:public hitable {
public:
	xz_rect() {}
	xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material* mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const{
		box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
				return true;
	}
	material* mp;
	float x0, x1, z0, z1, k;
};

class yz_rect:public hitable {
public:
	yz_rect() {}
	yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material* mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const{
		box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
				return true;
	}
	material* mp;
	float y0, y1, z0, z1, k;
};

class translate:public hitable {
public:
	translate(hitable *p, const vec3& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	hitable* ptr;
	vec3 offset;
};

class rotate_y:public hitable {
public:
	rotate_y(hitable *p, float angle);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const{
		box = bbox;
		return hasbox;
	}
	hitable* ptr;
	float sin_theta, cos_theta;
	bool hasbox;
	aabb bbox;
};

rotate_y::rotate_y(hitable* p, float angle):ptr(p){
	float radians = (M_PI / 180.) * angle;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			for(int k = 0; k < 2; k++){
				float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
				float newx = cos_theta * x + sin_theta * z;
				float newz = -sin_theta * x + cos_theta * z;
				vec3 tester(newx, y, newz);
				for(int c = 0; c < 3; c++){
					if(tester[c] > max[c]){
						max[c] = tester[c];
					}
					if(tester[c] < min[c]){
						min[c] = tester[c];
					}
				}
			}
		}
	}
	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
	vec3 origin = r.origin();
	vec3 dir = r.dir();
	origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
	origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
	dir[0] = cos_theta * r.dir()[0] - sin_theta * r.dir()[2];
	dir[2] = sin_theta * r.dir()[0] + cos_theta * r.dir()[2];
	ray rotated_r(origin, dir, r.time());
	if(ptr->hit(rotated_r, t_min, t_max, rec)){
		vec3 p = rec.p;
		vec3 norm = rec.norm;
		p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
		p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
		norm[0] = cos_theta * rec.norm[0] + sin_theta * rec.norm[2];
		norm[2] = -sin_theta * rec.norm[0] + cos_theta * rec.norm[2];
		rec.p = p;
		rec.norm = norm;
		return true;
	}else{
		return false;
	}
}

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
	ray moved(r.origin() - offset, r.dir(), r.time());
	if(ptr->hit(moved, t_min, t_max, rec)){
		rec.p += offset;
		return true;
	}else{
		return false;
	}
}

bool translate::bounding_box(float t0, float t1, aabb& box) const{
	if(ptr->bounding_box(t0, t1, box)){
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}else{
		return false;
	}
}

bool xy_rect::hit(const ray& r, float t0, float t1, hit_record& record) const {
	float t = (k - r.origin().z()) / r.dir().z();
	if(t < t0 || t > t1){
		return false;
	}
	float x = r.origin().x() + t * r.dir().x();
	float y = r.origin().y() + t * r.dir().y();
	if(x < x0 || x > x1 || y < y0 || y > y1){
		return false;
	}
	record.u = (x - x0) / (x1 - x0);
	record.v = (y - y0) / (y1 - y0);
	record.t = t;
	record.matPtr = mp;
	record.p = r.point(t);
	record.norm = vec3(0, 0, 1);
	return true;
}

bool xz_rect::hit(const ray& r, float t0, float t1, hit_record& record) const {
	float t = (k - r.origin().y()) / r.dir().y();
	if(t < t0 || t > t1){
		return false;
	}
	float x = r.origin().x() + t * r.dir().x();
	float z = r.origin().z() + t * r.dir().z();
	if(x < x0 || x > x1 || z < z0 || z > z1){
		return false;
	}
	record.u = (x - x0) / (x1 - x0);
	record.v = (z - z0) / (z1 - z0);
	record.t = t;
	record.matPtr = mp;
	record.p = r.point(t);
	record.norm = vec3(0, 1, 0);
	return true;
}

bool yz_rect::hit(const ray& r, float t0, float t1, hit_record& record) const {
	float t = (k - r.origin().x()) / r.dir().x();
	if(t < t0 || t > t1){
		return false;
	}
	float y = r.origin().y() + t * r.dir().y();
	float z = r.origin().z() + t * r.dir().z();
	if(y < y0 || y > y1 || z < z0 || z > z1){
		return false;
	}
	record.u = (y - y0) / (y - y0);
	record.v = (z - z0) / (z1 - z0);
	record.t = t;
	record.matPtr = mp;
	record.p = r.point(t);
	record.norm = vec3(0, 0, 1);
	return true;
}

void get_sphere_uv(const vec3& p, float& u, float& v){
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI/2) / M_PI;
}

#endif /* HITABLE_H_ */
