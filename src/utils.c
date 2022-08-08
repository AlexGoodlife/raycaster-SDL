#include <math.h>

#include "../include/utils.h"


float distance(float ax, float ay, float bx, float by){
	return sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay));
}

float fixAngle(float angle){
	if(angle < 0){
		angle += 2*PI;
	}
	if(angle > 2*PI){
		angle -= 2*PI;
	}
	return angle;
}

float max(float x, float y){
	return x > y ? x : y;
}

float min(float x,float y){
	return x < y ? x : y;
}

