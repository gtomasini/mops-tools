#pragma once
#include "daa_datatypes_icd/trackICD.h"
#include "daa_datatypes_icd/ownship_stateICD.h"

//intruder
void trackDisplay(const trackICD::track &track);
//ownship
void stateDisplay(const ownship_stateICD::aircraft_state &state);
