#ifndef __ReCalcuVariables__H
#define __ReCalcuVariables__H

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TVectorD.h"
#include "StructDef.h"
#include "ConfigureClass.h"

class ReCalcuVariables
{
  private:
    ConfigureClass* conf ;

  public:
    ReCalcuVariables() ;
    ~ReCalcuVariables() ;
    void Initialize() ;
    void ReCalcu() ;

  public:
    //informations of in files
    string inPath ; // path of thein data files
    string inFileName ; // in file name 
    TFile *inFile ;
    TTree *inTree ;
    //informations of out files
    string outPath ; // out path
    string outFileName ; // out file name
    TFile* outFile ; // out root file containing measured time and charge of Scintillor s and ED
    TTree* outTree ;

    int NEDs ;
    string EDName[8] ;

    int NEDNoiseHit[8] ; // Number of noise hits before trigger
    ED ElecDet[8] ;
    int EventQuality ;
    TOF Muon_TOF ;
    float TrackLength ;

    TVectorD* FuncSlope ;
    TH1F* ChargeDeltaTime[8] ;
};

#endif
