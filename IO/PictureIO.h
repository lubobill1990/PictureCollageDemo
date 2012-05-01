/*
 * PictureIO.h
 *
 *  Created on: Apr 27, 2012
 *      Author: lubo
 */

#ifndef PICTUREIO_H_
#define PICTUREIO_H_
#include "Picture.h"

namespace bee{
class PictureIO {
public:
	PictureIO();
	virtual ~PictureIO();
	vector<Picture> GetPictureVector(string dirname);
	void WriteToJsonFile(string filename, vector<Picture> &pic_info_vector);
	vector<Picture> GetPictureArrayFromJsonFile(string filename);
};

vector<string> findFiles(string dir_path, string reg_str);
}
#endif /* PICTUREIO_H_ */
