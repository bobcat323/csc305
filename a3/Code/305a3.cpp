//============================================================================
// Name        : a3.cpp
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
#include "moving_sphere.h"
#include "bvh.h"
#include "box.h"
#include "texture.h"
#include "perlin.h"
#include "constant_medium.h"
#include <cstdlib>	//rand_max, rand()
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

float randNum(){
	return ((float) rand() / (float)RAND_MAX);
}

vec3 colour(const ray &r, hitable *world, int depth) {
	hit_record record;
	if(world->hit(r, 0.001, FLT_MAX, record)){
		ray scattered;
		vec3 diminish;
		vec3 emitted = record.matPtr->emitted(record.u, record.v, record.p);
		if(depth < 50 && record.matPtr->scatter(r, record, diminish, scattered)){
			return emitted + diminish * colour(scattered, world, depth + 1); //"diminish;//colour()"
		}else{
			return emitted;
		}
	}else{
		/*
		vec3 unit_dir = unit_vector(r.dir());
		float t = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);//BG colour 0.5, 0.7, 1.0
		*/
		return vec3(0, 0, 0);
	}
}

hitable *random_scene(){
	int n = 2;
	hitable **list = new hitable*[n+1];
	//list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.3, 0.5, 0.5)));
	texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian( checker));
	int i = 1;
	for(int a = -10; a < 10; a++){
		for(int b = -10; b < 10; b++){
			float choose_mat = (float) rand() / (float)RAND_MAX;
			vec3 center(a + 0.9*(float) rand() / (float)RAND_MAX, 0.2, b+0.9*(float) rand() / (float)RAND_MAX);
			if((center-vec3(4, 0.2, 0)).length() > 0.9){
				if (choose_mat < 0.8) {  // diffuse
				    list[i++] = new moving_sphere(center, center+vec3(0,0.5*((float) rand() / (float)RAND_MAX), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(((float) rand() / (float)RAND_MAX)*((float) rand() / (float)RAND_MAX), ((float) rand() / (float)RAND_MAX)*((float) rand() / (float)RAND_MAX), ((float) rand() / (float)RAND_MAX)*((float) rand() / (float)RAND_MAX)))));
				}else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
				    new metal(vec3(0.5*(1 + ((float) rand() / (float)RAND_MAX)), 0.5*(1 + ((float) rand() / (float)RAND_MAX)), 0.5*(1 + ((float) rand() / (float)RAND_MAX))),  0.5*((float) rand() / (float)RAND_MAX)));
				}else {  // glass
				    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hit_list(list, i);
}


hitable* two_spheres(){
	texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	int n = 2;
	hitable** list = new hitable*[n+1];
	list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));

	return new hit_list(list, 2);
}

hitable* two_perlin_spheres(){
	texture* pertext = new noise_texture(4);
	hitable** list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	return new hit_list(list, 2);
}

hitable* earth(){
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat =  new lambertian(new image_texture(tex_data, nx, ny));
	return new sphere(vec3(0,0, 0), 2, mat);
}

hitable* simple_light(){
	texture* pertext = new noise_texture(4);
	hitable** list = new hitable*[5];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[4] = new sphere(vec3(0, 20, 0), 3, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hit_list(list, 5);
}

hitable* cornell_box() {
	hitable** list = new hitable*[8];//6
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	//list[i++] = new yz_rect(0, 555, 0, 555, 555, green);
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	//list[i++] = new box(vec3(130, 0, 65), vec3(295, 165, 230), white);
	//list[i++] = new box(vec3(265, 0, 295), vec3(430, 330, 460), white);

	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hit_list(list, i);

}

hitable* cornell_smoke(){
	hitable** list = new hitable*[8];
	int i = 0;

	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	hitable* b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), -18), vec3(130, 0, 65));
	hitable* b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
	return new hit_list(list, i);


}

hitable *final() {
    int nb = 1;
    hitable **list = new hitable*[30];
    hitable **boxlist = new hitable*[100];
    hitable **boxlist2 = new hitable*[100];
    material *white = new lambertian( new constant_texture(vec3(0.73, 0.73, 0.73)) );
    material *ground = new lambertian( new constant_texture(vec3(0.48, 0.83, 0.53)) );
    int b = 0;
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < nb; j++) {
            float w = 100;
            float x0 = -1000 + i*w;
            float z0 = -1000 + j*w;
            float y0 = 0;
            float x1 = x0 + w;
            float y1 = 100*(((float) rand() / (float)RAND_MAX) + 0.01);
            float z1 = z0 + w;
            boxlist[b++] = new box(vec3(x0,y0,z0), vec3(x1,y1,z1), ground);
        }
    }
    int l = 0;
    list[l++] = new bvh_node(boxlist, b, 0, 1);

    material *light = new diffuse_light( new constant_texture(vec3(7, 7, 7)) );
    list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
    vec3 center(400, 400, 200);

    list[l++] = new moving_sphere(center, center+vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));
    list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
    list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));

    hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
    boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
    list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));

    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    material *emat =  new lambertian(new image_texture(tex_data, nx, ny));
    list[l++] = new sphere(vec3(400,200, 400), 100, emat);
    texture *pertext = new noise_texture(0.1);
    list[l++] =  new sphere(vec3(220,280, 300), 80, new lambertian( pertext ));

    int ns = 2;
    for (int j = 0; j < ns; j++) {
    	 boxlist2[j] = new sphere(vec3(165*randNum(), 165*randNum(), 165*randNum()), 10, white);
    }
    list[l++] = new translate(new rotate_y(new bvh_node(boxlist2,ns, 0.0, 1.0), 15), vec3(-100,270,395));

    return new hit_list(list,l);
}

int main() {
	ofstream myfile;
	myfile.open ("test.ppm");
	int maxX = 1000;
	int maxY = 1000;
	int maxS = 100;

	vec3 camPos(278, 278, -800);
	vec3 objPos(278, 278, 0);
	float distToFocus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	camera cam(camPos, objPos, vec3(0, 1, 0), vfov, float(maxX) / float(maxY), aperture, distToFocus, 0.0, 1.0);

	//hitable* world = new hit_list(list, 0);
	//hitable* world = random_scene();//crashes
	//hitable* world = two_spheres();//works, patchy though
	//hitable* world = two_perlin_spheres();//works
	//hitable* world = earth();//works
	//hitable* world = simple_light();//works
	//hitable* world = cornell_box();//works
	//hitable* world = cornell_smoke();//works
	hitable* world = final();
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
