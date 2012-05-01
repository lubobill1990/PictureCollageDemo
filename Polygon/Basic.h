#ifndef BASIC_H
#define BASIC_H
#include <Box2D/Common/b2Math.h>
#ifndef INFINITY
#define INFINITY 1000000.0
#endif
#define EPSILON 0.001   // Define your own tolerance
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))

#define Min(x,y) ((x)>(y)?(y):(x))
#define Max(x,y) ((x)<(y)?(y):(x))

typedef int int32;
typedef float float32;
typedef unsigned int uint;

inline bool IsFloatValid(float32 x)
{
	if (x != x)
	{
		// NaN.
		return false;
	}

	float32 infinity = std::numeric_limits<float32>::infinity();
	return -infinity < x && x < infinity;
}

namespace bee{

	class Transform:public b2Transform{
	public:
		Transform(b2Transform &tx){

		}
		Transform(){}
	};
	class Color{
	public:
		Color(){}
		Color(int32 _r,int32 _g,int32 _b):r(_r),g(_g),b(_b){}
		int32 r,g,b;
	};
	class Point{
	public:
		Point() {}
		Point(b2Vec2 vec):x(vec.x),y(vec.y){}
		Point(const float *vec):x(vec[0]),y(vec[1]){}
		/// 构造向量
		Point(float32 x, float32 y) : x(x), y(y) {}
		/// 设为0向量
		void SetZero() {
			x = 0.0f; 
			y = 0.0f; 
		}
		/// 设置向量
		void Set(float32 x_, float32 y_) {
			x = x_;
			y = y_;
		}

		/// 取负
		Point operator -() const {
			Point v;
			v.Set(-x, -y); return v; 
		}
		/// 相加
		Point operator +(const Point &point) const{
			return Point(this->x+point.x,this->y+point.y);
		}

		/// 乘法
		Point operator *(float32 scalar){
			return Point(this->x*scalar,this->y*scalar);
		}
		/// 除以
		Point operator /(float32 number){
			return Point(this->x/number,this->y/number);
		}
		/// 根据索引读出向量值
		float32 operator () (int32 i) const
		{
			return (&x)[i];
		}
		/// 根据索引写入向量，如point(1)=1.2;
		float32& operator () (int32 i)
		{
			return (&x)[i];
		}
		/// 加法
		void operator += (const Point& v)
		{
			x += v.x; y += v.y;
		}
		/// 减法
		void operator -= (const Point& v)
		{
			x -= v.x; y -= v.y;
		}
		/// 乘法
		void operator *= (float32 a)
		{
			x *= a; y *= a;
		}
		/// 除法
		void operator /= (float32 a){
			x/=a; y/=a;
		}
		/// 向量的长度
		float32 Length() const
		{
			return sqrt(x * x + y * y);
		}
		/// 向量长度的平方
		float32 LengthSquared() const
		{
			return x * x + y * y;
		}
		/// 这个函数会改变结构体中的xy值，使之符合一个单位向量长度为1的条件
		float32 Normalize()
		{
			float32 length = Length();
			if (length < EPSILON)
			{
				return 0.0f;
			}
			float32 invLength = 1.0f / length;
			x *= invLength;
			y *= invLength;

			return length;
		}
		/// 是否为合法向量，就是判断里面的float是否合法
		bool IsValid() const
		{
			return IsFloatValid(x) && IsFloatValid(y);
		}

		float32 x, y;
	};

	inline bool operator ==(const Point &point1,const Point &point2){
		return FLOAT_EQ(point1.x,point2.x)&&FLOAT_EQ(point1.y,point2.y);
	}
	inline bool operator !=(const Point &point1,const Point &point2){
		return !(point1==point2);
	}
	/// 相减
	inline Point operator -(const Point &point1,const Point &point2){
		return Point(point1.x-point2.x,point1.y-point2.y);
	}
};
#endif

