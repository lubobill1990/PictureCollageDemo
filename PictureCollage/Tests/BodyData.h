#pragma once
#include "Testbed/Framework/FileReader.h"
class BodyData
{
public:
	BodyData(int id){
		this->m_Id=id;
	}
	~BodyData();
	int m_Id;
	Image_File *m_Image;
protected:
private:
};
