#include "PicsContactFilter.h"
#include "iostream"
using namespace std;
bool PicsContactFilter::IsCollisionOccurBetweenBoundAndSalient(b2Fixture *fixtureA,b2Fixture *fixtureB){
	FixtureData *fdA=(FixtureData*)fixtureA->GetUserData();
	FixtureData *fdB=(FixtureData*)fixtureB->GetUserData();
	return ((fdA->GetFixtureType()==inner&&fdB->GetFixtureType()==bound)||
		fdA->GetFixtureType()==bound&&fdB->GetFixtureType()==inner);
}

bool PicsContactFilter::ShouldCollideAccordingToFixtureData(b2Fixture *fixtureA,b2Fixture *fixtureB){

	FixtureData *fixDataA=(FixtureData*)fixtureA->GetUserData();
	FixtureData *fixDataB=(FixtureData*)fixtureB->GetUserData();
	if (fixDataA->GetFixtureType()==outer&&fixDataB->GetFixtureType()==outer)
	{
		return false;
	}
	b2Fixture* inner_fixture=fixDataA->GetFixtureType()==inner?fixtureA:fixtureB;
	b2Fixture* another_outer_fixture=fixDataA->GetFixtureType()==inner?fixtureB:fixtureA;
	FixtureData* inner_fixture_data=(FixtureData*)inner_fixture->GetUserData();
	//如果两个已经在fixtureData中的contactlist中,则返回true,表示可以相碰
	if (inner_fixture_data->HaveContactedFixture(another_outer_fixture))
	{
		return true;
	}else{
		//否则将相同body中的另一个outer_fixture加入另一个inner_fixture_data的contactlist中
		b2Fixture *outer_fixture=GetAnotherFixtureInSameBody(inner_fixture);
		b2Fixture *another_inner_fixture=GetAnotherFixtureInSameBody(another_outer_fixture);
		FixtureData* another_inner_fixture_data=(FixtureData*)another_inner_fixture->GetUserData();
		//如果还没加入
		if (!another_inner_fixture_data->HaveContactedFixture(outer_fixture))
		{
			//并且开始的一对的相交面积大于0
			float overlap_area=GetOverlapArea(inner_fixture,another_outer_fixture);
			if (overlap_area>0)
			{
				another_inner_fixture_data->AddContactedFixture(outer_fixture);
			}
		}
		return false;
	}
}

bool PicsContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	//bool collision_occur= b2ContactFilter::ShouldCollide(fixtureA,fixtureB);
	////如果按照AABB和mask来算，本来就不应该碰撞的，就不要再运行下面的复杂计算了
	//if (!collision_occur)
	//{
	//	return false;
	//}
	//碰撞如果发生在重要区域和边缘之间，则一定是能够碰撞的
	bool collisionOccurBetweenBoundAndSalient=IsCollisionOccurBetweenBoundAndSalient(fixtureA,fixtureB);
	if (collisionOccurBetweenBoundAndSalient==true)
	{
		return true;
	}
	//如果原始的检测表示已经碰撞，并且碰撞不是发生在重要区域和画布的边界之间 
	//经过fixtureData的判断表示应该碰撞了，则碰撞
	return ShouldCollideAccordingToFixtureData(fixtureA,fixtureB);
}