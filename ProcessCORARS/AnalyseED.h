#ifndef __AnalyseED__H
#define __AnalyseED__H

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> 
#include <vector> 
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TGraph.h"
#include "TF1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLine.h"
#include "TEnv.h"
#include "TLatex.h"
#include "TVectorD.h"
#include "TMatrixF.h"

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooFFTConvPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "HistToolKit.h"
#include "StructDef.h"
#include "ConfigureClass.h"

using namespace RooFit ;
using namespace std ;

class AnalyseED
{
  private:
    ConfigureClass* conf ;

  public:
    AnalyseED() ;
    ~AnalyseED() ;

    void Initialize() ;
    void NewHist() ;
    void FillHitPoint(int i) ;
    void FillDeltaTOF() ;
    void FillDeltaTime(int i) ;
    void FillCharge(int i) ;
    void GetDetEffi(int i) ;
    void GetSysTimeRes() ;
    std::pair <float, float> GetTimeRes(TH1F* EDSCTCorrectedDeltaTime) ;
    void GetTimeRes(int i) ;
    void GetTimeOffset(int i) ;
    void Convolution(TH1F* h, float fitlowedge, float fithighedge, float *singleparticlepeak, float *singleparticlepeakerror, float* resolution, float *resolutionerror, string canvasname) ;
    void GetEnergyRes(int i) ;
    TH2F* CloneTH2F(TMatrixF* matrix, string name, string title, string zaxislabel) ;
    void ScanED(int i) ;
    void Analyse() ;

  public:
    //informations of in files
    string CORARSCollectionTime ;
    string inPath ; // path of thein data files
    string inFileName ; // in file name 
    TFile *inFile ;
    TTree *inTree ;
    TFile  *EDPositionFile ;
    //informations of out files
    string outPath ; // out path
    string outFileName ; // out file name
    TFile* outFile ; // out root file containing measured time and charge of Scintillor s and ED

    int NEDs ; // totoal number of measured EDs
    string EDName[8] ; // ED name
    // name of scintillators in ED
    string EDSCT1Name[8] ;
    string EDSCT2Name[8] ;
    string EDSCT3Name[8] ;
    string EDSCT4Name[8] ;

    TVectorD* SCT1BLRotatedPoint ;
    TVectorD* SCT2BLRotatedPoint ;
    TVectorD* SCT3BLRotatedPoint ;
    TVectorD* SCT4BLRotatedPoint ;

    TAxis* XAxis[8] ;  // the bin edge is the SCT boundary
    TAxis* YAxis[8] ;    

    int NEDNoiseHit[8] ; // Number of noise hits before trigger
    ED ElecDet[8] ;
    TOF Muon_TOF ;
    int EventQuality ;

    TH2F *EDPlaneNHits[8] ; //Count all the hits crossing ED Plane within whole ED plane
    TH2F *EDPlaneNFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED Plane within whole ED plane
    TH2F *EDSCT1NHits[8] ; //Count all the hits crossing ED Plane within EDSCT1
    TH2F *EDSCT2NHits[8] ; //Count all the hits crossing ED Plane within EDSCT2
    TH2F *EDSCT3NHits[8] ; //Count all the hits crossing ED Plane within EDSCT3
    TH2F *EDSCT4NHits[8] ; //Count all the hits crossing ED Plane within EDSCT4
    TH2F *EDSCT1NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED Plane within EDSCT1
    TH2F *EDSCT2NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED Plane within EDSCT2
    TH2F *EDSCT3NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED Plane within EDSCT3
    TH2F *EDSCT4NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED Plane within EDSCT4

    TH2F *EDPlaneDetEffi2DScan[8] ;
    TH2F *EDSCT1DetEffi2DScan[8] ;
    TH2F *EDSCT2DetEffi2DScan[8] ;
    TH2F *EDSCT3DetEffi2DScan[8] ;
    TH2F *EDSCT4DetEffi2DScan[8] ;
    TH1F *EDSCT1DetEffi1DScan[8] ;
    TH1F *EDSCT2DetEffi1DScan[8] ;
    TH1F *EDSCT3DetEffi1DScan[8] ;
    TH1F *EDSCT4DetEffi1DScan[8] ;

    TH1F *DeltaTOF ;

