#ifndef IcdPlan_H
#define IcdPlan_H
#include <stdint.h>
namespace IcdPlan {
//Type hash codes
   const unsigned char WayPointType_hash[20] = {0xedU, 0xe1U, 0xa1U, 0x08U, 0x1cU, 0xf5U, 0x71U, 0x15U, 0x25U, 0xf3U, 0x68U, 0x2aU, 0x12U, 0x46U, 0xb2U, 0x00U, 0xafU, 0xd7U, 0xadU, 0x77U};
   const unsigned char WayPointType2_hash[20] = {0xcfU, 0xb2U, 0x08U, 0xf0U, 0x8eU, 0x07U, 0xd5U, 0x9dU, 0x00U, 0xdaU, 0x28U, 0xb2U, 0x58U, 0xf8U, 0xfaU, 0x69U, 0x5fU, 0x5dU, 0x7eU, 0x67U};
   const unsigned char LatLonAltPoint_hash[20] = {0x7fU, 0x9bU, 0x5fU, 0x54U, 0x2fU, 0x75U, 0xb0U, 0x13U, 0x5aU, 0xd0U, 0x52U, 0xb0U, 0x8eU, 0x73U, 0x79U, 0x0aU, 0x2fU, 0xd4U, 0xbeU, 0x71U};
   const unsigned char GuidancePoint_hash[20] = {0xbaU, 0x12U, 0xfcU, 0xd9U, 0xe2U, 0x09U, 0x52U, 0xb1U, 0x33U, 0x55U, 0x4bU, 0xe4U, 0xadU, 0x69U, 0xa6U, 0x06U, 0xe0U, 0xf1U, 0xa1U, 0x1fU};
   const unsigned char MissionPlan_hash[20] = {0x06U, 0x6aU, 0x1eU, 0x09U, 0xbcU, 0x95U, 0x13U, 0x50U, 0xa3U, 0xc4U, 0xc0U, 0xb1U, 0x76U, 0xddU, 0x95U, 0x73U, 0x45U, 0x22U, 0xcdU, 0x83U};

//Type definitions
   enum WayPointType
   {
      //TODO: Unknown needs documentation!
      Unknown = 0
,

      //TODO: Taxi needs documentation!
      Taxi = 1
,

      //TODO: Cruise needs documentation!
      Cruise = 2
,

      //TODO: Descent needs documentation!
      Descent = 3
,

      //TODO: Ascent needs documentation!
      Ascent = 4

   };

   enum WayPointType2
   {
      //TODO: FlyBy needs documentation!
      FlyBy = 0
,

      //TODO: FlyOver needs documentation!
      FlyOver = 1
,

      //TODO: FlyByLargeTrnAng needs documentation!
      FlyByLargeTrnAng = 2

   };

   typedef char LatLonAltPoint_buffer[12];
   struct LatLonAltPoint
   {
      //Latitude in degrees [-90, 90].

      float Latitude;
      //Longitude in degrees [-180, 180].

      float Longitude;
      //Altitude in meters

      float Altitude;
   };

   typedef char GuidancePoint_buffer[52];
   struct GuidancePoint
   {
      //The ID of the point.

      uint16_t ID;
      //The ID of the next point.

      uint16_t NextWyPtID;
      //Type relative to mission

      WayPointType Type;
      //How the waypoint is to be flown

      WayPointType2 Type2;
      //Latitude in degrees [-90, 90].

      float Latitude;
      //Longitude in degrees [-180, 180].

      float Longitude;
      //Altitude in meters

      float Altitude;
      //Ground speed in mtrs/sec.

      float GrndSpd;
      //Ground track [0, 360) in a clockwise direction.

      float GroundTrack;
      //Time of arrival in epoch-nanosec

      int64_t TOA;
      //Turn radius in meters

      float TurnRadius;
      //Turn angle in degrees (right is positive, left is negative)

      float TurnAngle;
      //Vertical speed in mtrs/sec

      float VerticalSpd;
   };

   typedef char MissionPlan_buffer[1043];
   struct MissionPlan
   {
      //A sequence of mission plan points with a max of 20 actually set (Note! don't know what Caravan max is- not determined).

      GuidancePoint Plan[20];
      //The number of points in the plan. Only the 1st NumPoints in the Plan sequence are valid.

      uint8_t NumPoints;
      //The ID of the 1st waypoint of the current segment if on a segment. If in a turn then the ID of the waypoint at the turn.

      uint16_t CurrWyPtID;
   };


//Reader Functions
   void read_LatLonAltPoint(LatLonAltPoint& data, const LatLonAltPoint_buffer& raw);
   void read_GuidancePoint(GuidancePoint& data, const GuidancePoint_buffer& raw);
   void read_MissionPlan(MissionPlan& data, const MissionPlan_buffer& raw);

//Writer functions
   void write_LatLonAltPoint(const LatLonAltPoint& data, LatLonAltPoint_buffer& raw);
   void write_GuidancePoint(const GuidancePoint& data, GuidancePoint_buffer& raw);
   void write_MissionPlan(const MissionPlan& data, MissionPlan_buffer& raw);

}
#endif //IcdPlan_H
