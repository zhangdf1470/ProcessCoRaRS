#ifndef ChargeCorrectTime_CXX
#define ChargeCorrectTime_CXX
#include "ProcessCORARS/ChargeCorrectTime.h"

ChargeCorrectTime::ChargeCorrectTime()
{
  inFile = 0 ;
  inTree = 0 ;
  outFile = 0 ;
  inPath = "" ;
  inFileName = "" ;
  outPath = "" ;
  outFileName = "" ;
  NEDs = 0 ;
  for(int i=0 ; i<8 ; i++)
    EDName[i] = "" ;

  Initialize() ;
}

ChargeCorrectTime::~ChargeCorrectTime()
{
  delete inTree ;
  delete inFile ;
  delete outFile ;
}

void ChargeCorrectTime::Initialize()
{
  conf = ConfigureClass::Instance() ;

  NEDs = conf->NEDs ;
  for(int i=0 ; i<NEDs ; i++)
    EDName[i] = conf->EDName[i] ;

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
  //branches of intree
  for(int i=0 ; i<NEDs ; i++)
  {
    inTree->SetBranchAddress(EDName[i].c_str(),ElecDet+i) ;
  }
 
  outPath = conf->outPath+conf->CORARSCollectionTime+"/"  ;
  outFileName = "ChargeCorrectTime_"+conf->CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;

  for(int i=0 ; i<NEDs ; i++)
  {
    ChargeDeltaTime[i] = new TH1F(TString::Format("ChargeDeltaTime%i",i),"",200,0,200);
    for(int j=0 ; j<ChargeDeltaTime[i]->GetNbinsX() ; j++)
      DeltaTime[i][j] = new TH1F(TString::Format("DeltaTime%i_%i",i,j),"",500,-180000,-80000) ;
  }
}

void ChargeCorrectTime::Correct()
{
  int NEntries = inTree->GetEntries() ;
  for(int n=0 ; n<NEntries ; n++)
  {
    if(n%10000==0)
      cout<<n<<endl ;
    inTree->GetEntry(n) ;
    for(int i=0 ; i<NEDs ; i++)
    {
      if(ElecDet[i].Measure_Time<=0||ElecDet[i].Charge>=200||ElecDet[i].Delta_Time<-180000||ElecDet[i].Delta_Time>-80000)
        continue ;
      int bin = (int)(ElecDet[i].Charge)/1 ;
      DeltaTime[i][bin]->Fill(ElecDet[i].Delta_Time) ;
    }
  }

  for(int i=0 ; i<NEDs ; i++)
  {
    for(int j=0 ; j<200 ; j++)
    {
      if(DeltaTime[i][j]->GetEntries()>200)
      {
        TF1* function = new TF1("function","gaus",DeltaTime[i][j]->GetMean()-DeltaTime[i][j]->GetRMS(),DeltaTime[i][j]->GetMean()+DeltaTime[i][j]->GetRMS()) ;
        DeltaTime[i][j]->Fit(function,"RQ") ;
        ChargeDeltaTime[i]->SetBinContent(j+1, function->GetParameter(1)) ;
        DeltaTime[i][j]->Write() ;
      }
      else
      {
        ChargeDeltaTime[i]->SetBinContent(j+1,DeltaTime[i][j]->GetMean()) ;
        DeltaTime[i][j]->Write() ;
      }
    }
    ChargeDeltaTime[i]->Write() ;
  }
  outFile->Write() ;
  outFile->Close() ;
}
#endif

