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

#endif /* HIT_LIST_H_ */
