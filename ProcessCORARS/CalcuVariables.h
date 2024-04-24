#ifndef __CalcuVariables__H
#define __CalcuVariables__H

// c++ includes
#include <istream>
#include <fstream>
#include <iostream>
//root includes
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "TGraphErrors.h"
//local includes
#include "StructDef.h"
#include "ConfigureClass.h"

using namespace std ;
class CalcuVariables
{
  private:
    ConfigureClass *conf ;

  public:
    CalcuVariables() ;
    ~CalcuVariables() ;
    void Initialize() ;
    bool Calcu_Scin() ;
    void Calcu_ED() ;
    void Calcu() ;

  public:
    TFile *PedestalFile ;
    string inPath ;
    string inFileName ;
    TFile *inFile ;
    TTree *inTree ;

    string outPath ;
    string outFileName ;
    TFile *outFile ;
    TTree *outTree ;

    vector< std::pair<float, float> > TSCTXY ; // x and y of the bottom left of SCT

    float KM2ACharge[16] ; // charge measured by new electronics pad
    float KM2ATime[16] ; // time measured by new electronics pad
    int NEDNoiseHit[8] ; // Number of noise hits before trigger

    int EventQuality ;

    TH2F *CalibMean[12] ;
    //branches of in tree
    UInt_t event_id ;
    Point Up_TGC ;
    Point Down_TGC ;
    Scin Up_Scin_ScinED ;
    Scin Down_Scin_ScinED ;
    Scin Up_Scin ;
    Scin Down_Scin ;
    ED ElecDet[8] ;
    TOF Muon_TOF ;
    float TrackLength ;

    int NEDs ; // totoal number of measured EDs
    string EDName[8] ; // ED name

    TH1F* DeltaTimeUDScins[4] ;
};
#endif
