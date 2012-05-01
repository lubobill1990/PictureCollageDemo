#ifndef POLYGON_H
#define POLYGON_H
#include <Polygon/Common.h>
#include <algorithm>
namespace bee{
	class PolygonList;
	class Point;
	class Polygon;

	typedef enum _Position{IN_POLYGON=1,ON_POLYGON=2,OUT_POLYGON=4,NOT_ON_LINE=8,ON_LINE_ENDPOINT=0x000f,ON_LINE_NOT_ENDPOINT=0x0010,ON_LINE=0x001f} Position;
	typedef std::vector<Point> PointVector;

	class LineSegment
	{
	public:
		LineSegment(void);
		LineSegment(const LineSegment &line);
		LineSegment(float ax,float ay,float bx, float by);
		LineSegment(const Point &a,const Point &b);
		~LineSegment(void){};
		bee::Point start,end;
		const Point& GetStart() const;
		const Point& GetEnd() const;
		void SetStart(const Point &s);
		void SetEnd(const Point &e);
		///一点是否在线段上,包括端点
		bool TestPointOn(const Point &point) const;
		///一点是否在线段所在的直线上
		bool TestPointOnLine(const Point &point);
		/// 另一条线段是否在本线段上
		bool TestLineSegmentOn(const LineSegment &line);
		///测试两条线段是否相交,并且可以返回交点
		Position TestIntersect(const LineSegment &line,Point &interPoint=Point()) const;
		///获取线段斜率
		float32 GetSlope();
		///测试两条线段否共线
		bool TestColinear(const LineSegment &line);
		Point GetMidPoint() const;
		//使线段向量的方向逆转
		void Reverse();
		bool operator ==(const LineSegment &line);
		bool operator !=(const LineSegment &line);
	private:
		float32 e_slope;
		bool e_SlopeChanged;
	};

	class Polygon
	{
	public:
		Polygon(){
			this->e_HasValidated=false;
			this->e_HasTrimColinearEdges=false;
		}
		Polygon(float *points,int count,Transform &tx){
			Polygon();
			for (int i=0;i<count;++i)
			{
				Point tmp_point(points[i*2],points[i*2+1]);
				int j=(i+1)>=count?0:i+1;
				Point tmp_point2(points[j*2],points[2*j+1]);
				Point curVertice=GetWorldPoint(tmp_point,tx);
				Point nextvertice=GetWorldPoint(tmp_point2,tx);
				LineSegment *line=new LineSegment(curVertice,nextvertice);
				this->AddEdge(*line);
			}
		}
		Polygon(Point vertices[],int count,Transform &tx){
			Polygon();
			for (int i=0;i<count;++i)
			{
				Point curVertice=GetWorldPoint(vertices[i],tx);
				Point nextvertice=GetWorldPoint(vertices[i+1>=count?0:i+1],tx);
				LineSegment *line=new LineSegment(Point(curVertice),Point(nextvertice));
				this->AddEdge(*line);
			}
		}
		
		~Polygon(){};
		void AddEdge(const LineSegment &line){
			this->e_HasValidated=false;
			this->e_HasTrimColinearEdges=false;
			this->e_Edges.push_back(LineSegment(line));
		};
		void DelEdge(const LineSegment &line){
			this->e_HasValidated=false;
			this->e_HasTrimColinearEdges=false;
			this->e_Edges.erase(
				std::remove(this->e_Edges.begin(),this->e_Edges.end(),line),
				this->e_Edges.end());
		};
		LineSegment &operator[](uint i){
			return const_cast<LineSegment &>( (*const_cast<const Polygon *>(this))[i]);
		};
		const LineSegment &operator[](uint i) const{
			return this->e_Edges[i];
		};
		uint Size() const{
			return this->e_Edges.size();
		}
		/// 返回一个多边形,这个多边形的顶点包含被调用多边形的顶点和被调用多边形与另一个多边形的交点
		Polygon GetPolygonAddInterPointAsVertex(const Polygon &polygon) const;
		/// 调用这个函数前，多边形的每条边可能不是首尾相连，并且成逆时针排列的
		PolygonList Reconstruct();
		/// 判断点是否在多边形内
		Position IsPointIn(const Point &point) const;
		/// 获取两个多边形不相互交叠的部分
		PolygonList GetNotOverlapedArea(const Polygon &polygon) const;
		/// 获取两个多边形相互交叠的部分
		PolygonList GetOverlapdArea(const Polygon &polygon) const;
		/// 本多边形和另一个多边形合并
		PolygonList Merge(const Polygon &polygon) const;
		/// 本多边形减去所给多边形
		PolygonList Segment(const Polygon &polygon) const;
		/// 本多边形减去多个多边形
		PolygonList Segment(const PolygonList &polygons);
		/// flush表示是否画完之后要刷新显示,默认为是;clear表示在画前是否情况界面,默认为否;color为画的颜色,默认为白色 
		void Draw(int flush=1,int clear=0,const Color& color=Color(1,1,1)) const;
		/// 获得某条边的下一条边
		LineSegment& GetNextEdge(uint index){
			return const_cast<LineSegment &>(const_cast<const Polygon *>(this)->GetNextEdge(index));
		}
		const LineSegment& GetNextEdge(uint index) const{
			index=index%this->e_Edges.size();
			if (index+1==this->e_Edges.size())
			{
				return this->e_Edges[0];
			}else{
				return this->e_Edges[index+1];
			}
		}
	private:
		std::vector<LineSegment> e_Edges;
		bool e_HasValidated;
		bool e_HasTrimColinearEdges;
		LineSegment& GetTailEdge()
		{
			return this->e_Edges[this->e_Edges.size()-1];
		}
		LineSegment& GetHeadEdge()
		{
			return this->e_Edges[0];
		}

		/// 检查多边形，使多边形合法化
		/// 去掉长度为0的边
		void TrimZeroLengthEdges();
		/// 合并共线的边
		void MergeColinearEdges();
	};

	class PolygonList{
	public:
		PolygonList(){};
		~PolygonList(){};
		void AddPolygon(Polygon *polygon)
		{
			this->e_PolygonList.push_back(polygon);
		}
		void AddPolygon(const PolygonList & polygons)
		{
			for (uint i=0;i<polygons.Size();++i)
			{
				Polygon *tmp_poly=new Polygon(polygons[i]);
				this->e_PolygonList.push_back(tmp_poly);
			}
		}
		void DelPolygon(const Polygon *polygon){
			this->e_PolygonList.erase(std::remove(this->e_PolygonList.begin(),this->e_PolygonList.end(),polygon));
		}
		void Clear()
		{
			e_PolygonList.clear();
		}
		uint Size() const{
			return this->e_PolygonList.size();
		}
		Polygon & operator[](uint i){
			return const_cast<Polygon &>(const_cast<const PolygonList *>(this)->operator[](i));
		}
		const Polygon & operator[] (uint i) const{
			return *this->e_PolygonList[i];
		}
		PolygonList Merge();
		void Draw(int flush=1,int clear=0,Color color=Color(1,1,1)) const;
	private:
		std::vector<Polygon*> e_PolygonList;
	};
};
#endif
