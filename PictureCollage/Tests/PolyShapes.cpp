#include "PolyShapes.h"
#include <iostream>
#define SCALAR ((float32)50)
#include <GL/glut.h>

float32 testDistance(b2Fixture* fixtureA,b2Fixture *fixtureB)
{
	b2DistanceInput input;
	input.proxyA.Set(fixtureA->GetShape());
	input.proxyB.Set(fixtureB->GetShape());
	input.transformA = fixtureA->GetBody()->GetTransform();
	input.transformB = fixtureB->GetBody()->GetTransform();
	input.useRadii = true;
	b2SimplexCache cache;
	cache.count = 0;
	b2DistanceOutput output;
	b2Distance(&output, &cache, &input);
	return output.distance;
}

PolyShapes::PolyShapes()
{
	this->body_num=0;
	ifv=get_img_file_v("d:\\pics");
	// Ground body
	{
		//画最初的背景
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);
		b2PolygonShape shape;
		shape.SetAsBox(2.5,30,b2Vec2(-24,19),0);
		//shape.SetAsEdge(b2Vec2(-21.5f, -5.0f), b2Vec2(21.5f, -5.0f));
		ground->CreateFixture(&shape, 0.0f);
		//shape.SetAsEdge(b2Vec2(-21.5f, 43.0f), b2Vec2(-21.5f, -5.0f));
		shape.SetAsBox(30,2.5,b2Vec2(0,-7.5),0);
		ground->CreateFixture(&shape, 0.0f);
		//shape.SetAsEdge(b2Vec2(21.5f, 43.0f), b2Vec2(21.5f, -5.0f));
		shape.SetAsBox(2.5,30,b2Vec2(24,19),0);
		ground->CreateFixture(&shape, 0.0f);
		//shape.SetAsEdge(b2Vec2(-21.5f, 43.0f), b2Vec2(21.5f, 43.0f));
		shape.SetAsBox(30,2.5,b2Vec2(0,45.5),0);
		ground->CreateFixture(&shape, 0.0f);
		for (b2Fixture* f = ground->GetFixtureList(); f; f = f->GetNext())
		{
			b2Filter b=f->GetFilterData();
			f->SetUserData(new FixtureData(bound));
			b.categoryBits=bound;
			b.maskBits=inner;
			f->SetFilterData(b);
		}
	}
	m_world->SetGravity(b2Vec2(0,-5));
	m_bodyIndex = 0;
	memset(m_bodies, 0, sizeof(m_bodies));
	//m_world->SetContactListener(new PicsCollisionCallback());
	m_world->SetContactFilter(new PicsContactFilter());

}
void PolyShapes::textureInit(Image_File *image_file)
{    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1,&image_file->texId);

	glBindTexture(GL_TEXTURE_2D,image_file->texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_file->width,
		image_file->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		image_file->imgData);

	glEnable(GL_TEXTURE_2D);
}



extern b2Vec2 ConvertWorldToScreen(float32 x,float32 y);
extern b2Vec2 ConvertScreenToWorld(int x,int y);
void DrawBodyId(b2World*world )
{
	for (b2Body* body=world->GetBodyList();body;body=body->GetNext())
	{
		if (body->GetType()==b2_staticBody)
		{
			return;
		}
		int body_id=((BodyData*)body->GetUserData())->m_Id;
		std::string str_id=Num2Str(body_id).c_str();
		const char *char_id=str_id.c_str();
		b2Fixture*fixture=body->GetFixtureList()->GetNext();
		b2AABB ab=fixture->GetAABB();

		b2Vec2 screenPosition=ConvertWorldToScreen((ab.lowerBound.x+ab.upperBound.x)/2,(ab.lowerBound.y+ab.upperBound.y)/2);
		DrawString(screenPosition.x,screenPosition.y,char_id);
	}
}

