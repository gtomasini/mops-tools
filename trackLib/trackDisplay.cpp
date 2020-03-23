#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>

#include "trackDisplay.h"

using namespace std;

template <typename T>
inline void displayMag(const string &title, bool flag, T value) {
	cout << setprecision(12);
	cout << title;//  "TOA...................:\t";
	if (!flag) cout << "NOT available.";
	else cout << value;// track.aircraft_state.toa;
	cout << endl;
}

//ownship
void
stateDisplay(const ownship_stateICD::aircraft_state &state) {
	static unsigned ii = 0;
	cout << "OWNSHIP Packet (#" << ii++ << ")" << endl;
	displayMag("toa....................:\t", state.available_states.TOA,
		state.toa);
	displayMag("latitude...............:\t", state.available_states.LATITUDE, 
		state.latitude);
	displayMag("longitude:.............:\t", state.available_states.LONGITUDE, 
		state.longitude);
	displayMag("geom altitude(m).......:\t", state.available_states.GEOMETRIC_ALTITUDE, 
		state.geometric_altitude);
	displayMag("barom altitude(m)......:\t", state.available_states.BAROMETRIC_ALTITUDE, 
		state.barometric_altitude);
	displayMag("true heading...........:\t", state.available_states.TRUE_HEADING, 
		state.true_heading);
	displayMag("magnetic heading.......:\t", state.available_states.MAGNETIC_HEADING, 
		state.magnetic_heading);
	displayMag("ground track angle.....:\t", state.available_states.GROUND_TRACK_ANGLE, 
		state.ground_track_angle);
	displayMag("true air speed.........:\t", state.available_states.TRUE_AIRSPEED, 
		state.true_air_speed);
	displayMag("indicated air speed....:\t", state.available_states.INDICATED_AISPEED, 
		state.indicated_air_speed);
	displayMag("ground speed north(mps):\t", state.available_states.GROUND_SPEED_NORTH, 
		state.ground_speed_north);
	displayMag("ground speed east(mps).:\t", state.available_states.GROUND_SPEED_EAST, 
		state.ground_speed_east);
	displayMag("geometric vertical rate:\t", state.available_states.GEOMETRIC_VERTICAL_RATE, 
		state.geometric_vertical_rate);
	displayMag("pressure vertical rate.:\t", state.available_states.PRESSURE_VERTICAL_RATE, 
		state.pressure_vertical_rate);
	displayMag("heading rate...........:\t", state.available_states.HEADING_RATE,
		state.heading_rate);
	displayMag("ground track rate......:\t", state.available_states.GROUND_TRACK_RATE, 
		state.ground_track_rate);
	displayMag("mode...................:\t", state.available_states.MODE, 
		state.mode);
}

//intruder
void 
trackDisplay(const trackICD::track &track) {
	static unsigned ii = 0;
	cout << "INTRUDER Packet (#" << ii++ << ")" << endl;
	cout << "Track ID..............:\t" << track.track_id << endl;
	cout << "Callsign..............:\t" << track.callsign << endl;
	cout << "Track Time............:\t" << track.track_time << endl;
	cout << "Aircraft Category.....:\t" << track.aircraft_category << endl;
	cout << "Aircraft Model........:\t" << track.aircraft_model << endl;
	cout << "Data Source...........:\t";
	if (track.source_data.ADSB_TRACK)			cout << "ADSB TRACK.";
	if (track.source_data.UNFUSED_TRACK)		cout << "UNFUSED TRACK.";
	if (track.source_data.FUSED_TRACK)			cout << "FUSED TRACK.";
	if (track.source_data.EO_IR_CAMERA_TRACK) 	cout << "EO IR CAMERA.";
	if (track.source_data.ACAS_TRACK) 			cout << "ACAS_TRACK.";
	if (track.source_data.RADAR_TRACK)			cout << "RADAR TRACK.";
	if (track.source_data.LIDAR)				cout << "LIDAR.";
	if (track.source_data.ACOUSTIC)				cout << "ACOUSTIC.";
	if (track.source_data.UNKNOWN_SOURCE)		cout << "UNKNOWN SOURCE";
	cout << endl;

	//aircraft state
	cout << std::fixed;
	cout << std::setprecision(8);
	displayMag("TOA...................:\t",
		track.aircraft_state.available_states.TOA, track.aircraft_state.toa);
	displayMag("Latitude..............:\t",
		track.aircraft_state.available_states.LATITUDE, track.aircraft_state.latitude);
	displayMag("Longitude.............:\t",
		track.aircraft_state.available_states.LONGITUDE, track.aircraft_state.longitude);
	displayMag("Geom Altitude.........:\t",
		track.aircraft_state.available_states.GEOMETRIC_ALTITUDE, track.aircraft_state.geometric_altitude);
	displayMag("Barom Altitude........:\t",
		track.aircraft_state.available_states.BAROMETRIC_ALTITUDE, track.aircraft_state.barometric_altitude);
	displayMag("Ground Speed East.....:\t",
		track.aircraft_state.available_states.GROUND_SPEED_EAST, track.aircraft_state.ground_speed_east);
	displayMag("Ground Speed North....:\t",
		track.aircraft_state.available_states.GROUND_SPEED_NORTH, track.aircraft_state.ground_speed_north);
	displayMag("Geom Vertical Rate....:\t",
		track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE, track.aircraft_state.geometric_vertical_rate);
	displayMag("True Heading..........:\t",
		track.aircraft_state.available_states.TRUE_HEADING, track.aircraft_state.true_heading);
	displayMag("Ground Track Angle....:\t",
		track.aircraft_state.available_states.GROUND_TRACK_ANGLE, track.aircraft_state.ground_track_angle);

	cout << "---------------------Track State-----------" << endl;
	cout << "relative bearing...........: " << track.track_state.relative_bearing << endl;
	cout << "relative bearing invalid...: " << boolalpha << (track.track_state.bearing_invalid != 0) << endl;
	cout << "relative_range.............: " << track.track_state.relative_range << endl;
	cout << "relative_altitude..........: " << track.track_state.relative_altitude << endl;
	cout << "relative_ground_speed_north: " << track.track_state.relative_ground_speed_north << endl;
	cout << "relative_ground_speed_east.: " << track.track_state.relative_ground_speed_east << endl;

	cout << "---------------------Uncertainty Values----" << endl;
	cout << "horizonal_position_uncertainty.........: " << track.aircraft_state.horizonal_position_uncertainty << endl;
	cout << "geometric_altitude_uncertainty.........: " << track.aircraft_state.geometric_altitude_uncertainty << endl;
	cout << "ground_speed_uncertainty...............: " << track.aircraft_state.ground_speed_uncertainty << endl;
	cout << "relative_range_uncertainty.............: " << track.track_state.relative_range_uncertainty << std::endl;
	cout << "relative_bearing_uncertainty...........: " << track.track_state.relative_bearing_uncertainty << endl;
	cout << "relative_ground_speed_north_uncertainty: " << track.track_state.relative_ground_speed_north_uncertainty << endl;
	cout << "relative_ground_speed_east_uncertainty.: " << track.track_state.relative_ground_speed_east_uncertainty << endl;
	cout << "relative_altitude_uncertainty..........: " << track.track_state.relative_altitude_uncertainty << endl;
	cout << "relative_vertical_speed_uncertainty....: " << track.track_state.relative_vertical_speed_uncertainty << endl;
	cout << "heading uncertainty....................: " << track.aircraft_state.heading_uncertainty << endl;
}

