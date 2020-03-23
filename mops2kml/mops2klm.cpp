#include <string>
#include <filesystem>

#include "remixerLib\parseCsv.h"
#include "remixerLib\mops2track.h"

//WARNING: Compile with CPP17 flag
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
	try {

		//using daa_track	= trackICD::track;
		//using track_buff	= trackICD::track_buffer;

		// input to app
		std::string ofname=((argc < 3) ? "output.kml": argv[2]);
		std::string ofname2 = ((argc < 4) ? "output2.kml" : argv[3]);
		std::string ship = ((argc < 5) ? "own" : argv[4]);
		std::string date = ((argc < 6) ? "2017-01-01": argv[5]);
		date += "T";

		if (argc < 3) {
			std::cerr << "\nNot enough arguments provided, please write:\n"
				<< argv[0] << " inputXXX.csv outputKmlFilename.kml outputKmlFilename2.kml [int|own] [yyyy-mm-dd]" << std::endl;
			std::cerr << "\toutputFilename.kml has waypoint time (<when> tags)"<<std::endl;
			std::cerr << "\toutputFilename2.kml has not time at all (not <when> tags)" << std::endl;
			std::cerr << "\tdefault ship: " << ship << std::endl;
			std::cerr << "\tdefault date: " << date << std::endl;
			return -1;
		}

		std::cout << "\tship: " << ship << ", date:" << date << std::endl;
		std::string ifname = argv[1];

		auto icon = mops2track::KML_ARROW_ICON;
		auto color = mops2track::KML_OPACITY0 + mops2track::KML_BLUE_COLOR;//blue
		if (!ship.compare("int")){
			color = mops2track::KML_OPACITY0+mops2track::KML_RED_COLOR;//red
		}

		std::error_code ec;
		if (fs::exists(ofname, ec)) {
			std::cerr << ofname << " exists, please use another output file name or erase it." << std::endl;
			return -1;
		}

		mops2track::processOwnshipCsvFile( ifname, ofname, 
			icon, color, true, date);

		mops2track::processOwnshipCsvFile( ifname, ofname2,
			icon, color, false, date);
	}
	catch (...) {
		std::cerr << "There is an exception in the application!" << std::endl;
		return -1;
	}

}