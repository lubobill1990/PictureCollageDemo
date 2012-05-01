#ifndef COMMON_H
#define COMMON_H
#include <Box2D/Box2D.h>
#include <Polygon/Polygon.h>
#include <freeglut/GL/glut.h>
#include <vector>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>
extern float32 zoomScale;
typedef unsigned int uint;
#define DEBUG_MODE 1
#define EPSILON 0.001   // Define your own tolerance
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#define	RAND_LIMIT	32767
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define Min(x,y) ((x)>(y)?(y):(x))
#define Max(x,y) ((x)<(y)?(y):(x))
#ifdef DEBUG_MODE
#define DEBUG_PRINT(x) std::cout<<__FILE__<<__LINE__<<(x)<<std::endl;
#else
#define DEBUG_PRINT(x) ;
#endif

inline b2Vec2 GetWorldPoint(const b2Vec2& local_point,const b2Transform &xf) 
{
	return b2Mul(xf, local_point);
}

#include "BodyData.h"

#include "FixtureData.h"
extern int texCount;
typedef std::vector<b2Vec2> VecVector;
typedef std::pair<b2Vec2,b2Vec2> VecPair;
typedef std::vector<VecPair> VecPairVector;
class ShowAera{
	b2Vec2 vertices[20];
	int vertics_count;
};
const int32 k_maxBodies = 2560;
std::vector<ShowAera> GetResultPicture(b2World *world);
b2Fixture *GetAnotherFixtureInSameBody(b2Fixture *fixture);
float32 testDistance(b2Fixture* fixtureA,b2Fixture *fixtureB);
inline float32 RandomFloat(float32 lo, float32 hi);
bool TestLineIntersect(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float &InterX, float &InterY);
float distance();
void PrintFixture(b2Fixture* fixture);
void DrawString(int x, int y, const char *string, ...);
void DrawCoordinary();


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
