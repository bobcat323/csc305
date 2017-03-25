//============================================================================
// Name        : a1.cpp
// Author      : Jason Louie (v00804645)
// Description : A ray tracing program that sends rays onto objects and reflect/refract based on the object's behaviour
// Credits	   : Supplementary code provided by Peter Shirley, Ray Tracing In One Weekend
//============================================================================

#include <iostream>
#include <fstream>
#include "float.h"	//flt_max
#include "hit_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <cstdlib>	//rand_max, rand()

using namespace std;

vec3 colour(const ray &r, hitable *world, int depth) {
	hit_record record;
	if(world->hit(r, 0.001, FLT_MAX, record)){
		ray scattered;
		vec3 diminish;
		if(depth < 50 && record.matPtr->scatter(r, record, diminish, scattered)){
			return diminish * colour(scattered, world, depth + 1);
		}else{
			return vec3(0,0,0);
		}
	}else{
		vec3 unit_dir = unit_vector(r.dir());
		float t = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	ofstream myfile;
	myfile.open ("test.ppm");
	int maxX = 200;
	int maxY = 100;
	int maxS = 100;

	vec3 camPos(3, 3, 2);
	vec3 objPos(0, 0, -1);
	float distToFocus = (camPos - objPos).length();
	float aperture = 2.0;
	camera cam(camPos, objPos, vec3(0, 1, 0), 20, float(maxX) / float(maxY), aperture, distToFocus);
	hitable *list[5];

	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));	//floor, don't make it a sphere!
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));

	hitable *world = new hit_list(list, 5);
	myfile << "P3\n" << maxX << " " << maxY << "\n255\n";

	for(int j = maxY - 1; j >= 0; j--){
		for(int i = 0; i < maxX; i++){
			vec3 colr(0,0,0);
			for(int k = 0; k < maxS; k++){
				float r = float(i + (((float) rand() / RAND_MAX))) / float(maxX);
				float g = float(j + (((float) rand() / RAND_MAX))) / float(maxY);

				ray ra = cam.getr(r, g);
				colr += colour(ra, world, 0);
			}
			colr /= float(maxS);
			colr = vec3(sqrt(colr[0]), sqrt(colr[1]), sqrt(colr[2]));
			int newR = int(255.99 * colr[0]);
			int newG = int(255.99 * colr[1]);
			int newB = int(255.99 * colr[2]);

			myfile << newR << " " << newG << " " << newB << endl;
		}
	}

	myfile.close();
	cout << "Written to file." << endl;
#ifdef _WIN32
    // Display the image automatically
    system("test.ppm");
#endif
	return 0;
}
