#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include <istream>
#include <fstream>
#include <iostream>
using namespace std ;

struct Point_CD
{
float x ;
float y ;
float z ;
};
struct Scin_CD
{
  Point_CD Scin_Coor ;
  int index ;
  Int_t Measure_Time ;
  Int_t Calibed_Time ;
};
struct ED_CD
{
  Point_CD ED_Coor ;
  int N841_Measure_Time ;
  int N844_Measure_Time ;
  int ReCon_Time ;
  int N841_Delta_Time ; //Measure_Time - ReCon_Time
  int N844_Delta_Time ; //Measure_Time - ReCon_Time
  int Charge ;
  float Calibed_Charge ;
};
struct TOF_CD
{
  Int_t Real_TOF ;
  Int_t Measure_TOF ;
  Int_t Delta_TOF ;
};
class ComDiscriminator
{
public:
ComDiscriminator() ;
~ComDiscriminator() ;
void Initialize() ;
int Get_Number(UInt_t *Signal) ;
bool Get_Valid_Strip_Wire_Number() ;
void Calcu_TGC_Coor() ;
bool Calcu_Scin() ;
void Calcu_ED5() ;
void Calcu() ;
private:

TFile *InPut_File ;
TString InPut_File_Name ;
TFile *OutPut_File ;
TTree *OutPut_Tree ;
TTree *InPut_Tree ;
TString OutPut_File_Name ;

UInt_t Up_TGC_Strip_One[32] ;
UInt_t Up_TGC_Wire_One[32] ;
UInt_t Down_TGC_Strip_One[32] ;
UInt_t Down_TGC_Wire_One[32] ;
UInt_t ScinED_Time_Signal[16] ;
UInt_t ED_Charge_Signal[16] ;

int Up_TGC_Strip_Number ;
int Up_TGC_Wire_Number ;
int Down_TGC_Strip_Number ;
int Down_TGC_Wire_Number ;
float params[189] ;
float calib_mean[8][10][48] ;
float calib_charge[8][55] ;

UInt_t event_id ;
Point_CD Up_TGC ;
Point_CD Down_TGC ;
Scin_CD Up_Scin ;
Scin_CD Down_Scin ;
ED_CD ElecDet5 ;
TOF_CD Muon_TOF ;
};
