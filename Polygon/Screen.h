#include "b2Math.h"
#include <GL/glut.h>
int tx,ty,tw,th;
int32 width = 640;
int32 height = 480;
float32 viewZoom = 1.0f;
b2Vec2 viewCenter(0.0f, 20.0f);

//相对于左下角的坐标
b2Vec2 ConvertWorldToScreen(float32 x,float32 y){
	float32 ratio = float32(tw) / float32(th);
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom;

	b2Vec2 lower = viewCenter - extents;
	b2Vec2 upper = viewCenter + extents;
	b2Vec2 p;
	p.x=(x-lower.x)*float32(tw)/(upper.x-lower.x);
	p.y=(y-lower.y)*float32(th)/(upper.y-lower.y); return p;
}
extern void DrawString(int x, int y, const char *string, ...);
b2Vec2 ConvertScreenToWorld(int32 x, int32 y)
{
	float32 u = x / float32(tw);
	float32 v = (th - y) / float32(th);

	float32 ratio = float32(tw) / float32(th);
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom;

	b2Vec2 lower = viewCenter - extents;
	b2Vec2 upper = viewCenter + extents;

	b2Vec2 p;
	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
	return p;
}
