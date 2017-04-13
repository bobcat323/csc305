/*
 * constant_medium.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Jason
 */

#ifndef CONSTANT_MEDIUM_H_
#define CONSTANT_MEDIUM_H_

#include "hitable.h"
#include <float.h>

class constant_medium:public hitable {
public:
	constant_medium(hitable* b, float d, texture* a):boundary(b), density(d) {phase_function = new isotropic(a);}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const{
		return boundary->bounding_box(t0, t1, box);
	}
	hitable* boundary;
	float density;
	material* phase_function;
};

bool constant_medium::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
	bool db = (((float) rand() / (float)RAND_MAX) < 0.00001);
	db = false;
	hit_record rec1, rec2;
	if(boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)){
		if(boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)){
			if(db){
				std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
			}
			if(rec1.t < t_min){
				rec1.t = t_min;
			}
			if(rec2.t > t_max){
				rec2.t = t_max;
			}
			if(rec1.t >= rec2.t){
				return false;
			}
			if(rec1.t < 0){
				rec1.t = 0;
			}
			float dist_inside_boundary = (rec2.t - rec1.t) * r.dir().length();
			float hit_dist = -(1/density) * log((float) rand() / (float)RAND_MAX);
			if(hit_dist < dist_inside_boundary){
				if(db) std::cerr << "hit_distance = " << hit_dist << "\n";
					rec.t = rec1.t + hit_dist / r.dir().length();
				if(db) std::cerr << "rec.t = " << rec.t << "\n";
					rec.p = r.point(rec.t);
				if(db) std::cerr << "rec.p = " << rec.p[0] << rec.p[1] << rec.p[2] << "\n";
					rec.norm = vec3(1, 0, 0);
					rec.matPtr = phase_function;
					return true;
			}
		}
	}
	return false;
}

#endif /* CONSTANT_MEDIUM_H_ */
