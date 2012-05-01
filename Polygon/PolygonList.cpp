#include "Polygon.h"
#include <algorithm>
namespace bee{

	PolygonList PolygonList::Merge()
	{
		PolygonList ret_val;
		return ret_val;
	}


	void PolygonList::Draw(int flush,int clear,Color color) const
	{
	
		if (clear==1)
		{
			glClear (GL_COLOR_BUFFER_BIT);
		}
		glColor4f(color.r, color.g, color.b, 1.0f);

		for (uint i=0;i<this->Size();++i)
		{
			(*this)[i].Draw(0,0,color);
		}
		if (flush==1)
		{
			glutSwapBuffers();
		}
	}

}
