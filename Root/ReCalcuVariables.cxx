#ifndef __ReCalcuVariables__CXX
#define __ReCalcuVariables__CXX

#include "ProcessCORARS/ReCalcuVariables.h"

using namespace std;
ReCalcuVariables::ReCalcuVariables()
{
  inFile = 0 ;
  inTree = 0 ;
  outFile = 0 ;
  outTree = 0 ;
  FuncSlope = 0 ;
  TrackLength = 0. ;
  NEDs = 0 ;
  for(int i=0 ; i<8 ; i++)
    EDName[i] = "" ;
  Initialize() ;
}

ReCalcuVariables::~ReCalcuVariables() 
{
  delete inTree ;
  delete inFile ;
  delete outTree ;
  delete outFile ;
  delete FuncSlope ;
}

void ReCalcuVariables::Initialize()
{
  conf = ConfigureClass::Instance() ;

  NEDs = conf->NEDs ; 
  for(int i=0 ; i<NEDs ; i++)
    EDName[i] = conf->EDName[i] ;

  TFile* EDPositions = new TFile((conf->outPath+"/"+conf->CORARSCollectionTime+"/EDPositions_"+conf->CORARSCollectionTime+".root").c_str(),"read") ;
  FuncSlope = (TVectorD*) EDPositions->Get("FuncSlope") ;

  TFile* ChargeCorrectTime = new TFile((conf->outPath+"/"+conf->CORARSCollectionTime+"/ChargeCorrectTime_"+conf->CORARSCollectionTime+".root").c_str(), "read") ;
  for(int i=0 ; i<NEDs ; i++)
    ChargeDeltaTime[i] = (TH1F*) ChargeCorrectTime->Get(TString::Format("ChargeDeltaTime%i",i)) ;

  inPath = conf->outPath+conf->CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  inFileName = "Stage_Two_"+conf->CORARSCollectionTime+".root" ;
  inFile = new TFile((inPath+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"In File: "<<inPath<<inFileName<<" doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  cout<<"In File: "<<inFile->GetName()<<endl ;
  inTree = (TTree*)(inFile->Get("Calcued_Variables")) ;
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
  inTree->SetBranchAddress("TrackLength",&TrackLength) ;

  outPath = conf->outPath+conf->CORARSCollectionTime+"/"  ;
  outFileName = "Stage_Three_"+conf->CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;
  outTree = new TTree("ReCalcued_Variables", "ReCalculated Variables With Corrections") ;
  outTree->Branch("EventQuality", &EventQuality,"EventQuality/I") ;
  outTree->Branch("Muon_TOF", &Muon_TOF,"Real_TOF/F:Measure_TOF/F:Delta_TOF/F") ;

  outTree->Branch("NEDNoiseHit",&NEDNoiseHit,"NEDNoiseHit[8]/I");
  for(int i=0 ; i<NEDs ; i++)
    outTree->Branch(EDName[i].c_str(),ElecDet+i,"x/F:y/F:z/F:Measure_Time/F:Charge/F:ReCon_Time/F:Delta_Time/F:Corrected_Delta_Time/F:NormalizedCharge/F");
}

void ReCalcuVariables::ReCalcu()
{
  int NEntries = inTree->GetEntries() ;
  for(int i=0 ; i<NEntries ; i++)
  {
    if(i%10000==0)
      cout<<i<<endl ;
    inTree->GetEntry(i) ;
    for(int n=0 ; n<NEDs ; n++)
    {
      ElecDet[n].ED_Coor.x = ElecDet[n].ED_Coor.x*cos(atan(abs((*FuncSlope)[n])))+ElecDet[n].ED_Coor.y*sin(atan(abs((*FuncSlope)[n]))) ;
      ElecDet[n].ED_Coor.y = ElecDet[n].ED_Coor.y*cos(atan(abs((*FuncSlope)[n])))-ElecDet[n].ED_Coor.x*sin(atan(abs((*FuncSlope)[n]))) ;
      if(ElecDet[n].Measure_Time>0&&ElecDet[n].Charge<=200)
      {
        int binindex = ChargeDeltaTime[n]->GetXaxis()->FindBin(ElecDet[n].Charge) ;
        ElecDet[n].Corrected_Delta_Time = ElecDet[n].Delta_Time-ChargeDeltaTime[n]->GetBinContent(binindex);
      }
      ElecDet[n].NormalizedCharge = ElecDet[n].Charge*(346.5-26.)/TrackLength ;
    }
    outTree->Fill() ;
  }
  outFile->Write() ;
}

#endif
