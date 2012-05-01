#include "Polygon.h"
#include <list>
#include <algorithm>
namespace bee{

	Point f_start_point;
	bool f_Less(const Point &p1,const Point &p2){
		return (p1-f_start_point).LengthSquared()<(p2-f_start_point).LengthSquared();
	}

	void sort_point_vector(PointVector &p_v,const Point&compare_point){
		f_start_point=compare_point;
		std::sort(p_v.begin(),p_v.end(),f_Less);
	}
	//tested
	Position Polygon::IsPointIn( const Point &point ) const
	{
		int n=this->e_Edges.size();
		int count=0;
		LineSegment line;
		line.start=point;
		line.end.x=-INFINITY;
		line.end.y=point.y;

		for (int i=0;i<n;++i)
		{
			LineSegment edge=this->e_Edges[i];
			//如果点在多边形的边上
			if(edge.TestPointOn(point)){
				return ON_POLYGON;
			}
			//如果edge平行于x轴,则判断下一个edge
			if (FLOAT_EQ(edge.start.y,edge.end.y))
			{
				continue;
			}
			//如果多边形的顶点在测试射线上或者测试射线穿过了多边形的边,则加1
			if (line.TestPointOn(edge.start))
			{
				//避免重复计算该点
				if (edge.start.y>edge.end.y)
				{
					++count;
				}
			}else if (line.TestPointOn(edge.end))
			{
				//避免重复计算该点
				if (edge.end.y>edge.start.y)
				{
					++count;
				}
			}else if (line.TestIntersect(edge)==ON_LINE_NOT_ENDPOINT)
			{
				++count;
			}
		}
		if (count%2==1)
		{
			return IN_POLYGON;
		} 
		else
		{
			return OUT_POLYGON;
		}
	}
	//tested
	PolygonList Polygon::Reconstruct()
	{
		this->TrimZeroLengthEdges();
		//存放返回值
		PolygonList ret_val;
		//创建多边形边的指针的list,创建list是为了erase的效率
		std::list<LineSegment*> edge_list;
		for (uint i=0;i<this->Size();++i)
		{
			edge_list.push_back(&this->e_Edges[i]);
		}
		//当前重组的多边形
		Polygon *cur_polygon=new Polygon();
	
		//遍历一遍列表,加入多少条边,如果便利一次一条边都没有加入,说明所给的一系列边不能组成首尾相连的多边形
		int operate_time=0;
		for(std::list<LineSegment*>::iterator itr=edge_list.begin();
			edge_list.size()>0;){
			//如果itr到达边列表的结尾,则重新赋值itr到首部
			if (itr==edge_list.end())
			{
				if (operate_time==0)
				{
					//DEBUG_PRINT("需要重新组合的边的列表不能首位连接");
					throw Polygon(*this);
				}
				operate_time=0;
				itr=edge_list.begin();
				continue;
			}
			//如果目前重组的多边形还没有元素,则把当前指针所指的边塞入
			if (cur_polygon->e_Edges.size()==0)
			{
				cur_polygon->AddEdge(**itr);
				itr=edge_list.erase(itr);
				continue;
			}

			/// 如果当前itr代表边的开始点等于当前重组多边形最后一条边的结尾边的结束点,则:
			/// 1.把当前itr所表示的边插入当前重组多边形
			/// 2.从重组边列表中删除该itr
			/// 3.重新赋值itr
			Point a=(*itr)->GetStart(),b=cur_polygon->GetTailEdge().GetEnd();
			if ((*itr)->GetStart()==cur_polygon->GetTailEdge().GetEnd())
			{
				cur_polygon->AddEdge(**itr);
				itr=edge_list.erase(itr);
				++operate_time;//表示有过边的连接操作
			}else if ((*itr)->GetEnd()==cur_polygon->GetTailEdge().GetEnd())
			{
				LineSegment tmp_edge=**itr;
				tmp_edge.Reverse();
				cur_polygon->AddEdge(tmp_edge);
				itr=edge_list.erase(itr);
				++operate_time;
			}
			else{
				++itr;
			}
			//如果头尾相连,则说明当前正在重组的多边形重组完毕,将其插入返回多边形列表
			if (cur_polygon->GetHeadEdge().GetStart()==cur_polygon->GetTailEdge().GetEnd())
			{
				ret_val.AddPolygon(cur_polygon);
				cur_polygon=new Polygon();
				continue;
			}
		}
		/// 经过上述步骤得到的多边形，可能会有相邻边共线的可能，调用合并共线相邻边来解决
		for (uint i=0;i<ret_val.Size();++i)
		{
			ret_val[i].MergeColinearEdges();
		}
		return ret_val;
	}

