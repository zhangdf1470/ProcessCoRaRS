#ifndef __ReGenerateTimeCalib__H
#define __ReGenerateTimeCalib__H

#include "ProcessCORARS/ConfigureClass.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TF1.h"
#include "TMatrixF.h"
#include "TString.h"
#include "StructDef.h"

class ReGenerateTimeCalib
{
  public:
    ReGenerateTimeCalib();
    ~ReGenerateTimeCalib();
    void Initialize() ;
    void CalcuScinCalib() ;
    void ReGenerate() ;

  public:
    string inPath ;
    string inFileName ;
    TFile *inFile ;
    TTree *inTree ;

    string outPath ;
    string outFileName ;
    TFile *outFile ;
    TTree *outTree ;
    vector< std::pair<float, float> > TSCTXY ; // x and y of the bottom left of SCT

    TMatrixF* FitParas ; // fitting parameters of calib vs DistanceToPMT
    TH1F *CalibBin[12][10][48] ;
    TH2F *CalibMean[12] ;

    Scin Up_Scin ;
    Scin Down_Scin ;
};
#endif
