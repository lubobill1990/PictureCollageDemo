#ifndef TEXTURE_H
#define TEXTURE_H
#include "../Framework/FileReader.h"
#include <vector>
#include <string>
#include <Box2D/Dynamics/b2World.h>
using namespace std;

typedef class _TexImage{
public:
	unsigned char *imgData;
	int width,height;
	double coodin[8];	//四个顶点的坐标
	unsigned int texHandle;
	~_TexImage(){
		delete imgData;
	};
}TexImage;
//int texCount;
typedef vector<TexImage*> TexImagePV;
extern TexImagePV texImageVector;

typedef vector<string> stringv;
typedef auto_ptr<stringv> stringv_ptr;
extern stringv pics;
extern const char* dir;

stringv_ptr get_files(const char *dir,const char* file_type);

TexImage* getImage(const char *imgFilename);

void getCoodinary();

void getTextureImages();

void textureInit(void);

void display(b2World* m_world_from_polyshapes);
void display(Image_File* i_f,b2Vec2 vertices[]);
void reshape(int w, int h);

#endif