	PolygonList Polygon::GetNotOverlapedArea(const  Polygon &polygon ) const
	{
		PolygonList ret_val;
		Polygon poly1=this->GetPolygonAddInterPointAsVertex(polygon);
		Polygon poly2=polygon.GetPolygonAddInterPointAsVertex(*this);
		//如果两个多边形没有交点
		if (poly1.Size()==this->Size()&&poly2.Size()==polygon.Size())
		{	
			if (this->IsPointIn(polygon[0].GetMidPoint())==IN_POLYGON)
			{
				//如果所给多边形在本多边形内，则本多边形只有所给多边形的部分被覆盖
				//由于目前不支持空心多边形，所以返回整个本多边形
				//TODO
				ret_val.AddPolygon(new Polygon(*this));
			}else if(polygon.IsPointIn((*this)[0].GetMidPoint())==IN_POLYGON)
			{
				//如果本多边形在所给多边形内，则本多边形没有不被覆盖的地方，所以返回为空

			}else{
				//剩余的情况为两多边形相离，则返回本多边形
				ret_val.AddPolygon(new Polygon(*this));
			}
			return ret_val;
		}
		//如果有交点，说明本多边形被所给多边形截取了一段
		//先取出本多边形在所给多边形外部的边，然后取出所给多边形在本多边形内部的边
		//最后把包含所有这些边的多边形调用Reconstruct，返回所有多边形
		Polygon edges;
		for (uint i=0;i<poly1.Size();++i)
		{
			Position pos=polygon.IsPointIn(poly1[i].GetMidPoint());
			if (pos==OUT_POLYGON)
			{
				edges.AddEdge(poly1[i]);
			}
		}
		for (uint i=0;i<poly2.Size();++i)
		{
			Position pos=this->IsPointIn(poly2[i].GetMidPoint());
			if (pos==IN_POLYGON)
			{
				LineSegment tmp_edge=poly2[i];
				tmp_edge.Reverse();
				edges.AddEdge(tmp_edge);
			}
		}
		ret_val=edges.Reconstruct();
		return ret_val;
	}

	PolygonList Polygon::GetOverlapdArea(const Polygon &polygon ) const
	{
		PolygonList ret_val;
		Polygon poly1=this->GetPolygonAddInterPointAsVertex(polygon);
		Polygon poly2=polygon.GetPolygonAddInterPointAsVertex(*this);
		//如果两个多边形没有交点,则返回两个多边形组合成的多边形列表
		if (poly1.Size()==this->Size()&&poly2.Size()==polygon.Size())
		{
			if (this->IsPointIn(polygon[0].GetMidPoint())==IN_POLYGON)
			{
				//如果所给的多边形在本多边形内，则返回所给的多边形
				ret_val.AddPolygon(new Polygon(polygon));
			}else if (polygon.IsPointIn((*this)[0].GetMidPoint())==IN_POLYGON)
			{
				//如果本多边形在所给多边形内，则返回本多边形
				ret_val.AddPolygon(new Polygon(*this));
			}else{
				//如果两个多边形相互没有关系，则返回两个多边形
				Polygon *p1=new Polygon(*this);
				Polygon *p2=new Polygon(polygon);
				ret_val.AddPolygon(p1);
				ret_val.AddPolygon(p2);
			}
			return ret_val;
		}
		//如果有交点,则外轮廓合并后返回
		Polygon edge_in_another;

		for (uint i=0;i<poly1.Size();++i)
		{
			Position pos=polygon.IsPointIn(poly1[i].GetMidPoint());
			
			if (pos==IN_POLYGON||/// poly1的边在poly2的里面，说明是要合并的边
				pos==ON_POLYGON)/// poly1的边在poly2的边上，说明是相同的边
			{
				edge_in_another.AddEdge(poly1[i]);
			}
		}

		for (uint i=0;i<poly2.Size();++i)
		{
			Position pos=this->IsPointIn(poly2[i].GetMidPoint());
			
			/// poly2的边的中点在poly1上，和前一个循环中poly1边的中点在poly2边上的情况相同，只需要加一次
			if (pos==IN_POLYGON)/// poly2的边的中点在poly1外面，说明是要合并到结果中的边
			{
				edge_in_another.AddEdge(poly2[i]);
			}
		}

		ret_val=edge_in_another.Reconstruct();
		return ret_val;
	}

