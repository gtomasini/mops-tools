#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "daa_datatypes_icd/trackICD.h"
#include "trackLib\trackDisplay.h"

using namespace std;

//void trackDisplay(const trackICD::track &track);//external (trackLib)

int main(int argc, char **argv) {

	if (argc < 2) {
		std::cerr << "write: " << argv[0] << " inputBinarFileName" << std::endl;
		return -1;
	}

	ifstream iFile(argv[1], ios::in | ios::binary);

	if (!iFile.is_open()) {
		std::cerr << "can't open " << argv[1] << std::endl;
		return -1;
	}

	for (;!iFile.eof();) {
		char buffer[192];
		iFile.read( buffer, sizeof(buffer) );
		if (!iFile) {
			std::cerr << "warning: only " << iFile.gcount() << " bytes could be read.";
			continue;
		}
		cout << "\n****************************" 
				<< iFile.gcount()<<" bytes read*******"<<std::endl;
		trackICD::track track;

		trackICD::read_track(track, buffer);

		trackDisplay(track);
	}//for()

	iFile.close();
	return 0;
}
