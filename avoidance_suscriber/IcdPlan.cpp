#include "IcdPlan.h"
namespace IcdPlan {
//Writer functions
   void write_LatLonAltPoint(const LatLonAltPoint& data, LatLonAltPoint_buffer& raw) {
      { //write data.Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Latitude;
         raw[0] = u.b[0];
         raw[1] = u.b[1];
         raw[2] = u.b[2];
         raw[3] = u.b[3];
      } 
      { //write data.Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Longitude;
         raw[4] = u.b[0];
         raw[5] = u.b[1];
         raw[6] = u.b[2];
         raw[7] = u.b[3];
      } 
      { //write data.Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Altitude;
         raw[8] = u.b[0];
         raw[9] = u.b[1];
         raw[10] = u.b[2];
         raw[11] = u.b[3];
      } 
   }
   void write_GuidancePoint(const GuidancePoint& data, GuidancePoint_buffer& raw) {
      { //write data.ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.ID;
         raw[0] = u.b[0];
         raw[1] = u.b[1];
      } 
      { //write data.NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.NextWyPtID;
         raw[2] = u.b[0];
         raw[3] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[4] = u.b[0];
         raw[5] = u.b[1];
         raw[6] = u.b[2];
         raw[7] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[8] = u.b[0];
         raw[9] = u.b[1];
         raw[10] = u.b[2];
         raw[11] = u.b[3];
      } 
      { //write data.Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Latitude;
         raw[12] = u.b[0];
         raw[13] = u.b[1];
         raw[14] = u.b[2];
         raw[15] = u.b[3];
      } 
      { //write data.Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Longitude;
         raw[16] = u.b[0];
         raw[17] = u.b[1];
         raw[18] = u.b[2];
         raw[19] = u.b[3];
      } 
      { //write data.Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Altitude;
         raw[20] = u.b[0];
         raw[21] = u.b[1];
         raw[22] = u.b[2];
         raw[23] = u.b[3];
      } 
      { //write data.GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.GrndSpd;
         raw[24] = u.b[0];
         raw[25] = u.b[1];
         raw[26] = u.b[2];
         raw[27] = u.b[3];
      } 
      { //write data.GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.GroundTrack;
         raw[28] = u.b[0];
         raw[29] = u.b[1];
         raw[30] = u.b[2];
         raw[31] = u.b[3];
      } 
      { //write data.TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.TOA;
         raw[32] = u.b[0];
         raw[33] = u.b[1];
         raw[34] = u.b[2];
         raw[35] = u.b[3];
         raw[36] = u.b[4];
         raw[37] = u.b[5];
         raw[38] = u.b[6];
         raw[39] = u.b[7];
      } 
      { //write data.TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.TurnRadius;
         raw[40] = u.b[0];
         raw[41] = u.b[1];
         raw[42] = u.b[2];
         raw[43] = u.b[3];
      } 
      { //write data.TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.TurnAngle;
         raw[44] = u.b[0];
         raw[45] = u.b[1];
         raw[46] = u.b[2];
         raw[47] = u.b[3];
      } 
      { //write data.VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.VerticalSpd;
         raw[48] = u.b[0];
         raw[49] = u.b[1];
         raw[50] = u.b[2];
         raw[51] = u.b[3];
      } 
   }
   void write_MissionPlan(const MissionPlan& data, MissionPlan_buffer& raw) {
      { //write data.Plan[0].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[0].ID;
         raw[0] = u.b[0];
         raw[1] = u.b[1];
      } 
      { //write data.Plan[0].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[0].NextWyPtID;
         raw[2] = u.b[0];
         raw[3] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[0].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[4] = u.b[0];
         raw[5] = u.b[1];
         raw[6] = u.b[2];
         raw[7] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[0].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[8] = u.b[0];
         raw[9] = u.b[1];
         raw[10] = u.b[2];
         raw[11] = u.b[3];
      } 
      { //write data.Plan[0].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].Latitude;
         raw[12] = u.b[0];
         raw[13] = u.b[1];
         raw[14] = u.b[2];
         raw[15] = u.b[3];
      } 
      { //write data.Plan[0].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].Longitude;
         raw[16] = u.b[0];
         raw[17] = u.b[1];
         raw[18] = u.b[2];
         raw[19] = u.b[3];
      } 
      { //write data.Plan[0].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].Altitude;
         raw[20] = u.b[0];
         raw[21] = u.b[1];
         raw[22] = u.b[2];
         raw[23] = u.b[3];
      } 
      { //write data.Plan[0].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].GrndSpd;
         raw[24] = u.b[0];
         raw[25] = u.b[1];
         raw[26] = u.b[2];
         raw[27] = u.b[3];
      } 
      { //write data.Plan[0].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].GroundTrack;
         raw[28] = u.b[0];
         raw[29] = u.b[1];
         raw[30] = u.b[2];
         raw[31] = u.b[3];
      } 
      { //write data.Plan[0].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[0].TOA;
         raw[32] = u.b[0];
         raw[33] = u.b[1];
         raw[34] = u.b[2];
         raw[35] = u.b[3];
         raw[36] = u.b[4];
         raw[37] = u.b[5];
         raw[38] = u.b[6];
         raw[39] = u.b[7];
      } 
      { //write data.Plan[0].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].TurnRadius;
         raw[40] = u.b[0];
         raw[41] = u.b[1];
         raw[42] = u.b[2];
         raw[43] = u.b[3];
      } 
      { //write data.Plan[0].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].TurnAngle;
         raw[44] = u.b[0];
         raw[45] = u.b[1];
         raw[46] = u.b[2];
         raw[47] = u.b[3];
      } 
      { //write data.Plan[0].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[0].VerticalSpd;
         raw[48] = u.b[0];
         raw[49] = u.b[1];
         raw[50] = u.b[2];
         raw[51] = u.b[3];
      } 
      { //write data.Plan[1].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[1].ID;
         raw[52] = u.b[0];
         raw[53] = u.b[1];
      } 
      { //write data.Plan[1].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[1].NextWyPtID;
         raw[54] = u.b[0];
         raw[55] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[1].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[56] = u.b[0];
         raw[57] = u.b[1];
         raw[58] = u.b[2];
         raw[59] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[1].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[60] = u.b[0];
         raw[61] = u.b[1];
         raw[62] = u.b[2];
         raw[63] = u.b[3];
      } 
      { //write data.Plan[1].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].Latitude;
         raw[64] = u.b[0];
         raw[65] = u.b[1];
         raw[66] = u.b[2];
         raw[67] = u.b[3];
      } 
      { //write data.Plan[1].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].Longitude;
         raw[68] = u.b[0];
         raw[69] = u.b[1];
         raw[70] = u.b[2];
         raw[71] = u.b[3];
      } 
      { //write data.Plan[1].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].Altitude;
         raw[72] = u.b[0];
         raw[73] = u.b[1];
         raw[74] = u.b[2];
         raw[75] = u.b[3];
      } 
      { //write data.Plan[1].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].GrndSpd;
         raw[76] = u.b[0];
         raw[77] = u.b[1];
         raw[78] = u.b[2];
         raw[79] = u.b[3];
      } 
      { //write data.Plan[1].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].GroundTrack;
         raw[80] = u.b[0];
         raw[81] = u.b[1];
         raw[82] = u.b[2];
         raw[83] = u.b[3];
      } 
      { //write data.Plan[1].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[1].TOA;
         raw[84] = u.b[0];
         raw[85] = u.b[1];
         raw[86] = u.b[2];
         raw[87] = u.b[3];
         raw[88] = u.b[4];
         raw[89] = u.b[5];
         raw[90] = u.b[6];
         raw[91] = u.b[7];
      } 
      { //write data.Plan[1].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].TurnRadius;
         raw[92] = u.b[0];
         raw[93] = u.b[1];
         raw[94] = u.b[2];
         raw[95] = u.b[3];
      } 
      { //write data.Plan[1].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].TurnAngle;
         raw[96] = u.b[0];
         raw[97] = u.b[1];
         raw[98] = u.b[2];
         raw[99] = u.b[3];
      } 
      { //write data.Plan[1].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[1].VerticalSpd;
         raw[100] = u.b[0];
         raw[101] = u.b[1];
         raw[102] = u.b[2];
         raw[103] = u.b[3];
      } 
      { //write data.Plan[2].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[2].ID;
         raw[104] = u.b[0];
         raw[105] = u.b[1];
      } 
      { //write data.Plan[2].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[2].NextWyPtID;
         raw[106] = u.b[0];
         raw[107] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[2].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[108] = u.b[0];
         raw[109] = u.b[1];
         raw[110] = u.b[2];
         raw[111] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[2].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[112] = u.b[0];
         raw[113] = u.b[1];
         raw[114] = u.b[2];
         raw[115] = u.b[3];
      } 
      { //write data.Plan[2].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].Latitude;
         raw[116] = u.b[0];
         raw[117] = u.b[1];
         raw[118] = u.b[2];
         raw[119] = u.b[3];
      } 
      { //write data.Plan[2].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].Longitude;
         raw[120] = u.b[0];
         raw[121] = u.b[1];
         raw[122] = u.b[2];
         raw[123] = u.b[3];
      } 
      { //write data.Plan[2].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].Altitude;
         raw[124] = u.b[0];
         raw[125] = u.b[1];
         raw[126] = u.b[2];
         raw[127] = u.b[3];
      } 
      { //write data.Plan[2].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].GrndSpd;
         raw[128] = u.b[0];
         raw[129] = u.b[1];
         raw[130] = u.b[2];
         raw[131] = u.b[3];
      } 
      { //write data.Plan[2].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].GroundTrack;
         raw[132] = u.b[0];
         raw[133] = u.b[1];
         raw[134] = u.b[2];
         raw[135] = u.b[3];
      } 
      { //write data.Plan[2].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[2].TOA;
         raw[136] = u.b[0];
         raw[137] = u.b[1];
         raw[138] = u.b[2];
         raw[139] = u.b[3];
         raw[140] = u.b[4];
         raw[141] = u.b[5];
         raw[142] = u.b[6];
         raw[143] = u.b[7];
      } 
      { //write data.Plan[2].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].TurnRadius;
         raw[144] = u.b[0];
         raw[145] = u.b[1];
         raw[146] = u.b[2];
         raw[147] = u.b[3];
      } 
      { //write data.Plan[2].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].TurnAngle;
         raw[148] = u.b[0];
         raw[149] = u.b[1];
         raw[150] = u.b[2];
         raw[151] = u.b[3];
      } 
      { //write data.Plan[2].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[2].VerticalSpd;
         raw[152] = u.b[0];
         raw[153] = u.b[1];
         raw[154] = u.b[2];
         raw[155] = u.b[3];
      } 
      { //write data.Plan[3].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[3].ID;
         raw[156] = u.b[0];
         raw[157] = u.b[1];
      } 
      { //write data.Plan[3].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[3].NextWyPtID;
         raw[158] = u.b[0];
         raw[159] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[3].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[160] = u.b[0];
         raw[161] = u.b[1];
         raw[162] = u.b[2];
         raw[163] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[3].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[164] = u.b[0];
         raw[165] = u.b[1];
         raw[166] = u.b[2];
         raw[167] = u.b[3];
      } 
      { //write data.Plan[3].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].Latitude;
         raw[168] = u.b[0];
         raw[169] = u.b[1];
         raw[170] = u.b[2];
         raw[171] = u.b[3];
      } 
      { //write data.Plan[3].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].Longitude;
         raw[172] = u.b[0];
         raw[173] = u.b[1];
         raw[174] = u.b[2];
         raw[175] = u.b[3];
      } 
      { //write data.Plan[3].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].Altitude;
         raw[176] = u.b[0];
         raw[177] = u.b[1];
         raw[178] = u.b[2];
         raw[179] = u.b[3];
      } 
      { //write data.Plan[3].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].GrndSpd;
         raw[180] = u.b[0];
         raw[181] = u.b[1];
         raw[182] = u.b[2];
         raw[183] = u.b[3];
      } 
      { //write data.Plan[3].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].GroundTrack;
         raw[184] = u.b[0];
         raw[185] = u.b[1];
         raw[186] = u.b[2];
         raw[187] = u.b[3];
      } 
      { //write data.Plan[3].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[3].TOA;
         raw[188] = u.b[0];
         raw[189] = u.b[1];
         raw[190] = u.b[2];
         raw[191] = u.b[3];
         raw[192] = u.b[4];
         raw[193] = u.b[5];
         raw[194] = u.b[6];
         raw[195] = u.b[7];
      } 
      { //write data.Plan[3].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].TurnRadius;
         raw[196] = u.b[0];
         raw[197] = u.b[1];
         raw[198] = u.b[2];
         raw[199] = u.b[3];
      } 
      { //write data.Plan[3].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].TurnAngle;
         raw[200] = u.b[0];
         raw[201] = u.b[1];
         raw[202] = u.b[2];
         raw[203] = u.b[3];
      } 
      { //write data.Plan[3].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[3].VerticalSpd;
         raw[204] = u.b[0];
         raw[205] = u.b[1];
         raw[206] = u.b[2];
         raw[207] = u.b[3];
      } 
      { //write data.Plan[4].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[4].ID;
         raw[208] = u.b[0];
         raw[209] = u.b[1];
      } 
      { //write data.Plan[4].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[4].NextWyPtID;
         raw[210] = u.b[0];
         raw[211] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[4].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[212] = u.b[0];
         raw[213] = u.b[1];
         raw[214] = u.b[2];
         raw[215] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[4].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[216] = u.b[0];
         raw[217] = u.b[1];
         raw[218] = u.b[2];
         raw[219] = u.b[3];
      } 
      { //write data.Plan[4].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].Latitude;
         raw[220] = u.b[0];
         raw[221] = u.b[1];
         raw[222] = u.b[2];
         raw[223] = u.b[3];
      } 
      { //write data.Plan[4].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].Longitude;
         raw[224] = u.b[0];
         raw[225] = u.b[1];
         raw[226] = u.b[2];
         raw[227] = u.b[3];
      } 
      { //write data.Plan[4].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].Altitude;
         raw[228] = u.b[0];
         raw[229] = u.b[1];
         raw[230] = u.b[2];
         raw[231] = u.b[3];
      } 
      { //write data.Plan[4].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].GrndSpd;
         raw[232] = u.b[0];
         raw[233] = u.b[1];
         raw[234] = u.b[2];
         raw[235] = u.b[3];
      } 
      { //write data.Plan[4].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].GroundTrack;
         raw[236] = u.b[0];
         raw[237] = u.b[1];
         raw[238] = u.b[2];
         raw[239] = u.b[3];
      } 
      { //write data.Plan[4].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[4].TOA;
         raw[240] = u.b[0];
         raw[241] = u.b[1];
         raw[242] = u.b[2];
         raw[243] = u.b[3];
         raw[244] = u.b[4];
         raw[245] = u.b[5];
         raw[246] = u.b[6];
         raw[247] = u.b[7];
      } 
      { //write data.Plan[4].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].TurnRadius;
         raw[248] = u.b[0];
         raw[249] = u.b[1];
         raw[250] = u.b[2];
         raw[251] = u.b[3];
      } 
      { //write data.Plan[4].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].TurnAngle;
         raw[252] = u.b[0];
         raw[253] = u.b[1];
         raw[254] = u.b[2];
         raw[255] = u.b[3];
      } 
      { //write data.Plan[4].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[4].VerticalSpd;
         raw[256] = u.b[0];
         raw[257] = u.b[1];
         raw[258] = u.b[2];
         raw[259] = u.b[3];
      } 
      { //write data.Plan[5].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[5].ID;
         raw[260] = u.b[0];
         raw[261] = u.b[1];
      } 
      { //write data.Plan[5].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[5].NextWyPtID;
         raw[262] = u.b[0];
         raw[263] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[5].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[264] = u.b[0];
         raw[265] = u.b[1];
         raw[266] = u.b[2];
         raw[267] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[5].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[268] = u.b[0];
         raw[269] = u.b[1];
         raw[270] = u.b[2];
         raw[271] = u.b[3];
      } 
      { //write data.Plan[5].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].Latitude;
         raw[272] = u.b[0];
         raw[273] = u.b[1];
         raw[274] = u.b[2];
         raw[275] = u.b[3];
      } 
      { //write data.Plan[5].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].Longitude;
         raw[276] = u.b[0];
         raw[277] = u.b[1];
         raw[278] = u.b[2];
         raw[279] = u.b[3];
      } 
      { //write data.Plan[5].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].Altitude;
         raw[280] = u.b[0];
         raw[281] = u.b[1];
         raw[282] = u.b[2];
         raw[283] = u.b[3];
      } 
      { //write data.Plan[5].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].GrndSpd;
         raw[284] = u.b[0];
         raw[285] = u.b[1];
         raw[286] = u.b[2];
         raw[287] = u.b[3];
      } 
      { //write data.Plan[5].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].GroundTrack;
         raw[288] = u.b[0];
         raw[289] = u.b[1];
         raw[290] = u.b[2];
         raw[291] = u.b[3];
      } 
      { //write data.Plan[5].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[5].TOA;
         raw[292] = u.b[0];
         raw[293] = u.b[1];
         raw[294] = u.b[2];
         raw[295] = u.b[3];
         raw[296] = u.b[4];
         raw[297] = u.b[5];
         raw[298] = u.b[6];
         raw[299] = u.b[7];
      } 
      { //write data.Plan[5].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].TurnRadius;
         raw[300] = u.b[0];
         raw[301] = u.b[1];
         raw[302] = u.b[2];
         raw[303] = u.b[3];
      } 
      { //write data.Plan[5].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].TurnAngle;
         raw[304] = u.b[0];
         raw[305] = u.b[1];
         raw[306] = u.b[2];
         raw[307] = u.b[3];
      } 
      { //write data.Plan[5].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[5].VerticalSpd;
         raw[308] = u.b[0];
         raw[309] = u.b[1];
         raw[310] = u.b[2];
         raw[311] = u.b[3];
      } 
      { //write data.Plan[6].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[6].ID;
         raw[312] = u.b[0];
         raw[313] = u.b[1];
      } 
      { //write data.Plan[6].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[6].NextWyPtID;
         raw[314] = u.b[0];
         raw[315] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[6].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[316] = u.b[0];
         raw[317] = u.b[1];
         raw[318] = u.b[2];
         raw[319] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[6].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[320] = u.b[0];
         raw[321] = u.b[1];
         raw[322] = u.b[2];
         raw[323] = u.b[3];
      } 
      { //write data.Plan[6].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].Latitude;
         raw[324] = u.b[0];
         raw[325] = u.b[1];
         raw[326] = u.b[2];
         raw[327] = u.b[3];
      } 
      { //write data.Plan[6].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].Longitude;
         raw[328] = u.b[0];
         raw[329] = u.b[1];
         raw[330] = u.b[2];
         raw[331] = u.b[3];
      } 
      { //write data.Plan[6].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].Altitude;
         raw[332] = u.b[0];
         raw[333] = u.b[1];
         raw[334] = u.b[2];
         raw[335] = u.b[3];
      } 
      { //write data.Plan[6].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].GrndSpd;
         raw[336] = u.b[0];
         raw[337] = u.b[1];
         raw[338] = u.b[2];
         raw[339] = u.b[3];
      } 
      { //write data.Plan[6].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].GroundTrack;
         raw[340] = u.b[0];
         raw[341] = u.b[1];
         raw[342] = u.b[2];
         raw[343] = u.b[3];
      } 
      { //write data.Plan[6].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[6].TOA;
         raw[344] = u.b[0];
         raw[345] = u.b[1];
         raw[346] = u.b[2];
         raw[347] = u.b[3];
         raw[348] = u.b[4];
         raw[349] = u.b[5];
         raw[350] = u.b[6];
         raw[351] = u.b[7];
      } 
      { //write data.Plan[6].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].TurnRadius;
         raw[352] = u.b[0];
         raw[353] = u.b[1];
         raw[354] = u.b[2];
         raw[355] = u.b[3];
      } 
      { //write data.Plan[6].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].TurnAngle;
         raw[356] = u.b[0];
         raw[357] = u.b[1];
         raw[358] = u.b[2];
         raw[359] = u.b[3];
      } 
      { //write data.Plan[6].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[6].VerticalSpd;
         raw[360] = u.b[0];
         raw[361] = u.b[1];
         raw[362] = u.b[2];
         raw[363] = u.b[3];
      } 
      { //write data.Plan[7].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[7].ID;
         raw[364] = u.b[0];
         raw[365] = u.b[1];
      } 
      { //write data.Plan[7].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[7].NextWyPtID;
         raw[366] = u.b[0];
         raw[367] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[7].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[368] = u.b[0];
         raw[369] = u.b[1];
         raw[370] = u.b[2];
         raw[371] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[7].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[372] = u.b[0];
         raw[373] = u.b[1];
         raw[374] = u.b[2];
         raw[375] = u.b[3];
      } 
      { //write data.Plan[7].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].Latitude;
         raw[376] = u.b[0];
         raw[377] = u.b[1];
         raw[378] = u.b[2];
         raw[379] = u.b[3];
      } 
      { //write data.Plan[7].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].Longitude;
         raw[380] = u.b[0];
         raw[381] = u.b[1];
         raw[382] = u.b[2];
         raw[383] = u.b[3];
      } 
      { //write data.Plan[7].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].Altitude;
         raw[384] = u.b[0];
         raw[385] = u.b[1];
         raw[386] = u.b[2];
         raw[387] = u.b[3];
      } 
      { //write data.Plan[7].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].GrndSpd;
         raw[388] = u.b[0];
         raw[389] = u.b[1];
         raw[390] = u.b[2];
         raw[391] = u.b[3];
      } 
      { //write data.Plan[7].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].GroundTrack;
         raw[392] = u.b[0];
         raw[393] = u.b[1];
         raw[394] = u.b[2];
         raw[395] = u.b[3];
      } 
      { //write data.Plan[7].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[7].TOA;
         raw[396] = u.b[0];
         raw[397] = u.b[1];
         raw[398] = u.b[2];
         raw[399] = u.b[3];
         raw[400] = u.b[4];
         raw[401] = u.b[5];
         raw[402] = u.b[6];
         raw[403] = u.b[7];
      } 
      { //write data.Plan[7].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].TurnRadius;
         raw[404] = u.b[0];
         raw[405] = u.b[1];
         raw[406] = u.b[2];
         raw[407] = u.b[3];
      } 
      { //write data.Plan[7].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].TurnAngle;
         raw[408] = u.b[0];
         raw[409] = u.b[1];
         raw[410] = u.b[2];
         raw[411] = u.b[3];
      } 
      { //write data.Plan[7].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[7].VerticalSpd;
         raw[412] = u.b[0];
         raw[413] = u.b[1];
         raw[414] = u.b[2];
         raw[415] = u.b[3];
      } 
      { //write data.Plan[8].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[8].ID;
         raw[416] = u.b[0];
         raw[417] = u.b[1];
      } 
      { //write data.Plan[8].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[8].NextWyPtID;
         raw[418] = u.b[0];
         raw[419] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[8].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[420] = u.b[0];
         raw[421] = u.b[1];
         raw[422] = u.b[2];
         raw[423] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[8].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[424] = u.b[0];
         raw[425] = u.b[1];
         raw[426] = u.b[2];
         raw[427] = u.b[3];
      } 
      { //write data.Plan[8].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].Latitude;
         raw[428] = u.b[0];
         raw[429] = u.b[1];
         raw[430] = u.b[2];
         raw[431] = u.b[3];
      } 
      { //write data.Plan[8].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].Longitude;
         raw[432] = u.b[0];
         raw[433] = u.b[1];
         raw[434] = u.b[2];
         raw[435] = u.b[3];
      } 
      { //write data.Plan[8].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].Altitude;
         raw[436] = u.b[0];
         raw[437] = u.b[1];
         raw[438] = u.b[2];
         raw[439] = u.b[3];
      } 
      { //write data.Plan[8].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].GrndSpd;
         raw[440] = u.b[0];
         raw[441] = u.b[1];
         raw[442] = u.b[2];
         raw[443] = u.b[3];
      } 
      { //write data.Plan[8].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].GroundTrack;
         raw[444] = u.b[0];
         raw[445] = u.b[1];
         raw[446] = u.b[2];
         raw[447] = u.b[3];
      } 
      { //write data.Plan[8].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[8].TOA;
         raw[448] = u.b[0];
         raw[449] = u.b[1];
         raw[450] = u.b[2];
         raw[451] = u.b[3];
         raw[452] = u.b[4];
         raw[453] = u.b[5];
         raw[454] = u.b[6];
         raw[455] = u.b[7];
      } 
      { //write data.Plan[8].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].TurnRadius;
         raw[456] = u.b[0];
         raw[457] = u.b[1];
         raw[458] = u.b[2];
         raw[459] = u.b[3];
      } 
      { //write data.Plan[8].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].TurnAngle;
         raw[460] = u.b[0];
         raw[461] = u.b[1];
         raw[462] = u.b[2];
         raw[463] = u.b[3];
      } 
      { //write data.Plan[8].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[8].VerticalSpd;
         raw[464] = u.b[0];
         raw[465] = u.b[1];
         raw[466] = u.b[2];
         raw[467] = u.b[3];
      } 
      { //write data.Plan[9].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[9].ID;
         raw[468] = u.b[0];
         raw[469] = u.b[1];
      } 
      { //write data.Plan[9].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[9].NextWyPtID;
         raw[470] = u.b[0];
         raw[471] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[9].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[472] = u.b[0];
         raw[473] = u.b[1];
         raw[474] = u.b[2];
         raw[475] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[9].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[476] = u.b[0];
         raw[477] = u.b[1];
         raw[478] = u.b[2];
         raw[479] = u.b[3];
      } 
      { //write data.Plan[9].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].Latitude;
         raw[480] = u.b[0];
         raw[481] = u.b[1];
         raw[482] = u.b[2];
         raw[483] = u.b[3];
      } 
      { //write data.Plan[9].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].Longitude;
         raw[484] = u.b[0];
         raw[485] = u.b[1];
         raw[486] = u.b[2];
         raw[487] = u.b[3];
      } 
      { //write data.Plan[9].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].Altitude;
         raw[488] = u.b[0];
         raw[489] = u.b[1];
         raw[490] = u.b[2];
         raw[491] = u.b[3];
      } 
      { //write data.Plan[9].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].GrndSpd;
         raw[492] = u.b[0];
         raw[493] = u.b[1];
         raw[494] = u.b[2];
         raw[495] = u.b[3];
      } 
      { //write data.Plan[9].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].GroundTrack;
         raw[496] = u.b[0];
         raw[497] = u.b[1];
         raw[498] = u.b[2];
         raw[499] = u.b[3];
      } 
      { //write data.Plan[9].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[9].TOA;
         raw[500] = u.b[0];
         raw[501] = u.b[1];
         raw[502] = u.b[2];
         raw[503] = u.b[3];
         raw[504] = u.b[4];
         raw[505] = u.b[5];
         raw[506] = u.b[6];
         raw[507] = u.b[7];
      } 
      { //write data.Plan[9].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].TurnRadius;
         raw[508] = u.b[0];
         raw[509] = u.b[1];
         raw[510] = u.b[2];
         raw[511] = u.b[3];
      } 
      { //write data.Plan[9].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].TurnAngle;
         raw[512] = u.b[0];
         raw[513] = u.b[1];
         raw[514] = u.b[2];
         raw[515] = u.b[3];
      } 
      { //write data.Plan[9].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[9].VerticalSpd;
         raw[516] = u.b[0];
         raw[517] = u.b[1];
         raw[518] = u.b[2];
         raw[519] = u.b[3];
      } 
      { //write data.Plan[10].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[10].ID;
         raw[520] = u.b[0];
         raw[521] = u.b[1];
      } 
      { //write data.Plan[10].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[10].NextWyPtID;
         raw[522] = u.b[0];
         raw[523] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[10].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[524] = u.b[0];
         raw[525] = u.b[1];
         raw[526] = u.b[2];
         raw[527] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[10].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[528] = u.b[0];
         raw[529] = u.b[1];
         raw[530] = u.b[2];
         raw[531] = u.b[3];
      } 
      { //write data.Plan[10].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].Latitude;
         raw[532] = u.b[0];
         raw[533] = u.b[1];
         raw[534] = u.b[2];
         raw[535] = u.b[3];
      } 
      { //write data.Plan[10].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].Longitude;
         raw[536] = u.b[0];
         raw[537] = u.b[1];
         raw[538] = u.b[2];
         raw[539] = u.b[3];
      } 
      { //write data.Plan[10].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].Altitude;
         raw[540] = u.b[0];
         raw[541] = u.b[1];
         raw[542] = u.b[2];
         raw[543] = u.b[3];
      } 
      { //write data.Plan[10].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].GrndSpd;
         raw[544] = u.b[0];
         raw[545] = u.b[1];
         raw[546] = u.b[2];
         raw[547] = u.b[3];
      } 
      { //write data.Plan[10].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].GroundTrack;
         raw[548] = u.b[0];
         raw[549] = u.b[1];
         raw[550] = u.b[2];
         raw[551] = u.b[3];
      } 
      { //write data.Plan[10].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[10].TOA;
         raw[552] = u.b[0];
         raw[553] = u.b[1];
         raw[554] = u.b[2];
         raw[555] = u.b[3];
         raw[556] = u.b[4];
         raw[557] = u.b[5];
         raw[558] = u.b[6];
         raw[559] = u.b[7];
      } 
      { //write data.Plan[10].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].TurnRadius;
         raw[560] = u.b[0];
         raw[561] = u.b[1];
         raw[562] = u.b[2];
         raw[563] = u.b[3];
      } 
      { //write data.Plan[10].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].TurnAngle;
         raw[564] = u.b[0];
         raw[565] = u.b[1];
         raw[566] = u.b[2];
         raw[567] = u.b[3];
      } 
      { //write data.Plan[10].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[10].VerticalSpd;
         raw[568] = u.b[0];
         raw[569] = u.b[1];
         raw[570] = u.b[2];
         raw[571] = u.b[3];
      } 
      { //write data.Plan[11].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[11].ID;
         raw[572] = u.b[0];
         raw[573] = u.b[1];
      } 
      { //write data.Plan[11].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[11].NextWyPtID;
         raw[574] = u.b[0];
         raw[575] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[11].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[576] = u.b[0];
         raw[577] = u.b[1];
         raw[578] = u.b[2];
         raw[579] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[11].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[580] = u.b[0];
         raw[581] = u.b[1];
         raw[582] = u.b[2];
         raw[583] = u.b[3];
      } 
      { //write data.Plan[11].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].Latitude;
         raw[584] = u.b[0];
         raw[585] = u.b[1];
         raw[586] = u.b[2];
         raw[587] = u.b[3];
      } 
      { //write data.Plan[11].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].Longitude;
         raw[588] = u.b[0];
         raw[589] = u.b[1];
         raw[590] = u.b[2];
         raw[591] = u.b[3];
      } 
      { //write data.Plan[11].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].Altitude;
         raw[592] = u.b[0];
         raw[593] = u.b[1];
         raw[594] = u.b[2];
         raw[595] = u.b[3];
      } 
      { //write data.Plan[11].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].GrndSpd;
         raw[596] = u.b[0];
         raw[597] = u.b[1];
         raw[598] = u.b[2];
         raw[599] = u.b[3];
      } 
      { //write data.Plan[11].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].GroundTrack;
         raw[600] = u.b[0];
         raw[601] = u.b[1];
         raw[602] = u.b[2];
         raw[603] = u.b[3];
      } 
      { //write data.Plan[11].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[11].TOA;
         raw[604] = u.b[0];
         raw[605] = u.b[1];
         raw[606] = u.b[2];
         raw[607] = u.b[3];
         raw[608] = u.b[4];
         raw[609] = u.b[5];
         raw[610] = u.b[6];
         raw[611] = u.b[7];
      } 
      { //write data.Plan[11].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].TurnRadius;
         raw[612] = u.b[0];
         raw[613] = u.b[1];
         raw[614] = u.b[2];
         raw[615] = u.b[3];
      } 
      { //write data.Plan[11].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].TurnAngle;
         raw[616] = u.b[0];
         raw[617] = u.b[1];
         raw[618] = u.b[2];
         raw[619] = u.b[3];
      } 
      { //write data.Plan[11].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[11].VerticalSpd;
         raw[620] = u.b[0];
         raw[621] = u.b[1];
         raw[622] = u.b[2];
         raw[623] = u.b[3];
      } 
      { //write data.Plan[12].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[12].ID;
         raw[624] = u.b[0];
         raw[625] = u.b[1];
      } 
      { //write data.Plan[12].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[12].NextWyPtID;
         raw[626] = u.b[0];
         raw[627] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[12].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[628] = u.b[0];
         raw[629] = u.b[1];
         raw[630] = u.b[2];
         raw[631] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[12].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[632] = u.b[0];
         raw[633] = u.b[1];
         raw[634] = u.b[2];
         raw[635] = u.b[3];
      } 
      { //write data.Plan[12].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].Latitude;
         raw[636] = u.b[0];
         raw[637] = u.b[1];
         raw[638] = u.b[2];
         raw[639] = u.b[3];
      } 
      { //write data.Plan[12].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].Longitude;
         raw[640] = u.b[0];
         raw[641] = u.b[1];
         raw[642] = u.b[2];
         raw[643] = u.b[3];
      } 
      { //write data.Plan[12].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].Altitude;
         raw[644] = u.b[0];
         raw[645] = u.b[1];
         raw[646] = u.b[2];
         raw[647] = u.b[3];
      } 
      { //write data.Plan[12].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].GrndSpd;
         raw[648] = u.b[0];
         raw[649] = u.b[1];
         raw[650] = u.b[2];
         raw[651] = u.b[3];
      } 
      { //write data.Plan[12].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].GroundTrack;
         raw[652] = u.b[0];
         raw[653] = u.b[1];
         raw[654] = u.b[2];
         raw[655] = u.b[3];
      } 
      { //write data.Plan[12].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[12].TOA;
         raw[656] = u.b[0];
         raw[657] = u.b[1];
         raw[658] = u.b[2];
         raw[659] = u.b[3];
         raw[660] = u.b[4];
         raw[661] = u.b[5];
         raw[662] = u.b[6];
         raw[663] = u.b[7];
      } 
      { //write data.Plan[12].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].TurnRadius;
         raw[664] = u.b[0];
         raw[665] = u.b[1];
         raw[666] = u.b[2];
         raw[667] = u.b[3];
      } 
      { //write data.Plan[12].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].TurnAngle;
         raw[668] = u.b[0];
         raw[669] = u.b[1];
         raw[670] = u.b[2];
         raw[671] = u.b[3];
      } 
      { //write data.Plan[12].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[12].VerticalSpd;
         raw[672] = u.b[0];
         raw[673] = u.b[1];
         raw[674] = u.b[2];
         raw[675] = u.b[3];
      } 
      { //write data.Plan[13].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[13].ID;
         raw[676] = u.b[0];
         raw[677] = u.b[1];
      } 
      { //write data.Plan[13].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[13].NextWyPtID;
         raw[678] = u.b[0];
         raw[679] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[13].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[680] = u.b[0];
         raw[681] = u.b[1];
         raw[682] = u.b[2];
         raw[683] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[13].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[684] = u.b[0];
         raw[685] = u.b[1];
         raw[686] = u.b[2];
         raw[687] = u.b[3];
      } 
      { //write data.Plan[13].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].Latitude;
         raw[688] = u.b[0];
         raw[689] = u.b[1];
         raw[690] = u.b[2];
         raw[691] = u.b[3];
      } 
      { //write data.Plan[13].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].Longitude;
         raw[692] = u.b[0];
         raw[693] = u.b[1];
         raw[694] = u.b[2];
         raw[695] = u.b[3];
      } 
      { //write data.Plan[13].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].Altitude;
         raw[696] = u.b[0];
         raw[697] = u.b[1];
         raw[698] = u.b[2];
         raw[699] = u.b[3];
      } 
      { //write data.Plan[13].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].GrndSpd;
         raw[700] = u.b[0];
         raw[701] = u.b[1];
         raw[702] = u.b[2];
         raw[703] = u.b[3];
      } 
      { //write data.Plan[13].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].GroundTrack;
         raw[704] = u.b[0];
         raw[705] = u.b[1];
         raw[706] = u.b[2];
         raw[707] = u.b[3];
      } 
      { //write data.Plan[13].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[13].TOA;
         raw[708] = u.b[0];
         raw[709] = u.b[1];
         raw[710] = u.b[2];
         raw[711] = u.b[3];
         raw[712] = u.b[4];
         raw[713] = u.b[5];
         raw[714] = u.b[6];
         raw[715] = u.b[7];
      } 
      { //write data.Plan[13].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].TurnRadius;
         raw[716] = u.b[0];
         raw[717] = u.b[1];
         raw[718] = u.b[2];
         raw[719] = u.b[3];
      } 
      { //write data.Plan[13].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].TurnAngle;
         raw[720] = u.b[0];
         raw[721] = u.b[1];
         raw[722] = u.b[2];
         raw[723] = u.b[3];
      } 
      { //write data.Plan[13].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[13].VerticalSpd;
         raw[724] = u.b[0];
         raw[725] = u.b[1];
         raw[726] = u.b[2];
         raw[727] = u.b[3];
      } 
      { //write data.Plan[14].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[14].ID;
         raw[728] = u.b[0];
         raw[729] = u.b[1];
      } 
      { //write data.Plan[14].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[14].NextWyPtID;
         raw[730] = u.b[0];
         raw[731] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[14].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[732] = u.b[0];
         raw[733] = u.b[1];
         raw[734] = u.b[2];
         raw[735] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[14].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[736] = u.b[0];
         raw[737] = u.b[1];
         raw[738] = u.b[2];
         raw[739] = u.b[3];
      } 
      { //write data.Plan[14].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].Latitude;
         raw[740] = u.b[0];
         raw[741] = u.b[1];
         raw[742] = u.b[2];
         raw[743] = u.b[3];
      } 
      { //write data.Plan[14].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].Longitude;
         raw[744] = u.b[0];
         raw[745] = u.b[1];
         raw[746] = u.b[2];
         raw[747] = u.b[3];
      } 
      { //write data.Plan[14].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].Altitude;
         raw[748] = u.b[0];
         raw[749] = u.b[1];
         raw[750] = u.b[2];
         raw[751] = u.b[3];
      } 
      { //write data.Plan[14].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].GrndSpd;
         raw[752] = u.b[0];
         raw[753] = u.b[1];
         raw[754] = u.b[2];
         raw[755] = u.b[3];
      } 
      { //write data.Plan[14].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].GroundTrack;
         raw[756] = u.b[0];
         raw[757] = u.b[1];
         raw[758] = u.b[2];
         raw[759] = u.b[3];
      } 
      { //write data.Plan[14].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[14].TOA;
         raw[760] = u.b[0];
         raw[761] = u.b[1];
         raw[762] = u.b[2];
         raw[763] = u.b[3];
         raw[764] = u.b[4];
         raw[765] = u.b[5];
         raw[766] = u.b[6];
         raw[767] = u.b[7];
      } 
      { //write data.Plan[14].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].TurnRadius;
         raw[768] = u.b[0];
         raw[769] = u.b[1];
         raw[770] = u.b[2];
         raw[771] = u.b[3];
      } 
      { //write data.Plan[14].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].TurnAngle;
         raw[772] = u.b[0];
         raw[773] = u.b[1];
         raw[774] = u.b[2];
         raw[775] = u.b[3];
      } 
      { //write data.Plan[14].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[14].VerticalSpd;
         raw[776] = u.b[0];
         raw[777] = u.b[1];
         raw[778] = u.b[2];
         raw[779] = u.b[3];
      } 
      { //write data.Plan[15].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[15].ID;
         raw[780] = u.b[0];
         raw[781] = u.b[1];
      } 
      { //write data.Plan[15].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[15].NextWyPtID;
         raw[782] = u.b[0];
         raw[783] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[15].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[784] = u.b[0];
         raw[785] = u.b[1];
         raw[786] = u.b[2];
         raw[787] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[15].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[788] = u.b[0];
         raw[789] = u.b[1];
         raw[790] = u.b[2];
         raw[791] = u.b[3];
      } 
      { //write data.Plan[15].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].Latitude;
         raw[792] = u.b[0];
         raw[793] = u.b[1];
         raw[794] = u.b[2];
         raw[795] = u.b[3];
      } 
      { //write data.Plan[15].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].Longitude;
         raw[796] = u.b[0];
         raw[797] = u.b[1];
         raw[798] = u.b[2];
         raw[799] = u.b[3];
      } 
      { //write data.Plan[15].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].Altitude;
         raw[800] = u.b[0];
         raw[801] = u.b[1];
         raw[802] = u.b[2];
         raw[803] = u.b[3];
      } 
      { //write data.Plan[15].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].GrndSpd;
         raw[804] = u.b[0];
         raw[805] = u.b[1];
         raw[806] = u.b[2];
         raw[807] = u.b[3];
      } 
      { //write data.Plan[15].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].GroundTrack;
         raw[808] = u.b[0];
         raw[809] = u.b[1];
         raw[810] = u.b[2];
         raw[811] = u.b[3];
      } 
      { //write data.Plan[15].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[15].TOA;
         raw[812] = u.b[0];
         raw[813] = u.b[1];
         raw[814] = u.b[2];
         raw[815] = u.b[3];
         raw[816] = u.b[4];
         raw[817] = u.b[5];
         raw[818] = u.b[6];
         raw[819] = u.b[7];
      } 
      { //write data.Plan[15].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].TurnRadius;
         raw[820] = u.b[0];
         raw[821] = u.b[1];
         raw[822] = u.b[2];
         raw[823] = u.b[3];
      } 
      { //write data.Plan[15].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].TurnAngle;
         raw[824] = u.b[0];
         raw[825] = u.b[1];
         raw[826] = u.b[2];
         raw[827] = u.b[3];
      } 
      { //write data.Plan[15].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[15].VerticalSpd;
         raw[828] = u.b[0];
         raw[829] = u.b[1];
         raw[830] = u.b[2];
         raw[831] = u.b[3];
      } 
      { //write data.Plan[16].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[16].ID;
         raw[832] = u.b[0];
         raw[833] = u.b[1];
      } 
      { //write data.Plan[16].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[16].NextWyPtID;
         raw[834] = u.b[0];
         raw[835] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[16].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[836] = u.b[0];
         raw[837] = u.b[1];
         raw[838] = u.b[2];
         raw[839] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[16].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[840] = u.b[0];
         raw[841] = u.b[1];
         raw[842] = u.b[2];
         raw[843] = u.b[3];
      } 
      { //write data.Plan[16].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].Latitude;
         raw[844] = u.b[0];
         raw[845] = u.b[1];
         raw[846] = u.b[2];
         raw[847] = u.b[3];
      } 
      { //write data.Plan[16].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].Longitude;
         raw[848] = u.b[0];
         raw[849] = u.b[1];
         raw[850] = u.b[2];
         raw[851] = u.b[3];
      } 
      { //write data.Plan[16].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].Altitude;
         raw[852] = u.b[0];
         raw[853] = u.b[1];
         raw[854] = u.b[2];
         raw[855] = u.b[3];
      } 
      { //write data.Plan[16].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].GrndSpd;
         raw[856] = u.b[0];
         raw[857] = u.b[1];
         raw[858] = u.b[2];
         raw[859] = u.b[3];
      } 
      { //write data.Plan[16].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].GroundTrack;
         raw[860] = u.b[0];
         raw[861] = u.b[1];
         raw[862] = u.b[2];
         raw[863] = u.b[3];
      } 
      { //write data.Plan[16].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[16].TOA;
         raw[864] = u.b[0];
         raw[865] = u.b[1];
         raw[866] = u.b[2];
         raw[867] = u.b[3];
         raw[868] = u.b[4];
         raw[869] = u.b[5];
         raw[870] = u.b[6];
         raw[871] = u.b[7];
      } 
      { //write data.Plan[16].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].TurnRadius;
         raw[872] = u.b[0];
         raw[873] = u.b[1];
         raw[874] = u.b[2];
         raw[875] = u.b[3];
      } 
      { //write data.Plan[16].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].TurnAngle;
         raw[876] = u.b[0];
         raw[877] = u.b[1];
         raw[878] = u.b[2];
         raw[879] = u.b[3];
      } 
      { //write data.Plan[16].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[16].VerticalSpd;
         raw[880] = u.b[0];
         raw[881] = u.b[1];
         raw[882] = u.b[2];
         raw[883] = u.b[3];
      } 
      { //write data.Plan[17].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[17].ID;
         raw[884] = u.b[0];
         raw[885] = u.b[1];
      } 
      { //write data.Plan[17].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[17].NextWyPtID;
         raw[886] = u.b[0];
         raw[887] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[17].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[888] = u.b[0];
         raw[889] = u.b[1];
         raw[890] = u.b[2];
         raw[891] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[17].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[892] = u.b[0];
         raw[893] = u.b[1];
         raw[894] = u.b[2];
         raw[895] = u.b[3];
      } 
      { //write data.Plan[17].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].Latitude;
         raw[896] = u.b[0];
         raw[897] = u.b[1];
         raw[898] = u.b[2];
         raw[899] = u.b[3];
      } 
      { //write data.Plan[17].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].Longitude;
         raw[900] = u.b[0];
         raw[901] = u.b[1];
         raw[902] = u.b[2];
         raw[903] = u.b[3];
      } 
      { //write data.Plan[17].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].Altitude;
         raw[904] = u.b[0];
         raw[905] = u.b[1];
         raw[906] = u.b[2];
         raw[907] = u.b[3];
      } 
      { //write data.Plan[17].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].GrndSpd;
         raw[908] = u.b[0];
         raw[909] = u.b[1];
         raw[910] = u.b[2];
         raw[911] = u.b[3];
      } 
      { //write data.Plan[17].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].GroundTrack;
         raw[912] = u.b[0];
         raw[913] = u.b[1];
         raw[914] = u.b[2];
         raw[915] = u.b[3];
      } 
      { //write data.Plan[17].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[17].TOA;
         raw[916] = u.b[0];
         raw[917] = u.b[1];
         raw[918] = u.b[2];
         raw[919] = u.b[3];
         raw[920] = u.b[4];
         raw[921] = u.b[5];
         raw[922] = u.b[6];
         raw[923] = u.b[7];
      } 
      { //write data.Plan[17].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].TurnRadius;
         raw[924] = u.b[0];
         raw[925] = u.b[1];
         raw[926] = u.b[2];
         raw[927] = u.b[3];
      } 
      { //write data.Plan[17].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].TurnAngle;
         raw[928] = u.b[0];
         raw[929] = u.b[1];
         raw[930] = u.b[2];
         raw[931] = u.b[3];
      } 
      { //write data.Plan[17].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[17].VerticalSpd;
         raw[932] = u.b[0];
         raw[933] = u.b[1];
         raw[934] = u.b[2];
         raw[935] = u.b[3];
      } 
      { //write data.Plan[18].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[18].ID;
         raw[936] = u.b[0];
         raw[937] = u.b[1];
      } 
      { //write data.Plan[18].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[18].NextWyPtID;
         raw[938] = u.b[0];
         raw[939] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[18].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[940] = u.b[0];
         raw[941] = u.b[1];
         raw[942] = u.b[2];
         raw[943] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[18].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[944] = u.b[0];
         raw[945] = u.b[1];
         raw[946] = u.b[2];
         raw[947] = u.b[3];
      } 
      { //write data.Plan[18].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].Latitude;
         raw[948] = u.b[0];
         raw[949] = u.b[1];
         raw[950] = u.b[2];
         raw[951] = u.b[3];
      } 
      { //write data.Plan[18].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].Longitude;
         raw[952] = u.b[0];
         raw[953] = u.b[1];
         raw[954] = u.b[2];
         raw[955] = u.b[3];
      } 
      { //write data.Plan[18].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].Altitude;
         raw[956] = u.b[0];
         raw[957] = u.b[1];
         raw[958] = u.b[2];
         raw[959] = u.b[3];
      } 
      { //write data.Plan[18].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].GrndSpd;
         raw[960] = u.b[0];
         raw[961] = u.b[1];
         raw[962] = u.b[2];
         raw[963] = u.b[3];
      } 
      { //write data.Plan[18].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].GroundTrack;
         raw[964] = u.b[0];
         raw[965] = u.b[1];
         raw[966] = u.b[2];
         raw[967] = u.b[3];
      } 
      { //write data.Plan[18].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[18].TOA;
         raw[968] = u.b[0];
         raw[969] = u.b[1];
         raw[970] = u.b[2];
         raw[971] = u.b[3];
         raw[972] = u.b[4];
         raw[973] = u.b[5];
         raw[974] = u.b[6];
         raw[975] = u.b[7];
      } 
      { //write data.Plan[18].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].TurnRadius;
         raw[976] = u.b[0];
         raw[977] = u.b[1];
         raw[978] = u.b[2];
         raw[979] = u.b[3];
      } 
      { //write data.Plan[18].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].TurnAngle;
         raw[980] = u.b[0];
         raw[981] = u.b[1];
         raw[982] = u.b[2];
         raw[983] = u.b[3];
      } 
      { //write data.Plan[18].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[18].VerticalSpd;
         raw[984] = u.b[0];
         raw[985] = u.b[1];
         raw[986] = u.b[2];
         raw[987] = u.b[3];
      } 
      { //write data.Plan[19].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[19].ID;
         raw[988] = u.b[0];
         raw[989] = u.b[1];
      } 
      { //write data.Plan[19].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.Plan[19].NextWyPtID;
         raw[990] = u.b[0];
         raw[991] = u.b[1];
      } 
      { //write enum WayPointType
         uint32_t ev;
         switch(data.Plan[19].Type) {
         case WayPointType::Unknown:
            ev = 0;
            break;
         case WayPointType::Taxi:
            ev = 1;
            break;
         case WayPointType::Cruise:
            ev = 2;
            break;
         case WayPointType::Descent:
            ev = 3;
            break;
         case WayPointType::Ascent:
            ev = 4;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[992] = u.b[0];
         raw[993] = u.b[1];
         raw[994] = u.b[2];
         raw[995] = u.b[3];
      } 
      { //write enum WayPointType2
         uint32_t ev;
         switch(data.Plan[19].Type2) {
         case WayPointType2::FlyBy:
            ev = 0;
            break;
         case WayPointType2::FlyOver:
            ev = 1;
            break;
         case WayPointType2::FlyByLargeTrnAng:
            ev = 2;
            break;
         }
         union {
            uint32_t a;
            char b[4];
         } u;
         u.a = ev;
         raw[996] = u.b[0];
         raw[997] = u.b[1];
         raw[998] = u.b[2];
         raw[999] = u.b[3];
      } 
      { //write data.Plan[19].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].Latitude;
         raw[1000] = u.b[0];
         raw[1001] = u.b[1];
         raw[1002] = u.b[2];
         raw[1003] = u.b[3];
      } 
      { //write data.Plan[19].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].Longitude;
         raw[1004] = u.b[0];
         raw[1005] = u.b[1];
         raw[1006] = u.b[2];
         raw[1007] = u.b[3];
      } 
      { //write data.Plan[19].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].Altitude;
         raw[1008] = u.b[0];
         raw[1009] = u.b[1];
         raw[1010] = u.b[2];
         raw[1011] = u.b[3];
      } 
      { //write data.Plan[19].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].GrndSpd;
         raw[1012] = u.b[0];
         raw[1013] = u.b[1];
         raw[1014] = u.b[2];
         raw[1015] = u.b[3];
      } 
      { //write data.Plan[19].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].GroundTrack;
         raw[1016] = u.b[0];
         raw[1017] = u.b[1];
         raw[1018] = u.b[2];
         raw[1019] = u.b[3];
      } 
      { //write data.Plan[19].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.a = data.Plan[19].TOA;
         raw[1020] = u.b[0];
         raw[1021] = u.b[1];
         raw[1022] = u.b[2];
         raw[1023] = u.b[3];
         raw[1024] = u.b[4];
         raw[1025] = u.b[5];
         raw[1026] = u.b[6];
         raw[1027] = u.b[7];
      } 
      { //write data.Plan[19].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].TurnRadius;
         raw[1028] = u.b[0];
         raw[1029] = u.b[1];
         raw[1030] = u.b[2];
         raw[1031] = u.b[3];
      } 
      { //write data.Plan[19].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].TurnAngle;
         raw[1032] = u.b[0];
         raw[1033] = u.b[1];
         raw[1034] = u.b[2];
         raw[1035] = u.b[3];
      } 
      { //write data.Plan[19].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.a = data.Plan[19].VerticalSpd;
         raw[1036] = u.b[0];
         raw[1037] = u.b[1];
         raw[1038] = u.b[2];
         raw[1039] = u.b[3];
      } 
      { //write data.NumPoints        uint8        1 bytes
         union { uint8_t a;  char b[1]; } u;
         u.a = data.NumPoints;
         raw[1040] = u.b[0];
      } 
      { //write data.CurrWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.a = data.CurrWyPtID;
         raw[1041] = u.b[0];
         raw[1042] = u.b[1];
      } 
   }

//NEW -- Reader functions
   void read_LatLonAltPoint(LatLonAltPoint& data, const LatLonAltPoint_buffer& raw) {
      { //READ data.Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[0];
         u.b[1] = raw[1];
         u.b[2] = raw[2];
         u.b[3] = raw[3];
         data.Latitude = u.a;
      } 
      { //READ data.Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[4];
         u.b[1] = raw[5];
         u.b[2] = raw[6];
         u.b[3] = raw[7];
         data.Longitude = u.a;
      } 
      { //READ data.Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[8];
         u.b[1] = raw[9];
         u.b[2] = raw[10];
         u.b[3] = raw[11];
         data.Altitude = u.a;
      } 
   }
   void read_GuidancePoint(GuidancePoint& data, const GuidancePoint_buffer& raw) {
      { //READ data.ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[0];
         u.b[1] = raw[1];
         data.ID = u.a;
      } 
      { //READ data.NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[2];
         u.b[1] = raw[3];
         data.NextWyPtID = u.a;
      } 
      { //READ enum data.Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[4];
         u.b[1] = raw[5];
         u.b[2] = raw[6];
         u.b[3] = raw[7];
        switch(u.a) {
         case 0:
            data.Type = WayPointType::Unknown;
            break;
         case 1:
            data.Type = WayPointType::Taxi;
            break;
         case 2:
            data.Type = WayPointType::Cruise;
            break;
         case 3:
            data.Type = WayPointType::Descent;
            break;
         case 4:
            data.Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[8];
         u.b[1] = raw[9];
         u.b[2] = raw[10];
         u.b[3] = raw[11];
        switch(u.a) {
         case 0:
            data.Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[12];
         u.b[1] = raw[13];
         u.b[2] = raw[14];
         u.b[3] = raw[15];
         data.Latitude = u.a;
      } 
      { //READ data.Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[16];
         u.b[1] = raw[17];
         u.b[2] = raw[18];
         u.b[3] = raw[19];
         data.Longitude = u.a;
      } 
      { //READ data.Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[20];
         u.b[1] = raw[21];
         u.b[2] = raw[22];
         u.b[3] = raw[23];
         data.Altitude = u.a;
      } 
      { //READ data.GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[24];
         u.b[1] = raw[25];
         u.b[2] = raw[26];
         u.b[3] = raw[27];
         data.GrndSpd = u.a;
      } 
      { //READ data.GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[28];
         u.b[1] = raw[29];
         u.b[2] = raw[30];
         u.b[3] = raw[31];
         data.GroundTrack = u.a;
      } 
      { //READ data.TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[32];
         u.b[1] = raw[33];
         u.b[2] = raw[34];
         u.b[3] = raw[35];
         u.b[4] = raw[36];
         u.b[5] = raw[37];
         u.b[6] = raw[38];
         u.b[7] = raw[39];
         data.TOA = u.a;
      } 
      { //READ data.TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[40];
         u.b[1] = raw[41];
         u.b[2] = raw[42];
         u.b[3] = raw[43];
         data.TurnRadius = u.a;
      } 
      { //READ data.TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[44];
         u.b[1] = raw[45];
         u.b[2] = raw[46];
         u.b[3] = raw[47];
         data.TurnAngle = u.a;
      } 
      { //READ data.VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[48];
         u.b[1] = raw[49];
         u.b[2] = raw[50];
         u.b[3] = raw[51];
         data.VerticalSpd = u.a;
      } 
   }
   void read_MissionPlan(MissionPlan& data, const MissionPlan_buffer& raw) {
      { //READ data.Plan[0].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[0];
         u.b[1] = raw[1];
         data.Plan[0].ID = u.a;
      } 
      { //READ data.Plan[0].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[2];
         u.b[1] = raw[3];
         data.Plan[0].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[0].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[4];
         u.b[1] = raw[5];
         u.b[2] = raw[6];
         u.b[3] = raw[7];
        switch(u.a) {
         case 0:
            data.Plan[0].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[0].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[0].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[0].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[0].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[0].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[8];
         u.b[1] = raw[9];
         u.b[2] = raw[10];
         u.b[3] = raw[11];
        switch(u.a) {
         case 0:
            data.Plan[0].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[0].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[0].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[0].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[12];
         u.b[1] = raw[13];
         u.b[2] = raw[14];
         u.b[3] = raw[15];
         data.Plan[0].Latitude = u.a;
      } 
      { //READ data.Plan[0].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[16];
         u.b[1] = raw[17];
         u.b[2] = raw[18];
         u.b[3] = raw[19];
         data.Plan[0].Longitude = u.a;
      } 
      { //READ data.Plan[0].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[20];
         u.b[1] = raw[21];
         u.b[2] = raw[22];
         u.b[3] = raw[23];
         data.Plan[0].Altitude = u.a;
      } 
      { //READ data.Plan[0].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[24];
         u.b[1] = raw[25];
         u.b[2] = raw[26];
         u.b[3] = raw[27];
         data.Plan[0].GrndSpd = u.a;
      } 
      { //READ data.Plan[0].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[28];
         u.b[1] = raw[29];
         u.b[2] = raw[30];
         u.b[3] = raw[31];
         data.Plan[0].GroundTrack = u.a;
      } 
      { //READ data.Plan[0].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[32];
         u.b[1] = raw[33];
         u.b[2] = raw[34];
         u.b[3] = raw[35];
         u.b[4] = raw[36];
         u.b[5] = raw[37];
         u.b[6] = raw[38];
         u.b[7] = raw[39];
         data.Plan[0].TOA = u.a;
      } 
      { //READ data.Plan[0].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[40];
         u.b[1] = raw[41];
         u.b[2] = raw[42];
         u.b[3] = raw[43];
         data.Plan[0].TurnRadius = u.a;
      } 
      { //READ data.Plan[0].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[44];
         u.b[1] = raw[45];
         u.b[2] = raw[46];
         u.b[3] = raw[47];
         data.Plan[0].TurnAngle = u.a;
      } 
      { //READ data.Plan[0].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[48];
         u.b[1] = raw[49];
         u.b[2] = raw[50];
         u.b[3] = raw[51];
         data.Plan[0].VerticalSpd = u.a;
      } 
      { //READ data.Plan[1].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[52];
         u.b[1] = raw[53];
         data.Plan[1].ID = u.a;
      } 
      { //READ data.Plan[1].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[54];
         u.b[1] = raw[55];
         data.Plan[1].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[1].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[56];
         u.b[1] = raw[57];
         u.b[2] = raw[58];
         u.b[3] = raw[59];
        switch(u.a) {
         case 0:
            data.Plan[1].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[1].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[1].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[1].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[1].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[1].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[60];
         u.b[1] = raw[61];
         u.b[2] = raw[62];
         u.b[3] = raw[63];
        switch(u.a) {
         case 0:
            data.Plan[1].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[1].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[1].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[1].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[64];
         u.b[1] = raw[65];
         u.b[2] = raw[66];
         u.b[3] = raw[67];
         data.Plan[1].Latitude = u.a;
      } 
      { //READ data.Plan[1].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[68];
         u.b[1] = raw[69];
         u.b[2] = raw[70];
         u.b[3] = raw[71];
         data.Plan[1].Longitude = u.a;
      } 
      { //READ data.Plan[1].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[72];
         u.b[1] = raw[73];
         u.b[2] = raw[74];
         u.b[3] = raw[75];
         data.Plan[1].Altitude = u.a;
      } 
      { //READ data.Plan[1].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[76];
         u.b[1] = raw[77];
         u.b[2] = raw[78];
         u.b[3] = raw[79];
         data.Plan[1].GrndSpd = u.a;
      } 
      { //READ data.Plan[1].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[80];
         u.b[1] = raw[81];
         u.b[2] = raw[82];
         u.b[3] = raw[83];
         data.Plan[1].GroundTrack = u.a;
      } 
      { //READ data.Plan[1].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[84];
         u.b[1] = raw[85];
         u.b[2] = raw[86];
         u.b[3] = raw[87];
         u.b[4] = raw[88];
         u.b[5] = raw[89];
         u.b[6] = raw[90];
         u.b[7] = raw[91];
         data.Plan[1].TOA = u.a;
      } 
      { //READ data.Plan[1].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[92];
         u.b[1] = raw[93];
         u.b[2] = raw[94];
         u.b[3] = raw[95];
         data.Plan[1].TurnRadius = u.a;
      } 
      { //READ data.Plan[1].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[96];
         u.b[1] = raw[97];
         u.b[2] = raw[98];
         u.b[3] = raw[99];
         data.Plan[1].TurnAngle = u.a;
      } 
      { //READ data.Plan[1].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[100];
         u.b[1] = raw[101];
         u.b[2] = raw[102];
         u.b[3] = raw[103];
         data.Plan[1].VerticalSpd = u.a;
      } 
      { //READ data.Plan[2].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[104];
         u.b[1] = raw[105];
         data.Plan[2].ID = u.a;
      } 
      { //READ data.Plan[2].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[106];
         u.b[1] = raw[107];
         data.Plan[2].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[2].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[108];
         u.b[1] = raw[109];
         u.b[2] = raw[110];
         u.b[3] = raw[111];
        switch(u.a) {
         case 0:
            data.Plan[2].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[2].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[2].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[2].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[2].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[2].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[112];
         u.b[1] = raw[113];
         u.b[2] = raw[114];
         u.b[3] = raw[115];
        switch(u.a) {
         case 0:
            data.Plan[2].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[2].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[2].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[2].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[116];
         u.b[1] = raw[117];
         u.b[2] = raw[118];
         u.b[3] = raw[119];
         data.Plan[2].Latitude = u.a;
      } 
      { //READ data.Plan[2].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[120];
         u.b[1] = raw[121];
         u.b[2] = raw[122];
         u.b[3] = raw[123];
         data.Plan[2].Longitude = u.a;
      } 
      { //READ data.Plan[2].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[124];
         u.b[1] = raw[125];
         u.b[2] = raw[126];
         u.b[3] = raw[127];
         data.Plan[2].Altitude = u.a;
      } 
      { //READ data.Plan[2].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[128];
         u.b[1] = raw[129];
         u.b[2] = raw[130];
         u.b[3] = raw[131];
         data.Plan[2].GrndSpd = u.a;
      } 
      { //READ data.Plan[2].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[132];
         u.b[1] = raw[133];
         u.b[2] = raw[134];
         u.b[3] = raw[135];
         data.Plan[2].GroundTrack = u.a;
      } 
      { //READ data.Plan[2].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[136];
         u.b[1] = raw[137];
         u.b[2] = raw[138];
         u.b[3] = raw[139];
         u.b[4] = raw[140];
         u.b[5] = raw[141];
         u.b[6] = raw[142];
         u.b[7] = raw[143];
         data.Plan[2].TOA = u.a;
      } 
      { //READ data.Plan[2].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[144];
         u.b[1] = raw[145];
         u.b[2] = raw[146];
         u.b[3] = raw[147];
         data.Plan[2].TurnRadius = u.a;
      } 
      { //READ data.Plan[2].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[148];
         u.b[1] = raw[149];
         u.b[2] = raw[150];
         u.b[3] = raw[151];
         data.Plan[2].TurnAngle = u.a;
      } 
      { //READ data.Plan[2].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[152];
         u.b[1] = raw[153];
         u.b[2] = raw[154];
         u.b[3] = raw[155];
         data.Plan[2].VerticalSpd = u.a;
      } 
      { //READ data.Plan[3].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[156];
         u.b[1] = raw[157];
         data.Plan[3].ID = u.a;
      } 
      { //READ data.Plan[3].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[158];
         u.b[1] = raw[159];
         data.Plan[3].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[3].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[160];
         u.b[1] = raw[161];
         u.b[2] = raw[162];
         u.b[3] = raw[163];
        switch(u.a) {
         case 0:
            data.Plan[3].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[3].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[3].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[3].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[3].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[3].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[164];
         u.b[1] = raw[165];
         u.b[2] = raw[166];
         u.b[3] = raw[167];
        switch(u.a) {
         case 0:
            data.Plan[3].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[3].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[3].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[3].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[168];
         u.b[1] = raw[169];
         u.b[2] = raw[170];
         u.b[3] = raw[171];
         data.Plan[3].Latitude = u.a;
      } 
      { //READ data.Plan[3].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[172];
         u.b[1] = raw[173];
         u.b[2] = raw[174];
         u.b[3] = raw[175];
         data.Plan[3].Longitude = u.a;
      } 
      { //READ data.Plan[3].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[176];
         u.b[1] = raw[177];
         u.b[2] = raw[178];
         u.b[3] = raw[179];
         data.Plan[3].Altitude = u.a;
      } 
      { //READ data.Plan[3].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[180];
         u.b[1] = raw[181];
         u.b[2] = raw[182];
         u.b[3] = raw[183];
         data.Plan[3].GrndSpd = u.a;
      } 
      { //READ data.Plan[3].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[184];
         u.b[1] = raw[185];
         u.b[2] = raw[186];
         u.b[3] = raw[187];
         data.Plan[3].GroundTrack = u.a;
      } 
      { //READ data.Plan[3].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[188];
         u.b[1] = raw[189];
         u.b[2] = raw[190];
         u.b[3] = raw[191];
         u.b[4] = raw[192];
         u.b[5] = raw[193];
         u.b[6] = raw[194];
         u.b[7] = raw[195];
         data.Plan[3].TOA = u.a;
      } 
      { //READ data.Plan[3].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[196];
         u.b[1] = raw[197];
         u.b[2] = raw[198];
         u.b[3] = raw[199];
         data.Plan[3].TurnRadius = u.a;
      } 
      { //READ data.Plan[3].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[200];
         u.b[1] = raw[201];
         u.b[2] = raw[202];
         u.b[3] = raw[203];
         data.Plan[3].TurnAngle = u.a;
      } 
      { //READ data.Plan[3].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[204];
         u.b[1] = raw[205];
         u.b[2] = raw[206];
         u.b[3] = raw[207];
         data.Plan[3].VerticalSpd = u.a;
      } 
      { //READ data.Plan[4].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[208];
         u.b[1] = raw[209];
         data.Plan[4].ID = u.a;
      } 
      { //READ data.Plan[4].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[210];
         u.b[1] = raw[211];
         data.Plan[4].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[4].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[212];
         u.b[1] = raw[213];
         u.b[2] = raw[214];
         u.b[3] = raw[215];
        switch(u.a) {
         case 0:
            data.Plan[4].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[4].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[4].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[4].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[4].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[4].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[216];
         u.b[1] = raw[217];
         u.b[2] = raw[218];
         u.b[3] = raw[219];
        switch(u.a) {
         case 0:
            data.Plan[4].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[4].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[4].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[4].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[220];
         u.b[1] = raw[221];
         u.b[2] = raw[222];
         u.b[3] = raw[223];
         data.Plan[4].Latitude = u.a;
      } 
      { //READ data.Plan[4].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[224];
         u.b[1] = raw[225];
         u.b[2] = raw[226];
         u.b[3] = raw[227];
         data.Plan[4].Longitude = u.a;
      } 
      { //READ data.Plan[4].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[228];
         u.b[1] = raw[229];
         u.b[2] = raw[230];
         u.b[3] = raw[231];
         data.Plan[4].Altitude = u.a;
      } 
      { //READ data.Plan[4].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[232];
         u.b[1] = raw[233];
         u.b[2] = raw[234];
         u.b[3] = raw[235];
         data.Plan[4].GrndSpd = u.a;
      } 
      { //READ data.Plan[4].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[236];
         u.b[1] = raw[237];
         u.b[2] = raw[238];
         u.b[3] = raw[239];
         data.Plan[4].GroundTrack = u.a;
      } 
      { //READ data.Plan[4].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[240];
         u.b[1] = raw[241];
         u.b[2] = raw[242];
         u.b[3] = raw[243];
         u.b[4] = raw[244];
         u.b[5] = raw[245];
         u.b[6] = raw[246];
         u.b[7] = raw[247];
         data.Plan[4].TOA = u.a;
      } 
      { //READ data.Plan[4].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[248];
         u.b[1] = raw[249];
         u.b[2] = raw[250];
         u.b[3] = raw[251];
         data.Plan[4].TurnRadius = u.a;
      } 
      { //READ data.Plan[4].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[252];
         u.b[1] = raw[253];
         u.b[2] = raw[254];
         u.b[3] = raw[255];
         data.Plan[4].TurnAngle = u.a;
      } 
      { //READ data.Plan[4].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[256];
         u.b[1] = raw[257];
         u.b[2] = raw[258];
         u.b[3] = raw[259];
         data.Plan[4].VerticalSpd = u.a;
      } 
      { //READ data.Plan[5].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[260];
         u.b[1] = raw[261];
         data.Plan[5].ID = u.a;
      } 
      { //READ data.Plan[5].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[262];
         u.b[1] = raw[263];
         data.Plan[5].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[5].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[264];
         u.b[1] = raw[265];
         u.b[2] = raw[266];
         u.b[3] = raw[267];
        switch(u.a) {
         case 0:
            data.Plan[5].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[5].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[5].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[5].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[5].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[5].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[268];
         u.b[1] = raw[269];
         u.b[2] = raw[270];
         u.b[3] = raw[271];
        switch(u.a) {
         case 0:
            data.Plan[5].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[5].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[5].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[5].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[272];
         u.b[1] = raw[273];
         u.b[2] = raw[274];
         u.b[3] = raw[275];
         data.Plan[5].Latitude = u.a;
      } 
      { //READ data.Plan[5].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[276];
         u.b[1] = raw[277];
         u.b[2] = raw[278];
         u.b[3] = raw[279];
         data.Plan[5].Longitude = u.a;
      } 
      { //READ data.Plan[5].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[280];
         u.b[1] = raw[281];
         u.b[2] = raw[282];
         u.b[3] = raw[283];
         data.Plan[5].Altitude = u.a;
      } 
      { //READ data.Plan[5].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[284];
         u.b[1] = raw[285];
         u.b[2] = raw[286];
         u.b[3] = raw[287];
         data.Plan[5].GrndSpd = u.a;
      } 
      { //READ data.Plan[5].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[288];
         u.b[1] = raw[289];
         u.b[2] = raw[290];
         u.b[3] = raw[291];
         data.Plan[5].GroundTrack = u.a;
      } 
      { //READ data.Plan[5].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[292];
         u.b[1] = raw[293];
         u.b[2] = raw[294];
         u.b[3] = raw[295];
         u.b[4] = raw[296];
         u.b[5] = raw[297];
         u.b[6] = raw[298];
         u.b[7] = raw[299];
         data.Plan[5].TOA = u.a;
      } 
      { //READ data.Plan[5].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[300];
         u.b[1] = raw[301];
         u.b[2] = raw[302];
         u.b[3] = raw[303];
         data.Plan[5].TurnRadius = u.a;
      } 
      { //READ data.Plan[5].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[304];
         u.b[1] = raw[305];
         u.b[2] = raw[306];
         u.b[3] = raw[307];
         data.Plan[5].TurnAngle = u.a;
      } 
      { //READ data.Plan[5].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[308];
         u.b[1] = raw[309];
         u.b[2] = raw[310];
         u.b[3] = raw[311];
         data.Plan[5].VerticalSpd = u.a;
      } 
      { //READ data.Plan[6].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[312];
         u.b[1] = raw[313];
         data.Plan[6].ID = u.a;
      } 
      { //READ data.Plan[6].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[314];
         u.b[1] = raw[315];
         data.Plan[6].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[6].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[316];
         u.b[1] = raw[317];
         u.b[2] = raw[318];
         u.b[3] = raw[319];
        switch(u.a) {
         case 0:
            data.Plan[6].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[6].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[6].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[6].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[6].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[6].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[320];
         u.b[1] = raw[321];
         u.b[2] = raw[322];
         u.b[3] = raw[323];
        switch(u.a) {
         case 0:
            data.Plan[6].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[6].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[6].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[6].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[324];
         u.b[1] = raw[325];
         u.b[2] = raw[326];
         u.b[3] = raw[327];
         data.Plan[6].Latitude = u.a;
      } 
      { //READ data.Plan[6].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[328];
         u.b[1] = raw[329];
         u.b[2] = raw[330];
         u.b[3] = raw[331];
         data.Plan[6].Longitude = u.a;
      } 
      { //READ data.Plan[6].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[332];
         u.b[1] = raw[333];
         u.b[2] = raw[334];
         u.b[3] = raw[335];
         data.Plan[6].Altitude = u.a;
      } 
      { //READ data.Plan[6].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[336];
         u.b[1] = raw[337];
         u.b[2] = raw[338];
         u.b[3] = raw[339];
         data.Plan[6].GrndSpd = u.a;
      } 
      { //READ data.Plan[6].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[340];
         u.b[1] = raw[341];
         u.b[2] = raw[342];
         u.b[3] = raw[343];
         data.Plan[6].GroundTrack = u.a;
      } 
      { //READ data.Plan[6].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[344];
         u.b[1] = raw[345];
         u.b[2] = raw[346];
         u.b[3] = raw[347];
         u.b[4] = raw[348];
         u.b[5] = raw[349];
         u.b[6] = raw[350];
         u.b[7] = raw[351];
         data.Plan[6].TOA = u.a;
      } 
      { //READ data.Plan[6].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[352];
         u.b[1] = raw[353];
         u.b[2] = raw[354];
         u.b[3] = raw[355];
         data.Plan[6].TurnRadius = u.a;
      } 
      { //READ data.Plan[6].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[356];
         u.b[1] = raw[357];
         u.b[2] = raw[358];
         u.b[3] = raw[359];
         data.Plan[6].TurnAngle = u.a;
      } 
      { //READ data.Plan[6].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[360];
         u.b[1] = raw[361];
         u.b[2] = raw[362];
         u.b[3] = raw[363];
         data.Plan[6].VerticalSpd = u.a;
      } 
      { //READ data.Plan[7].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[364];
         u.b[1] = raw[365];
         data.Plan[7].ID = u.a;
      } 
      { //READ data.Plan[7].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[366];
         u.b[1] = raw[367];
         data.Plan[7].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[7].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[368];
         u.b[1] = raw[369];
         u.b[2] = raw[370];
         u.b[3] = raw[371];
        switch(u.a) {
         case 0:
            data.Plan[7].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[7].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[7].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[7].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[7].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[7].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[372];
         u.b[1] = raw[373];
         u.b[2] = raw[374];
         u.b[3] = raw[375];
        switch(u.a) {
         case 0:
            data.Plan[7].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[7].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[7].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[7].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[376];
         u.b[1] = raw[377];
         u.b[2] = raw[378];
         u.b[3] = raw[379];
         data.Plan[7].Latitude = u.a;
      } 
      { //READ data.Plan[7].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[380];
         u.b[1] = raw[381];
         u.b[2] = raw[382];
         u.b[3] = raw[383];
         data.Plan[7].Longitude = u.a;
      } 
      { //READ data.Plan[7].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[384];
         u.b[1] = raw[385];
         u.b[2] = raw[386];
         u.b[3] = raw[387];
         data.Plan[7].Altitude = u.a;
      } 
      { //READ data.Plan[7].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[388];
         u.b[1] = raw[389];
         u.b[2] = raw[390];
         u.b[3] = raw[391];
         data.Plan[7].GrndSpd = u.a;
      } 
      { //READ data.Plan[7].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[392];
         u.b[1] = raw[393];
         u.b[2] = raw[394];
         u.b[3] = raw[395];
         data.Plan[7].GroundTrack = u.a;
      } 
      { //READ data.Plan[7].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[396];
         u.b[1] = raw[397];
         u.b[2] = raw[398];
         u.b[3] = raw[399];
         u.b[4] = raw[400];
         u.b[5] = raw[401];
         u.b[6] = raw[402];
         u.b[7] = raw[403];
         data.Plan[7].TOA = u.a;
      } 
      { //READ data.Plan[7].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[404];
         u.b[1] = raw[405];
         u.b[2] = raw[406];
         u.b[3] = raw[407];
         data.Plan[7].TurnRadius = u.a;
      } 
      { //READ data.Plan[7].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[408];
         u.b[1] = raw[409];
         u.b[2] = raw[410];
         u.b[3] = raw[411];
         data.Plan[7].TurnAngle = u.a;
      } 
      { //READ data.Plan[7].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[412];
         u.b[1] = raw[413];
         u.b[2] = raw[414];
         u.b[3] = raw[415];
         data.Plan[7].VerticalSpd = u.a;
      } 
      { //READ data.Plan[8].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[416];
         u.b[1] = raw[417];
         data.Plan[8].ID = u.a;
      } 
      { //READ data.Plan[8].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[418];
         u.b[1] = raw[419];
         data.Plan[8].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[8].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[420];
         u.b[1] = raw[421];
         u.b[2] = raw[422];
         u.b[3] = raw[423];
        switch(u.a) {
         case 0:
            data.Plan[8].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[8].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[8].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[8].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[8].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[8].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[424];
         u.b[1] = raw[425];
         u.b[2] = raw[426];
         u.b[3] = raw[427];
        switch(u.a) {
         case 0:
            data.Plan[8].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[8].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[8].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[8].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[428];
         u.b[1] = raw[429];
         u.b[2] = raw[430];
         u.b[3] = raw[431];
         data.Plan[8].Latitude = u.a;
      } 
      { //READ data.Plan[8].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[432];
         u.b[1] = raw[433];
         u.b[2] = raw[434];
         u.b[3] = raw[435];
         data.Plan[8].Longitude = u.a;
      } 
      { //READ data.Plan[8].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[436];
         u.b[1] = raw[437];
         u.b[2] = raw[438];
         u.b[3] = raw[439];
         data.Plan[8].Altitude = u.a;
      } 
      { //READ data.Plan[8].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[440];
         u.b[1] = raw[441];
         u.b[2] = raw[442];
         u.b[3] = raw[443];
         data.Plan[8].GrndSpd = u.a;
      } 
      { //READ data.Plan[8].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[444];
         u.b[1] = raw[445];
         u.b[2] = raw[446];
         u.b[3] = raw[447];
         data.Plan[8].GroundTrack = u.a;
      } 
      { //READ data.Plan[8].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[448];
         u.b[1] = raw[449];
         u.b[2] = raw[450];
         u.b[3] = raw[451];
         u.b[4] = raw[452];
         u.b[5] = raw[453];
         u.b[6] = raw[454];
         u.b[7] = raw[455];
         data.Plan[8].TOA = u.a;
      } 
      { //READ data.Plan[8].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[456];
         u.b[1] = raw[457];
         u.b[2] = raw[458];
         u.b[3] = raw[459];
         data.Plan[8].TurnRadius = u.a;
      } 
      { //READ data.Plan[8].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[460];
         u.b[1] = raw[461];
         u.b[2] = raw[462];
         u.b[3] = raw[463];
         data.Plan[8].TurnAngle = u.a;
      } 
      { //READ data.Plan[8].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[464];
         u.b[1] = raw[465];
         u.b[2] = raw[466];
         u.b[3] = raw[467];
         data.Plan[8].VerticalSpd = u.a;
      } 
      { //READ data.Plan[9].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[468];
         u.b[1] = raw[469];
         data.Plan[9].ID = u.a;
      } 
      { //READ data.Plan[9].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[470];
         u.b[1] = raw[471];
         data.Plan[9].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[9].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[472];
         u.b[1] = raw[473];
         u.b[2] = raw[474];
         u.b[3] = raw[475];
        switch(u.a) {
         case 0:
            data.Plan[9].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[9].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[9].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[9].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[9].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[9].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[476];
         u.b[1] = raw[477];
         u.b[2] = raw[478];
         u.b[3] = raw[479];
        switch(u.a) {
         case 0:
            data.Plan[9].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[9].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[9].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[9].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[480];
         u.b[1] = raw[481];
         u.b[2] = raw[482];
         u.b[3] = raw[483];
         data.Plan[9].Latitude = u.a;
      } 
      { //READ data.Plan[9].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[484];
         u.b[1] = raw[485];
         u.b[2] = raw[486];
         u.b[3] = raw[487];
         data.Plan[9].Longitude = u.a;
      } 
      { //READ data.Plan[9].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[488];
         u.b[1] = raw[489];
         u.b[2] = raw[490];
         u.b[3] = raw[491];
         data.Plan[9].Altitude = u.a;
      } 
      { //READ data.Plan[9].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[492];
         u.b[1] = raw[493];
         u.b[2] = raw[494];
         u.b[3] = raw[495];
         data.Plan[9].GrndSpd = u.a;
      } 
      { //READ data.Plan[9].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[496];
         u.b[1] = raw[497];
         u.b[2] = raw[498];
         u.b[3] = raw[499];
         data.Plan[9].GroundTrack = u.a;
      } 
      { //READ data.Plan[9].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[500];
         u.b[1] = raw[501];
         u.b[2] = raw[502];
         u.b[3] = raw[503];
         u.b[4] = raw[504];
         u.b[5] = raw[505];
         u.b[6] = raw[506];
         u.b[7] = raw[507];
         data.Plan[9].TOA = u.a;
      } 
      { //READ data.Plan[9].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[508];
         u.b[1] = raw[509];
         u.b[2] = raw[510];
         u.b[3] = raw[511];
         data.Plan[9].TurnRadius = u.a;
      } 
      { //READ data.Plan[9].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[512];
         u.b[1] = raw[513];
         u.b[2] = raw[514];
         u.b[3] = raw[515];
         data.Plan[9].TurnAngle = u.a;
      } 
      { //READ data.Plan[9].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[516];
         u.b[1] = raw[517];
         u.b[2] = raw[518];
         u.b[3] = raw[519];
         data.Plan[9].VerticalSpd = u.a;
      } 
      { //READ data.Plan[10].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[520];
         u.b[1] = raw[521];
         data.Plan[10].ID = u.a;
      } 
      { //READ data.Plan[10].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[522];
         u.b[1] = raw[523];
         data.Plan[10].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[10].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[524];
         u.b[1] = raw[525];
         u.b[2] = raw[526];
         u.b[3] = raw[527];
        switch(u.a) {
         case 0:
            data.Plan[10].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[10].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[10].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[10].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[10].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[10].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[528];
         u.b[1] = raw[529];
         u.b[2] = raw[530];
         u.b[3] = raw[531];
        switch(u.a) {
         case 0:
            data.Plan[10].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[10].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[10].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[10].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[532];
         u.b[1] = raw[533];
         u.b[2] = raw[534];
         u.b[3] = raw[535];
         data.Plan[10].Latitude = u.a;
      } 
      { //READ data.Plan[10].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[536];
         u.b[1] = raw[537];
         u.b[2] = raw[538];
         u.b[3] = raw[539];
         data.Plan[10].Longitude = u.a;
      } 
      { //READ data.Plan[10].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[540];
         u.b[1] = raw[541];
         u.b[2] = raw[542];
         u.b[3] = raw[543];
         data.Plan[10].Altitude = u.a;
      } 
      { //READ data.Plan[10].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[544];
         u.b[1] = raw[545];
         u.b[2] = raw[546];
         u.b[3] = raw[547];
         data.Plan[10].GrndSpd = u.a;
      } 
      { //READ data.Plan[10].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[548];
         u.b[1] = raw[549];
         u.b[2] = raw[550];
         u.b[3] = raw[551];
         data.Plan[10].GroundTrack = u.a;
      } 
      { //READ data.Plan[10].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[552];
         u.b[1] = raw[553];
         u.b[2] = raw[554];
         u.b[3] = raw[555];
         u.b[4] = raw[556];
         u.b[5] = raw[557];
         u.b[6] = raw[558];
         u.b[7] = raw[559];
         data.Plan[10].TOA = u.a;
      } 
      { //READ data.Plan[10].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[560];
         u.b[1] = raw[561];
         u.b[2] = raw[562];
         u.b[3] = raw[563];
         data.Plan[10].TurnRadius = u.a;
      } 
      { //READ data.Plan[10].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[564];
         u.b[1] = raw[565];
         u.b[2] = raw[566];
         u.b[3] = raw[567];
         data.Plan[10].TurnAngle = u.a;
      } 
      { //READ data.Plan[10].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[568];
         u.b[1] = raw[569];
         u.b[2] = raw[570];
         u.b[3] = raw[571];
         data.Plan[10].VerticalSpd = u.a;
      } 
      { //READ data.Plan[11].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[572];
         u.b[1] = raw[573];
         data.Plan[11].ID = u.a;
      } 
      { //READ data.Plan[11].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[574];
         u.b[1] = raw[575];
         data.Plan[11].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[11].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[576];
         u.b[1] = raw[577];
         u.b[2] = raw[578];
         u.b[3] = raw[579];
        switch(u.a) {
         case 0:
            data.Plan[11].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[11].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[11].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[11].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[11].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[11].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[580];
         u.b[1] = raw[581];
         u.b[2] = raw[582];
         u.b[3] = raw[583];
        switch(u.a) {
         case 0:
            data.Plan[11].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[11].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[11].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[11].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[584];
         u.b[1] = raw[585];
         u.b[2] = raw[586];
         u.b[3] = raw[587];
         data.Plan[11].Latitude = u.a;
      } 
      { //READ data.Plan[11].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[588];
         u.b[1] = raw[589];
         u.b[2] = raw[590];
         u.b[3] = raw[591];
         data.Plan[11].Longitude = u.a;
      } 
      { //READ data.Plan[11].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[592];
         u.b[1] = raw[593];
         u.b[2] = raw[594];
         u.b[3] = raw[595];
         data.Plan[11].Altitude = u.a;
      } 
      { //READ data.Plan[11].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[596];
         u.b[1] = raw[597];
         u.b[2] = raw[598];
         u.b[3] = raw[599];
         data.Plan[11].GrndSpd = u.a;
      } 
      { //READ data.Plan[11].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[600];
         u.b[1] = raw[601];
         u.b[2] = raw[602];
         u.b[3] = raw[603];
         data.Plan[11].GroundTrack = u.a;
      } 
      { //READ data.Plan[11].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[604];
         u.b[1] = raw[605];
         u.b[2] = raw[606];
         u.b[3] = raw[607];
         u.b[4] = raw[608];
         u.b[5] = raw[609];
         u.b[6] = raw[610];
         u.b[7] = raw[611];
         data.Plan[11].TOA = u.a;
      } 
      { //READ data.Plan[11].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[612];
         u.b[1] = raw[613];
         u.b[2] = raw[614];
         u.b[3] = raw[615];
         data.Plan[11].TurnRadius = u.a;
      } 
      { //READ data.Plan[11].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[616];
         u.b[1] = raw[617];
         u.b[2] = raw[618];
         u.b[3] = raw[619];
         data.Plan[11].TurnAngle = u.a;
      } 
      { //READ data.Plan[11].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[620];
         u.b[1] = raw[621];
         u.b[2] = raw[622];
         u.b[3] = raw[623];
         data.Plan[11].VerticalSpd = u.a;
      } 
      { //READ data.Plan[12].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[624];
         u.b[1] = raw[625];
         data.Plan[12].ID = u.a;
      } 
      { //READ data.Plan[12].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[626];
         u.b[1] = raw[627];
         data.Plan[12].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[12].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[628];
         u.b[1] = raw[629];
         u.b[2] = raw[630];
         u.b[3] = raw[631];
        switch(u.a) {
         case 0:
            data.Plan[12].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[12].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[12].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[12].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[12].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[12].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[632];
         u.b[1] = raw[633];
         u.b[2] = raw[634];
         u.b[3] = raw[635];
        switch(u.a) {
         case 0:
            data.Plan[12].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[12].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[12].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[12].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[636];
         u.b[1] = raw[637];
         u.b[2] = raw[638];
         u.b[3] = raw[639];
         data.Plan[12].Latitude = u.a;
      } 
      { //READ data.Plan[12].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[640];
         u.b[1] = raw[641];
         u.b[2] = raw[642];
         u.b[3] = raw[643];
         data.Plan[12].Longitude = u.a;
      } 
      { //READ data.Plan[12].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[644];
         u.b[1] = raw[645];
         u.b[2] = raw[646];
         u.b[3] = raw[647];
         data.Plan[12].Altitude = u.a;
      } 
      { //READ data.Plan[12].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[648];
         u.b[1] = raw[649];
         u.b[2] = raw[650];
         u.b[3] = raw[651];
         data.Plan[12].GrndSpd = u.a;
      } 
      { //READ data.Plan[12].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[652];
         u.b[1] = raw[653];
         u.b[2] = raw[654];
         u.b[3] = raw[655];
         data.Plan[12].GroundTrack = u.a;
      } 
      { //READ data.Plan[12].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[656];
         u.b[1] = raw[657];
         u.b[2] = raw[658];
         u.b[3] = raw[659];
         u.b[4] = raw[660];
         u.b[5] = raw[661];
         u.b[6] = raw[662];
         u.b[7] = raw[663];
         data.Plan[12].TOA = u.a;
      } 
      { //READ data.Plan[12].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[664];
         u.b[1] = raw[665];
         u.b[2] = raw[666];
         u.b[3] = raw[667];
         data.Plan[12].TurnRadius = u.a;
      } 
      { //READ data.Plan[12].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[668];
         u.b[1] = raw[669];
         u.b[2] = raw[670];
         u.b[3] = raw[671];
         data.Plan[12].TurnAngle = u.a;
      } 
      { //READ data.Plan[12].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[672];
         u.b[1] = raw[673];
         u.b[2] = raw[674];
         u.b[3] = raw[675];
         data.Plan[12].VerticalSpd = u.a;
      } 
      { //READ data.Plan[13].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[676];
         u.b[1] = raw[677];
         data.Plan[13].ID = u.a;
      } 
      { //READ data.Plan[13].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[678];
         u.b[1] = raw[679];
         data.Plan[13].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[13].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[680];
         u.b[1] = raw[681];
         u.b[2] = raw[682];
         u.b[3] = raw[683];
        switch(u.a) {
         case 0:
            data.Plan[13].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[13].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[13].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[13].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[13].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[13].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[684];
         u.b[1] = raw[685];
         u.b[2] = raw[686];
         u.b[3] = raw[687];
        switch(u.a) {
         case 0:
            data.Plan[13].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[13].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[13].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[13].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[688];
         u.b[1] = raw[689];
         u.b[2] = raw[690];
         u.b[3] = raw[691];
         data.Plan[13].Latitude = u.a;
      } 
      { //READ data.Plan[13].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[692];
         u.b[1] = raw[693];
         u.b[2] = raw[694];
         u.b[3] = raw[695];
         data.Plan[13].Longitude = u.a;
      } 
      { //READ data.Plan[13].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[696];
         u.b[1] = raw[697];
         u.b[2] = raw[698];
         u.b[3] = raw[699];
         data.Plan[13].Altitude = u.a;
      } 
      { //READ data.Plan[13].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[700];
         u.b[1] = raw[701];
         u.b[2] = raw[702];
         u.b[3] = raw[703];
         data.Plan[13].GrndSpd = u.a;
      } 
      { //READ data.Plan[13].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[704];
         u.b[1] = raw[705];
         u.b[2] = raw[706];
         u.b[3] = raw[707];
         data.Plan[13].GroundTrack = u.a;
      } 
      { //READ data.Plan[13].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[708];
         u.b[1] = raw[709];
         u.b[2] = raw[710];
         u.b[3] = raw[711];
         u.b[4] = raw[712];
         u.b[5] = raw[713];
         u.b[6] = raw[714];
         u.b[7] = raw[715];
         data.Plan[13].TOA = u.a;
      } 
      { //READ data.Plan[13].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[716];
         u.b[1] = raw[717];
         u.b[2] = raw[718];
         u.b[3] = raw[719];
         data.Plan[13].TurnRadius = u.a;
      } 
      { //READ data.Plan[13].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[720];
         u.b[1] = raw[721];
         u.b[2] = raw[722];
         u.b[3] = raw[723];
         data.Plan[13].TurnAngle = u.a;
      } 
      { //READ data.Plan[13].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[724];
         u.b[1] = raw[725];
         u.b[2] = raw[726];
         u.b[3] = raw[727];
         data.Plan[13].VerticalSpd = u.a;
      } 
      { //READ data.Plan[14].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[728];
         u.b[1] = raw[729];
         data.Plan[14].ID = u.a;
      } 
      { //READ data.Plan[14].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[730];
         u.b[1] = raw[731];
         data.Plan[14].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[14].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[732];
         u.b[1] = raw[733];
         u.b[2] = raw[734];
         u.b[3] = raw[735];
        switch(u.a) {
         case 0:
            data.Plan[14].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[14].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[14].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[14].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[14].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[14].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[736];
         u.b[1] = raw[737];
         u.b[2] = raw[738];
         u.b[3] = raw[739];
        switch(u.a) {
         case 0:
            data.Plan[14].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[14].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[14].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[14].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[740];
         u.b[1] = raw[741];
         u.b[2] = raw[742];
         u.b[3] = raw[743];
         data.Plan[14].Latitude = u.a;
      } 
      { //READ data.Plan[14].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[744];
         u.b[1] = raw[745];
         u.b[2] = raw[746];
         u.b[3] = raw[747];
         data.Plan[14].Longitude = u.a;
      } 
      { //READ data.Plan[14].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[748];
         u.b[1] = raw[749];
         u.b[2] = raw[750];
         u.b[3] = raw[751];
         data.Plan[14].Altitude = u.a;
      } 
      { //READ data.Plan[14].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[752];
         u.b[1] = raw[753];
         u.b[2] = raw[754];
         u.b[3] = raw[755];
         data.Plan[14].GrndSpd = u.a;
      } 
      { //READ data.Plan[14].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[756];
         u.b[1] = raw[757];
         u.b[2] = raw[758];
         u.b[3] = raw[759];
         data.Plan[14].GroundTrack = u.a;
      } 
      { //READ data.Plan[14].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[760];
         u.b[1] = raw[761];
         u.b[2] = raw[762];
         u.b[3] = raw[763];
         u.b[4] = raw[764];
         u.b[5] = raw[765];
         u.b[6] = raw[766];
         u.b[7] = raw[767];
         data.Plan[14].TOA = u.a;
      } 
      { //READ data.Plan[14].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[768];
         u.b[1] = raw[769];
         u.b[2] = raw[770];
         u.b[3] = raw[771];
         data.Plan[14].TurnRadius = u.a;
      } 
      { //READ data.Plan[14].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[772];
         u.b[1] = raw[773];
         u.b[2] = raw[774];
         u.b[3] = raw[775];
         data.Plan[14].TurnAngle = u.a;
      } 
      { //READ data.Plan[14].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[776];
         u.b[1] = raw[777];
         u.b[2] = raw[778];
         u.b[3] = raw[779];
         data.Plan[14].VerticalSpd = u.a;
      } 
      { //READ data.Plan[15].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[780];
         u.b[1] = raw[781];
         data.Plan[15].ID = u.a;
      } 
      { //READ data.Plan[15].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[782];
         u.b[1] = raw[783];
         data.Plan[15].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[15].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[784];
         u.b[1] = raw[785];
         u.b[2] = raw[786];
         u.b[3] = raw[787];
        switch(u.a) {
         case 0:
            data.Plan[15].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[15].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[15].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[15].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[15].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[15].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[788];
         u.b[1] = raw[789];
         u.b[2] = raw[790];
         u.b[3] = raw[791];
        switch(u.a) {
         case 0:
            data.Plan[15].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[15].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[15].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[15].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[792];
         u.b[1] = raw[793];
         u.b[2] = raw[794];
         u.b[3] = raw[795];
         data.Plan[15].Latitude = u.a;
      } 
      { //READ data.Plan[15].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[796];
         u.b[1] = raw[797];
         u.b[2] = raw[798];
         u.b[3] = raw[799];
         data.Plan[15].Longitude = u.a;
      } 
      { //READ data.Plan[15].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[800];
         u.b[1] = raw[801];
         u.b[2] = raw[802];
         u.b[3] = raw[803];
         data.Plan[15].Altitude = u.a;
      } 
      { //READ data.Plan[15].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[804];
         u.b[1] = raw[805];
         u.b[2] = raw[806];
         u.b[3] = raw[807];
         data.Plan[15].GrndSpd = u.a;
      } 
      { //READ data.Plan[15].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[808];
         u.b[1] = raw[809];
         u.b[2] = raw[810];
         u.b[3] = raw[811];
         data.Plan[15].GroundTrack = u.a;
      } 
      { //READ data.Plan[15].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[812];
         u.b[1] = raw[813];
         u.b[2] = raw[814];
         u.b[3] = raw[815];
         u.b[4] = raw[816];
         u.b[5] = raw[817];
         u.b[6] = raw[818];
         u.b[7] = raw[819];
         data.Plan[15].TOA = u.a;
      } 
      { //READ data.Plan[15].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[820];
         u.b[1] = raw[821];
         u.b[2] = raw[822];
         u.b[3] = raw[823];
         data.Plan[15].TurnRadius = u.a;
      } 
      { //READ data.Plan[15].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[824];
         u.b[1] = raw[825];
         u.b[2] = raw[826];
         u.b[3] = raw[827];
         data.Plan[15].TurnAngle = u.a;
      } 
      { //READ data.Plan[15].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[828];
         u.b[1] = raw[829];
         u.b[2] = raw[830];
         u.b[3] = raw[831];
         data.Plan[15].VerticalSpd = u.a;
      } 
      { //READ data.Plan[16].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[832];
         u.b[1] = raw[833];
         data.Plan[16].ID = u.a;
      } 
      { //READ data.Plan[16].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[834];
         u.b[1] = raw[835];
         data.Plan[16].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[16].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[836];
         u.b[1] = raw[837];
         u.b[2] = raw[838];
         u.b[3] = raw[839];
        switch(u.a) {
         case 0:
            data.Plan[16].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[16].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[16].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[16].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[16].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[16].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[840];
         u.b[1] = raw[841];
         u.b[2] = raw[842];
         u.b[3] = raw[843];
        switch(u.a) {
         case 0:
            data.Plan[16].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[16].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[16].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[16].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[844];
         u.b[1] = raw[845];
         u.b[2] = raw[846];
         u.b[3] = raw[847];
         data.Plan[16].Latitude = u.a;
      } 
      { //READ data.Plan[16].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[848];
         u.b[1] = raw[849];
         u.b[2] = raw[850];
         u.b[3] = raw[851];
         data.Plan[16].Longitude = u.a;
      } 
      { //READ data.Plan[16].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[852];
         u.b[1] = raw[853];
         u.b[2] = raw[854];
         u.b[3] = raw[855];
         data.Plan[16].Altitude = u.a;
      } 
      { //READ data.Plan[16].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[856];
         u.b[1] = raw[857];
         u.b[2] = raw[858];
         u.b[3] = raw[859];
         data.Plan[16].GrndSpd = u.a;
      } 
      { //READ data.Plan[16].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[860];
         u.b[1] = raw[861];
         u.b[2] = raw[862];
         u.b[3] = raw[863];
         data.Plan[16].GroundTrack = u.a;
      } 
      { //READ data.Plan[16].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[864];
         u.b[1] = raw[865];
         u.b[2] = raw[866];
         u.b[3] = raw[867];
         u.b[4] = raw[868];
         u.b[5] = raw[869];
         u.b[6] = raw[870];
         u.b[7] = raw[871];
         data.Plan[16].TOA = u.a;
      } 
      { //READ data.Plan[16].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[872];
         u.b[1] = raw[873];
         u.b[2] = raw[874];
         u.b[3] = raw[875];
         data.Plan[16].TurnRadius = u.a;
      } 
      { //READ data.Plan[16].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[876];
         u.b[1] = raw[877];
         u.b[2] = raw[878];
         u.b[3] = raw[879];
         data.Plan[16].TurnAngle = u.a;
      } 
      { //READ data.Plan[16].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[880];
         u.b[1] = raw[881];
         u.b[2] = raw[882];
         u.b[3] = raw[883];
         data.Plan[16].VerticalSpd = u.a;
      } 
      { //READ data.Plan[17].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[884];
         u.b[1] = raw[885];
         data.Plan[17].ID = u.a;
      } 
      { //READ data.Plan[17].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[886];
         u.b[1] = raw[887];
         data.Plan[17].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[17].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[888];
         u.b[1] = raw[889];
         u.b[2] = raw[890];
         u.b[3] = raw[891];
        switch(u.a) {
         case 0:
            data.Plan[17].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[17].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[17].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[17].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[17].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[17].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[892];
         u.b[1] = raw[893];
         u.b[2] = raw[894];
         u.b[3] = raw[895];
        switch(u.a) {
         case 0:
            data.Plan[17].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[17].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[17].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[17].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[896];
         u.b[1] = raw[897];
         u.b[2] = raw[898];
         u.b[3] = raw[899];
         data.Plan[17].Latitude = u.a;
      } 
      { //READ data.Plan[17].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[900];
         u.b[1] = raw[901];
         u.b[2] = raw[902];
         u.b[3] = raw[903];
         data.Plan[17].Longitude = u.a;
      } 
      { //READ data.Plan[17].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[904];
         u.b[1] = raw[905];
         u.b[2] = raw[906];
         u.b[3] = raw[907];
         data.Plan[17].Altitude = u.a;
      } 
      { //READ data.Plan[17].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[908];
         u.b[1] = raw[909];
         u.b[2] = raw[910];
         u.b[3] = raw[911];
         data.Plan[17].GrndSpd = u.a;
      } 
      { //READ data.Plan[17].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[912];
         u.b[1] = raw[913];
         u.b[2] = raw[914];
         u.b[3] = raw[915];
         data.Plan[17].GroundTrack = u.a;
      } 
      { //READ data.Plan[17].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[916];
         u.b[1] = raw[917];
         u.b[2] = raw[918];
         u.b[3] = raw[919];
         u.b[4] = raw[920];
         u.b[5] = raw[921];
         u.b[6] = raw[922];
         u.b[7] = raw[923];
         data.Plan[17].TOA = u.a;
      } 
      { //READ data.Plan[17].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[924];
         u.b[1] = raw[925];
         u.b[2] = raw[926];
         u.b[3] = raw[927];
         data.Plan[17].TurnRadius = u.a;
      } 
      { //READ data.Plan[17].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[928];
         u.b[1] = raw[929];
         u.b[2] = raw[930];
         u.b[3] = raw[931];
         data.Plan[17].TurnAngle = u.a;
      } 
      { //READ data.Plan[17].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[932];
         u.b[1] = raw[933];
         u.b[2] = raw[934];
         u.b[3] = raw[935];
         data.Plan[17].VerticalSpd = u.a;
      } 
      { //READ data.Plan[18].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[936];
         u.b[1] = raw[937];
         data.Plan[18].ID = u.a;
      } 
      { //READ data.Plan[18].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[938];
         u.b[1] = raw[939];
         data.Plan[18].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[18].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[940];
         u.b[1] = raw[941];
         u.b[2] = raw[942];
         u.b[3] = raw[943];
        switch(u.a) {
         case 0:
            data.Plan[18].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[18].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[18].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[18].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[18].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[18].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[944];
         u.b[1] = raw[945];
         u.b[2] = raw[946];
         u.b[3] = raw[947];
        switch(u.a) {
         case 0:
            data.Plan[18].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[18].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[18].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[18].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[948];
         u.b[1] = raw[949];
         u.b[2] = raw[950];
         u.b[3] = raw[951];
         data.Plan[18].Latitude = u.a;
      } 
      { //READ data.Plan[18].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[952];
         u.b[1] = raw[953];
         u.b[2] = raw[954];
         u.b[3] = raw[955];
         data.Plan[18].Longitude = u.a;
      } 
      { //READ data.Plan[18].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[956];
         u.b[1] = raw[957];
         u.b[2] = raw[958];
         u.b[3] = raw[959];
         data.Plan[18].Altitude = u.a;
      } 
      { //READ data.Plan[18].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[960];
         u.b[1] = raw[961];
         u.b[2] = raw[962];
         u.b[3] = raw[963];
         data.Plan[18].GrndSpd = u.a;
      } 
      { //READ data.Plan[18].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[964];
         u.b[1] = raw[965];
         u.b[2] = raw[966];
         u.b[3] = raw[967];
         data.Plan[18].GroundTrack = u.a;
      } 
      { //READ data.Plan[18].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[968];
         u.b[1] = raw[969];
         u.b[2] = raw[970];
         u.b[3] = raw[971];
         u.b[4] = raw[972];
         u.b[5] = raw[973];
         u.b[6] = raw[974];
         u.b[7] = raw[975];
         data.Plan[18].TOA = u.a;
      } 
      { //READ data.Plan[18].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[976];
         u.b[1] = raw[977];
         u.b[2] = raw[978];
         u.b[3] = raw[979];
         data.Plan[18].TurnRadius = u.a;
      } 
      { //READ data.Plan[18].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[980];
         u.b[1] = raw[981];
         u.b[2] = raw[982];
         u.b[3] = raw[983];
         data.Plan[18].TurnAngle = u.a;
      } 
      { //READ data.Plan[18].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[984];
         u.b[1] = raw[985];
         u.b[2] = raw[986];
         u.b[3] = raw[987];
         data.Plan[18].VerticalSpd = u.a;
      } 
      { //READ data.Plan[19].ID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[988];
         u.b[1] = raw[989];
         data.Plan[19].ID = u.a;
      } 
      { //READ data.Plan[19].NextWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[990];
         u.b[1] = raw[991];
         data.Plan[19].NextWyPtID = u.a;
      } 
      { //READ enum data.Plan[19].Type
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[992];
         u.b[1] = raw[993];
         u.b[2] = raw[994];
         u.b[3] = raw[995];
        switch(u.a) {
         case 0:
            data.Plan[19].Type = WayPointType::Unknown;
            break;
         case 1:
            data.Plan[19].Type = WayPointType::Taxi;
            break;
         case 2:
            data.Plan[19].Type = WayPointType::Cruise;
            break;
         case 3:
            data.Plan[19].Type = WayPointType::Descent;
            break;
         case 4:
            data.Plan[19].Type = WayPointType::Ascent;
            break;
        }
      } 
      { //READ enum data.Plan[19].Type2
         union {
            uint32_t a;
            char b[4];
         } u;
         u.b[0] = raw[996];
         u.b[1] = raw[997];
         u.b[2] = raw[998];
         u.b[3] = raw[999];
        switch(u.a) {
         case 0:
            data.Plan[19].Type2 = WayPointType2::FlyBy;
            break;
         case 1:
            data.Plan[19].Type2 = WayPointType2::FlyOver;
            break;
         case 2:
            data.Plan[19].Type2 = WayPointType2::FlyByLargeTrnAng;
            break;
        }
      } 
      { //READ data.Plan[19].Latitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1000];
         u.b[1] = raw[1001];
         u.b[2] = raw[1002];
         u.b[3] = raw[1003];
         data.Plan[19].Latitude = u.a;
      } 
      { //READ data.Plan[19].Longitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1004];
         u.b[1] = raw[1005];
         u.b[2] = raw[1006];
         u.b[3] = raw[1007];
         data.Plan[19].Longitude = u.a;
      } 
      { //READ data.Plan[19].Altitude        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1008];
         u.b[1] = raw[1009];
         u.b[2] = raw[1010];
         u.b[3] = raw[1011];
         data.Plan[19].Altitude = u.a;
      } 
      { //READ data.Plan[19].GrndSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1012];
         u.b[1] = raw[1013];
         u.b[2] = raw[1014];
         u.b[3] = raw[1015];
         data.Plan[19].GrndSpd = u.a;
      } 
      { //READ data.Plan[19].GroundTrack        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1016];
         u.b[1] = raw[1017];
         u.b[2] = raw[1018];
         u.b[3] = raw[1019];
         data.Plan[19].GroundTrack = u.a;
      } 
      { //READ data.Plan[19].TOA        int64        8 bytes
         union { int64_t a;  char b[8]; } u;
         u.b[0] = raw[1020];
         u.b[1] = raw[1021];
         u.b[2] = raw[1022];
         u.b[3] = raw[1023];
         u.b[4] = raw[1024];
         u.b[5] = raw[1025];
         u.b[6] = raw[1026];
         u.b[7] = raw[1027];
         data.Plan[19].TOA = u.a;
      } 
      { //READ data.Plan[19].TurnRadius        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1028];
         u.b[1] = raw[1029];
         u.b[2] = raw[1030];
         u.b[3] = raw[1031];
         data.Plan[19].TurnRadius = u.a;
      } 
      { //READ data.Plan[19].TurnAngle        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1032];
         u.b[1] = raw[1033];
         u.b[2] = raw[1034];
         u.b[3] = raw[1035];
         data.Plan[19].TurnAngle = u.a;
      } 
      { //READ data.Plan[19].VerticalSpd        float32        4 bytes
         union { float a;  char b[4]; } u;
         u.b[0] = raw[1036];
         u.b[1] = raw[1037];
         u.b[2] = raw[1038];
         u.b[3] = raw[1039];
         data.Plan[19].VerticalSpd = u.a;
      } 
      { //READ data.NumPoints        uint8        1 bytes
         union { uint8_t a;  char b[1]; } u;
         u.b[0] = raw[1040];
         data.NumPoints = u.a;
      } 
      { //READ data.CurrWyPtID        uint16        2 bytes
         union { uint16_t a;  char b[2]; } u;
         u.b[0] = raw[1041];
         u.b[1] = raw[1042];
         data.CurrWyPtID = u.a;
      } 
   }

}
