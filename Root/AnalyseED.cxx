#ifndef __AnalyseED__CXX
#define __AnalyseED__CXX

#include "ProcessCORARS/AnalyseED.h"

using namespace RooFit ;
using namespace std ;
AnalyseED::AnalyseED()
{
  EDPositionFile = 0 ;

  inFileName = "" ;
  inFile = 0 ;
  inTree = 0 ;
  outFileName = "" ;
  outFile = 0 ;

  SingleRate = 0 ;
  EDSCTDetEffi = 0 ;
  EDSCTDetEffiErrors = 0 ;
  SysTimeRes = 0 ;
  SysTimeResError = 0 ;
  ScinTimeRes = 0 ;
  ScinTimeResError = 0 ;
  EDSCTTimeRes = 0 ;
  EDSCTTimeResErrors = 0 ;
  EDSCTTimeOffset = 0 ;
  EDSCTTimeOffsetErrors = 0 ;
  EDSCTEnergyRes = 0 ;
  EDSCTEnergyResErrors = 0 ;
  EDSCTReEnergyRes = 0 ;
  EDSCTReEnergyResErrors = 0 ;
  EDSCTSingleParticlePeak = 0 ;
  EDSCTSingleParticlePeakErrors = 0 ;

  SCT1BLRotatedPoint = 0 ;
  SCT2BLRotatedPoint = 0 ;
  SCT3BLRotatedPoint = 0 ;
  SCT4BLRotatedPoint = 0 ;

  Initialize() ;
}

