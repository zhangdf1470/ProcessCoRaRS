#ifndef GenerateTimeCalib_H
#define GenerateTimeCalib_H

#include "ProcessCORARS/ConfigureClass.h"
#include <iostream>
#include <string>
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TString.h"
#include "TDatime.h"
#include "TChain.h"
#include "StructDef.h"

class GenerateTimeCalib
{
  public:
    GenerateTimeCalib();
    ~GenerateTimeCalib();

    void Initialize() ;
    bool GetDownCalib() ;
    void Generate() ;

  public:
    TChain *chain ;
    string inPath ;
    string outPath ;
    string inFileName ;

    TFile *outFile ;
    TTree *outTree ;
    TTree *SoutTree ;
    string outFileName ;

    vector< std::pair<float, float> > TSCTXY ; // x and y of the bottom left of SCT

    Scin Up_Scin_Calib ;
    Scin Down_Scin_Calib ;
    Scin Up_Scin ;
    Scin Down_Scin ;
    int EventQuality ;
};
#endif