	PolygonList Polygon::Merge(const Polygon &polygon) const
	{
		PolygonList ret_val;
		Polygon poly1=this->GetPolygonAddInterPointAsVertex(polygon);
		Polygon poly2=polygon.GetPolygonAddInterPointAsVertex(*this);
		//如果两个多边形没有交点,则返回两个多边形组合成的多边形列表
		if (poly1.Size()==this->Size()&&poly2.Size()==poly2.Size())
		{
			if (this->IsPointIn(polygon[0].GetMidPoint())==IN_POLYGON)
			{
				//如果所给的多边形在本多边形内，则返回本多边形
				ret_val.AddPolygon(new Polygon(*this));
			}else if (polygon.IsPointIn((*this)[0].GetMidPoint())==IN_POLYGON)
			{
				//如果本多边形在所给多边形内，则返回所给多边形
				ret_val.AddPolygon(new Polygon(polygon));
			}else{
				//如果两个多边形相互没有关系，则返回两个多边形
				Polygon *p1=new Polygon(*this);
				Polygon *p2=new Polygon(polygon);
				ret_val.AddPolygon(p1);
				ret_val.AddPolygon(p2);
			}
			return ret_val;
		}
		//如果有交点,则外轮廓合并后返回
		Polygon edge_out_another;

		for (uint i=0;i<poly1.Size();++i)
		{
			Position pos=polygon.IsPointIn(poly1[i].GetMidPoint());
			
			if (pos==OUT_POLYGON||/// poly1的边在poly2的外面，说明是要合并的边
				pos==ON_POLYGON)/// poly1的边在poly2的边上，说明是相同的边
			{
				edge_out_another.AddEdge(poly1[i]);
			}
		}

		for (uint i=0;i<poly2.Size();++i)
		{
			Position pos=this->IsPointIn(poly2[i].GetMidPoint());
			
			/// poly2的边的中点在poly1上，和前一个循环中poly1边的中点在poly2边上的情况相同，只需要加一次
			if (pos==OUT_POLYGON)/// poly2的边的中点在poly1外面，说明是要合并到结果中的边
			{
				edge_out_another.AddEdge(poly2[i]);
			}
		}

		ret_val=edge_out_another.Reconstruct();

		return ret_val;
	}

	PolygonList Polygon::Segment(const Polygon &polygon ) const
	{
		PolygonList ret_val;
		ret_val=this->GetNotOverlapedArea(polygon);
		return ret_val;
	}

	PolygonList Polygon::Segment(const PolygonList &polygons )
	{
		PolygonList ret_val;
		//需要被分割的多边形
		PolygonList segmented_polygons;
		segmented_polygons.AddPolygon(this);

		for (uint i=0;i<polygons.Size();++i)
		{
			ret_val.Clear();
			const Polygon &segmenter=polygons[i];
			for(uint j=0;j<segmented_polygons.Size();++j)
			{
				Polygon &tmp_poly=segmented_polygons[j];
				PolygonList tmp_ret=tmp_poly.Segment(segmenter);
				ret_val.AddPolygon(tmp_ret);
			}
			segmented_polygons=ret_val;
		}
		return ret_val;
	}

