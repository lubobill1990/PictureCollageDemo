/*
 * Picture.cpp
 *
 *  Created on: Apr 27, 2012
 *      Author: lubo
 */

/* namespace bee */
#include "Picture.h"
#include <iostream>
#include <fstream>
namespace bee {
using namespace boost::filesystem;
using namespace boost;
using namespace std;

Picture::~Picture() {
	// TODO Auto-generated destructor stub
}
;
Picture::Picture() {

}
;

Picture::Picture(const json_spirit::Object &obj) {
	for (json_spirit::Object::size_type i = 0; i != obj.size(); ++i) {
		const json_spirit::Pair& jpair = obj[i];
		const string& name = jpair.name_;
		const json_spirit::Value& value = jpair.value_;

		if (name == "filePath") {
			this->filePath = value.get_str();
		} else if (name == "height") {
			this->height = value.get_int();
		} else if (name == "width") {
			this->width = value.get_int();
		} else if (name == "salientArea") {
			json_spirit::Array json_salient_array = value.get_array();

			vector<pair<int, int> > salientArea;
			int x, y;
			for (uint j = 0; j < json_salient_array.size(); ++j) {
				const json_spirit::Object &object =
						json_salient_array[j].get_obj();
				for (uint k = 0; k < object.size(); ++k) {
					const json_spirit::Pair& pair = object[k];
					const string& name = pair.name_;
					const json_spirit::Value& value = pair.value_;
					if (name == "x") {
						x = value.get_int();
					} else if (name == "y") {
						y = value.get_int();
					}
				}
				salientArea.push_back(std::make_pair(x, y));
			}
			this->salientArea = salientArea;
		}
	}
}
json_spirit::Object Picture::TransToJsonObject() {
	json_spirit::Object json_obj;
	json_obj.push_back(json_spirit::Pair("filePath", this->filePath));

	json_spirit::Array salientArea;
	for (uint i = 0; i < this->salientArea.size(); ++i) {
		json_spirit::Object point;
		point.push_back(json_spirit::Pair("x", this->salientArea[i].first));
		point.push_back(json_spirit::Pair("y", this->salientArea[i].second));
		salientArea.push_back(point);
	}
	json_obj.push_back(json_spirit::Pair("salientArea", salientArea));
	json_obj.push_back(json_spirit::Pair("width", this->width));
	json_obj.push_back(json_spirit::Pair("height", this->height));
	return json_obj;
}
}