void AnalyseED::Initialize()
{
  conf = ConfigureClass::Instance() ;

  NEDs = conf->GetNEDs() ;
  for(int i=0 ; i<NEDs ; i++)
  {
    EDName[i] = conf->GetEDName(i) ;
    EDSCT1Name[i] = conf->GetEDSCT1Name(i) ;
    EDSCT2Name[i] = conf->GetEDSCT2Name(i) ;
    EDSCT3Name[i] = conf->GetEDSCT3Name(i) ;
    EDSCT4Name[i] = conf->GetEDSCT4Name(i) ;

    NPreHitsOnSCT[i] = 0 ;
  }

  CORARSCollectionTime = conf->GetCORARSCollectionTime() ;
  inPath =  conf->GetoutPath()+"/"+CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  EDPositionFile = new TFile((inPath+"EDPositions_"+CORARSCollectionTime+".root").c_str(),"read") ;
  if(!EDPositionFile)
  {
    cout<<"ED Position File: "<<inPath+"EDPositions_"+CORARSCollectionTime+".root doesn't exist!!!"<<endl ;
    exit(0) ;
  }
  SCT1BLRotatedPoint =(TVectorD*) EDPositionFile->Get("SCT1BLRotatedPoint") ;
  SCT2BLRotatedPoint =(TVectorD*) EDPositionFile->Get("SCT2BLRotatedPoint") ;
  SCT3BLRotatedPoint =(TVectorD*) EDPositionFile->Get("SCT3BLRotatedPoint") ;
  SCT4BLRotatedPoint =(TVectorD*) EDPositionFile->Get("SCT4BLRotatedPoint") ;
  if(!SCT1BLRotatedPoint||!SCT2BLRotatedPoint||!SCT3BLRotatedPoint||!SCT4BLRotatedPoint)
  {
    cout<<"IntersectionPoint doesn't exist!!!"<<endl ;
    exit(0) ;
  }
  for(int i=0 ; i<NEDs ; i++)
  {
    double xarrary[] = {0, (*SCT1BLRotatedPoint)[2*i]+2.5, (*SCT1BLRotatedPoint)[2*i]+25-2.5,
                     (*SCT2BLRotatedPoint)[2*i]+2.5, (*SCT2BLRotatedPoint)[2*i]+25-2.5,
                     (*SCT3BLRotatedPoint)[2*i]+2.5, (*SCT3BLRotatedPoint)[2*i]+25-2.5,
                     (*SCT4BLRotatedPoint)[2*i]+2.5, (*SCT4BLRotatedPoint)[2*i]+25-2.5} ;
    double yarrary[] = {0, (*SCT1BLRotatedPoint)[2*i+1]+2.5, (*SCT1BLRotatedPoint)[2*i+1]+100-2.5} ;
    XAxis[i] = new TAxis(8, xarrary) ;
    YAxis[i] = new TAxis(2, yarrary) ;
  }

  // get the infile
  inFileName = "Stage_Three_"+CORARSCollectionTime+".root" ;
  inFile = new TFile((inPath+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"In File: "<<inPath<<inFileName<<" doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  cout<<"inFile: "<<inFile->GetName()<<endl ;
  inTree = (TTree*)(inFile->Get("ReCalcued_Variables")) ;
  if(!inTree)
  {
    cout<<"inTree doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  inTree->SetBranchAddress("EventQuality",&EventQuality) ;
  inTree->SetBranchAddress("Muon_TOF",&Muon_TOF) ;
  inTree->SetBranchAddress("NEDNoiseHit",&NEDNoiseHit) ;
  for(int i=0 ; i<NEDs ; i++)
    inTree->SetBranchAddress(EDName[i].c_str(),ElecDet+i) ;
  // create out file
  outPath = conf->GetoutPath()+"/"+CORARSCollectionTime+"/"  ;
  outFileName = "AnalysedResult_"+CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;

  SysTimeRes = new TVectorD(1) ;
  SysTimeResError = new TVectorD(1) ;
  ScinTimeRes = new TVectorD(1) ;
  ScinTimeResError = new TVectorD(1) ;
  SingleRate = new TVectorD(NEDs) ;
  for(int i=0 ; i<NEDs ; i++)
  {
    EDSCTDetEffi = new TVectorD(4*NEDs) ;
    EDSCTDetEffiErrors = new TVectorD(4*NEDs) ;
    EDSCTTimeRes = new TVectorD(4*NEDs) ;
    EDSCTTimeResErrors = new TVectorD(4*NEDs) ;
    EDSCTTimeOffset = new TVectorD(4*NEDs) ;
    EDSCTTimeOffsetErrors = new TVectorD(4*NEDs) ;
    EDSCTEnergyRes = new TVectorD(4*NEDs) ;
    EDSCTEnergyResErrors = new TVectorD(4*NEDs) ;
    EDSCTReEnergyRes = new TVectorD(4*NEDs) ;
    EDSCTReEnergyResErrors = new TVectorD(4*NEDs) ;
    EDSCTSingleParticlePeak = new TVectorD(4*NEDs) ;
    EDSCTSingleParticlePeakErrors = new TVectorD(4*NEDs) ;

    EDPlaneDetEffi[i] = new TMatrixF(26, 26);
    EDPlaneDetEffiErrors[i] = new TMatrixF(26, 26);
    EDSCT1DetEffi[i] = new TMatrixF(19, 4);
    EDSCT1DetEffiErrors[i] = new TMatrixF(19, 4);
    EDSCT2DetEffi[i] = new TMatrixF(19, 4);
    EDSCT2DetEffiErrors[i] = new TMatrixF(19, 4);
    EDSCT3DetEffi[i] = new TMatrixF(19, 4);
    EDSCT3DetEffiErrors[i] = new TMatrixF(19, 4);
    EDSCT4DetEffi[i] = new TMatrixF(19, 4);
    EDSCT4DetEffiErrors[i] = new TMatrixF(19, 4);
    EDSCT1TimeRes[i] = new TMatrixF(19, 4);
    EDSCT1TimeResErrors[i] = new TMatrixF(19, 4);
    EDSCT1TimeOffset[i] = new TMatrixF(19, 4);
    EDSCT1TimeOffsetErrors[i] = new TMatrixF(19, 4);
    EDSCT2TimeRes[i] = new TMatrixF(19, 4);
    EDSCT2TimeResErrors[i] = new TMatrixF(19, 4);
    EDSCT2TimeOffset[i] = new TMatrixF(19, 4);
    EDSCT2TimeOffsetErrors[i] = new TMatrixF(19, 4);
    EDSCT3TimeRes[i] = new TMatrixF(19, 4);
    EDSCT3TimeResErrors[i] = new TMatrixF(19, 4);
    EDSCT3TimeOffset[i] = new TMatrixF(19, 4);
    EDSCT3TimeOffsetErrors[i] = new TMatrixF(19, 4);
    EDSCT4TimeRes[i] = new TMatrixF(19, 4);
    EDSCT4TimeResErrors[i] = new TMatrixF(19, 4);
    EDSCT4TimeOffset[i] = new TMatrixF(19, 4);
    EDSCT4TimeOffsetErrors[i] = new TMatrixF(19, 4);

    EDSCT1EnergyRes[i] = new TMatrixF(19, 4);
    EDSCT1EnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT2EnergyRes[i] = new TMatrixF(19, 4);
    EDSCT2EnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT3EnergyRes[i] = new TMatrixF(19, 4);
    EDSCT3EnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT4EnergyRes[i] = new TMatrixF(19, 4);
    EDSCT4EnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT1ReEnergyRes[i] = new TMatrixF(19, 4);
    EDSCT1ReEnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT2ReEnergyRes[i] = new TMatrixF(19, 4);
    EDSCT2ReEnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT3ReEnergyRes[i] = new TMatrixF(19, 4);
    EDSCT3ReEnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT4ReEnergyRes[i] = new TMatrixF(19, 4);
    EDSCT4ReEnergyResErrors[i] = new TMatrixF(19, 4);
    EDSCT1SingleParticlePeak[i] = new TMatrixF(19, 4);
    EDSCT1SingleParticlePeakErrors[i] = new TMatrixF(19, 4);
    EDSCT2SingleParticlePeak[i] = new TMatrixF(19, 4);
    EDSCT2SingleParticlePeakErrors[i] = new TMatrixF(19, 4);
    EDSCT3SingleParticlePeak[i] = new TMatrixF(19, 4);
    EDSCT3SingleParticlePeakErrors[i] = new TMatrixF(19, 4);
    EDSCT4SingleParticlePeak[i] = new TMatrixF(19, 4);
    EDSCT4SingleParticlePeakErrors[i] = new TMatrixF(19, 4);
  }
}

AnalyseED::~AnalyseED(){}

void AnalyseED::NewHist()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    EDPlaneNHits[i] = bookTH2F(EDName[i]+"PlaneNHits", EDName[i]+" Hits",  "X (cm)", "Y (cm)", "Events", 26, 20., 150., 26, 0., 130.) ;
    EDPlaneNFiredHits[i] = bookTH2F(EDName[i]+"PlaneNFiredHits", EDName[i]+" Fired Hits", "X (cm)", "Y (cm)", "Events", 26, 20., 150., 26, 0., 130.) ;

    EDSCT1NHits[i] = bookTH2F(EDName[i]+EDSCT1Name[i]+"NHits",EDName[i]+EDSCT1Name[i]+" Hits", "X (cm)", "Y (cm)", "Events",4,(*SCT1BLRotatedPoint)[2*i]+2.5,(*SCT1BLRotatedPoint)[2*i]+25-2.5,19,(*SCT1BLRotatedPoint)[2*i+1]+2.5,(*SCT1BLRotatedPoint)[2*i+1]+100-2.5) ;
    EDSCT2NHits[i] = bookTH2F(EDName[i]+EDSCT2Name[i]+"NHits",EDName[i]+EDSCT2Name[i]+" Hits", "X (cm)", "Y (cm)", "Events",4,(*SCT2BLRotatedPoint)[2*i]+2.5,(*SCT2BLRotatedPoint)[2*i]+25-2.5,19,(*SCT2BLRotatedPoint)[2*i+1]+2.5,(*SCT2BLRotatedPoint)[2*i+1]+100-2.5) ;
    EDSCT3NHits[i] = bookTH2F(EDName[i]+EDSCT3Name[i]+"NHits",EDName[i]+EDSCT3Name[i]+" Hits", "X (cm)", "Y (cm)", "Events",4,(*SCT3BLRotatedPoint)[2*i]+2.5,(*SCT3BLRotatedPoint)[2*i]+25-2.5,19,(*SCT3BLRotatedPoint)[2*i+1]+2.5,(*SCT3BLRotatedPoint)[2*i+1]+100-2.5) ;
    EDSCT4NHits[i] = bookTH2F(EDName[i]+EDSCT4Name[i]+"NHits",EDName[i]+EDSCT4Name[i]+" Hits", "X (cm)", "Y (cm)", "Events",4,(*SCT4BLRotatedPoint)[2*i]+2.5,(*SCT4BLRotatedPoint)[2*i]+25.-2.5,19,(*SCT4BLRotatedPoint)[2*i+1]+2.5,(*SCT4BLRotatedPoint)[2*i+1]+100-2.5) ;

    EDSCT1NFiredHits[i] = bookTH2F(EDName[i]+EDSCT1Name[i]+"NFiredHits",EDName[i]+EDSCT1Name[i]+" NFiredHits", "X (cm)", "Y (cm)", "Events",4,(*SCT1BLRotatedPoint)[2*i]+2.5,(*SCT1BLRotatedPoint)[2*i]+25-2.5,19,(*SCT1BLRotatedPoint)[2*i+1]+2.5,(*SCT1BLRotatedPoint)[2*i+1]+100-2.5) ;
    EDSCT2NFiredHits[i] = bookTH2F(EDName[i]+EDSCT2Name[i]+"NFiredHits",EDName[i]+EDSCT2Name[i]+" NFiredHits", "X (cm)", "Y (cm)", "Events",4,(*SCT2BLRotatedPoint)[2*i]+2.5,(*SCT2BLRotatedPoint)[2*i]+25.-2.5,19,(*SCT2BLRotatedPoint)[2*i+1]+2.5,(*SCT2BLRotatedPoint)[2*i+1]+100-2.5) ;
    EDSCT3NFiredHits[i] = bookTH2F(EDName[i]+EDSCT3Name[i]+"NFiredHits",EDName[i]+EDSCT3Name[i]+" NFiredHits", "X (cm)", "Y (cm)", "Events",4,(*SCT3BLRotatedPoint)[2*i]+2.5,(*SCT3BLRotatedPoint)[2*i]+25.-2.5,19,(*SCT3BLRotatedPoint)[2*i+1]+2.5,(*SCT3BLRotatedPoint)[2*i+1]+100-2.5) ;
    EDSCT4NFiredHits[i] = bookTH2F(EDName[i]+EDSCT4Name[i]+"NFiredHits",EDName[i]+EDSCT4Name[i]+" NFiredHits", "X (cm)", "Y (cm)", "Events",4,(*SCT4BLRotatedPoint)[2*i]+2.5,(*SCT4BLRotatedPoint)[2*i]+25.-2.5,19,(*SCT4BLRotatedPoint)[2*i+1]+2.5,(*SCT4BLRotatedPoint)[2*i+1]+100-2.5) ;

    EDSCT1DeltaTime[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"DeltaTime",EDName[i]+EDSCT1Name[i]+" DeltaTime","#DeltaT (ps)","Events",500, -180000.,-80000.) ;
    EDSCT2DeltaTime[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"DeltaTime",EDName[i]+EDSCT2Name[i]+" DeltaTime","#DeltaT (ps)","Events",500, -180000., -80000.) ;
    EDSCT3DeltaTime[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"DeltaTime",EDName[i]+EDSCT3Name[i]+" DeltaTime","#DeltaT (ps)","Events",500, -180000., -80000.) ;
    EDSCT4DeltaTime[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"DeltaTime",EDName[i]+EDSCT4Name[i]+" DeltaTime","#DeltaT (ps)","Events",500, -180000., -80000.) ;

    EDSCT1MeasureTime[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"MeasureTime",EDName[i]+EDSCT1Name[i]+" MeasureTime","#MeasureT (ps)","Events",100, 4800000, 5000000.) ;
    EDSCT2MeasureTime[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"MeasureTime",EDName[i]+EDSCT2Name[i]+" MeasureTime","#MeasureT (ps)","Events",100, 4800000, 5000000.) ;
    EDSCT3MeasureTime[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"MeasureTime",EDName[i]+EDSCT3Name[i]+" MeasureTime","#MeasureT (ps)","Events",100, 4800000, 5000000.) ;
    EDSCT4MeasureTime[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"MeasureTime",EDName[i]+EDSCT4Name[i]+" MeasureTime","#MeasureT (ps)","Events",100, 4800000, 5000000.) ;

    EDSCT1ReconTime[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"ReconTime",EDName[i]+EDSCT1Name[i]+" ReconTime","#ReconT (ps)","Events",100, 1000000., 6000000.) ;
    EDSCT2ReconTime[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"ReconTime",EDName[i]+EDSCT2Name[i]+" ReconTime","#ReconT (ps)","Events",100, 1000000., 6000000.) ;
    EDSCT3ReconTime[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"ReconTime",EDName[i]+EDSCT3Name[i]+" ReconTime","#ReconT (ps)","Events",100, 1000000., 6000000.) ;
    EDSCT4ReconTime[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"ReconTime",EDName[i]+EDSCT4Name[i]+" ReconTime","#ReconT (ps)","Events",100, 1000000., 6000000.) ;


    EDSCT1CorrectedDeltaTime[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"CorrectedDeltaTime",EDName[i]+EDSCT1Name[i]+"Corrected DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;
    EDSCT2CorrectedDeltaTime[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"CorrectedDeltaTime",EDName[i]+EDSCT2Name[i]+"Corrected DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;
    EDSCT3CorrectedDeltaTime[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"CorrectedDeltaTime",EDName[i]+EDSCT3Name[i]+"Corrected DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;
    EDSCT4CorrectedDeltaTime[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"CorrectedDeltaTime",EDName[i]+EDSCT4Name[i]+"Corrected DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;

    EDSCT1SignalCharge[i] =bookTH1F(EDName[i]+EDSCT1Name[i]+"SignalCharge",EDName[i]+EDSCT1Name[i]+" Charge","Charge (Counts)","Events",100,0.,100.);
    EDSCT2SignalCharge[i] =bookTH1F(EDName[i]+EDSCT2Name[i]+"SignalCharge",EDName[i]+EDSCT2Name[i]+" Charge","Charge (Counts)","Events",100,0.,100.);
    EDSCT3SignalCharge[i] =bookTH1F(EDName[i]+EDSCT3Name[i]+"SignalCharge",EDName[i]+EDSCT3Name[i]+" Charge","Charge (Counts)","Events",100,0.,100.);
    EDSCT4SignalCharge[i] =bookTH1F(EDName[i]+EDSCT4Name[i]+"SignalCharge",EDName[i]+EDSCT4Name[i]+" Charge","Charge (Counts)","Events",100,0.,100.);

    for(int j=0 ; j<4 ; j++)
    {
      for(int k=0 ; k<19 ; k++)
      {
        EDSCT1CorrectedDeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT1Name[i]+string(TString::Format("CorrectedDeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;
        EDSCT2CorrectedDeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT2Name[i]+string(TString::Format("CorrectedDeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;
        EDSCT3CorrectedDeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT3Name[i]+string(TString::Format("CorrectedDeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;
        EDSCT4CorrectedDeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT4Name[i]+string(TString::Format("CorrectedDeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events",100, -20000., 20000.) ;

        EDSCT1DeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT1Name[i]+string(TString::Format("DeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events", 500, -180000., -80000.) ;
        EDSCT2DeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT2Name[i]+string(TString::Format("DeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events", 500, -180000., -80000.) ;
        EDSCT3DeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT3Name[i]+string(TString::Format("DeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events", 500, -180000., -80000.) ;
        EDSCT4DeltaTimeBin[i][j][k] =bookTH1F(EDName[i]+EDSCT4Name[i]+string(TString::Format("DeltaTimeBin_%i_%i",j,k)),"DeltaTime","#DeltaT (ps)","Events", 500, -180000., -80000.) ;

        EDSCT1SignalChargeBin[i][j][k] = bookTH1F(EDName[i]+EDSCT1Name[i]+string(TString::Format("SignalChargeBin_%i_%i",j,k)),EDName[i]+EDSCT1Name[i]+string(TString::Format("_%i_%i Signal",j+1,k+1)),"Charge (Counts)","Events", 100,0.,100.);
        EDSCT2SignalChargeBin[i][j][k] = bookTH1F(EDName[i]+EDSCT2Name[i]+string(TString::Format("SignalChargeBin_%i_%i",j,k)),EDName[i]+EDSCT2Name[i]+string(TString::Format("_%i_%i Signal",j+1,k+1)),"Charge (Counts)","Events", 100,0.,100.);
        EDSCT3SignalChargeBin[i][j][k] = bookTH1F(EDName[i]+EDSCT3Name[i]+string(TString::Format("SignalChargeBin_%i_%i",j,k)),EDName[i]+EDSCT3Name[i]+string(TString::Format("_%i_%i Signal",j+1,k+1)),"Charge (Counts)","Events", 100,0.,100.);
        EDSCT4SignalChargeBin[i][j][k] = bookTH1F(EDName[i]+EDSCT4Name[i]+string(TString::Format("SignalChargeBin_%i_%i",j,k)),EDName[i]+EDSCT4Name[i]+string(TString::Format("_%i_%i Signal",j+1,k+1)),"Charge (Counts)","Events", 100,0.,100.);
      }
    }
  }
  DeltaTOF = bookTH1F("Delta_TOF","","#DeltaTOF (ps)","Events",100,-10000.,10000.) ;
  SetHistAttributes(DeltaTOF) ;
}

void AnalyseED::FillHitPoint(int i)
{
  EDPlaneNHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
  if(ElecDet[i].Measure_Time>0)
    EDPlaneNFiredHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;

  int xbin = XAxis[i]->FindBin(ElecDet[i].ED_Coor.x) ;
  int ybin = YAxis[i]->FindBin(ElecDet[i].ED_Coor.y) ;

  if(xbin%2!=0||ybin%2!=0)
    return ;

  switch(xbin)
  {
    case 2:
    {
      EDSCT1NHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
      if(ElecDet[i].Measure_Time>0)
      {
        EDSCT1NFiredHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
        NPreHitsOnSCT[i] += NEDNoiseHit[i] ;
      }
      break ;
    }
    case 4:
    {
      EDSCT2NHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
      if(ElecDet[i].Measure_Time>0)
      {
        EDSCT2NFiredHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
        NPreHitsOnSCT[i] += NEDNoiseHit[i] ;
      }
      break ;
    }
    case 6:
    {
      EDSCT3NHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
      if(ElecDet[i].Measure_Time>0)
      {
        EDSCT3NFiredHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
        NPreHitsOnSCT[i] += NEDNoiseHit[i] ;
      }
      break ;
    }
    case 8:
    {
      EDSCT4NHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
      if(ElecDet[i].Measure_Time>0)
      {
        EDSCT4NFiredHits[i]->Fill(ElecDet[i].ED_Coor.x,ElecDet[i].ED_Coor.y) ;
        NPreHitsOnSCT[i] += NEDNoiseHit[i] ;
      }
      break ;
    }
    default:
      break ;
  }
  return  ;
}
void AnalyseED::GetDetEffi(int i)
{
  for(int j=0 ; j<EDPlaneNHits[i]->GetXaxis()->GetNbins() ; j++)
  {
    for(int k=0;k<EDPlaneNHits[i]->GetYaxis()->GetNbins();k++)
    {
      if(EDPlaneNHits[i]->GetBinContent(j+1,k+1)==0)
        (*EDPlaneDetEffi[i])[j][k] = 0. ;
      else
        (*EDPlaneDetEffi[i])[k][j] = 100*(float)(EDPlaneNFiredHits[i]->GetBinContent(j+1,k+1))/(EDPlaneNHits[i]->GetBinContent(j+1,k+1)) ;
    }
  }
  
  float CountEDSCT1Hits = 0 , CountEDSCT1FiredHits = 0 ;
  float CountEDSCT2Hits = 0 , CountEDSCT2FiredHits = 0 ;
  float CountEDSCT3Hits = 0 , CountEDSCT3FiredHits = 0 ;
  float CountEDSCT4Hits = 0 , CountEDSCT4FiredHits = 0 ;
  for(int j=0 ; j<EDSCT1NFiredHits[i]->GetXaxis()->GetNbins() ; j++)
  {
    for(int k=0 ; k<EDSCT1NFiredHits[i]->GetYaxis()->GetNbins() ; k++)
    {
      (*EDSCT1DetEffi[i])[k][j] = 100.*(float)(EDSCT1NFiredHits[i]->GetBinContent(j+1,k+1))/(EDSCT1NHits[i]->GetBinContent(j+1,k+1)) ;
      (*EDSCT2DetEffi[i])[k][j] = 100.*(float)(EDSCT2NFiredHits[i]->GetBinContent(j+1,k+1))/(EDSCT2NHits[i]->GetBinContent(j+1,k+1)) ;
      (*EDSCT3DetEffi[i])[k][j] = 100.*(float)(EDSCT3NFiredHits[i]->GetBinContent(j+1,k+1))/(EDSCT3NHits[i]->GetBinContent(j+1,k+1)) ;
      (*EDSCT4DetEffi[i])[k][j] = 100.*(float)(EDSCT4NFiredHits[i]->GetBinContent(j+1,k+1))/(EDSCT4NHits[i]->GetBinContent(j+1,k+1)) ;
      CountEDSCT1Hits += EDSCT1NHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT1FiredHits += EDSCT1NFiredHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT2Hits += EDSCT2NHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT2FiredHits += EDSCT2NFiredHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT3Hits += EDSCT3NHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT3FiredHits += EDSCT3NFiredHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT4Hits += EDSCT4NHits[i]->GetBinContent(j+1,k+1) ;
      CountEDSCT4FiredHits += EDSCT4NFiredHits[i]->GetBinContent(j+1,k+1) ;
    }
  } 
  //calculate the detection efficiency
  (*EDSCTDetEffi)[4*i] = 100.*CountEDSCT1FiredHits/CountEDSCT1Hits ;
  (*EDSCTDetEffi)[4*i+1] = 100.*CountEDSCT2FiredHits/CountEDSCT2Hits ;
  (*EDSCTDetEffi)[4*i+2] = 100.*CountEDSCT3FiredHits/CountEDSCT3Hits ;
  (*EDSCTDetEffi)[4*i+3] = 100.*CountEDSCT4FiredHits/CountEDSCT4Hits ;

  EDPlaneNHits[i]->Write() ;
  EDPlaneNFiredHits[i]->Write() ;
  EDSCT1NHits[i]->Write() ;
  EDSCT2NHits[i]->Write() ;
  EDSCT3NHits[i]->Write() ;
  EDSCT4NHits[i]->Write() ;
  EDSCT1NFiredHits[i]->Write() ;
  EDSCT2NFiredHits[i]->Write() ;
  EDSCT3NFiredHits[i]->Write() ;
  EDSCT4NFiredHits[i]->Write() ;
}

void AnalyseED::FillDeltaTOF()
{
  if(EventQuality==1) 
    DeltaTOF->Fill(Muon_TOF.Delta_TOF) ;
}

void AnalyseED::GetSysTimeRes()
{
  TF1 *fun = new TF1("fun","gaus",DeltaTOF->GetMean()-DeltaTOF->GetRMS(),DeltaTOF->GetMean()+DeltaTOF->GetRMS()) ;
  fun->SetLineColor(kRed) ;
  DeltaTOF->Fit(fun,"RQ") ;
  // convert the unit from ps to ns
  (*SysTimeRes)[0] = fun->GetParameter(2)/1000 ;
  (*SysTimeResError)[0] = fun->GetParError(2)/1000 ;
  (*ScinTimeRes)[0] = (*SysTimeRes)[0]/sqrt(2.)/1000 ;
  (*ScinTimeResError)[0] = fun->GetParError(2)/2./1000 ;
  DeltaTOF->Write() ;
}

void AnalyseED::FillDeltaTime(int i)
{
  if(ElecDet[i].Charge>=200||ElecDet[i].Measure_Time<=0)
    return ;
 
  int xbin = XAxis[i]->FindBin(ElecDet[i].ED_Coor.x) ;
  int ybin = YAxis[i]->FindBin(ElecDet[i].ED_Coor.y) ;
  if(xbin%2!=0||ybin%2!=0)
    return ;

  switch(xbin)
  {
    case 2:
    {
      EDSCT1MeasureTime[i]->Fill(ElecDet[i].Measure_Time);
      EDSCT1ReconTime[i]->Fill(ElecDet[i].ReCon_Time);
      EDSCT1DeltaTime[i]->Fill(ElecDet[i].Delta_Time);
      EDSCT1CorrectedDeltaTime[i]->Fill(ElecDet[i].Corrected_Delta_Time);
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT1BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT1BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT1CorrectedDeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Corrected_Delta_Time);
      EDSCT1DeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Delta_Time);
      break ;
    }
    case 4:
    {
      EDSCT2MeasureTime[i]->Fill(ElecDet[i].Measure_Time);
      EDSCT2ReconTime[i]->Fill(ElecDet[i].ReCon_Time);
      EDSCT2DeltaTime[i]->Fill(ElecDet[i].Delta_Time);
      EDSCT2CorrectedDeltaTime[i]->Fill(ElecDet[i].Corrected_Delta_Time);
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT2BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT2BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT2CorrectedDeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Corrected_Delta_Time);
      EDSCT2DeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Delta_Time);
      break ;
    }
    case 6:
    {
      EDSCT3MeasureTime[i]->Fill(ElecDet[i].Measure_Time);
      EDSCT3ReconTime[i]->Fill(ElecDet[i].ReCon_Time);
      EDSCT3DeltaTime[i]->Fill(ElecDet[i].Delta_Time);
      EDSCT3CorrectedDeltaTime[i]->Fill(ElecDet[i].Corrected_Delta_Time);
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT3BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT3BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT3CorrectedDeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Corrected_Delta_Time);
      EDSCT3DeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Delta_Time);
      break ;
    }
    case 8:
    {
      EDSCT4MeasureTime[i]->Fill(ElecDet[i].Measure_Time);
      EDSCT4ReconTime[i]->Fill(ElecDet[i].ReCon_Time);
      EDSCT4DeltaTime[i]->Fill(ElecDet[i].Delta_Time);
      EDSCT4CorrectedDeltaTime[i]->Fill(ElecDet[i].Corrected_Delta_Time);
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT4BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT4BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT4CorrectedDeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Corrected_Delta_Time);
      EDSCT4DeltaTimeBin[i][xindex][yindex]->Fill(ElecDet[i].Delta_Time);
      break ;
    }
    default:
      break ;
  }
}

void AnalyseED::GetTimeOffset(int i)
{
  gStyle->SetOptFit(1) ;
  TF1* function ;
  function = new TF1("sct1function","gaus",EDSCT1DeltaTime[i]->GetMean()-1.5*EDSCT1DeltaTime[i]->GetRMS(),EDSCT1DeltaTime[i]->GetMean()+1.5*EDSCT1DeltaTime[i]->GetRMS()) ;
  function->SetLineColor(kRed) ;
  EDSCT1DeltaTime[i]->Fit(function,"RQ") ;
  EDSCT1DeltaTime[i]->Write();
  (*EDSCTTimeOffset)[4*i] = function->GetParameter(1)/1000;
  (*EDSCTTimeOffsetErrors)[4*i] = function->GetParError(1)/1000;
  delete function ;
  EDSCT1MeasureTime[i]->Write();
  EDSCT1ReconTime[i]->Write();

  function = new TF1("sct2function","gaus",EDSCT2DeltaTime[i]->GetMean()-1.5*EDSCT2DeltaTime[i]->GetRMS(),EDSCT2DeltaTime[i]->GetMean()+1.5*EDSCT2DeltaTime[i]->GetRMS()) ;
  function->SetLineColor(kRed) ;
  EDSCT2DeltaTime[i]->Fit(function,"RQ") ;
  EDSCT2DeltaTime[i]->Write();
  (*EDSCTTimeOffset)[4*i+1] = function->GetParameter(1)/1000;
  (*EDSCTTimeOffsetErrors)[4*i+1] = function->GetParError(1)/1000;
  delete function ;
  EDSCT2MeasureTime[i]->Write();
  EDSCT2ReconTime[i]->Write();

  function = new TF1("sct3function","gaus",EDSCT3DeltaTime[i]->GetMean()-1.5*EDSCT3DeltaTime[i]->GetRMS(),EDSCT3DeltaTime[i]->GetMean()+1.5*EDSCT3DeltaTime[i]->GetRMS()) ;
  function->SetLineColor(kRed) ;
  EDSCT3DeltaTime[i]->Fit(function,"RQ") ;
  EDSCT3DeltaTime[i]->Write();
  (*EDSCTTimeOffset)[4*i+2] = function->GetParameter(1)/1000;
  (*EDSCTTimeOffsetErrors)[4*i+2] = function->GetParError(1)/1000;
  delete function ;
  EDSCT3MeasureTime[i]->Write();
  EDSCT3ReconTime[i]->Write();

  function = new TF1("sct4function","gaus",EDSCT4DeltaTime[i]->GetMean()-1.5*EDSCT4DeltaTime[i]->GetRMS(),EDSCT4DeltaTime[i]->GetMean()+1.5*EDSCT4DeltaTime[i]->GetRMS()) ;
  function->SetLineColor(kRed) ;
  EDSCT4DeltaTime[i]->Fit(function,"RQ") ;
  EDSCT4DeltaTime[i]->Write();
  (*EDSCTTimeOffset)[4*i+3] = function->GetParameter(1)/1000;
  (*EDSCTTimeOffsetErrors)[4*i+3] = function->GetParError(1)/1000;
  delete function ;
  EDSCT4MeasureTime[i]->Write();
  EDSCT4ReconTime[i]->Write();

  for(int j=0 ; j<4 ; j++)
  {
    for(int k=0 ; k<19 ; k++)
    {
      function = new TF1(TString::Format("sct1function[%i][%i][%i]",i,j,k),"gaus",EDSCT1DeltaTimeBin[i][j][k]->GetMean()-1.5*EDSCT1DeltaTimeBin[i][j][k]->GetRMS(),EDSCT1DeltaTimeBin[i][j][k]->GetMean()+1.5*EDSCT1DeltaTimeBin[i][j][k]->GetRMS()) ;
      EDSCT1DeltaTimeBin[i][j][k]->Fit(function,"RQ") ;
      (*EDSCT1TimeOffset[i])[k][j] = function->GetParameter(1)/1000. ;
      (*EDSCT1TimeOffsetErrors[i])[k][j] = function->GetParError(1)/1000. ;
      EDSCT1DeltaTimeBin[i][j][k]->Write() ;
      delete function ;

      function = new TF1(TString::Format("sct2function[%i][%i][%i]",i,j,k),"gaus",EDSCT2DeltaTimeBin[i][j][k]->GetMean()-2*EDSCT2DeltaTimeBin[i][j][k]->GetRMS(),EDSCT2DeltaTimeBin[i][j][k]->GetMean()+2*EDSCT2DeltaTimeBin[i][j][k]->GetRMS()) ;
      EDSCT2DeltaTimeBin[i][j][k]->Fit(function,"RQ") ;
      (*EDSCT2TimeOffset[i])[k][j] = function->GetParameter(1)/1000. ;
      (*EDSCT2TimeOffsetErrors[i])[k][j] = function->GetParError(1)/1000. ;
      EDSCT2DeltaTimeBin[i][j][k]->Write() ;
      delete function ;

      function = new TF1(TString::Format("sct3function[%i][%i][%i]",i,j,k),"gaus",EDSCT3DeltaTimeBin[i][j][k]->GetMean()-2*EDSCT3DeltaTimeBin[i][j][k]->GetRMS(),EDSCT3DeltaTimeBin[i][j][k]->GetMean()+2*EDSCT3DeltaTimeBin[i][j][k]->GetRMS()) ;
      EDSCT3DeltaTimeBin[i][j][k]->Fit(function,"RQ") ;
      (*EDSCT3TimeOffset[i])[k][j] = function->GetParameter(1)/1000. ;
      (*EDSCT3TimeOffsetErrors[i])[k][j] = function->GetParError(1)/1000. ;
      EDSCT3DeltaTimeBin[i][j][k]->Write() ;
      delete function ;

      function = new TF1(TString::Format("sct4function[%i][%i][%i]",i,j,k),"gaus",EDSCT4DeltaTimeBin[i][j][k]->GetMean()-2*EDSCT4DeltaTimeBin[i][j][k]->GetRMS(),EDSCT4DeltaTimeBin[i][j][k]->GetMean()+2*EDSCT4DeltaTimeBin[i][j][k]->GetRMS()) ;
      EDSCT3DeltaTimeBin[i][j][k]->Fit(function,"RQ") ;
      (*EDSCT3TimeOffset[i])[k][j] = function->GetParameter(1)/1000. ;
      (*EDSCT3TimeOffsetErrors[i])[k][j] = function->GetParError(1)/1000. ;
      EDSCT3DeltaTimeBin[i][j][k]->Write() ;
      delete function ;
    }
  }
}

std::pair <float, float> AnalyseED::GetTimeRes(TH1F* EDSCTCorrectedDeltaTime)
{
  gStyle->SetOptFit(1) ;
  TF1* function =  new TF1("function","gaus",EDSCTCorrectedDeltaTime->GetMean()-1.5*EDSCTCorrectedDeltaTime->GetRMS(),EDSCTCorrectedDeltaTime->GetMean()+1.5*EDSCTCorrectedDeltaTime->GetRMS()) ;
  function->SetLineColor(kRed) ;
  EDSCTCorrectedDeltaTime->Fit(function,"RQ") ;
  EDSCTCorrectedDeltaTime->Write();
  std::pair <float, float> mean = std::make_pair(function->GetParameter(2), function->GetParError(2)) ;
  delete function ;
  return mean ;
}
void AnalyseED::GetTimeRes(int i)
{
  (*EDSCTTimeRes)[4*i] = GetTimeRes(EDSCT1CorrectedDeltaTime[i]).first/1000;
  (*EDSCTTimeResErrors)[4*i] = GetTimeRes(EDSCT1CorrectedDeltaTime[i]).second/1000;

  (*EDSCTTimeRes)[4*i+1] = GetTimeRes(EDSCT2CorrectedDeltaTime[i]).first/1000;
  (*EDSCTTimeResErrors)[4*i+1] = GetTimeRes(EDSCT2CorrectedDeltaTime[i]).second/1000;

  (*EDSCTTimeRes)[4*i+2] = GetTimeRes(EDSCT3CorrectedDeltaTime[i]).first/1000;
  (*EDSCTTimeResErrors)[4*i+2] = GetTimeRes(EDSCT3CorrectedDeltaTime[i]).second/1000;

  (*EDSCTTimeRes)[4*i+3] = GetTimeRes(EDSCT4CorrectedDeltaTime[i]).first/1000;
  (*EDSCTTimeResErrors)[4*i+3] = GetTimeRes(EDSCT4CorrectedDeltaTime[i]).second/1000;
  //--------------------------------------------
  double re_sigma = sqrt((pow(346.5-26,2)-2*(346.5-26)*(346.5-conf->EDZ[i])+2*pow(346.5-conf->EDZ[i],2))/pow(346.5-26,2))*(*ScinTimeRes)[0] ;
  double re_sigmaerror = sqrt((pow(346.5-26,2)-2*(346.5-26)*(346.5-conf->EDZ[i])+2*pow(346.5-conf->EDZ[i],2))/pow(346.5-26,2))*(*ScinTimeResError)[0] ;
  (*EDSCTTimeRes)[4*i] = sqrt(pow((*EDSCTTimeRes)[4*i],2)-pow(re_sigma,2)) ;
  (*EDSCTTimeResErrors)[4*i] = sqrt(pow((*EDSCTTimeResErrors)[4*i],2)-pow(re_sigmaerror,2)) ;
  (*EDSCTTimeRes)[4*i+1] = sqrt(pow((*EDSCTTimeRes)[4*i+1],2)-pow(re_sigma,2)) ;
  (*EDSCTTimeResErrors)[4*i+1] = sqrt(pow((*EDSCTTimeResErrors)[4*i+1],2)-pow(re_sigmaerror,2)) ;
  (*EDSCTTimeRes)[4*i+2] = sqrt(pow((*EDSCTTimeRes)[4*i+2],2)-pow(re_sigma,2)) ;
  (*EDSCTTimeResErrors)[4*i+2] = sqrt(pow((*EDSCTTimeResErrors)[4*i+2],2)-pow(re_sigmaerror,2)) ;
  (*EDSCTTimeRes)[4*i+3] = sqrt(pow((*EDSCTTimeRes)[4*i+3],2)-pow(re_sigma,2)) ;
  (*EDSCTTimeResErrors)[4*i+3] = sqrt(pow((*EDSCTTimeResErrors)[4*i+3],2)-pow(re_sigmaerror,2)) ;
  //---------------------------------------------
  for(int j=0 ; j<4 ; j++)
  {
    for(int k=0 ; k<19 ; k++)
    {
      (*EDSCT1TimeRes[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT1CorrectedDeltaTimeBin[i][j][k]).first/1000,2)-pow(re_sigma,2)) ;
      (*EDSCT1TimeResErrors[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT1CorrectedDeltaTimeBin[i][j][k]).second/1000,2)-pow(re_sigmaerror,2)) ;

      (*EDSCT2TimeRes[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT2CorrectedDeltaTimeBin[i][j][k]).first/1000,2)-pow(re_sigma,2)) ;
      (*EDSCT2TimeResErrors[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT2CorrectedDeltaTimeBin[i][j][k]).second/1000,2)-pow(re_sigmaerror,2)) ;

      (*EDSCT3TimeRes[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT3CorrectedDeltaTimeBin[i][j][k]).first/1000,2)-pow(re_sigma,2)) ;
      (*EDSCT3TimeResErrors[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT3CorrectedDeltaTimeBin[i][j][k]).second/1000,2)-pow(re_sigmaerror,2)) ;

      (*EDSCT4TimeRes[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT4CorrectedDeltaTimeBin[i][j][k]).first/1000,2)-pow(re_sigma,2)) ;
      (*EDSCT4TimeResErrors[i])[k][j] = sqrt(pow(GetTimeRes(EDSCT4CorrectedDeltaTimeBin[i][j][k]).second/1000,2)-pow(re_sigmaerror,2)) ;
    }
  }
}

void AnalyseED::FillCharge(int i)
{
  float Charge = ElecDet[i].Charge ;
  if(conf->useNormalizedCharge)
    Charge = ElecDet[i].NormalizedCharge ;

  if(ElecDet[i].Charge>=200||ElecDet[i].Measure_Time<=0)
    return ;

  int xbin = XAxis[i]->FindBin(ElecDet[i].ED_Coor.x) ;
  int ybin = YAxis[i]->FindBin(ElecDet[i].ED_Coor.y) ;
  if(xbin%2!=0||ybin%2!=0)
    return ;

  switch(xbin)
  {
    case 2:
    {
      EDSCT1SignalCharge[i]->Fill(Charge) ;
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT1BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT1BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT1SignalChargeBin[i][xindex][yindex]->Fill(Charge) ;
      break ;
    }
    case 4:
    {
      EDSCT2SignalCharge[i]->Fill(Charge) ;
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT2BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT2BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT2SignalChargeBin[i][xindex][yindex]->Fill(Charge) ;
      break ;
    }
    case 6:
    {
      EDSCT3SignalCharge[i]->Fill(Charge) ;
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT3BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT3BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT3SignalChargeBin[i][xindex][yindex]->Fill(Charge) ;
      break ;
    }
    case 8:
    {
      EDSCT4SignalCharge[i]->Fill(Charge) ;
      int xindex = (int)((ElecDet[i].ED_Coor.x-(*SCT4BLRotatedPoint)[2*i]-2.5)/5) ;
      int yindex = (int)((ElecDet[i].ED_Coor.y-(*SCT4BLRotatedPoint)[2*i+1]-2.5)/5) ;
      EDSCT4SignalChargeBin[i][xindex][yindex]->Fill(Charge) ;
      break ;
    }
    default:
      break ;
  }
  return ;
}

void AnalyseED::Convolution(TH1F* h, float fitlowedge, float fithighedge, float *singleparticlepeak, float *singleparticlepeakerror, float* resolution, float *resolutionerror, string canvasname)
{
  RooRealVar x("x","Counts", fitlowedge, fithighedge) ;
  RooRealVar landaumean("mean","meanoflandau",h->GetMean(), h->GetMean()-h->GetRMS(), h->GetMean()+h->GetRMS()) ;
  RooRealVar landausigma("sigma","width of landau", h->GetRMS()*0.2, h->GetRMS()*0.05, h->GetRMS()*0.4) ;
  RooRealVar gaussigma("gaussigma","width of gaussian",h->GetRMS()*0.5, h->GetRMS()*0.2, h->GetRMS()*0.8) ;
  RooRealVar gausmean("gausmean","mean of gaussian",0.) ;

  RooLandau landau("landau","landau PDF",x,landaumean,landausigma) ;
  RooGaussian gaus("gaus","gaus",x,gausmean,gaussigma);
  RooFFTConvPdf lxg("lxg","landau (X) gaus",x,landau,gaus) ;

  RooDataHist* data = new RooDataHist("charge_distribution","charge distribution",RooArgList(x),h);
  RooFitResult* result = lxg.fitTo(*data, Save(), PrintLevel(-1) ) ;
  RooPlot* frame = x.frame(Title(h->GetTitle())) ;
  data->plotOn(frame) ;
  lxg.plotOn(frame, LineColor(kRed)) ;
  //landau.plotOn(frame,LineColor(kBlue),LineStyle(kDashed)) ;
  float chi2 = frame->chiSquare(4) ;

  TF1* lxgfunc1 =(TF1*) lxg.asTF(RooArgList(x))->Clone() ;
  float resolution1 = lxgfunc1->GetX(lxgfunc1->GetMaximum()/2.0,lxgfunc1->GetMaximumX(),1000)-lxgfunc1->GetX(lxgfunc1->GetMaximum()/2.0,-100,lxgfunc1->GetMaximumX()) ;
  float singleparticlepeak1 = lxgfunc1->GetMaximumX();

  std::vector<float> landaumeanvec ;
  landaumeanvec.push_back(landaumean.getVal()) ;
  landaumeanvec.push_back(landaumean.getVal()+landaumean.getError()) ;
  landaumeanvec.push_back(landaumean.getVal()-landaumean.getError()) ;
  std::vector< float > landausigmavec ;
  landausigmavec.push_back(landausigma.getVal()) ;
  landausigmavec.push_back(landausigma.getVal()+landausigma.getError()) ;
  landausigmavec.push_back(landausigma.getVal()-landausigma.getError()) ;
  std::vector< float > gaussigmavec ;
  gaussigmavec.push_back(gaussigma.getVal()) ;
  gaussigmavec.push_back(gaussigma.getVal()+gaussigma.getError()) ;
  gaussigmavec.push_back(gaussigma.getVal()-gaussigma.getError()) ;

  float maxsingleparticlepeakerror= 0. ;
  float maxresoltuionerror= 0. ;
  for(int i=0 ; i<(int)landaumeanvec.size() ; i++)
  {
    for(int j=0 ; j<(int)landausigmavec.size() ; j++)
    {
      for(int k=0 ; k<(int)gaussigmavec.size() ; k++)
      {
        landaumean.setVal(landaumeanvec.at(i)) ;
        landausigma.setVal(landausigmavec.at(j)) ;
        gaussigma.setVal(gaussigmavec.at(k)) ;
        TF1* lxgfunc2 =(TF1*) lxg.asTF(RooArgList(x))->Clone() ;
        float resolution2 = lxgfunc2->GetX(lxgfunc2->GetMaximum()/2.0,lxgfunc2->GetMaximumX(),1000)-lxgfunc2->GetX(lxgfunc2->GetMaximum()/2.0,-100,lxgfunc2->GetMaximumX()) ;
        float singleparticlepeak2 = lxgfunc2->GetMaximumX();
        if(abs(resolution2-resolution1)>maxresoltuionerror)
          maxresoltuionerror = abs(resolution2-resolution1) ;
        if(abs(singleparticlepeak2-singleparticlepeak1)>maxsingleparticlepeakerror)
          maxsingleparticlepeakerror = abs(singleparticlepeak2-singleparticlepeak1) ;
        delete lxgfunc2 ;
      }
    }
  }

  *singleparticlepeak = singleparticlepeak1 ;
  *singleparticlepeakerror = maxsingleparticlepeakerror ;
  *resolution = resolution1 ;
  *resolutionerror = maxresoltuionerror ;

  TCanvas* c = new TCanvas("c", "", 800, 400) ;
  c->Divide(2,1) ;
  c->cd(1) ;
  frame->Draw() ;
  TLatex latex1;
  latex1.SetNDC() ;
  latex1.SetTextSize(0.04) ;
  latex1.SetTextAlign(13);  //align at top
  latex1.DrawLatex(0.4,0.85,TString::Format("Peak: %0.2f#pm%0.2f", *singleparticlepeak, *singleparticlepeakerror));
  latex1.DrawLatex(0.4,0.8,TString::Format("Resolution: %0.2f#pm%0.2f", *resolution, *resolutionerror));
  latex1.DrawLatex(0.4,0.75,TString::Format("Chi2: %0.2f", chi2));
  c->cd(2) ;
  h->Draw() ;
  c->Print((canvasname+".pdf").c_str()) ;
  delete data ;
  delete frame ;
  delete lxgfunc1 ;
  //delete lxgfunc2 ;
  delete c ;
  delete result ;
  h->Write() ;
}

void AnalyseED::GetEnergyRes(int i)
{
  float singleparticlepeak = 0.; 
  float singleparticlepeakerror = 0. ; 
  float resulution = 0. ; 
  float resulutionerror = 0. ; 
  float lowedge = 0. ; float highedge =  0. ; 
  int binwidth = int(EDSCT1SignalCharge[i]->GetBinWidth(1)) ;
  lowedge = int(EDSCT1SignalCharge[i]->GetMean()-2.*EDSCT1SignalCharge[i]->GetRMS())/binwidth*binwidth;
  highedge = int(EDSCT1SignalCharge[i]->GetMean()+4*EDSCT1SignalCharge[i]->GetRMS())/binwidth*binwidth ;
  Convolution(EDSCT1SignalCharge[i], lowedge, highedge, &singleparticlepeak, &singleparticlepeakerror, &resulution, &resulutionerror, std::string(outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT1Name[i]+"_Charge")) ;
  (*EDSCTSingleParticlePeak)[4*i] = singleparticlepeak;
  (*EDSCTSingleParticlePeakErrors)[4*i] = sqrt(pow(singleparticlepeakerror,2)) ;
  (*EDSCTEnergyRes)[4*i] = resulution ;
  (*EDSCTEnergyResErrors)[4*i] = resulutionerror ;
  (*EDSCTReEnergyRes)[4*i] = (*EDSCTEnergyRes)[4*i]/(*EDSCTSingleParticlePeak)[4*i] ;
  (*EDSCTReEnergyResErrors)[4*i] = (*EDSCTReEnergyRes)[4*i]*sqrt(pow((*EDSCTEnergyResErrors)[4*i]/(*EDSCTEnergyRes)[4*i],2)+pow((*EDSCTSingleParticlePeakErrors)[4*i]/(*EDSCTSingleParticlePeak)[4*i],2)) ;

  binwidth = int(EDSCT2SignalCharge[i]->GetBinWidth(1)) ;
  lowedge = int(EDSCT2SignalCharge[i]->GetMean()-2*EDSCT2SignalCharge[i]->GetRMS())/binwidth*binwidth;
  highedge = int(EDSCT2SignalCharge[i]->GetMean()+4*EDSCT2SignalCharge[i]->GetRMS())/binwidth*binwidth ;
  Convolution(EDSCT2SignalCharge[i], lowedge, highedge, &singleparticlepeak, &singleparticlepeakerror, &resulution, &resulutionerror, string(outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT2Name[i]+"_Charge")) ;
  (*EDSCTSingleParticlePeak)[4*i+1] = singleparticlepeak;
  (*EDSCTSingleParticlePeakErrors)[4*i+1] = sqrt(pow(singleparticlepeakerror,2)) ;
  (*EDSCTEnergyRes)[4*i+1] = resulution ;
  (*EDSCTEnergyResErrors)[4*i+1]=resulutionerror ;
  (*EDSCTReEnergyRes)[4*i+1] = (*EDSCTEnergyRes)[4*i+1]/(*EDSCTSingleParticlePeak)[4*i+1] ;
  (*EDSCTReEnergyResErrors)[4*i+1] = (*EDSCTReEnergyRes)[4*i+1]*sqrt(pow((*EDSCTEnergyResErrors)[4*i+1]/(*EDSCTEnergyRes)[4*i+1],2)+pow((*EDSCTSingleParticlePeakErrors)[4*i+1]/(*EDSCTSingleParticlePeak)[4*i+1],2)) ;

  binwidth = int(EDSCT3SignalCharge[i]->GetBinWidth(1)) ;
  lowedge = int(EDSCT3SignalCharge[i]->GetMean()-2*EDSCT3SignalCharge[i]->GetRMS())/binwidth*binwidth;
  highedge = int(EDSCT3SignalCharge[i]->GetMean()+4*EDSCT3SignalCharge[i]->GetRMS())/binwidth*binwidth ;
  Convolution(EDSCT3SignalCharge[i], lowedge, highedge, &singleparticlepeak, &singleparticlepeakerror, &resulution, &resulutionerror, string(outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT3Name[i]+"_Charge")) ;
  (*EDSCTSingleParticlePeak)[4*i+2] = singleparticlepeak;
  (*EDSCTSingleParticlePeakErrors)[4*i+2] = sqrt(pow(singleparticlepeakerror,2)) ;
  (*EDSCTEnergyRes)[4*i+2] = resulution ;
  (*EDSCTEnergyResErrors)[4*i+2]=resulutionerror ;
  (*EDSCTReEnergyRes)[4*i+2] = (*EDSCTEnergyRes)[4*i+2]/(*EDSCTSingleParticlePeak)[4*i+2] ;
  (*EDSCTReEnergyResErrors)[4*i+2] = (*EDSCTReEnergyRes)[4*i+2]*sqrt(pow((*EDSCTEnergyResErrors)[4*i+2]/(*EDSCTEnergyRes)[4*i+2],2)+pow((*EDSCTSingleParticlePeakErrors)[4*i+2]/(*EDSCTSingleParticlePeak)[4*i+2],2)) ;

  binwidth = int(EDSCT4SignalCharge[i]->GetBinWidth(1)) ;
  lowedge = int(EDSCT4SignalCharge[i]->GetMean()-2*EDSCT4SignalCharge[i]->GetRMS())/binwidth*binwidth;
  highedge = int(EDSCT4SignalCharge[i]->GetMean()+4*EDSCT4SignalCharge[i]->GetRMS())/binwidth*binwidth ;
  Convolution(EDSCT4SignalCharge[i], lowedge, highedge, &singleparticlepeak, &singleparticlepeakerror, &resulution, &resulutionerror, string(outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT4Name[i]+"_Charge")) ;
  (*EDSCTSingleParticlePeak)[4*i+3] = singleparticlepeak;
  (*EDSCTSingleParticlePeakErrors)[4*i+3] = sqrt(pow(singleparticlepeakerror,3)) ;
  (*EDSCTEnergyRes)[4*i+3] = resulution ;
  (*EDSCTEnergyResErrors)[4*i+3]=resulutionerror ;
  (*EDSCTReEnergyRes)[4*i+3] = (*EDSCTEnergyRes)[4*i+3]/(*EDSCTSingleParticlePeak)[4*i+3] ;
  (*EDSCTReEnergyResErrors)[4*i+3] = (*EDSCTReEnergyRes)[4*i+3]*sqrt(pow((*EDSCTEnergyResErrors)[4*i+3]/(*EDSCTEnergyRes)[4*i+3],2)+pow((*EDSCTSingleParticlePeakErrors)[4*i+3]/(*EDSCTSingleParticlePeak)[4*i+3],2)) ;

  TCanvas *c1 = new TCanvas(TString::Format("canvas[%i]_1",i),"",800,400) ;
  TCanvas *c2 = new TCanvas(TString::Format("canvas[%i]_2",i),"",800,400) ;
  TCanvas *c3 = new TCanvas(TString::Format("canvas[%i]_3",i),"",800,400) ;
  TCanvas *c4 = new TCanvas(TString::Format("canvas[%i]_4",i),"",800,400) ;
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleAlign(23);
  c1->SetGrid() ;
  c2->SetGrid() ;
  c3->SetGrid() ;
  c4->SetGrid() ;
  c1->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT1Name[i]+"_Charge_Pad.pdf[").c_str()) ;
  c2->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT2Name[i]+"_Charge_Pad.pdf[").c_str()) ;
  c3->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT3Name[i]+"_Charge_Pad.pdf[").c_str()) ;
  c4->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT4Name[i]+"_Charge_Pad.pdf[").c_str()) ;
  for(Int_t j=0 ; j<4 ; j++)
  {
    for(Int_t k=0 ; k<19 ; k++)
    {
      binwidth = int(EDSCT1SignalChargeBin[i][j][k]->GetBinWidth(1)) ;
      lowedge = int(EDSCT1SignalChargeBin[i][j][k]->GetMean()-2.*EDSCT1SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth;
      highedge = int(EDSCT1SignalChargeBin[i][j][k]->GetMean()+4*EDSCT1SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth ;
      Convolution(EDSCT1SignalChargeBin[i][j][k], lowedge, highedge, &singleparticlepeak, &singleparticlepeakerror, &resulution, &resulutionerror, string(outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT1Name[i]+"_Charge_Pad")) ;
      (*EDSCT1SingleParticlePeak[i])[k][j] = singleparticlepeak;
      (*EDSCT1SingleParticlePeakErrors[i])[k][j] = sqrt(pow(singleparticlepeakerror,2)) ;
      (*EDSCT1EnergyRes[i])[k][j] = resulution ;
      (*EDSCT1EnergyResErrors[i])[k][j] = resulutionerror ;
      (*EDSCT1ReEnergyRes[i])[k][j] = (*EDSCT1EnergyRes[i])[k][j]/(*EDSCT1SingleParticlePeak[i])[k][j] ;
      (*EDSCT1ReEnergyResErrors[i])[k][j] = (*EDSCT1ReEnergyRes[i])[k][j]*sqrt(pow((*EDSCT1EnergyResErrors[i])[k][j]/(*EDSCT1EnergyRes[i])[k][j],2)+pow((*EDSCT1SingleParticlePeakErrors[i])[k][j]/(*EDSCT1SingleParticlePeak[i])[k][j],2));

      binwidth = int(EDSCT2SignalChargeBin[i][j][k]->GetBinWidth(1)) ;
      lowedge = int(EDSCT2SignalChargeBin[i][j][k]->GetMean()-2.*EDSCT2SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth;
      highedge = int(EDSCT2SignalChargeBin[i][j][k]->GetMean()+4*EDSCT2SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth ;
      Convolution(EDSCT2SignalChargeBin[i][j][k], lowedge, highedge, &singleparticlepeak,  &singleparticlepeakerror, &resulution, &resulutionerror, string(outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT2Name[i]+"_Charge_Pad")) ;
      (*EDSCT2SingleParticlePeak[i])[k][j] = singleparticlepeak;
      (*EDSCT2SingleParticlePeakErrors[i])[k][j] = sqrt(pow(singleparticlepeakerror,2)) ;
      (*EDSCT2EnergyRes[i])[k][j] = resulution ;
      (*EDSCT2EnergyResErrors[i])[k][j] = resulutionerror ;
      (*EDSCT2ReEnergyRes[i])[k][j] = (*EDSCT2EnergyRes[i])[k][j]/(*EDSCT2SingleParticlePeak[i])[k][j] ;
      (*EDSCT2ReEnergyResErrors[i])[k][j] = (*EDSCT2ReEnergyRes[i])[k][j]*sqrt(pow((*EDSCT2EnergyResErrors[i])[k][j]/(*EDSCT2EnergyRes[i])[k][j],2)+pow((*EDSCT2SingleParticlePeakErrors[i])[k][j]/(*EDSCT2SingleParticlePeak[i])[k][j],2)) ;

      binwidth = int(EDSCT3SignalChargeBin[i][j][k]->GetBinWidth(1)) ;
      lowedge = int(EDSCT3SignalChargeBin[i][j][k]->GetMean()-2.*EDSCT3SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth;
      highedge = int(EDSCT3SignalChargeBin[i][j][k]->GetMean()+4*EDSCT3SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth ;
      Convolution(EDSCT3SignalChargeBin[i][j][k], lowedge, highedge, &singleparticlepeak,  &singleparticlepeakerror, &resulution, &resulutionerror, outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT3Name[i]+"_Charge_Pad") ;
      (*EDSCT3SingleParticlePeak[i])[k][j] = singleparticlepeak;
      (*EDSCT3SingleParticlePeakErrors[i])[k][j] = sqrt(pow(singleparticlepeakerror,2)) ;
      (*EDSCT3EnergyRes[i])[k][j] = resulution ;
      (*EDSCT3EnergyResErrors[i])[k][j] = resulutionerror ;
      (*EDSCT3ReEnergyRes[i])[k][j] = (*EDSCT3EnergyRes[i])[k][j]/(*EDSCT3SingleParticlePeak[i])[k][j] ;
      (*EDSCT3ReEnergyResErrors[i])[k][j] = (*EDSCT3ReEnergyRes[i])[k][j]*sqrt(pow((*EDSCT3EnergyResErrors[i])[k][j]/(*EDSCT3EnergyRes[i])[k][j],2)+pow((*EDSCT3SingleParticlePeakErrors[i])[k][j]/(*EDSCT3SingleParticlePeak[i])[k][j],2)) ;

      binwidth = int(EDSCT4SignalChargeBin[i][j][k]->GetBinWidth(1)) ;
      lowedge = int(EDSCT4SignalChargeBin[i][j][k]->GetMean()-2.*EDSCT4SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth;
      highedge = int(EDSCT4SignalChargeBin[i][j][k]->GetMean()+4*EDSCT4SignalChargeBin[i][j][k]->GetRMS())/binwidth*binwidth ;
      Convolution(EDSCT4SignalChargeBin[i][j][k], lowedge, highedge, &singleparticlepeak,  &singleparticlepeakerror, &resulution, &resulutionerror, outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT4Name[i]+"_Charge_Pad") ;
      (*EDSCT4SingleParticlePeak[i])[k][j] = singleparticlepeak;
      (*EDSCT4SingleParticlePeakErrors[i])[k][j] = sqrt(pow(singleparticlepeakerror,2)) ;
      (*EDSCT4EnergyRes[i])[k][j] = resulution ;
      (*EDSCT4EnergyResErrors[i])[k][j] = resulutionerror ;
      (*EDSCT4ReEnergyRes[i])[k][j] = (*EDSCT4EnergyRes[i])[k][j]/(*EDSCT4SingleParticlePeak[i])[k][j] ;
      (*EDSCT4ReEnergyResErrors[i])[k][j] = (*EDSCT4ReEnergyRes[i])[k][j]*sqrt(pow((*EDSCT4EnergyResErrors[i])[k][j]/(*EDSCT4EnergyRes[i])[k][j],2)+pow((*EDSCT4SingleParticlePeakErrors[i])[k][j]/(*EDSCT4SingleParticlePeak[i])[k][j],2)) ;
    }
  }
  c1->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT1Name[i]+"_Charge_Pad.pdf]").c_str()) ;
  c2->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT2Name[i]+"_Charge_Pad.pdf]").c_str()) ;
  c3->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT3Name[i]+"_Charge_Pad.pdf]").c_str()) ;
  c4->Print((outPath+CORARSCollectionTime+"_"+EDName[i]+EDSCT4Name[i]+"_Charge_Pad.pdf]").c_str()) ;

  return ;
}

