#ifndef __StructDef__H
#define __StructDef__H

#ifndef Struct_Point
#define Struct_Point
struct Point
{
  float x ;
  float y ;
  float z ;
};
#endif
#ifndef Struct_TGC
#define Struct_TGC
struct TGC
{
  Point TGC_Coor ;
  int Valid_Strip_Number ;
  int Valid_Wire_Number ;
};
#endif
#ifndef Struct_Scin
#define Struct_Scin
struct Scin
{
  Point Scin_Coor ;
  int index ;
  float Measure_Time ;
  float DistanceToPMT ;
  float TimeCalib ;
  float Calibed_Time ;
};
#endif
#ifndef Struct_ED
#define Struct_ED
struct ED
{
  Point ED_Coor ;
  float Measure_Time ;
  float Charge ;
  float ReCon_Time ;
  float Delta_Time ; //Measure_Time - ReCon_Time
  float Corrected_Delta_Time ;
  float NormalizedCharge ;
};
#endif
#ifndef Struct_TOF
#define Struct_TOF
struct TOF
{
  float Real_TOF ;
  float Measure_TOF ;
  float Delta_TOF ;
};
#endif

#endif
