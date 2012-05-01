/*
 * PictureIO.cpp
 *
 *  Created on: Apr 27, 2012
 *      Author: lubo
 */

#include "PictureIO.h"
#include <fstream>
namespace bee {
using namespace boost::filesystem;
using namespace boost;
using namespace std;
PictureIO::PictureIO() {
	// TODO Auto-generated constructor stub

}

PictureIO::~PictureIO() {
	// TODO Auto-generated destructor stub
}

vector<Picture> PictureIO::GetPictureVector(string dirname) {
	vector<Picture> ret_val;

	vector<string> data_file = findFiles(dirname, ".*\\.txt");
	if (data_file.size() == 0) {
		throw MsgException(
				"Data file does not exist in directory: " + dirname + "\n");
	}
	string datafilepath = data_file[0];
	ifstream inf(datafilepath.c_str());

	if (!inf) {
		throw MsgException(
				"file: " + datafilepath + " does not open successfully\n");
	} else {
		cout << "File " + datafilepath + " is opened successfully" << endl;
	}

	regex exp1("(\\w+)_(\\d+):(\\d+)");
	regex exp2("(\\d+),(\\d+)");
	string buffer;

	while (!inf.eof()) {
		std::getline(inf, buffer);
		if (buffer == "") {
			continue;
		}
		Picture new_pic_info;
		std::string::const_iterator start, end;
		match_results<string::const_iterator> what;

		start = buffer.begin();
		end = buffer.end();
		while (regex_search(start, end, what, exp1)) {
			new_pic_info.filePath = dirname + DIR_SEP + what[2] + ".jpg";
			// update search position:
			start = what[0].second;
		}
		std::getline(inf, buffer);
		start = buffer.begin();
		end = buffer.end();
		while (regex_search(start, end, what, exp2)) {
			new_pic_info.salientArea.push_back(
					std::make_pair(atoi(string(what[1]).c_str()),
							atoi(string(what[2]).c_str())));
			// update search position:
			start = what[0].second;
		}
		ret_val.push_back(new_pic_info);
	}
	return ret_val;
}

void PictureIO::WriteToJsonFile(string filename,
		vector<Picture> &pic_info_vector) {
	json_spirit::Array json_obj;
	for (uint i = 0; i < pic_info_vector.size(); ++i) {
		json_obj.push_back(pic_info_vector[i].TransToJsonObject());
	}
	ofstream ofs(filename.c_str());
	json_spirit::write_formatted(json_obj, ofs);
	ofs.close();
}
vector<Picture> PictureIO::GetPictureArrayFromJsonFile(string filename) {
	vector<Picture> ret_val;
	ifstream ifs(filename.c_str());
	json_spirit::Value value;
	json_spirit::read(ifs, value);
	const json_spirit::Array& pic_info_array = value.get_array();
	for (uint i = 0; i < pic_info_array.size(); ++i) {
		ret_val.push_back(Picture(pic_info_array[i].get_obj()));
	}
	ifs.close();
	return ret_val;
}
vector<string> findFiles(string dir_path, string reg_str) {
	vector<string> ret_val;
	path p(dir_path);
	regex reg_exp(reg_str);
	try {
		if (!exists(p)) {
			throw MsgException("Path: " + dir_path + " does not exist\n");
		} else if (!is_directory(p)) {
			throw MsgException("Path: " + dir_path + " is not a directory\n");
		} else {
			typedef vector<path> pathvec;
			pathvec v;
			copy(directory_iterator(p), directory_iterator(), back_inserter(v));
			for (pathvec::const_iterator it(v.begin()), it_end(v.end());
					it != it_end; ++it) {
				if (regex_match(it->string(), reg_exp)) {
					ret_val.push_back(it->string());
				}
			}
		}
	} catch (const filesystem_error& ex) {
		cout<<ex.what()<<endl;
	}

	return ret_val;
}
}