TH2F* AnalyseED::CloneTH2F(TMatrixF* matrix, string name, string title, string zaxislabel)
{
  TH2F* hclone = new TH2F(*matrix) ;
  hclone->SetName(name.c_str()) ;
  hclone->SetTitle(title.c_str()) ;
  hclone->GetZaxis()->SetTitle(zaxislabel.c_str()) ;
  return hclone ;
}

void AnalyseED::ScanED(int i)
{
  EDPlaneDetEffi2DScan[i] = CloneTH2F(EDPlaneDetEffi[i], EDName[i]+"PlaneDetEffi2DScan", EDName[i]+" Plane Detection Efficiency", "Detection Efficiency %") ;
  EDSCT1DetEffi2DScan[i] = CloneTH2F(EDSCT1DetEffi[i], EDName[i]+EDSCT1Name[i]+"DetEffi2DScan", EDName[i]+EDSCT1Name[i]+" Detection Efficiency", "Detection Efficiency %") ;
  EDSCT2DetEffi2DScan[i] = CloneTH2F(EDSCT2DetEffi[i], EDName[i]+EDSCT2Name[i]+"DetEffi2DScan", EDName[i]+EDSCT2Name[i]+" Detection Efficiency", "Detection Efficiency %") ;
  EDSCT3DetEffi2DScan[i] = CloneTH2F(EDSCT3DetEffi[i], EDName[i]+EDSCT3Name[i]+"DetEffi2DScan", EDName[i]+EDSCT3Name[i]+" Detection Efficiency", "Detection Efficiency %") ;
  EDSCT4DetEffi2DScan[i] = CloneTH2F(EDSCT4DetEffi[i], EDName[i]+EDSCT4Name[i]+"DetEffi2DScan", EDName[i]+EDSCT4Name[i]+" Detection Efficiency", "Detection Efficiency %") ;


  EDSCT1DetEffi1DScan[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"DetEffi1DScan", EDName[i]+EDSCT1Name[i]+"Detection Efficiency", "", "Detection Efficiency %", EDSCT1DetEffi[i]->GetNcols()*EDSCT1DetEffi[i]->GetNrows(),0.,EDSCT1DetEffi[i]->GetNcols()*EDSCT1DetEffi[i]->GetNrows()) ;
  EDSCT2DetEffi1DScan[i] =  bookTH1F(EDName[i]+EDSCT2Name[i]+"DetEffi1DScan", EDName[i]+EDSCT2Name[i]+"Detection Efficiency", "", "Detection Efficiency %", EDSCT2DetEffi[i]->GetNcols()*EDSCT2DetEffi[i]->GetNrows(),0.,EDSCT2DetEffi[i]->GetNcols()*EDSCT2DetEffi[i]->GetNrows()) ;
  EDSCT3DetEffi1DScan[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"DetEffi1DScan", EDName[i]+EDSCT3Name[i]+"Detection Efficiency", "", "Detection Efficiency %", EDSCT3DetEffi[i]->GetNcols()*EDSCT3DetEffi[i]->GetNrows(),0.,EDSCT3DetEffi[i]->GetNcols()*EDSCT3DetEffi[i]->GetNrows()) ;
  EDSCT4DetEffi1DScan[i] =  bookTH1F(EDName[i]+EDSCT4Name[i]+"DetEffi1DScan", EDName[i]+EDSCT4Name[i]+"Detection Efficiency", "", "Detection Efficiency %", EDSCT4DetEffi[i]->GetNcols()*EDSCT4DetEffi[i]->GetNrows(),0.,EDSCT4DetEffi[i]->GetNcols()*EDSCT4DetEffi[i]->GetNrows()) ;

  EDSCT1TimeRes2DScan[i] = CloneTH2F(EDSCT1TimeRes[i], EDName[i]+EDSCT1Name[i]+"TimeRes2DScan", EDName[i]+EDSCT1Name[i]+" Time Resolution", "Time Resolution (ns)") ;
  EDSCT2TimeRes2DScan[i] = CloneTH2F(EDSCT2TimeRes[i], EDName[i]+EDSCT2Name[i]+"TimeRes2DScan", EDName[i]+EDSCT2Name[i]+" Time Resolution", "Time Resolution (ns)") ;
  EDSCT3TimeRes2DScan[i] = CloneTH2F(EDSCT3TimeRes[i], EDName[i]+EDSCT3Name[i]+"TimeRes2DScan", EDName[i]+EDSCT3Name[i]+" Time Resolution", "Time Resolution (ns)") ;
  EDSCT4TimeRes2DScan[i] = CloneTH2F(EDSCT4TimeRes[i], EDName[i]+EDSCT4Name[i]+"TimeRes2DScan", EDName[i]+EDSCT4Name[i]+" Time Resolution", "Time Resolution (ns)") ;

  EDSCT1TimeOffset2DScan[i] = CloneTH2F(EDSCT1TimeOffset[i], EDName[i]+EDSCT1Name[i]+"TimeOffset2DScan", EDName[i]+EDSCT1Name[i]+" Time Offset", "Time Offset (ns)") ;
  EDSCT2TimeOffset2DScan[i] = CloneTH2F(EDSCT2TimeOffset[i], EDName[i]+EDSCT2Name[i]+"TimeOffset2DScan", EDName[i]+EDSCT2Name[i]+" Time Offset", "Time Offset (ns)") ;
  EDSCT3TimeOffset2DScan[i] = CloneTH2F(EDSCT3TimeOffset[i], EDName[i]+EDSCT3Name[i]+"TimeOffset2DScan", EDName[i]+EDSCT3Name[i]+" Time Offset", "Time Offset (ns)") ;
  EDSCT4TimeOffset2DScan[i] = CloneTH2F(EDSCT4TimeOffset[i], EDName[i]+EDSCT4Name[i]+"TimeOffset2DScan", EDName[i]+EDSCT4Name[i]+" Time Offset", "Time Offset (ns)") ;

  EDSCT1TimeRes1DScan[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"TimeRes1DScan", EDName[i]+EDSCT1Name[i]+"Time Resolution", "", "Time Resolution (ns)", EDSCT1TimeRes[i]->GetNcols()*EDSCT1TimeRes[i]->GetNrows(),0.,EDSCT1TimeRes[i]->GetNcols()*EDSCT1TimeRes[i]->GetNrows()) ;
  EDSCT2TimeRes1DScan[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"TimeRes1DScan", EDName[i]+EDSCT2Name[i]+"Time Resolution", "", "Time Resolution (ns)", EDSCT2TimeRes[i]->GetNcols()*EDSCT2TimeRes[i]->GetNrows(),0.,EDSCT2TimeRes[i]->GetNcols()*EDSCT2TimeRes[i]->GetNrows()) ;
  EDSCT3TimeRes1DScan[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"TimeRes1DScan", EDName[i]+EDSCT3Name[i]+"Time Resolution", "", "Time Resolution (ns)", EDSCT3TimeRes[i]->GetNcols()*EDSCT3TimeRes[i]->GetNrows(),0.,EDSCT3TimeRes[i]->GetNcols()*EDSCT3TimeRes[i]->GetNrows()) ;
  EDSCT4TimeRes1DScan[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"TimeRes1DScan", EDName[i]+EDSCT4Name[i]+"Time Resolution", "", "Time Resolution (ns)", EDSCT4TimeRes[i]->GetNcols()*EDSCT4TimeRes[i]->GetNrows(),0.,EDSCT4TimeRes[i]->GetNcols()*EDSCT4TimeRes[i]->GetNrows()) ;

  EDSCT1TimeOffset1DScan[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"TimeOffset1DScan", EDName[i]+EDSCT1Name[i]+"Time Offset", "", "Time Offset (ns)", EDSCT1TimeOffset[i]->GetNcols()*EDSCT1TimeOffset[i]->GetNrows(),0.,EDSCT1TimeOffset[i]->GetNcols()*EDSCT1TimeOffset[i]->GetNrows()) ;
  EDSCT2TimeOffset1DScan[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"TimeOffset1DScan", EDName[i]+EDSCT2Name[i]+"Time Offset", "", "Time Offset (ns)", EDSCT2TimeOffset[i]->GetNcols()*EDSCT2TimeOffset[i]->GetNrows(),0.,EDSCT2TimeOffset[i]->GetNcols()*EDSCT2TimeOffset[i]->GetNrows()) ;
  EDSCT3TimeOffset1DScan[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"TimeOffset1DScan", EDName[i]+EDSCT3Name[i]+"Time Offset", "", "Time Offset (ns)", EDSCT3TimeOffset[i]->GetNcols()*EDSCT3TimeOffset[i]->GetNrows(),0.,EDSCT3TimeOffset[i]->GetNcols()*EDSCT3TimeOffset[i]->GetNrows()) ;
  EDSCT4TimeOffset1DScan[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"TimeOffset1DScan", EDName[i]+EDSCT4Name[i]+"Time Offset", "", "Time Offset (ns)", EDSCT4TimeOffset[i]->GetNcols()*EDSCT4TimeOffset[i]->GetNrows(),0.,EDSCT4TimeOffset[i]->GetNcols()*EDSCT4TimeOffset[i]->GetNrows()) ;

  EDSCT1EnergyRes2DScan[i] = CloneTH2F(EDSCT1EnergyRes[i], EDName[i]+EDSCT1Name[i]+"EnergyRes2DScan", EDName[i]+EDSCT1Name[i]+"Energy Resolution", "Energy Resolution (Counts)") ;
  EDSCT2EnergyRes2DScan[i] = CloneTH2F(EDSCT2EnergyRes[i], EDName[i]+EDSCT2Name[i]+"EnergyRes2DScan", EDName[i]+EDSCT2Name[i]+"Energy Resolution", "Energy Resolution (Counts)") ;
  EDSCT3EnergyRes2DScan[i] = CloneTH2F(EDSCT3EnergyRes[i], EDName[i]+EDSCT3Name[i]+"EnergyRes2DScan", EDName[i]+EDSCT3Name[i]+"Energy Resolution", "Energy Resolution (Counts)") ;
  EDSCT4EnergyRes2DScan[i] = CloneTH2F(EDSCT4EnergyRes[i], EDName[i]+EDSCT4Name[i]+"EnergyRes2DScan", EDName[i]+EDSCT4Name[i]+"Energy Resolution", "Energy Resolution (Counts)") ;

  EDSCT1ReEnergyRes2DScan[i] = CloneTH2F(EDSCT1ReEnergyRes[i], EDName[i]+EDSCT1Name[i]+"ReEnergyRes2DScan", EDName[i]+EDSCT1Name[i]+"Relative Energy Resolution", "Relative Energy Resolution") ;
  EDSCT2ReEnergyRes2DScan[i] = CloneTH2F(EDSCT2ReEnergyRes[i], EDName[i]+EDSCT2Name[i]+"ReEnergyRes2DScan", EDName[i]+EDSCT2Name[i]+"Relative Energy Resolution", "Relative Energy Resolution") ;
  EDSCT3ReEnergyRes2DScan[i] = CloneTH2F(EDSCT3ReEnergyRes[i], EDName[i]+EDSCT3Name[i]+"ReEnergyRes2DScan", EDName[i]+EDSCT3Name[i]+"Relative Energy Resolution", "Relative Energy Resolution") ;
  EDSCT4ReEnergyRes2DScan[i] = CloneTH2F(EDSCT4ReEnergyRes[i], EDName[i]+EDSCT4Name[i]+"ReEnergyRes2DScan", EDName[i]+EDSCT4Name[i]+"Relative Energy Resolution", "Relative Energy Resolution") ;

  EDSCT1SingleParticlePeak2DScan[i] = CloneTH2F(EDSCT1SingleParticlePeak[i], EDName[i]+EDSCT1Name[i]+"SingleParticlePeak2DScan", EDName[i]+EDSCT1Name[i]+"Single Particle Peak", "SingleParticlePeak (Counts)") ;
  EDSCT2SingleParticlePeak2DScan[i] = CloneTH2F(EDSCT2SingleParticlePeak[i], EDName[i]+EDSCT2Name[i]+"SingleParticlePeak2DScan", EDName[i]+EDSCT2Name[i]+"Single Particle Peak", "SingleParticlePeak (Counts)") ;
  EDSCT3SingleParticlePeak2DScan[i] = CloneTH2F(EDSCT3SingleParticlePeak[i], EDName[i]+EDSCT3Name[i]+"SingleParticlePeak2DScan", EDName[i]+EDSCT3Name[i]+"Single Particle Peak", "SingleParticlePeak (Counts)") ;
  EDSCT4SingleParticlePeak2DScan[i] = CloneTH2F(EDSCT4SingleParticlePeak[i], EDName[i]+EDSCT4Name[i]+"SingleParticlePeak2DScan", EDName[i]+EDSCT4Name[i]+"Single Particle Peak", "SingleParticlePeak (Counts)") ;

  EDSCT1EnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"EnergyRes1DScan", EDName[i]+EDSCT1Name[i]+"Energy Resolution", "", "Energy Resolution (Counts)", EDSCT1EnergyRes[i]->GetNcols()*EDSCT1EnergyRes[i]->GetNrows(),0.,EDSCT1EnergyRes[i]->GetNcols()*EDSCT1EnergyRes[i]->GetNrows()) ;
  EDSCT2EnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"EnergyRes1DScan", EDName[i]+EDSCT2Name[i]+"Energy Resolution", "", "Energy Resolution (Counts)", EDSCT2EnergyRes[i]->GetNcols()*EDSCT2EnergyRes[i]->GetNrows(),0.,EDSCT2EnergyRes[i]->GetNcols()*EDSCT2EnergyRes[i]->GetNrows()) ;
  EDSCT3EnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"EnergyRes1DScan", EDName[i]+EDSCT3Name[i]+"Energy Resolution", "", "Energy Resolution (Counts)", EDSCT3EnergyRes[i]->GetNcols()*EDSCT3EnergyRes[i]->GetNrows(),0.,EDSCT3EnergyRes[i]->GetNcols()*EDSCT3EnergyRes[i]->GetNrows()) ;
  EDSCT4EnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"EnergyRes1DScan", EDName[i]+EDSCT4Name[i]+"Energy Resolution", "", "Energy Resolution (Counts)", EDSCT4EnergyRes[i]->GetNcols()*EDSCT4EnergyRes[i]->GetNrows(),0.,EDSCT4EnergyRes[i]->GetNcols()*EDSCT4EnergyRes[i]->GetNrows()) ;

  EDSCT1ReEnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"ReEnergyRes1DScan", EDName[i]+EDSCT1Name[i]+"ReEnergy Resolution", "", "Relative Energy Resolution", EDSCT1ReEnergyRes[i]->GetNcols()*EDSCT1ReEnergyRes[i]->GetNrows(),0.,EDSCT1ReEnergyRes[i]->GetNcols()*EDSCT1ReEnergyRes[i]->GetNrows()) ;
  EDSCT2ReEnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"ReEnergyRes1DScan", EDName[i]+EDSCT2Name[i]+"ReEnergy Resolution", "", "Relative Energy Resolution", EDSCT2ReEnergyRes[i]->GetNcols()*EDSCT2ReEnergyRes[i]->GetNrows(),0.,EDSCT2ReEnergyRes[i]->GetNcols()*EDSCT2ReEnergyRes[i]->GetNrows()) ;
  EDSCT3ReEnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"ReEnergyRes1DScan", EDName[i]+EDSCT3Name[i]+"ReEnergy Resolution", "", "Relative Energy Resolution", EDSCT3ReEnergyRes[i]->GetNcols()*EDSCT3ReEnergyRes[i]->GetNrows(),0.,EDSCT3ReEnergyRes[i]->GetNcols()*EDSCT3ReEnergyRes[i]->GetNrows()) ;
  EDSCT4ReEnergyRes1DScan[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"ReEnergyRes1DScan", EDName[i]+EDSCT4Name[i]+"ReEnergy Resolution", "", "Relative Energy Resolution", EDSCT4ReEnergyRes[i]->GetNcols()*EDSCT4ReEnergyRes[i]->GetNrows(),0.,EDSCT4ReEnergyRes[i]->GetNcols()*EDSCT4ReEnergyRes[i]->GetNrows()) ;

  EDSCT1SingleParticlePeak1DScan[i] = bookTH1F(EDName[i]+EDSCT1Name[i]+"SingleParticlePeak1DScan", EDName[i]+EDSCT1Name[i]+"Single Particle Peak", "", "SingleParticlePeak (Counts)", EDSCT1SingleParticlePeak[i]->GetNcols()*EDSCT1SingleParticlePeak[i]->GetNrows(),0.,EDSCT1SingleParticlePeak[i]->GetNcols()*EDSCT1SingleParticlePeak[i]->GetNrows()) ;
  EDSCT2SingleParticlePeak1DScan[i] = bookTH1F(EDName[i]+EDSCT2Name[i]+"SingleParticlePeak1DScan", EDName[i]+EDSCT2Name[i]+"Single Particle Peak", "", "SingleParticlePeak (Counts)", EDSCT2SingleParticlePeak[i]->GetNcols()*EDSCT2SingleParticlePeak[i]->GetNrows(),0.,EDSCT2SingleParticlePeak[i]->GetNcols()*EDSCT2SingleParticlePeak[i]->GetNrows()) ;
  EDSCT3SingleParticlePeak1DScan[i] = bookTH1F(EDName[i]+EDSCT3Name[i]+"SingleParticlePeak1DScan", EDName[i]+EDSCT3Name[i]+"Single Particle Peak", "", "SingleParticlePeak (Counts)", EDSCT3SingleParticlePeak[i]->GetNcols()*EDSCT3SingleParticlePeak[i]->GetNrows(),0.,EDSCT3SingleParticlePeak[i]->GetNcols()*EDSCT3SingleParticlePeak[i]->GetNrows()) ;
  EDSCT4SingleParticlePeak1DScan[i] = bookTH1F(EDName[i]+EDSCT4Name[i]+"SingleParticlePeak1DScan", EDName[i]+EDSCT4Name[i]+"Single Particle Peak", "", "SingleParticlePeak (Counts)", EDSCT4SingleParticlePeak[i]->GetNcols()*EDSCT4SingleParticlePeak[i]->GetNrows(),0.,EDSCT4SingleParticlePeak[i]->GetNcols()*EDSCT4SingleParticlePeak[i]->GetNrows()) ;

  for(int m=0 ; m<EDSCT1SingleParticlePeak[i]->GetNcols() ; m++)
  {
    for(int n=0 ; n<EDSCT1SingleParticlePeak[i]->GetNrows() ; n++)
    {
      EDSCT1DetEffi1DScan[i]->SetBinContent(m*EDSCT1DetEffi[i]->GetNrows()+n+1, (*EDSCT1DetEffi[i])[n][m]) ;
      EDSCT2DetEffi1DScan[i]->SetBinContent(m*EDSCT2DetEffi[i]->GetNrows()+n+1, (*EDSCT2DetEffi[i])[n][m]) ;
      EDSCT3DetEffi1DScan[i]->SetBinContent(m*EDSCT3DetEffi[i]->GetNrows()+n+1, (*EDSCT3DetEffi[i])[n][m]) ;
      EDSCT4DetEffi1DScan[i]->SetBinContent(m*EDSCT4DetEffi[i]->GetNrows()+n+1, (*EDSCT4DetEffi[i])[n][m]) ;

      EDSCT1TimeRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT1TimeResErrors[i])[n][m]) ;
      EDSCT1TimeRes1DScan[i]->SetBinContent(m*EDSCT1TimeRes[i]->GetNrows()+n+1, (*EDSCT1TimeRes[i])[n][m]) ;
      EDSCT1TimeRes1DScan[i]->SetBinError(m*EDSCT1TimeRes[i]->GetNrows()+n+1, (*EDSCT1TimeResErrors[i])[n][m]) ;
      EDSCT2TimeRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT2TimeResErrors[i])[n][m]) ;
      EDSCT2TimeRes1DScan[i]->SetBinContent(m*EDSCT2TimeRes[i]->GetNrows()+n+1, (*EDSCT2TimeRes[i])[n][m]) ;
      EDSCT2TimeRes1DScan[i]->SetBinError(m*EDSCT2TimeRes[i]->GetNrows()+n+1, (*EDSCT2TimeResErrors[i])[n][m]) ;

      EDSCT3TimeRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT3TimeResErrors[i])[n][m]) ;
      EDSCT3TimeRes1DScan[i]->SetBinContent(m*EDSCT3TimeRes[i]->GetNrows()+n+1, (*EDSCT3TimeRes[i])[n][m]) ;
      EDSCT3TimeRes1DScan[i]->SetBinError(m*EDSCT3TimeRes[i]->GetNrows()+n+1, (*EDSCT3TimeResErrors[i])[n][m]) ;

      EDSCT4TimeRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT4TimeResErrors[i])[n][m]) ;
      EDSCT4TimeRes1DScan[i]->SetBinContent(m*EDSCT4TimeRes[i]->GetNrows()+n+1, (*EDSCT4TimeRes[i])[n][m]) ;
      EDSCT4TimeRes1DScan[i]->SetBinError(m*EDSCT4TimeRes[i]->GetNrows()+n+1, (*EDSCT4TimeResErrors[i])[n][m]) ;

      EDSCT1TimeOffset2DScan[i]->SetBinError(m+1, n+1, (*EDSCT1TimeOffsetErrors[i])[n][m]) ;
      EDSCT1TimeOffset1DScan[i]->SetBinContent(m*EDSCT1TimeOffset[i]->GetNrows()+n+1, (*EDSCT1TimeOffset[i])[n][m]) ;
      EDSCT1TimeOffset1DScan[i]->SetBinError(m*EDSCT1TimeOffset[i]->GetNrows()+n+1, (*EDSCT1TimeOffsetErrors[i])[n][m]) ;

      EDSCT2TimeOffset2DScan[i]->SetBinError(m+1, n+1, (*EDSCT2TimeOffsetErrors[i])[n][m]) ;
      EDSCT2TimeOffset1DScan[i]->SetBinContent(m*EDSCT2TimeOffset[i]->GetNrows()+n+1, (*EDSCT2TimeOffset[i])[n][m]) ;
      EDSCT2TimeOffset1DScan[i]->SetBinError(m*EDSCT2TimeOffset[i]->GetNrows()+n+1, (*EDSCT2TimeOffsetErrors[i])[n][m]) ;

      EDSCT3TimeOffset2DScan[i]->SetBinError(m+1, n+1, (*EDSCT3TimeOffsetErrors[i])[n][m]) ;
      EDSCT3TimeOffset1DScan[i]->SetBinContent(m*EDSCT3TimeOffset[i]->GetNrows()+n+1, (*EDSCT3TimeOffset[i])[n][m]) ;
      EDSCT3TimeOffset1DScan[i]->SetBinError(m*EDSCT3TimeOffset[i]->GetNrows()+n+1, (*EDSCT3TimeOffsetErrors[i])[n][m]) ;

      EDSCT4TimeOffset2DScan[i]->SetBinError(m+1, n+1, (*EDSCT4TimeOffsetErrors[i])[n][m]) ;
      EDSCT4TimeOffset1DScan[i]->SetBinContent(m*EDSCT4TimeOffset[i]->GetNrows()+n+1, (*EDSCT4TimeOffset[i])[n][m]) ;
      EDSCT4TimeOffset1DScan[i]->SetBinError(m*EDSCT4TimeOffset[i]->GetNrows()+n+1, (*EDSCT4TimeOffsetErrors[i])[n][m]) ;

      EDSCT1EnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT1EnergyResErrors[i])[n][m]) ;
      EDSCT1EnergyRes1DScan[i]->SetBinContent(m*EDSCT1EnergyRes[i]->GetNrows()+n+1, (*EDSCT1EnergyRes[i])[n][m]) ;
      EDSCT1EnergyRes1DScan[i]->SetBinError(m*EDSCT1EnergyRes[i]->GetNrows()+n+1, (*EDSCT1EnergyResErrors[i])[n][m]) ;

      EDSCT2EnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT2EnergyResErrors[i])[n][m]) ;
      EDSCT2EnergyRes1DScan[i]->SetBinContent(m*EDSCT2EnergyRes[i]->GetNrows()+n+1, (*EDSCT2EnergyRes[i])[n][m]) ;
      EDSCT2EnergyRes1DScan[i]->SetBinError(m*EDSCT2EnergyRes[i]->GetNrows()+n+1, (*EDSCT2EnergyResErrors[i])[n][m]) ;

      EDSCT3EnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT3EnergyResErrors[i])[n][m]) ;
      EDSCT3EnergyRes1DScan[i]->SetBinContent(m*EDSCT3EnergyRes[i]->GetNrows()+n+1, (*EDSCT3EnergyRes[i])[n][m]) ;
      EDSCT3EnergyRes1DScan[i]->SetBinError(m*EDSCT3EnergyRes[i]->GetNrows()+n+1, (*EDSCT3EnergyResErrors[i])[n][m]) ;

      EDSCT4EnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT4EnergyResErrors[i])[n][m]) ;
      EDSCT4EnergyRes1DScan[i]->SetBinContent(m*EDSCT4EnergyRes[i]->GetNrows()+n+1, (*EDSCT4EnergyRes[i])[n][m]) ;
      EDSCT4EnergyRes1DScan[i]->SetBinError(m*EDSCT4EnergyRes[i]->GetNrows()+n+1, (*EDSCT4EnergyResErrors[i])[n][m]) ;

      EDSCT1ReEnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT1ReEnergyResErrors[i])[n][m]) ;
      EDSCT1ReEnergyRes1DScan[i]->SetBinContent(m*EDSCT1ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT1ReEnergyRes[i])[n][m]) ;
      EDSCT1ReEnergyRes1DScan[i]->SetBinError(m*EDSCT1ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT1ReEnergyResErrors[i])[n][m]) ;

      EDSCT2ReEnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT2ReEnergyResErrors[i])[n][m]) ;
      EDSCT2ReEnergyRes1DScan[i]->SetBinContent(m*EDSCT2ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT2ReEnergyRes[i])[n][m]) ;
      EDSCT2ReEnergyRes1DScan[i]->SetBinError(m*EDSCT2ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT2ReEnergyResErrors[i])[n][m]) ;

      EDSCT3ReEnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT3ReEnergyResErrors[i])[n][m]) ;
      EDSCT3ReEnergyRes1DScan[i]->SetBinContent(m*EDSCT3ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT3ReEnergyRes[i])[n][m]) ;
      EDSCT3ReEnergyRes1DScan[i]->SetBinError(m*EDSCT3ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT3ReEnergyResErrors[i])[n][m]) ;

      EDSCT4ReEnergyRes2DScan[i]->SetBinError(m+1, n+1, (*EDSCT4ReEnergyResErrors[i])[n][m]) ;
      EDSCT4ReEnergyRes1DScan[i]->SetBinContent(m*EDSCT4ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT4ReEnergyRes[i])[n][m]) ;
      EDSCT4ReEnergyRes1DScan[i]->SetBinError(m*EDSCT4ReEnergyRes[i]->GetNrows()+n+1, (*EDSCT4ReEnergyResErrors[i])[n][m]) ;

      EDSCT1SingleParticlePeak2DScan[i]->SetBinError(m+1, n+1, (*EDSCT1SingleParticlePeakErrors[i])[n][m]) ;
      EDSCT1SingleParticlePeak1DScan[i]->SetBinContent(m*EDSCT1SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT1SingleParticlePeak[i])[n][m]) ;
      EDSCT1SingleParticlePeak1DScan[i]->SetBinError(m*EDSCT1SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT1SingleParticlePeakErrors[i])[n][m]) ;

      EDSCT2SingleParticlePeak2DScan[i]->SetBinError(m+1, n+1, (*EDSCT2SingleParticlePeakErrors[i])[n][m]) ;
      EDSCT2SingleParticlePeak1DScan[i]->SetBinContent(m*EDSCT2SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT2SingleParticlePeak[i])[n][m]) ;
      EDSCT2SingleParticlePeak1DScan[i]->SetBinError(m*EDSCT2SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT2SingleParticlePeakErrors[i])[n][m]) ;

      EDSCT3SingleParticlePeak2DScan[i]->SetBinError(m+1, n+1, (*EDSCT3SingleParticlePeakErrors[i])[n][m]) ;
      EDSCT3SingleParticlePeak1DScan[i]->SetBinContent(m*EDSCT3SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT3SingleParticlePeak[i])[n][m]) ;
      EDSCT3SingleParticlePeak1DScan[i]->SetBinError(m*EDSCT3SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT3SingleParticlePeakErrors[i])[n][m]) ;

      EDSCT4SingleParticlePeak2DScan[i]->SetBinError(m+1, n+1, (*EDSCT4SingleParticlePeakErrors[i])[n][m]) ;
      EDSCT4SingleParticlePeak1DScan[i]->SetBinContent(m*EDSCT4SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT4SingleParticlePeak[i])[n][m]) ;
      EDSCT4SingleParticlePeak1DScan[i]->SetBinError(m*EDSCT4SingleParticlePeak[i]->GetNrows()+n+1, (*EDSCT4SingleParticlePeakErrors[i])[n][m]) ;
    }
  }
}

