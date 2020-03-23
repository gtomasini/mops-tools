#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>

#include "daa_datatypes_icd/trackICD.h"

#include "trackLib/daa_iotools/daa_file.h"
//#include "trackLib/daa_iotools/daa_udp.h"

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

//using namespace std;

int main(int argc, char *argv[]){
    try{       

        //using daa_track	= trackICD::track;
        //using track_buff	= trackICD::track_buffer;

        // input to app
        std::string filename;
        std::string ip = "127.0.0.1";

        int port = 2200; // daa default port

        if (argc < 2) {
            std::cerr << "\n No arguments provided, please use: bin2udp filename [ip_address] [port]"
                << "\n example: binudp file.bin " << ip << " " << port << "\n\n";
			return -1;
        }
        else {
            filename = argv[1];
            if (argc < 3)
                std::cerr << "\n no IP provided using default 127.0.0.1\n";
            else {
                ip = argv[2];
                if (argc < 4)
                    std::cerr << "\n no port provided, using 5400\n";
                else
                    port = atoi(argv[3]);
            }
        }

        // read file and store buffers
        const auto& all_buffers = daa::io::file::read<trackICD::track_buffer>(filename);
        std::cout << "\n read " << all_buffers.size() << " track_buffer elements\n";

        // translate buffers into data structures
        std::vector<trackICD::track> all_tracks;
        for (const auto& trk_buffer : all_buffers) {
            //trackICD::track_buffer my_buffer;
            trackICD::track current_track;

            trackICD::read_track(current_track, trk_buffer.data());
            all_tracks.push_back(current_track);
        }

        // print some global information on the track scenario     
        if (all_tracks.size() == 0){
            std::cerr << "\nNo tracks in the file";
            return -1;
        }

        std::cout << "\n read " << all_tracks.size() << " track_buffer"
            << "\nfirst time [s] is " << all_tracks.front().track_time
            << "\nlast time [s] is " << all_tracks.back().track_time
            << "\nduration [s] is " << all_tracks.back().track_time - all_tracks.front().track_time;
    
        // output each package though UDP
        //daa::net::udp<trackICD::track_buffer> my_udp(ip, port);
		Poco::Net::SocketAddress sa(ip, port);

		Poco::Net::DatagramSocket dgs;
		dgs.connect(sa);
        std::cout << "\n sending track through " << ip << ":" << port << "\n";

        for (const auto& trk : all_tracks){            
            using namespace std;
            std::this_thread::sleep_for(1s);//configurable!            

			trackICD::track_buffer my_buffer;
            trackICD::write_track(trk, my_buffer);
            
            std::cout << "\n sending track: " << trk.track_id << " track time [s]: " << trk.track_time << std::endl;
			
            //my_udp.send(my_buffer);                                   
			dgs.sendBytes(&my_buffer, sizeof(my_buffer));
        }        
        return 0;
    }
    catch (...) {
        std::cerr << "There is an exception in the application!" << std::endl;
        return -1;
    }

    return 0;
}

