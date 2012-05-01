#include "Common.h"

//#define TEST_MERGE
//#define TEST_OVERLAP
//#define TEST_NOT_OVERLAP
#define TEST_SEGMENT

float points1[]={-15,15,15,15,15,-15,-15,-15};
float points2[]={-5,5,20,5,20,-25,-5,-25};
float points3[]={0,10,20,10,20,0,0,0};
float points4[]={-5,5,15,5,15,-5,-5,-5};
float points5[]={-15,5,15,5,15,-5,-15,-5};
float points6[]={-5,15,5,15,5,-15,-5,-15};
float points7[]={-13,3,10,3,10,-4,-13,-4};
float points8[]={-10,3,10,3,10,-5,-10,-5};
float points9[]={-20,5,25,5,25,-5,-20,-5};
float points10[]={-10,5,10,5,10,-10,-10,-10};
float points11[]={0,10,20,9,18,3,-4,-1};
float points12[]={0,10,15,9.25,25,5,-8,6};
float points13[]={-20,10,20,10,20,-10,-20,-10};
float points14[]={-20,10,1,10,1,-5,-20,-5};
float points15[]={-10,10,10,10,10,-5,-10,-5};
float points16[]={-10,10,10,10,10,-10,-10,-10};
float points17[]={-5,10,-8,-4,10,-10,13,4};
float points18[]={5,11,13,0,4,-11,-5,-1};
float points19[]={9,6,18,1,14,-9,4,-4};

float pointss1[]={-8,6,-4,-7,10,-4,5,8};
float pointss2[]={-2,-8,5,-10,10,7,4,9};
float pointss3[]={-2,4,6,-2,10,7,4,13};
float pointss4[]={-9,-4,-9,-14,3,-13,2,-3};

float pointcolinear[]={10,0,10,-10,0,-10,-10,-10,-5,0,10,10};

using namespace bee;
PolygonList polygons;

