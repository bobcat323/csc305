/*
 * hit_list.h
 *
 *  Created on: Jan 23, 2017
 *      Author: Jason
 */

#ifndef HIT_LIST_H_
#define HIT_LIST_H_

#include "hitable.h"

/*
 * hit_list class used to keep track of objects on the image
 */
class hit_list : public hitable{
public:
	hit_list() { }
	hit_list(hitable **l, int size) { list = l, listSize = size; }
	virtual bool hit(const ray &r, float min, float max, hit_record &record) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	hitable **list;
	int listSize;
};

/*
 * Overloading function
 */
bool hit_list::hit(const ray &r, float min, float max, hit_record &record) const{
	hit_record tempRecord;
	bool hitAnything = false;
	double closestYet = max;	//double?
	for(int i = 0; i < listSize; i++){
		if(list[i]->hit(r, min, closestYet, tempRecord)){	//closestYet
			hitAnything = true;
			closestYet = tempRecord.t;
			record = tempRecord;
		}
	}
	return hitAnything;
}

bool hit_list::bounding_box(float t0, float t1, aabb& box) const {
	if(listSize < 1){
		return false;
	}
	aabb temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if(!first_true){
		return false;
	}else{
		box = temp_box;
	}
	for(int i = 1; i < listSize; i++){
		if(list[0]->bounding_box(t0, t1, temp_box)) {
			box = surrounding_box(box, temp_box);
		}else{
			return false;
		}
	}
	return true;
}

#endif /* HIT_LIST_H_ */
