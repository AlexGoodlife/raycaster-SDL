#ifndef UTILS_H
#define UTILS_H

#define PI 3.1415926535
#define PI2 PI/2
#define PI3 3*PI/2
#define DEGREE 0.0174533

//Calculates euclidian distance between two points
float distance(float ax, float ay, float bx, float by);

// Fixes angle given, i.e if over 2PI, reset it back subracting 2PI and inverse for over 0
float fixAngle(float angle);

// Max between 2 floats
float max(float x, float y);

// Min between 2 floats
float min(float x,float y);

#endif