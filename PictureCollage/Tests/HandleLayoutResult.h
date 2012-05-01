#include "common.h"
#include "Polygon.h"

VecVector ReorderVertices(VecVector vertices);

float GetOverlapArea(b2Fixture* fixtureA,b2Fixture* fixtureB);
std::vector<ShowAera> GetResultPicture(b2World *world);
void PostHandleContactFixture(b2World *world);
void AdjustAngle(b2World *world);
std::vector<bee::PolygonList> GetAreaToShow(b2World *world);
void DrawAreaToShow(b2World *world);