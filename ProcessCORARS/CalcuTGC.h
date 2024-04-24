#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include <istream>
#include <fstream>
#include <iostream>
using namespace std ;
struct Point_TGC
{
float x ;
float y ;
float z ;
};

class CalcuTGC
{
public:
CalcuTGC() ;
~CalcuTGC() ;
bool Remove_Null_Signal_Two() ;
int Get_Number(UInt_t *Signal) ;
bool Get_Valid_Strip_Wire_Number_One() ;
bool Get_Valid_Strip_Wire_Number_Two() ;
void Calcu_TGC_Coor_Two() ;
void Calcu_Two() ;

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

UInt_t Up_TGC_Strip_Leading_Two[32] ;
UInt_t Up_TGC_Strip_Trailing_Two[32] ;
UInt_t Up_TGC_Wire_Leading_Two[32] ;
UInt_t Up_TGC_Wire_Trailing_Two[32] ;
UInt_t Down_TGC_Strip_Leading_Two[32] ;
UInt_t Down_TGC_Strip_Trailing_Two[32] ;
UInt_t Down_TGC_Wire_Leading_Two[32] ;
UInt_t Down_TGC_Wire_Trailing_Two[32] ;


int Up_TGC_Strip_Number_One ;
int Up_TGC_Wire_Number_One ;
int Down_TGC_Strip_Number_One ;
int Down_TGC_Wire_Number_One ;
int Up_TGC_Strip_Number_Two ;
int Up_TGC_Wire_Number_Two ;
int Down_TGC_Strip_Number_Two ;
int Down_TGC_Wire_Number_Two ;
float params[189] ;
Point_TGC Up_TGC_One ;
Point_TGC Down_TGC_One ;
Point_TGC Up_TGC_Two ;
Point_TGC Down_TGC_Two ;
};
