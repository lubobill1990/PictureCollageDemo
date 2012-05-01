#ifndef COMMON_H
#define COMMON_H
#include <vector>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>
#include <Polygon/Basic.h>
#include <freeglut/GL/glut.h>

extern float32 zoomScale;
#define DEBUG_MODE 1
#define	RAND_LIMIT	32767
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#ifdef DEBUG_MODE
#define DEBUG_PRINT(x) std::cout<<__FILE__<<__LINE__<<(x)<<std::endl;
#else
#define DEBUG_PRINT(x) ;
#endif



inline bee::Point GetWorldPoint(const bee::Point& local_point,const bee::Transform &xf){
	return bee::Point(b2Mul(b2Transform(xf.position,xf.R),b2Vec2(local_point.x,local_point.y)));
}
#include "Polygon.h"

extern int texCount;

bool TestLineIntersect(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float &InterX, float &InterY);
float distance();
void DrawString(int x, int y, const char *string, ...);


template <class Type>
Type Str2Num(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;    
}

template <class Type>
std::string Num2Str(Type num){
	std::ostringstream oss;
	oss<<num;
	return oss.str();
}

#endif
