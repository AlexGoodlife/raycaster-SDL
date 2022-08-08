#ifndef UTILS_H
#define UTILS_H


#define PI 3.1415926535
#define PI2 PI/2
#define PI3 3*PI/2
#define DEGREE 0.0174533

float distance(float ax, float ay, float bx, float by, float angle);

float fixAngle(float angle);

float max(float x, float y);

float min(float x,float y);

#endif