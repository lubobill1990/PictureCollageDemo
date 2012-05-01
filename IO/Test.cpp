#include "PictureIO.h"
using namespace bee;
int main() {
	PictureIO pi = PictureIO();
	vector<Picture> ret2 = pi.GetPictureVector("c:\\Users\\Lubo\\car");
	pi.WriteToJsonFile("c:\\Users\\Lubo\\a.json",ret2);
	vector<Picture> ret = pi.GetPictureArrayFromJsonFile("c:\\Users\\Lubo\\a.json");
	for (uint i = 0; i < ret.size(); ++i) {
		cout << "filename: " << ret[i].filePath << endl;
		for (uint j = 0; j < ret[i].salientArea.size(); ++j) {
			cout << "\t" << ret[i].salientArea[j].first << " "
					<< ret[i].salientArea[j].second << endl;
		}
	}

}
