#include "FileReader.h"
#include <stdlib.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <direct.h>
#ifdef WIN32
#include <io.h>
#endif
#include <Box2D/Box2D.h>
#include <algorithm>
#include <string>
#include <boost/filesystem.hpp>
#include "deelx.h"

#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;
//重载坐标的减法函数
b2Vec2 operator-(coodin p1,coodin p2){
	b2Vec2 vec;
	vec.x=p1.first-p2.first;
	vec.y=p1.second-p2.second;
	return vec;
}
//移除vector中的指定位置元素
void vector_remove(coordin_v *cv_ptr,int pos){
	coordin_v::iterator coordin_v_itr=cv_ptr->begin();
	for (int i=0;i<pos&&coordin_v_itr!=cv_ptr->end();i++)
	{
		++coordin_v_itr;
	}
	cv_ptr->erase(coordin_v_itr);
}
//调整坐标向量使其符合Box2d对坐标走向为逆时针的凸多边形的要求
coordin_v modify_salient_area_location_convex(coordin_v cv){
	//把顺时针的cv改成逆时针的，以满足Box2d构造多边形fixture
	//for (int i=0;i<cv.size()/2;++i)
	//{
	//	coodin tmp=cv[cv.size()-1-i];
	//	cv[cv.size()-1-i]=cv[i];
	//	cv[i]=tmp;
	//}
	//把凹多边形的那些点去掉
	unsigned int i1=0;
	unsigned int i2=1;
	unsigned int i3=2;
	while(i1<cv.size())
	{
		i2=(i2)>=cv.size()?0:i2;
		i3=(i3)>=cv.size()?0:i3;
		b2Vec2 edge1=cv[i2]-cv[i1];
		b2Vec2 edge2=cv[i3]-cv[i2];
		double res=b2Cross(edge1,edge2);
		if (res>=0)
		{
			vector_remove(&cv,i2);
			--i1;
		}else{

		}
		++i1;
		++i2;
		++i3;
	}
	return cv;
}
//将多边形的顶点个数控制在8以内，以满足Box2d对构造凸多边形的顶点的要求
coordin_v modify_salient_area_location_vertex(coordin_v cv){
	coordin_v cv2;
	//确定原有顶点数和8的比例，确定隔几个点选取一个顶点
	double scalar=double(cv.size())/8;
	if (scalar<=1.0)
	{
		return cv;
	}else{
		scalar=ceil(scalar);
	}
	coordin_v::iterator cv_itr;
	int count=0;
	for (cv_itr=cv.begin();cv_itr!=cv.end();++cv_itr)
	{
		if (count>=scalar)
		{
			cv2.push_back(*cv_itr);
			count=0;
		}
		++count;
	}
	return cv2;
}

//修改框出重要区域的多边形，使其满足Box2d对多边形fixture的要求
void modify_salient_area_location(img_file_v *ifv){
	std::vector<Image_File>::iterator img_file_vec_itr; 
	for (img_file_vec_itr=ifv->begin();img_file_vec_itr!=ifv->end();++img_file_vec_itr)
	{
		img_file_vec_itr->salient_aera_location=modify_salient_area_location_vertex(img_file_vec_itr->salient_aera_location);
		img_file_vec_itr->salient_aera_location=modify_salient_area_location_convex(img_file_vec_itr->salient_aera_location);
	}
}
//在某文件目录里搜索形如file_type参数所指的所有文件，非递归
my_stringv* search_files(const char *dir,const char* file_type="*.*")
{
    my_stringv *file_name_v=new my_stringv();
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
                    file_name_v->push_back(it->string());
                }
            }
        }
    }catch(const filesystem_error& ex){
    }
    return file_name_v;
}
//获取指定文件图片的满足贴图要求的像素信息和高宽信息
unsigned char * get_image_data(const char *imgFilename,int &height,int &width){
    IplImage* img=cvLoadImage(imgFilename,1);
    if (img==NULL)
    {
        return NULL;
    }
    height=img->height;
    width=img->width;
    unsigned char *imgData;
    imgData=new unsigned char[img->height*img->width*4];	//分配imgData的空间

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            CvScalar s=cvGet2D(img,img->height-1-i,j);
            imgData[(i*img->width+j)*4+0]=s.val[2];
            imgData[(i*img->width+j)*4+1]=s.val[1];
            imgData[(i*img->width+j)*4+2]=s.val[0];
            imgData[(i*img->width+j)*4+3]=255;
        }
    }
    cvReleaseImage(&img);
    return imgData;
}
//这个文件中最主要的函数，获取图片文件以及贴图、构造重要区域所需要的全部信息
img_file_v* get_img_file_v(char * dir_path){
    img_file_v *imgInfoV=new img_file_v();
    //查找存放重要区域信息的文件
    my_stringv* file_names_v=search_files(dir_path,"*.txt");
    if (file_names_v->size()==0)
    {
        std::cout<<"Can not find txt file of salenct information in directory "<<dir_path<<std::endl;
        return imgInfoV;
    }else if (file_names_v->size()!=1)
    {
        std::cout<<"There are too many .txt file in directory "<<dir_path<<", make sure there are only one .txt file containing salient information in this directory."<<std::endl;
        return imgInfoV;
    }
    const char* file_name=file_names_v->at(0).c_str();
    std::ifstream fin(file_name);
    if (!fin)
    {
        std::cout<<"Can not open file "<<file_name<<std::endl;
        return imgInfoV;
    }
    char line[1024];
    while(!fin.eof())
    {
        Image_File ifi;
        fin.getline(line,1024);
        std::string line_str(line);

        int offset=line_str.find("_");
        int offset2=line_str.find(":");
        std::string img_file_name=line_str.substr(offset+1,offset2-offset-1);
        ifi.img_file_name=string(dir_path)+string("\\")+img_file_name+".jpg";
        fin.getline(line,1024);
        line_str.assign(line);
        for (int i=line_str.find("(");i!=std::string::npos;i=line_str.find("(",i))
        {
            ++i;
            std::string num1,num2;
            for (;;++i)
            {
                if (line_str[i]==' ')
                {
                    continue;
                }
                if (line_str[i]<='9'&&line_str[i]>='0')
                {
                    num1+=line_str[i];
                }else{
                    break;
                }
            }
            i=line_str.find(",",i);
            ++i;
            for (;;++i)
            {
                if (line_str[i]==' ')
                {
                    continue;
                }
                if (line_str[i]<='9'&&line_str[i]>='0')
                {
                    num2+=line_str[i];
                }else{
                    break;
                }
            }
            ifi.salient_aera_location.push_back(std::make_pair(atoi(num1.c_str()),atoi(num2.c_str())));
        }
        ifi.imgData=get_image_data(ifi.img_file_name.c_str(),ifi.height,ifi.width);
        if (ifi.imgData==NULL)
        {
            continue;
        }else{
            imgInfoV->push_back(ifi);
        }
    }
    modify_salient_area_location(imgInfoV);
    return imgInfoV;
}
