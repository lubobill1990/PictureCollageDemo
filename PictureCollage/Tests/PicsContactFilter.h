#ifndef PIC_SCONTACT_FILTER_H
#define PIC_SCONTACT_FILTER_H

#include <Box2D/Box2D.h>
#include "FixtureData.h"
#include "HandleLayoutResult.h"
class PicsContactFilter:public b2ContactFilter
{
private:
	//碰撞是否发生在重要区域和画布的边界之间
	bool IsCollisionOccurBetweenBoundAndSalient(b2Fixture *fixtureA,b2Fixture *fixtureB);
	
	//根据自己定义的FixtureData中的信息，来判断是否应该碰撞了
	bool ShouldCollideAccordingToFixtureData(b2Fixture *fixtureA,b2Fixture *fixtureB);
	
public:
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	
};
#endif