void PolyShapes::Step(Settings* settings)
{
	//首先调用在每一步中，每个test都要做的事情
	Test::Step(settings);
	texCount=0;
	int texImgCount=0;
	DrawBodyId(m_world);
	//画xy轴坐标刻度
	DrawCoordinary();
	//去掉不相接触的fixture之间的user_data中的contact关系	
	PostHandleContactFixture(m_world);
	//规定角度
	AdjustAngle(m_world);

#ifdef USE_TEXTURE
		for (b2Body *body=m_world->GetBodyList();body;body=body->GetNext())
		{

			Image_File *i_f=((Image_File*)(body->GetUserData()));

			b2Vec2 vertices[b2_maxPolygonVertices];
			for (b2Fixture *fixture=body->GetFixtureList();fixture;fixture=fixture->GetNext())
			{
				if (fixture->GetFilterData().categoryBits==outer)
				{//以下就是找出当前body的纹理坐标
					const b2Transform& xf = fixture->GetBody()->GetTransform();
					b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
					int32 vertexCount = poly->m_vertexCount;
					b2Assert(vertexCount <= b2_maxPolygonVertices);

					for (int32 i = 0; i < vertexCount; ++i)
					{
						vertices[i] = b2Mul(xf, poly->m_vertices[i]);
					}
					display(i_f,vertices);
					break;
				} 
			}

		}

#endif
}

void PolyShapes::Create(int32 index)
{
	if (this->body_num>=34)
	{
		return;
	}
	++this->body_num;
	//当前bodyIndex上是否已经有了body，如果有的话，则消除已有body，空出位置
	//用于body个数超出最大限制时
	if (m_bodies[m_bodyIndex] != NULL)
	{
		m_world->DestroyBody(m_bodies[m_bodyIndex]);
		m_bodies[m_bodyIndex] = NULL;
	}
	Image_File &image_file=(*ifv)[m_bodyIndex];
	////////创建和初始化Body//////////////////////////////////////////////
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	float32 x = RandomFloat(-5.0f, 5.0f);
	float32 y=RandomFloat(3.0f,10.0f);
	bd.position.Set(x, 20);
	bd.angle = RandomFloat(-50.0f,50.0f)/180*b2_pi;

	m_bodies[m_bodyIndex] = m_world->CreateBody(&bd);
	/////////组成Body的图形////////////////////////////////////////////////
	b2FixtureDef outer_bound;
	outer_bound.restitution=0.1f;
	outer_bound.userData=new FixtureData(outer);
	b2Vec2 vertices[4];

	vertices[0].Set(0,0);
	vertices[1].Set(image_file.width/SCALAR,0);
	vertices[2].Set(image_file.width/SCALAR,image_file.height/SCALAR);
	vertices[3].Set(0,image_file.height/SCALAR);

	b2PolygonShape m_polygons2;
	m_polygons2.Set(vertices,4);
	outer_bound.shape=&m_polygons2;
	outer_bound.density=1.0f;
	outer_bound.friction=0.0f;
	///////设置图形之间的可碰撞关系/////////////////////////////////////////
	outer_bound.filter.categoryBits=outer;
	outer_bound.filter.maskBits=inner;

	b2FixtureDef salent_bound;

	salent_bound.userData=new FixtureData(inner);
	b2Vec2 salient_vertices[8];
	for (unsigned int i=0;i<image_file.salient_aera_location.size();i++)
	{
		//因为给的文件是以右下角为原点的，所以需要转换点坐标
		salient_vertices[i].Set((image_file.salient_aera_location[i].first)/SCALAR,
			(image_file.height-image_file.salient_aera_location[i].second)/SCALAR);
	}
	b2PolygonShape m_polygons3;
	m_polygons3.Set(salient_vertices,image_file.salient_aera_location.size());
	salent_bound.shape=&m_polygons3;
	salent_bound.density = 1.0f;
	salent_bound.friction = 0.0f;
	///////设置图形之间的可碰撞关系/////////////////////////////////////////
	salent_bound.filter.categoryBits=inner;
	salent_bound.filter.maskBits=outer|bound;

	b2Fixture* outerFixture=m_bodies[m_bodyIndex]->CreateFixture(&outer_bound);//把图形拼到Body上去
	b2Fixture* salientFixture=m_bodies[m_bodyIndex]->CreateFixture(&salent_bound);
	((FixtureData*)outerFixture->GetUserData())->SetFixture(outerFixture);
	((FixtureData*)salientFixture->GetUserData())->SetFixture(salientFixture);
	m_bodies[m_bodyIndex]->SetUserData(new BodyData(this->body_num));
	BodyData *bodyData=new BodyData(this->body_num);
	bodyData->m_Image=&image_file;
	m_bodies[m_bodyIndex]->SetUserData(bodyData);
#ifdef USE_TEXTURE
	textureInit(&image_file);
#endif

	m_bodyIndex = (m_bodyIndex + 1) % k_maxBodies;
}