    TH1F *EDSCT1MeasureTime[8] ;
    TH1F *EDSCT2MeasureTime[8] ;
    TH1F *EDSCT3MeasureTime[8] ;
    TH1F *EDSCT4MeasureTime[8] ;
    TH1F *EDSCT1ReconTime[8] ;
    TH1F *EDSCT2ReconTime[8] ;
    TH1F *EDSCT3ReconTime[8] ;
    TH1F *EDSCT4ReconTime[8] ;
    TH1F *EDSCT1DeltaTime[8] ;
    TH1F *EDSCT2DeltaTime[8] ;
    TH1F *EDSCT3DeltaTime[8] ;
    TH1F *EDSCT4DeltaTime[8] ;
    TH1F *EDSCT1CorrectedDeltaTime[8] ;
    TH1F *EDSCT2CorrectedDeltaTime[8] ;
    TH1F *EDSCT3CorrectedDeltaTime[8] ;
    TH1F *EDSCT4CorrectedDeltaTime[8] ;

    TH1F *EDSCT1DeltaTimeBin[8][4][19] ;
    TH1F *EDSCT2DeltaTimeBin[8][4][19] ;
    TH1F *EDSCT3DeltaTimeBin[8][4][19] ;
    TH1F *EDSCT4DeltaTimeBin[8][4][19] ;
    TH1F *EDSCT1CorrectedDeltaTimeBin[8][4][19] ;
    TH1F *EDSCT2CorrectedDeltaTimeBin[8][4][19] ;
    TH1F *EDSCT3CorrectedDeltaTimeBin[8][4][19] ;
    TH1F *EDSCT4CorrectedDeltaTimeBin[8][4][19] ;
    TH2F *EDSCT1TimeRes2DScan[8] ;
    TH2F *EDSCT2TimeRes2DScan[8] ;
    TH2F *EDSCT3TimeRes2DScan[8] ;
    TH2F *EDSCT4TimeRes2DScan[8] ;
    TH1F *EDSCT1TimeRes1DScan[8] ;
    TH1F *EDSCT2TimeRes1DScan[8] ;
    TH1F *EDSCT3TimeRes1DScan[8] ;
    TH1F *EDSCT4TimeRes1DScan[8] ;
    TH2F *EDSCT1TimeOffset2DScan[8] ;
    TH2F *EDSCT2TimeOffset2DScan[8] ;
    TH2F *EDSCT3TimeOffset2DScan[8] ;
    TH2F *EDSCT4TimeOffset2DScan[8] ;
    TH1F *EDSCT1TimeOffset1DScan[8] ;
    TH1F *EDSCT2TimeOffset1DScan[8] ;
    TH1F *EDSCT3TimeOffset1DScan[8] ;
    TH1F *EDSCT4TimeOffset1DScan[8] ;

    TH1F *EDSCT1SignalCharge[8] ;
    TH1F *EDSCT2SignalCharge[8] ;
    TH1F *EDSCT3SignalCharge[8] ;
    TH1F *EDSCT4SignalCharge[8] ;
    TH1F *EDSCT1SignalChargeBin[8][4][19] ;
    TH1F *EDSCT2SignalChargeBin[8][4][19] ;
    TH1F *EDSCT3SignalChargeBin[8][4][19] ;
    TH1F *EDSCT4SignalChargeBin[8][4][19] ;
    TH2F *EDSCT1EnergyRes2DScan[8] ;
    TH2F *EDSCT2EnergyRes2DScan[8] ;
    TH2F *EDSCT3EnergyRes2DScan[8] ;
    TH2F *EDSCT4EnergyRes2DScan[8] ;
    TH1F *EDSCT1EnergyRes1DScan[8] ;
    TH1F *EDSCT2EnergyRes1DScan[8] ;
    TH1F *EDSCT3EnergyRes1DScan[8] ;
    TH1F *EDSCT4EnergyRes1DScan[8] ;
    TH2F *EDSCT1ReEnergyRes2DScan[8] ;
    TH2F *EDSCT2ReEnergyRes2DScan[8] ;
    TH2F *EDSCT3ReEnergyRes2DScan[8] ;
    TH2F *EDSCT4ReEnergyRes2DScan[8] ;
    TH1F *EDSCT1ReEnergyRes1DScan[8] ;
    TH1F *EDSCT2ReEnergyRes1DScan[8] ;
    TH1F *EDSCT3ReEnergyRes1DScan[8] ;
    TH1F *EDSCT4ReEnergyRes1DScan[8] ;
    TH2F *EDSCT1SingleParticlePeak2DScan[8] ;
    TH2F *EDSCT2SingleParticlePeak2DScan[8] ;
    TH2F *EDSCT3SingleParticlePeak2DScan[8] ;
    TH2F *EDSCT4SingleParticlePeak2DScan[8] ;
    TH1F *EDSCT1SingleParticlePeak1DScan[8] ;
    TH1F *EDSCT2SingleParticlePeak1DScan[8] ;
    TH1F *EDSCT3SingleParticlePeak1DScan[8] ;
    TH1F *EDSCT4SingleParticlePeak1DScan[8] ;