void local_display(){
	Transform tx;
	tx.SetIdentity();

	bee::Polygon poly1=bee::Polygon(points1,4,tx);
	bee::Polygon poly2=bee::Polygon(points2,4,tx);
	bee::Polygon poly3=bee::Polygon(points3,4,tx);
	bee::Polygon poly4=bee::Polygon(points4,4,tx);
	bee::Polygon poly5=bee::Polygon(points5,4,tx);
	bee::Polygon poly6=bee::Polygon(points6,4,tx);
	bee::Polygon poly7=bee::Polygon(points7,4,tx);
	bee::Polygon poly8=bee::Polygon(points8,4,tx);
	bee::Polygon poly9=bee::Polygon(points9,4,tx);
	bee::Polygon poly10=bee::Polygon(points10,4,tx);
	bee::Polygon poly11=bee::Polygon(points11,4,tx);
	bee::Polygon poly12=bee::Polygon(points12,4,tx);
	bee::Polygon poly13=bee::Polygon(points13,4,tx);
	bee::Polygon poly14=bee::Polygon(points14,4,tx);
	bee::Polygon poly15=bee::Polygon(points15,4,tx);
	bee::Polygon poly16=bee::Polygon(points16,4,tx);
	bee::Polygon poly17=bee::Polygon(points17,4,tx);
	bee::Polygon poly18=bee::Polygon(points18,4,tx);
	bee::Polygon poly19=bee::Polygon(points19,4,tx);

	bee::Polygon polys1=bee::Polygon(pointss1,4,tx);
	bee::Polygon polys2=bee::Polygon(pointss2,4,tx);
	bee::Polygon polys3=bee::Polygon(pointss3,4,tx);
	bee::Polygon polys4=bee::Polygon(pointss4,4,tx);

	bee::Polygon polycoline=bee::Polygon(pointcolinear,6,tx);


#ifdef TEST_SEGMENT
	PolygonList poly_list;
	poly_list.AddPolygon(&poly2);
	poly_list.AddPolygon(&poly3);
	polygons=poly1.Segment(poly_list);

	//polygons.Draw(1,1);

	poly17.Draw(0,1);
	poly18.Draw(0,0);
	poly19.Draw(1,0);
	poly_list.Clear();
	poly_list.AddPolygon(&poly18);
	poly_list.AddPolygon(&poly19);
	polygons=poly17.Segment(poly18);
	polygons.Draw(1,1);

	polygons=poly17.Segment(poly19);
	polygons.Draw(1,1);

	polygons=poly17.Segment(poly_list);
	polygons.Draw(1,1);

	polys1.Draw(0,1);
	polys2.Draw(0,0);
	polys3.Draw(0,0);
	polys4.Draw(1,0);

	poly_list.Clear();
	poly_list.AddPolygon(&polys2);
	poly_list.AddPolygon(&polys3);
	poly_list.AddPolygon(&polys4);
	polygons=polys1.Segment(poly_list);
	polygons.Draw(1,1);


#endif
#ifdef TEST_NOT_OVERLAP
	polygons=poly1.GetNotOverlapedArea(poly2);
	polygons.Draw(1,1);

	polygons=poly3.GetNotOverlapedArea(poly4);
	polygons.Draw(1,1);

	polygons=poly5.GetNotOverlapedArea(poly6);
	polygons.Draw(1,1);

	polygons=poly7.GetNotOverlapedArea(poly8);
	polygons.Draw(1,1);

	polygons=poly9.GetNotOverlapedArea(poly10);
	polygons.Draw(1,1);

polygons=poly11.GetNotOverlapedArea(poly12);
	polygons.Draw(1,1);

	polygons=poly13.GetNotOverlapedArea(poly14);
	polygons.Draw(1,1);

	polygons=poly15.GetNotOverlapedArea(poly16);
	polygons.Draw(1,1);
#endif
#ifdef TEST_OVERLAP
	polygons=poly1.GetOverlapdArea(poly2);
	polygons.Draw(1,1);

	polygons=poly3.GetOverlapdArea(poly4);
	polygons.Draw(1,1);

	polygons=poly5.GetOverlapdArea(poly6);
	polygons.Draw(1,1);

	polygons=poly7.GetOverlapdArea(poly8);
	polygons.Draw(1,1);

	polygons=poly9.GetOverlapdArea(poly10);
	polygons.Draw(1,1);

	polygons=poly11.GetOverlapdArea(poly12);
	polygons.Draw(1,1);

	polygons=poly13.GetOverlapdArea(poly14);
	polygons.Draw(1,1);

	polygons=poly13.GetOverlapdArea(poly15);
	polygons.Draw(1,1);

	polygons=poly13.GetOverlapdArea(poly16);
	polygons.Draw(1,1);

#endif


#if defined(TEST_MERGE)
	polygons=poly1.Merge(poly2);
	polygons.Draw(1,1);

	polygons=poly3.Merge(poly4);
	polygons.Draw(1,1);

	polygons=poly5.Merge(poly6);
	polygons.Draw(1,1);

	polygons=poly7.Merge(poly8);
	polygons.Draw(1,1);

	polygons=poly9.Merge(poly10);
	polygons.Draw(1,1);

	polygons=poly11.Merge(poly12);
	polygons.Draw(1,1);

	polygons=poly13.Merge(poly14);
	polygons.Draw(1,1);

	polygons=poly13.Merge(poly15);
	polygons.Draw(1,1);

	polygons=poly13.Merge(poly16);
	polygons.Draw(1,1);
#endif
}
void MyReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho (-50.0, 50.0, -50.0*(GLfloat)h/(GLfloat)w,
		50.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
	else
		glOrtho (-50.0*(GLfloat)w/(GLfloat)h,
		50.0*(GLfloat)w/(GLfloat)h, -50.0, 50.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 400);
	glutCreateWindow("Polygon test");
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(local_display);

	glutMainLoop();
}
