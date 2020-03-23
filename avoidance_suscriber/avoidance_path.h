#pragma once

#include <string>
#include <vector>

    struct waypoint
    {
        bool operator==(waypoint const& other) const
        {
            return this->_id == other._id;
        }

        std::string _id{};
        double _longitude_rad{};
        double _latitude_rad{};
        double _altitude_m{};
        double _ground_speed_mps{};
        double _ground_track_rad{};
    };

    struct avoidance_path
    {
        bool operator==(avoidance_path const& other) const
        {
            return this->_date == other._date;
        }

        std::string _date{};
        std::vector<waypoint> _waypoints;
    };