    int NPreHitsOnSCT[8] ;// N hits on SCT collected before the trigger
    TVectorD* SingleRate ;

    TVectorD* EDSCTDetEffi ;
    TVectorD* EDSCTDetEffiErrors ;
    TMatrixF* EDPlaneDetEffi[8] ;
    TMatrixF* EDPlaneDetEffiErrors[8] ;
    TMatrixF* EDSCT1DetEffi[8] ;
    TMatrixF* EDSCT2DetEffi[8] ;
    TMatrixF* EDSCT3DetEffi[8] ;
    TMatrixF* EDSCT4DetEffi[8] ;
    TMatrixF* EDSCT1DetEffiErrors[8] ;
    TMatrixF* EDSCT2DetEffiErrors[8] ;
    TMatrixF* EDSCT3DetEffiErrors[8] ;
    TMatrixF* EDSCT4DetEffiErrors[8] ;

    TVectorD* SysTimeRes ;
    TVectorD* SysTimeResError ;
    TVectorD* ScinTimeRes ;
    TVectorD* ScinTimeResError ;

    TVectorD* EDSCTTimeRes ;
    TVectorD* EDSCTTimeResErrors ;
    TVectorD* EDSCTTimeOffset ;
    TVectorD* EDSCTTimeOffsetErrors ;

    TMatrixF* EDSCT1TimeOffset[8] ;
    TMatrixF* EDSCT2TimeOffset[8] ;
    TMatrixF* EDSCT3TimeOffset[8] ;
    TMatrixF* EDSCT4TimeOffset[8] ;
    TMatrixF* EDSCT1TimeOffsetErrors[8] ;
    TMatrixF* EDSCT2TimeOffsetErrors[8] ;
    TMatrixF* EDSCT3TimeOffsetErrors[8] ;
    TMatrixF* EDSCT4TimeOffsetErrors[8] ;
    TMatrixF* EDSCT1TimeRes[8] ;
    TMatrixF* EDSCT2TimeRes[8] ;
    TMatrixF* EDSCT3TimeRes[8] ;
    TMatrixF* EDSCT4TimeRes[8] ;
    TMatrixF* EDSCT1TimeResErrors[8] ;
    TMatrixF* EDSCT2TimeResErrors[8] ;
    TMatrixF* EDSCT3TimeResErrors[8] ;
    TMatrixF* EDSCT4TimeResErrors[8] ;

    TVectorD* EDSCTEnergyRes;
    TVectorD* EDSCTEnergyResErrors;
    TMatrixF* EDSCT1EnergyRes[8] ;
    TMatrixF* EDSCT2EnergyRes[8] ;
    TMatrixF* EDSCT3EnergyRes[8] ;
    TMatrixF* EDSCT4EnergyRes[8] ;
    TMatrixF* EDSCT1EnergyResErrors[8] ;
    TMatrixF* EDSCT2EnergyResErrors[8] ;
    TMatrixF* EDSCT3EnergyResErrors[8] ;
    TMatrixF* EDSCT4EnergyResErrors[8] ;

    TVectorD* EDSCTReEnergyRes ;
    TVectorD* EDSCTReEnergyResErrors ;
    TMatrixF* EDSCT1ReEnergyRes[8] ;
    TMatrixF* EDSCT2ReEnergyRes[8] ;
    TMatrixF* EDSCT3ReEnergyRes[8] ;
    TMatrixF* EDSCT4ReEnergyRes[8] ;
    TMatrixF* EDSCT1ReEnergyResErrors[8] ;
    TMatrixF* EDSCT2ReEnergyResErrors[8] ;
    TMatrixF* EDSCT3ReEnergyResErrors[8] ;
    TMatrixF* EDSCT4ReEnergyResErrors[8] ;

    TVectorD* EDSCTSingleParticlePeak ;
    TVectorD* EDSCTSingleParticlePeakErrors ;
    TMatrixF* EDSCT1SingleParticlePeak[8] ;
    TMatrixF* EDSCT2SingleParticlePeak[8] ;
    TMatrixF* EDSCT3SingleParticlePeak[8] ;
    TMatrixF* EDSCT4SingleParticlePeak[8] ;
    TMatrixF* EDSCT1SingleParticlePeakErrors[8] ;
    TMatrixF* EDSCT2SingleParticlePeakErrors[8] ;
    TMatrixF* EDSCT3SingleParticlePeakErrors[8] ;
    TMatrixF* EDSCT4SingleParticlePeakErrors[8] ;
};
#endif
