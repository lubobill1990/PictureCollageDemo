/************************************************************************/
/* 本文件的要求：
读入要被贴图的图片，通过TexImage存储，都放入TexImage的*/
/************************************************************************/

#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <fstream>
#include "Texture.h"
#include <freeglut/GL/glut.h>
#include "../Framework/deelx.h"
#include <boost/filesystem.hpp>
#ifdef GL_VERSION_1_1

const char *dir="d:\\pics";
TexImagePV texImageVector;
stringv pics;
int texCount=0;
using namespace std;
using namespace boost::filesystem;
// 获取目录文件名，并存在二维数组里面
stringv_ptr get_files(const char *dir,const char* file_type="*.*")
{
    stringv_ptr fileNames(new stringv());

    path p (dir);
    try{
        if(exists(p)&&is_directory(p)){
            CRegexpT <char> regexp(file_type);
            typedef vector<path> pathvec;
            pathvec v;
            copy(directory_iterator(p),directory_iterator(),back_inserter(v));
            for(pathvec::const_iterator it(v.begin()),it_end(v.end());it!=it_end;++it){
                MatchResult result=regexp.MatchExact(it->string().c_str());
                if(result.IsMatched()){
                    fileNames->push_back(it->string());
                }
            }
        }
    }catch(const filesystem_error& ex){
    }
    return fileNames;
}

//读出图片
TexImage* getImage(const char *imgFilename){
    IplImage* img=cvLoadImage(imgFilename,1);

    TexImage* ti=new TexImage();
    ti->imgData=new GLubyte[img->height*img->width*4];	//分配imgData的空间
    ti->height=img->height;
    ti->width=img->width;

    int i, j;
    for (i = 0; i < ti->height; i++) {
        for (j = 0; j < ti->width; j++) {
            CvScalar s=cvGet2D(img,ti->height-1-i,j);
            ti->imgData[(i*ti->width+j)*4+0]=s.val[2];
            ti->imgData[(i*ti->width+j)*4+1]=s.val[1];
            ti->imgData[(i*ti->width+j)*4+2]=s.val[0];
            ti->imgData[(i*ti->width+j)*4+3]=255;
        }
    }
    cvReleaseImage(&img);
    return ti;
}

//设定坐标
//void getCoodinary(){
//	int tmp;
//	fstream fin(dir+string("\\for_paste.txt"));
//	for (int i=0;i<texImageVector.size();i++)
//	{
//		for (int j=0;j<8;j++)
//		{
//			fin>>tmp;
//			texImageVector[i]->coodin[j]=(double)tmp/100-2;
//		}
//	}
//}

void getTextureImages(){
    stringv_ptr svp=get_files(dir,".*\\.jpg");
    for (int i=0;i< (svp->size());i++)
    {
        string file_path=dir+string("\\")+(*svp.get())[i];
        TexImage* ti=getImage(file_path.data());
        texImageVector.push_back(ti);
    }
}

void textureInit(void)
{    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    int texSize=100;
    GLuint texHandle[100];
    glGenTextures(texSize,texHandle);
    for (int i=0;i<texImageVector.size();i++)
    {
        texImageVector[i]->texHandle=texHandle[i];
        glBindTexture(GL_TEXTURE_2D,texImageVector[i]->texHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageVector[i]->width,
                texImageVector[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                texImageVector[i]->imgData);
    }
    glEnable(GL_TEXTURE_2D);
}

void display(b2World* m_world_from_polyshapes)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    for (int i=0;i<texCount;i++)
    {
        glBindTexture(GL_TEXTURE_2D, texImageVector[i]->texHandle);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(texImageVector[i]->coodin[0],texImageVector[i]->coodin[1] , 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(texImageVector[i]->coodin[2],texImageVector[i]->coodin[3], 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(texImageVector[i]->coodin[4],texImageVector[i]->coodin[5], 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(texImageVector[i]->coodin[6],texImageVector[i]->coodin[7], 0.0);
        glEnd();
        glFlush();
    }
    glDisable(GL_TEXTURE_2D);
}

void display(Image_File* i_f,b2Vec2 vertices[]){
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D,i_f->texId);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(vertices[0].x,vertices[0].y, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(vertices[1].x,vertices[1].y, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(vertices[2].x,vertices[2].y, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(vertices[3].x,vertices[3].y, 0.0);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

//
//int main(int argc, char** argv)
//{
//	getImages();
//	getCoodinary();
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
//	glutInitWindowSize(250, 250);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow(argv[0]);
//	init();
//	glutReshapeFunc(reshape);
//	glutDisplayFunc(display);
//	glutKeyboardFunc (keyboard);
//	glutMainLoop();
//	return 0; 
//}
//#else
//int main(int argc, char** argv)
//{
//	fprintf (stderr, "This program demonstrates a feature which is not in OpenGL Version 1.0.\n");
//	fprintf (stderr, "If your implementation of OpenGL Version 1.0 has the right extensions,\n");
//	fprintf (stderr, "you may be able to modify this program to make it run.\n");
//	return 0;
//}
#endif
//
