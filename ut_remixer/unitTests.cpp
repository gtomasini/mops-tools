// unitTests.cpp: define el punto de entrada de la aplicación de consola.
//

#include <string>
#include <map>
#include <iostream>
#include <gtest/gtest.h>

#include "remixerLib\mops2track.h"

// A new one of these is created for each test
class mops2trackTest : public testing::Test {
public:
	//vector<int> m_vector;
	mops2track csv2track;
	std::map<std::string, std::string> row;
	trackICD::track track;

	virtual void SetUp() {
		//std::string fname = "C1_ADSB_ADSB_adsbLog_TVInt1.csv";
		//csv2track.processCsvFile(fname);

		//ADSB
		//ToR(s),ICAO,ToA(s),Lat(deg),Lon(deg),Alt(ft),EWV(kts),NSV(kts),NACP,NIC,NACV,VR(ft/min)
		//2.3, 2, 2, 33.79712105, -116.96841002,10000, -14,	146, 7,	11,	2, 0

		row["ToA"] = "2";
		row["TID"] = "111";
		row["Lat"] = "33.5";
		row["Lon"] = "34.5";
		row["Alt"] = "10000.5";
		row["EWV"] = "35.5";
		row["NSV"] = "36.5";
		row["VR"] = "101.5";

		/*mapsRow(std::map<std::string, std::string> const &i_row,
			trackICD::track &track,
			source src,
			const std::string &callsign,
			const std::string &loglevel) -> int;*/
		
		ownship_stateICD::aircraft_state state;
		mops2track::mapsRow(row, track, state, mops2track::source::adsb, 
			std::string("C1_INTR"), std::string("notice") );
	}

	virtual void TearDown() {
	}
};

TEST_F(mops2trackTest, toATest) {
	EXPECT_DOUBLE_EQ(2, track.aircraft_state.toa);
}

TEST_F(mops2trackTest, toABoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.TOA);
}

TEST_F(mops2trackTest, tidTest) {
	EXPECT_DOUBLE_EQ(111, track.track_id);
}

TEST_F(mops2trackTest, latTest) {
	EXPECT_FLOAT_EQ (33.5, track.aircraft_state.latitude);
}

TEST_F(mops2trackTest, latBoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.LATITUDE);
}

TEST_F(mops2trackTest, lonTest) {
	EXPECT_FLOAT_EQ(34.5, track.aircraft_state.longitude);
}

TEST_F(mops2trackTest, lonBoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.LONGITUDE);
}

TEST_F(mops2trackTest, altTest) {
	EXPECT_FLOAT_EQ(3048.1523, track.aircraft_state.geometric_altitude);
}

TEST_F(mops2trackTest, altBoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.GEOMETRIC_ALTITUDE);
}

TEST_F(mops2trackTest, ewvTest) {
	EXPECT_FLOAT_EQ(18.262779, track.aircraft_state.ground_speed_east);
}

TEST_F(mops2trackTest, ewvBoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.GROUND_SPEED_EAST);
}

TEST_F(mops2trackTest, nsvTest) {
	EXPECT_FLOAT_EQ(18.777224, track.aircraft_state.ground_speed_north);
}

TEST_F(mops2trackTest, nsvBoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.GROUND_SPEED_NORTH);
}

TEST_F(mops2trackTest, vrTest) {
	EXPECT_FLOAT_EQ(0.51562101, track.aircraft_state.geometric_vertical_rate);
}

TEST_F(mops2trackTest, vrBoolTest) {
	EXPECT_TRUE(track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE);
}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

