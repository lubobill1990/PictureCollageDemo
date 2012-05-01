#include <PictureCollage/Tests/Common.h>
#include <iostream>
float32 zoomScale=1;
void WindowPos(GLint xx,GLint yy)
{/*old method */
	GLint viewport[4];
	GLdouble modelview[16],projection[16];
	GLdouble x,y,z;
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);

	gluUnProject(xx,yy,0.5,modelview,projection,viewport,
		&x,&y,&z);
	glRasterPos3d(x,y,z);

}
b2Fixture *GetAnotherFixtureInSameBody(b2Fixture *fixture)
{
	b2Body *body=fixture->GetBody();
	for (b2Fixture *tmpFixture=body->GetFixtureList();tmpFixture!=NULL;tmpFixture=tmpFixture->GetNext())
	{
		if (tmpFixture!=fixture)
		{
			return tmpFixture;
		}
	}
	throw MsgException("Another fixture not exist");
}
/// Random floating point number in range [lo, hi]
inline float32 RandomFloat(float32 lo, float32 hi)
{
	float32 r = (float32)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}


using namespace std;
void PrintFixture(b2Fixture* fixture){
	cout<<((BodyData*)fixture->GetBody()->GetUserData())->m_Id<<" "<<(((FixtureData*)fixture->GetUserData())->GetFixtureType()==outer?"outer":"inner");
}
extern b2Vec2 ConvertWorldToScreen(float32 x,float32 y);
void DrawCoordinary(){
	for (float i=-40;i<=40;i+=2)
	{
		std::string str=Num2Str((int)i);
		b2Vec2 screen_posion_x=ConvertWorldToScreen(i,0.0);
		b2Vec2 screen_posion_y=ConvertWorldToScreen(0.0,i);
		DrawString(screen_posion_x.x,screen_posion_x.y,str.c_str());
		DrawString(screen_posion_y.x,screen_posion_y.y,str.c_str());
	}
}
void DrawString(int x, int y, const char *string, ...)
{
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	WindowPos(x,y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
bool TestLineIntersect(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float &InterX, float &InterY)
{ //两条线段是否相交X0X1 AND X1X2
	float x, y;
	float Minx01 = Min(x0, x1);
	float Miny01 = Min(y0, y1);
	float Minx23 = Min(x2, x3);
	float Miny23 = Min(y2, y3);
	float Maxx01 = Max(x0, x1);
	float Maxy01 = Max(y0, y1);
	float Maxx23 = Max(x2, x3);
	float Maxy23 = Max(y2, y3);

	if(x1!=x0 && x2!=x3)
	{
		float k1 = (y1-y0)/(x1-x0);
		float k2 = (y3-y2)/(x3-x2);
		float Den = (y1-y0)*(x3-x2) - (y3-y2)*(x1-x0);
		if(k1==k2)
		{ //平行不相交
			float d1 = abs(y0*(x1-x0)-x0*(y1-y0)-y2*(x3-x2)+x2*(y3-y2)); //距离公式d = abs(c1-c2) / sqrt(a*a+b*b)
			if(d1==0)
			{//直线重合
				if((x2>Minx01 && x2<Maxy01 && y2>Miny01 && y2<Maxy01) || (x3>Minx01 && x3<Maxy01 && y3>Miny01 && y3<Maxy01)
					|| (x0>Minx23 && x0<Maxy23 && y0>Miny23 && y0<Maxy23) || (x1>Minx23 && x1<Maxy23 && y1>Miny23 && y1<Maxy23))
				{  //实际碰撞问题线段重合认为相交了
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}   
		}
		x = ((y2-y0)*(x1-x0)*(x3-x2)+(y1-y0)*(x3-x2)*x0-(y3-y2)*(x1-x0)*x2)/Den;
		y = ((y1-y0)*(x-x0))/(x1-x0) + y0;

		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			InterX = x;
			InterY = y;
			return true;
		}
	}
	else if(x1==x0 && x2!=x3)
	{
		x = x0;
		y = ((y3-y2)*(x0-x2))/(x3-x2) + y2;
		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			InterX = x;
			InterY = y;
			return true;
		}
	}
	else if(x1!=x0 && x2==x3)
	{
		x = x2;
		y = ((y1-y0)*(x2-x0))/(x1-x0) + y0;
		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			InterX = x;
			InterY = y;
			return true;
		}       
	}
	return false;
}
