#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class Image_File;
typedef std::pair<int,int> coodin;
typedef std::vector<coodin> coordin_v;
typedef std::vector<Image_File> img_file_v;
typedef std::vector<std::string> my_stringv;
typedef std::auto_ptr<my_stringv> my_stringv_ptr;

my_stringv* search_files(const char *dir,const char* file_type);

img_file_v* get_img_file_v(char * dir_path);

class Image_File
{
public:
	Image_File(){}
	~Image_File(){}

	unsigned int texId;
	int width,height;
	unsigned char *imgData;
	std::string img_file_name;
	int img_size[2];
	std::vector<coodin> salient_aera_location;
	double coodina[8];//纹理贴图的四个顶点
};
#endif
