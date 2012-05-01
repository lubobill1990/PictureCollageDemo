/*
 * Picture.h
 *
 *  Created on: Apr 27, 2012
 *      Author: lubo
 */

#ifndef PICTURE_H_
#define PICTURE_H_
#include <json_spirit/json_spirit.h>
#include <string>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <utility>
#include <Exception/Exception.hpp>
#include <ProjectCommon.h>

using namespace std;
#ifdef WIN32
#define DIR_SEP "\\"
#else
#define DIR_SEP "/"
#endif
namespace bee{
class Picture {
public:
	Picture();
	Picture(const json_spirit::Object &obj);
	virtual ~Picture();
	json_spirit::Object TransToJsonObject();
	string filePath;
	vector<pair<int, int> > salientArea;
	int width, height;
};
}
 /* namespace bee */
#endif /* PICTURE_H_ */