void AnalyseED::Analyse()
{
  NewHist() ;
  int nEntries = inTree->GetEntries() ;
  for(int n=0 ; n<nEntries ; n++)
  {
    if(n%10000==0)
      cout<<n<<endl ;
    inTree->GetEntry(n) ;
    FillDeltaTOF() ;
  }

  GetSysTimeRes() ;

  for(int n=0 ; n<nEntries ; n++)
  {
    if(n%10000==0)
      cout<<n<<endl ;
    inTree->GetEntry(n) ;
    for(int i=0 ; i<NEDs ; i++)
    {
      FillHitPoint(i) ;
      FillDeltaTime(i) ;
      FillCharge(i) ;
    }
  }

  for(int i=0 ; i<NEDs ; i++)
  {
    int totalNFiredHits = 0 ;
    totalNFiredHits += (EDSCT1NFiredHits[i]->GetEntries()) ;
    totalNFiredHits += (EDSCT2NFiredHits[i]->GetEntries()) ;
    totalNFiredHits += (EDSCT3NFiredHits[i]->GetEntries()) ;
    totalNFiredHits += (EDSCT4NFiredHits[i]->GetEntries()) ;
    (*SingleRate)[i] = NPreHitsOnSCT[i]/(totalNFiredHits*4700./1000000000) ;
  }
  for(int i=0 ; i<NEDs ; i++)
  {
    GetDetEffi(i) ;
    GetTimeRes(i) ;
    GetTimeOffset(i) ;
    GetEnergyRes(i) ;
    ScanED(i) ;
  }

  for(int i=0 ; i<NEDs ; i++)
  {
    EDPlaneDetEffi2DScan[i]->Write() ;
    EDSCT1DetEffi2DScan[i]->Write() ;
    EDSCT1DetEffi1DScan[i]->Write() ;
    EDSCT2DetEffi2DScan[i]->Write() ;
    EDSCT2DetEffi1DScan[i]->Write() ;
    EDSCT3DetEffi2DScan[i]->Write() ;
    EDSCT3DetEffi1DScan[i]->Write() ;
    EDSCT4DetEffi2DScan[i]->Write() ;
    EDSCT4DetEffi1DScan[i]->Write() ;
    EDSCT1TimeRes2DScan[i]->Write() ;
    EDSCT1TimeRes1DScan[i]->Write() ;
    EDSCT2TimeRes2DScan[i]->Write() ;
    EDSCT2TimeRes1DScan[i]->Write() ;
    EDSCT3TimeRes2DScan[i]->Write() ;
    EDSCT3TimeRes1DScan[i]->Write() ;
    EDSCT4TimeRes2DScan[i]->Write() ;
    EDSCT4TimeRes1DScan[i]->Write() ;
    EDSCT1TimeOffset2DScan[i]->Write() ;
    EDSCT1TimeOffset1DScan[i]->Write() ;
    EDSCT2TimeOffset2DScan[i]->Write() ;
    EDSCT2TimeOffset1DScan[i]->Write() ;
    EDSCT3TimeOffset2DScan[i]->Write() ;
    EDSCT3TimeOffset1DScan[i]->Write() ;
    EDSCT4TimeOffset2DScan[i]->Write() ;
    EDSCT4TimeOffset1DScan[i]->Write() ;
    EDSCT1EnergyRes2DScan[i]->Write() ;
    EDSCT1EnergyRes1DScan[i]->Write() ;
    EDSCT2EnergyRes2DScan[i]->Write() ;
    EDSCT2EnergyRes1DScan[i]->Write() ;
    EDSCT3EnergyRes2DScan[i]->Write() ;
    EDSCT3EnergyRes1DScan[i]->Write() ;
    EDSCT4EnergyRes2DScan[i]->Write() ;
    EDSCT4EnergyRes1DScan[i]->Write() ;
    EDSCT1ReEnergyRes2DScan[i]->Write() ;
    EDSCT1ReEnergyRes1DScan[i]->Write() ;
    EDSCT2ReEnergyRes2DScan[i]->Write() ;
    EDSCT2ReEnergyRes1DScan[i]->Write() ;
    EDSCT3ReEnergyRes2DScan[i]->Write() ;
    EDSCT3ReEnergyRes1DScan[i]->Write() ;
    EDSCT4ReEnergyRes2DScan[i]->Write() ;
    EDSCT4ReEnergyRes1DScan[i]->Write() ;
    EDSCT1SingleParticlePeak2DScan[i]->Write() ;
    EDSCT1SingleParticlePeak1DScan[i]->Write() ;
    EDSCT2SingleParticlePeak2DScan[i]->Write() ;
    EDSCT2SingleParticlePeak1DScan[i]->Write() ;
    EDSCT3SingleParticlePeak2DScan[i]->Write() ;
    EDSCT3SingleParticlePeak1DScan[i]->Write() ;
    EDSCT4SingleParticlePeak2DScan[i]->Write() ;
    EDSCT4SingleParticlePeak1DScan[i]->Write() ;
  }
  for(int i=0 ; i<NEDs ; i++)
  {
    SingleRate->Write("SingleRate") ;
    EDPlaneDetEffi[i]->Write((EDName[i]+"PlaneDetEffi").c_str()) ;
    EDSCT1DetEffi[i]->Write((EDName[i]+EDSCT1Name[i]+"DetEffi").c_str()) ;
    EDSCT2DetEffi[i]->Write((EDName[i]+EDSCT2Name[i]+"DetEffi").c_str()) ;
    EDSCT3DetEffi[i]->Write((EDName[i]+EDSCT3Name[i]+"DetEffi").c_str()) ;
    EDSCT4DetEffi[i]->Write((EDName[i]+EDSCT4Name[i]+"DetEffi").c_str()) ;
    EDSCT1TimeRes[i]->Write((EDName[i]+EDSCT1Name[i]+"TimeRes").c_str()) ;
    EDSCT1TimeResErrors[i]->Write((EDName[i]+EDSCT1Name[i]+"TimeResErrors").c_str()) ;
    EDSCT1TimeOffset[i]->Write((EDName[i]+EDSCT1Name[i]+"TimeOffset").c_str()) ;
    EDSCT1TimeOffsetErrors[i]->Write((EDName[i]+EDSCT1Name[i]+"TimeOffsetErrors").c_str()) ;
    EDSCT2TimeRes[i]->Write((EDName[i]+EDSCT2Name[i]+"TimeRes").c_str()) ;
    EDSCT2TimeResErrors[i]->Write((EDName[i]+EDSCT2Name[i]+"TimeResErrors").c_str()) ;
    EDSCT2TimeOffset[i]->Write((EDName[i]+EDSCT2Name[i]+"TimeOffset").c_str()) ;
    EDSCT2TimeOffsetErrors[i]->Write((EDName[i]+EDSCT2Name[i]+"TimeOffsetErrors").c_str()) ;
    EDSCT3TimeRes[i]->Write((EDName[i]+EDSCT3Name[i]+"TimeRes").c_str()) ;
    EDSCT3TimeResErrors[i]->Write((EDName[i]+EDSCT3Name[i]+"TimeResErrors").c_str()) ;
    EDSCT3TimeOffset[i]->Write((EDName[i]+EDSCT3Name[i]+"TimeOffset").c_str()) ;
    EDSCT3TimeOffsetErrors[i]->Write((EDName[i]+EDSCT3Name[i]+"TimeOffsetErrors").c_str()) ;
    EDSCT4TimeRes[i]->Write((EDName[i]+EDSCT4Name[i]+"TimeRes").c_str()) ;
    EDSCT4TimeResErrors[i]->Write((EDName[i]+EDSCT4Name[i]+"TimeResErrors").c_str()) ;
    EDSCT4TimeOffset[i]->Write((EDName[i]+EDSCT4Name[i]+"TimeOffset").c_str()) ;
    EDSCT4TimeOffsetErrors[i]->Write((EDName[i]+EDSCT4Name[i]+"TimeOffsetErrors").c_str()) ;
    EDSCT1EnergyRes[i]->Write((EDName[i]+EDSCT1Name[i]+"EnergyRes").c_str()) ;
    EDSCT1EnergyResErrors[i]->Write((EDName[i]+EDSCT1Name[i]+"EnergyResErrors").c_str()) ;
    EDSCT2EnergyRes[i]->Write((EDName[i]+EDSCT2Name[i]+"EnergyRes").c_str()) ;
    EDSCT2EnergyResErrors[i]->Write((EDName[i]+EDSCT2Name[i]+"EnergyResErrors").c_str()) ;
    EDSCT3EnergyRes[i]->Write((EDName[i]+EDSCT3Name[i]+"EnergyRes").c_str()) ;
    EDSCT3EnergyResErrors[i]->Write((EDName[i]+EDSCT3Name[i]+"EnergyResErrors").c_str()) ;
    EDSCT4EnergyRes[i]->Write((EDName[i]+EDSCT4Name[i]+"EnergyRes").c_str()) ;
    EDSCT4EnergyResErrors[i]->Write((EDName[i]+EDSCT4Name[i]+"EnergyResErrors").c_str()) ;
    EDSCT1ReEnergyRes[i]->Write((EDName[i]+EDSCT1Name[i]+"ReEnergyRes").c_str()) ;
    EDSCT1ReEnergyResErrors[i]->Write((EDName[i]+EDSCT1Name[i]+"ReEnergyResErrors").c_str()) ;
    EDSCT2ReEnergyRes[i]->Write((EDName[i]+EDSCT2Name[i]+"ReEnergyRes").c_str()) ;
    EDSCT2ReEnergyResErrors[i]->Write((EDName[i]+EDSCT2Name[i]+"ReEnergyResErrors").c_str()) ;
    EDSCT3ReEnergyRes[i]->Write((EDName[i]+EDSCT3Name[i]+"ReEnergyRes").c_str()) ;
    EDSCT3ReEnergyResErrors[i]->Write((EDName[i]+EDSCT3Name[i]+"ReEnergyResErrors").c_str()) ;
    EDSCT4ReEnergyRes[i]->Write((EDName[i]+EDSCT4Name[i]+"ReEnergyRes").c_str()) ;
    EDSCT4ReEnergyResErrors[i]->Write((EDName[i]+EDSCT4Name[i]+"ReEnergyResErrors").c_str()) ;
    EDSCT1SingleParticlePeak[i]->Write((EDName[i]+EDSCT1Name[i]+"SingleParticlePeak").c_str()) ;
    EDSCT1SingleParticlePeakErrors[i]->Write((EDName[i]+EDSCT1Name[i]+"SingleParticlePeakErrors").c_str()) ;
    EDSCT2SingleParticlePeak[i]->Write((EDName[i]+EDSCT2Name[i]+"SingleParticlePeak").c_str()) ;
    EDSCT2SingleParticlePeakErrors[i]->Write((EDName[i]+EDSCT2Name[i]+"SingleParticlePeakErrors").c_str()) ;
    EDSCT3SingleParticlePeak[i]->Write((EDName[i]+EDSCT3Name[i]+"SingleParticlePeak").c_str()) ;
    EDSCT3SingleParticlePeakErrors[i]->Write((EDName[i]+EDSCT3Name[i]+"SingleParticlePeakErrors").c_str()) ;
    EDSCT4SingleParticlePeak[i]->Write((EDName[i]+EDSCT4Name[i]+"SingleParticlePeak").c_str()) ;
    EDSCT4SingleParticlePeakErrors[i]->Write((EDName[i]+EDSCT4Name[i]+"SingleParticlePeakErrors").c_str()) ;
  }

  EDSCTDetEffi->Write("EDSCTDetEffi") ;
  SysTimeRes->Write("SysTimeRes") ;
  ScinTimeRes->Write("ScinTimeRes") ;
  EDSCTTimeRes->Write("EDSCTTimeRes") ;
  EDSCTTimeResErrors->Write("EDSCTTimeResErrors") ;
  EDSCTTimeOffset->Write("EDSCTTimeOffset") ;
  EDSCTTimeOffsetErrors->Write("EDSCTTimeOffsetErrors") ;
  EDSCTEnergyRes->Write("EDSCTEnergyRes") ;
  EDSCTEnergyResErrors->Write("EDSCTEnergyResErrors") ;
  EDSCTReEnergyRes->Write("EDSCTReEnergyRes") ;
  EDSCTReEnergyResErrors->Write("EDSCTReEnergyResErrors") ;
  EDSCTSingleParticlePeak->Write("EDSCTSingleParticlePeak") ;
  EDSCTSingleParticlePeakErrors->Write("EDSCTSingleParticlePeakErrors") ;

}
#endif
