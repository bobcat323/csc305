/*
 * vec3.h
 *
 *  Created on: Jan 18, 2017
 *      Author: Jason
 */
#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>

/*
 * Vec3 class that holds x,y,z coordinates, overloaded functions and additional characteristic functions
 */
class vec3  {
public:
    vec3() {}
    vec3(float a, float b, float c) { ar[0] = a; ar[1] = b; ar[2] = c; }
    inline float x() const { return ar[0]; }
    inline float y() const { return ar[1]; }
    inline float z() const { return ar[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-ar[0], -ar[1], -ar[2]); }
    inline float operator[](int i) const { return ar[i]; }
    inline float& operator[](int i) { return ar[i]; };

    inline vec3& operator+=(const vec3 &b);
    inline vec3& operator/=(const float t);

    inline float length() const { return sqrt(ar[0] * ar[0] + ar[1] * ar[1] + ar[2] * ar[2]); }
    inline float squared_length() const { return ar[0]*ar[0] + ar[1]*ar[1] + ar[2]*ar[2]; }
    inline void make_unit_vector();
    float ar[3];
};

/*
 * Overloading functions for types vec3
 */
inline vec3 operator+(const vec3 &a, const vec3 &b) {
    return vec3(a.ar[0] + b.ar[0], a.ar[1] + b.ar[1], a.ar[2] + b.ar[2]);
}

inline vec3 operator-(const vec3 &a, const vec3 &b) {
    return vec3(a.ar[0] - b.ar[0], a.ar[1] - b.ar[1], a.ar[2] - b.ar[2]);
}

inline vec3 operator*(const vec3 &a, const vec3 &b) {
    return vec3(a.ar[0] * b.ar[0], a.ar[1] * b.ar[1], a.ar[2] * b.ar[2]);
}

inline vec3 operator/(const vec3 &a, const vec3 &b) {
    return vec3(a.ar[0] / b.ar[0], a.ar[1] / b.ar[1], a.ar[2] / b.ar[2]);
}

inline vec3 operator*(float val, const vec3 &vec) {
    return vec3(val * vec.ar[0], val * vec.ar[1], val * vec.ar[2]);
}

inline vec3 operator/(vec3 vec, float val) {
    return vec3(vec.ar[0] / val, vec.ar[1] / val, vec.ar[2] / val);
}

inline vec3 operator*(const vec3 &vec, float val) {
    return vec3(val * vec.ar[0], val * vec.ar[1], val * vec.ar[2]);
}

inline float dot(const vec3 &a, const vec3 &b) {
    return a.ar[0] *b.ar[0] + a.ar[1] *b.ar[1]  + a.ar[2] *b.ar[2];
}

inline vec3 cross(const vec3 &a, const vec3 &b) {
    return vec3( (a.ar[1]*b.ar[2] - a.ar[2]*b.ar[1]),
                (-(a.ar[0]*b.ar[2] - a.ar[2]*b.ar[0])),
                (a.ar[0]*b.ar[1] - a.ar[1]*b.ar[0]));
}

inline vec3& vec3::operator+=(const vec3 &v){
    ar[0]  += v.ar[0];
    ar[1]  += v.ar[1];
    ar[2]  += v.ar[2];
    return *this;
}

inline vec3& vec3::operator/=(const float t) {
    float k = 1.0/t;
    ar[0]  *= k;
    ar[1]  *= k;
    ar[2]  *= k;
    return *this;
}

inline vec3 unit_vector(vec3 vec) {
    return vec / vec.length();
}

#endif