	void Polygon::Draw(int flush/*=1*/,int clear/*=0*/,const Color& color/*=b2Color(1,1,1)*/) const
	{
		if (clear==1)
		{
			glClear (GL_COLOR_BUFFER_BIT);
		}
		glColor4f(color.r, color.g, color.b, 1.0f);

		// 设置正面为填充模式
		glPolygonMode(GL_FRONT, GL_FILL); 
		// 设置反面为线形模式
		glPolygonMode(GL_BACK, GL_LINE); 

		// 设置逆时针绘制一个多边形
		glBegin(GL_LINE_LOOP);
		for (uint i=0;i<this->Size();++i)
		{
			glVertex2f((*this)[i].start.x,(*this)[i].start.y);
		}
		glEnd();

		glPointSize(4);//点的大小  
		glEnable(GL_POINT_SMOOTH);  

		glBegin(GL_POINTS);
		for (uint i=0;i<this->Size();++i)
		{
			glVertex2f((*this)[i].start.x,(*this)[i].start.y);
		}
		glEnd();
		if (flush)
		{
			glutSwapBuffers();
		}
	}
	/// 对于本多边形的每一条边,和参数多边形的每一条边进行测试
	/// 本多边形测试过后,会有一系列交点,对交点进行排序,生成边,加入到返回的多边形中
	Polygon Polygon::GetPolygonAddInterPointAsVertex( const Polygon &polygon ) const
	{
		Polygon ret_val;
		PointVector p_v;
		//对于被测试多边形的每一条边
		for (uint i=0;i<this->Size();++i)
		{
			LineSegment edge_a=(*this)[i];
			for (uint j=0;j<polygon.Size();++j)
			{
				LineSegment edge_b=polygon[j];
				// 如果测试的两条边是共线,则算没有交点,不要加如交点
				if (!edge_a.TestColinear(edge_b))
				{
					Point inter_point;
					// 如果有交点,并且交点不在端点上
					if (edge_a.TestIntersect(edge_b,inter_point)==ON_LINE_NOT_ENDPOINT)
					{
						p_v.push_back(inter_point);
					}else if(edge_a.TestIntersect(edge_b,inter_point)==ON_LINE_ENDPOINT){

						if (inter_point!=edge_a.start&&inter_point!=edge_a.end)
						{
							p_v.push_back(inter_point);
						}
					}
				}
			}
			// 如果有交点,则排序,并且构造边,插入
			if (p_v.size()!=0)
			{
				//将p_v内的值排序,使从前往后的点离被测试边的起始点越来越远
				sort_point_vector(p_v,edge_a.GetStart());
				//将被测试边的起始点和p_v中的第一个点构造一条边,并且插入返回的多边形
				LineSegment tmp_edge=LineSegment(edge_a.GetStart(),p_v[0]);
				ret_val.AddEdge(tmp_edge);
				//将被测试边的末尾点加入p_v中,使得添加边的过程在一个for循环中完成
				p_v.push_back(edge_a.GetEnd());
				for (uint k=1;k<p_v.size();++k)
				{
					tmp_edge=LineSegment(p_v[k-1],p_v[k]);
					ret_val.AddEdge(tmp_edge);
				}
				//清空
				p_v.clear();
			}else{
				// 如果没有交点,则直接插入该被测试边
				ret_val.AddEdge(edge_a);
			}
		}
		ret_val.TrimZeroLengthEdges();
		return ret_val;
	}
	//tested
	void Polygon::TrimZeroLengthEdges()
	{
		if (this->e_HasValidated==true)
		{
			return;
		}
		/// 1.去掉长度为0的边(即开头和结尾都相同的边)
		for (std::vector<LineSegment>::iterator itr=this->e_Edges.begin();
			itr!=e_Edges.end();)
		{
			//去掉起始点和终止点相同的边
			if (itr->start==itr->end)
			{
				itr=this->e_Edges.erase(itr);
			}else{
				++itr; //fix the bug. in the previous version, the iter increase even after one edge is erased from the polygon 
			}
		}		
		this->e_HasValidated=true;
	}
	//tested
	void Polygon::MergeColinearEdges()
	{
		if (this->e_HasTrimColinearEdges==true)
		{
			return;
		}
		/// 合并共线的边
		bool firstscan=true;
		for(std::vector<LineSegment>::iterator itr=this->e_Edges.begin();firstscan;)
		{
			std::vector<LineSegment>::iterator next_itr=itr+1;
			if(next_itr==this->e_Edges.end())
			{
				next_itr=this->e_Edges.begin();
				firstscan=false;
			}
			while(itr->TestColinear(*next_itr))
			{
				itr->end=next_itr->end;
				next_itr=this->e_Edges.erase(next_itr);
				if(next_itr==this->e_Edges.end()||!firstscan)
				{
					goto next_loop;
				}
			}
			++itr;
next_loop:
			;
		}
		this->e_HasTrimColinearEdges=true;
	}
}
