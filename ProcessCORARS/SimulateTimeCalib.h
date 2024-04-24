#ifndef SimulateTimeCalib_H
#define SimulateTimeCalib_H

#include "ProcessCORARS/ConfigureClass.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TMath.h"
#include "TVectorD.h"
#include "TMatrixF.h"
#include "StructDef.h"

class SimulateTimeCalib
{
  public:

    SimulateTimeCalib() ;
    ~SimulateTimeCalib() ;
    void Initialize() ;
    void GetDownFitParas() ;
    void GetCalib() ;
    void Simulate() ;

  private:

    string inPath ;
    string outPath ;
    TFile *inFile ;
    TTree* inTree ;
    TTree* SinTree ;
    string inFileName ;
    TFile *outFile ;
    TTree *outTree ;
    string outFileName ;

    TH1F *DownCalibToDistance[6] ;
    TH1F *DownCalinBin[6][62] ;
    TH1F *CalibToDistance[12] ;
    TH1F *CalinBin[12][62] ;

    TMatrixF* DownFitParas ; // initial fitting parameters of downcalib vs DistanceToPMT
    TMatrixF* FitParas ;  // fitting parameters of calib vs DistanceToPMT
    Scin Up_Scin ;
    Scin Down_Scin ;
};
#endif
