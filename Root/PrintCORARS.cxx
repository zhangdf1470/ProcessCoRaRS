#ifndef __PrintCORARS__CXX
#define __PrintCORARS__CXX

#include "ProcessCORARS/PrintCORARS.h"

using namespace std;
PrintCORARS::PrintCORARS()
{
  CORARSCollectionTime = "" ;
  inPath = "" ;
  inFileName = "" ;
  inFile = 0 ;
  outPath = "" ;
  outFileName = "" ;
  outFile = 0 ;
  NEDs = 0 ;
  Initialize() ;
}

PrintCORARS::~PrintCORARS() 
{
  delete inFile ;
  delete outFile ;
}

void PrintCORARS::Initialize()
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
  }

  CORARSCollectionTime = conf->GetCORARSCollectionTime() ;
  inPath =  conf->GetoutPath()+"/"+CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  inFileName = "AnalysedResult_"+CORARSCollectionTime+".root" ;
  inFile = new TFile((inPath+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"In File: "<<inPath<<inFileName<<" doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  cout<<"inFile: "<<inFile->GetName()<<endl ;

  GlobalInfo = new TFile((inPath+"/GlobalInfo_"+CORARSCollectionTime+".root").c_str(), "read") ;
  if(!GlobalInfo)
  {
    cout<<"GlobalFile: "<<inPath+"/GlobalInfo_"+CORARSCollectionTime+".root"<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  NEvents = (TVectorD*) GlobalInfo->Get("NEvents") ;
  if(!NEvents)
  {
    cout<<"Can't get NEvents!!!!"<<endl;
    exit(0) ;
  }
  DeltaTimeUDScins = (TGraphErrors*) GlobalInfo->Get("DeltaTimeUDScins") ;
  if(!DeltaTimeUDScins)
  {
    cout<<"Can't get DeltaTimeUDScins!!!!"<<endl;
    exit(0) ;
  }
  EDDetEffi = (TVectorD*) inFile->Get("EDSCTDetEffi") ;

  SingleRate = (TVectorD*) inFile->Get("SingleRate") ;
  if(!EDDetEffi)
  {
    cout<<"Can't get EDDetEffi!!!!"<<endl;
    exit(0) ;
  }
  EDTimeRes = (TVectorD*) inFile->Get("EDSCTTimeRes") ;
  if(!EDTimeRes)
  {
    cout<<"Can't get EDTimeRes!!!!!"<<endl;
    exit(0) ;
  }
  EDEnergyRes = (TVectorD*) inFile->Get("EDSCTEnergyRes") ;
  if(!EDEnergyRes)
  {
    cout<<"Can't get EDEnergyRes!!!!"<<endl;
    exit(0) ;
  }
  EDReEnergyRes = (TVectorD*) inFile->Get("EDSCTReEnergyRes") ;
  if(!EDReEnergyRes)
  {
    cout<<"Can't get EDReEnergyRes!!!!!"<<endl;
    exit(0) ;
  }
  EDSingleParticlePeak = (TVectorD*) inFile->Get("EDSCTSingleParticlePeak") ;
  if(!EDSingleParticlePeak)
  {
    cout<<"Can't get EDSingleParticlePeak!!!!"<<endl;
    exit(0) ;
  }
  SysTimeRes = (TVectorD*) inFile->Get("SysTimeRes") ;
  if(!SysTimeRes)
  {
    cout<<"Can't get SysTimeRes!!!!"<<endl;
    exit(0) ;
  }

  outPath = conf->GetoutPath()+"/"+CORARSCollectionTime+"/"  ;
  outFileName = "Print_"+CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;
}

void PrintCORARS::GetHists()
{
  FiredNStripsUpTGCOne  =(TH1I*) GlobalInfo->Get("FiredNStripsUpTGCOne");
  FiredNWiresUpTGCOne =(TH1I*) GlobalInfo->Get("FiredNWiresUpTGCOne");
  FiredNStripsUpTGCTwo =(TH1I*) GlobalInfo->Get("FiredNStripsUpTGCTwo");
  FiredNWiresUpTGCTwo =(TH1I*) GlobalInfo->Get("FiredNWiresUpTGCTwo");
  FiredNStripsDownTGCOne =(TH1I*) GlobalInfo->Get("FiredNStripsDownTGCOne");
  FiredNWiresDownTGCOne =(TH1I*) GlobalInfo->Get("FiredNWiresDownTGCOne");
  FiredNStripsDownTGCTwo =(TH1I*) GlobalInfo->Get("FiredNStripsDownTGCTwo");
  FiredNWiresDownTGCTwo =(TH1I*) GlobalInfo->Get("FiredNWiresDownTGCTwo");

  EventQuality =(TH1F*) GlobalInfo->Get("EventQuality") ;
  UpTGCOneStripSignalWidth =(TH1F*) GlobalInfo->Get("UpTGCOneStripSignalWidth");
  UpTGCOneWireSignalWidth =(TH1F*) GlobalInfo->Get("UpTGCOneWireSignalWidth");
  UpTGCTwoStripSignalWidth =(TH1F*) GlobalInfo->Get("UpTGCTwoStripSignalWidth");
  UpTGCTwoWireSignalWidth =(TH1F*) GlobalInfo->Get("UpTGCTwoWireSignalWidth");
  DownTGCOneStripSignalWidth =(TH1F*) GlobalInfo->Get("DownTGCOneStripSignalWidth");
  DownTGCOneWireSignalWidth =(TH1F*) GlobalInfo->Get("DownTGCOneWireSignalWidth");
  DownTGCTwoStripSignalWidth =(TH1F*) GlobalInfo->Get("DownTGCTwoStripSignalWidth");
  DownTGCTwoWireSignalWidth =(TH1F*) GlobalInfo->Get("DownTGCTwoWireSignalWidth");

  UpScinX =(TH1F*) GlobalInfo->Get("UpScinX");
  DownScinX =(TH1F*) GlobalInfo->Get("DownScinX");

  DeltaTOF = (TH1F*) inFile->Get("Delta_TOF") ;
  for(int i=0 ; i<conf->NEDs ; i++)
  {
    EDPlaneNHits[i] = (TH2F*) inFile->Get((EDName[i]+"PlaneNHits").c_str()) ;
    EDPlaneNFiredHits[i] = (TH2F*) inFile->Get((EDName[i]+"PlaneNFiredHits").c_str()) ;
    EDSCT1NHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"NHits").c_str()) ;
    EDSCT1NFiredHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"NFiredHits").c_str()) ;
    EDSCT2NHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"NHits").c_str()) ;
    EDSCT2NFiredHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"NFiredHits").c_str()) ;
    EDSCT3NHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"NHits").c_str()) ;
    EDSCT3NFiredHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"NFiredHits").c_str()) ;
    EDSCT4NHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"NHits").c_str()) ;
    EDSCT4NFiredHits[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"NFiredHits").c_str()) ;


    EDSCT1CorrectedDeltaTime[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"CorrectedDeltaTime").c_str()) ;
    EDSCT2CorrectedDeltaTime[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"CorrectedDeltaTime").c_str()) ;
    EDSCT3CorrectedDeltaTime[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"CorrectedDeltaTime").c_str()) ;
    EDSCT4CorrectedDeltaTime[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"CorrectedDeltaTime").c_str()) ;

    EDSCT1SignalCharge[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"SignalCharge").c_str()) ;
    EDSCT2SignalCharge[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"SignalCharge").c_str()) ;
    EDSCT3SignalCharge[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"SignalCharge").c_str()) ;
    EDSCT4SignalCharge[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"SignalCharge").c_str()) ;

    EDPlaneDetEffi2DScan[i] = (TH2F*) inFile->Get((EDName[i]+"PlaneDetEffi2DScan").c_str()) ;
    EDSCT1DetEffi2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"DetEffi2DScan").c_str()) ;
    EDSCT2DetEffi2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"DetEffi2DScan").c_str()) ;
    EDSCT3DetEffi2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"DetEffi2DScan").c_str()) ;
    EDSCT4DetEffi2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"DetEffi2DScan").c_str()) ;
    EDSCT1DetEffi1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"DetEffi1DScan").c_str()) ;
    EDSCT2DetEffi1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"DetEffi1DScan").c_str()) ;
    EDSCT3DetEffi1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"DetEffi1DScan").c_str()) ;
    EDSCT4DetEffi1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"DetEffi1DScan").c_str()) ;

    EDSCT1TimeRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"TimeRes2DScan").c_str()) ;
    EDSCT2TimeRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"TimeRes2DScan").c_str()) ;
    EDSCT3TimeRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"TimeRes2DScan").c_str()) ;
    EDSCT4TimeRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"TimeRes2DScan").c_str()) ;
    EDSCT1TimeRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"TimeRes1DScan").c_str()) ;
    EDSCT2TimeRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"TimeRes1DScan").c_str()) ;
    EDSCT3TimeRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"TimeRes1DScan").c_str()) ;
    EDSCT4TimeRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"TimeRes1DScan").c_str()) ;

    EDSCT1EnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"EnergyRes2DScan").c_str()) ;
    EDSCT2EnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"EnergyRes2DScan").c_str()) ;
    EDSCT3EnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"EnergyRes2DScan").c_str()) ;
    EDSCT4EnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"EnergyRes2DScan").c_str()) ;
    EDSCT1EnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"EnergyRes1DScan").c_str()) ;
    EDSCT2EnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"EnergyRes1DScan").c_str()) ;
    EDSCT3EnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"EnergyRes1DScan").c_str()) ;
    EDSCT4EnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"EnergyRes1DScan").c_str()) ;

    EDSCT1ReEnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"ReEnergyRes2DScan").c_str()) ;
    EDSCT2ReEnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"ReEnergyRes2DScan").c_str()) ;
    EDSCT3ReEnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"ReEnergyRes2DScan").c_str()) ;
    EDSCT4ReEnergyRes2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"ReEnergyRes2DScan").c_str()) ;

    EDSCT1ReEnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"ReEnergyRes1DScan").c_str()) ;
    EDSCT2ReEnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"ReEnergyRes1DScan").c_str()) ;
    EDSCT3ReEnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"ReEnergyRes1DScan").c_str()) ;
    EDSCT4ReEnergyRes1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"ReEnergyRes1DScan").c_str()) ;

    EDSCT1SingleParticlePeak2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"SingleParticlePeak2DScan").c_str()) ;
    EDSCT2SingleParticlePeak2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"SingleParticlePeak2DScan").c_str()) ;
    EDSCT3SingleParticlePeak2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"SingleParticlePeak2DScan").c_str()) ;
    EDSCT4SingleParticlePeak2DScan[i] = (TH2F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"SingleParticlePeak2DScan").c_str()) ;
    EDSCT1SingleParticlePeak1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT1Name[i]+"SingleParticlePeak1DScan").c_str()) ;
    EDSCT2SingleParticlePeak1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT2Name[i]+"SingleParticlePeak1DScan").c_str()) ;
    EDSCT3SingleParticlePeak1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT3Name[i]+"SingleParticlePeak1DScan").c_str()) ;
    EDSCT4SingleParticlePeak1DScan[i] = (TH1F*) inFile->Get((EDName[i]+EDSCT4Name[i]+"SingleParticlePeak1DScan").c_str()) ;
  }
}
void PrintCORARS::GetGraphs()
{
  UpScinHitsGraph =(TGraph*) GlobalInfo->Get("UpScinHitsGraph");
  DownScinHitsGraph =(TGraph*) GlobalInfo->Get("DownScinHitsGraph");
  for(int i=0 ; i<NEDs ; i++)
  {
    EDHitsGraphs[i] =(TGraph*) GlobalInfo->Get((EDName[i]+"HitsGraph").c_str()) ;
    EDNoisesGraphs[i] =(TGraph*) GlobalInfo->Get((EDName[i]+"NoisesGraph").c_str()) ;
    RotatedEDHitsGraphs[i] =(TGraph*) GlobalInfo->Get(("Rotated"+EDName[i]+"HitsGraph").c_str()) ;
    RotatedEDNoisesGraphs[i] =(TGraph*) GlobalInfo->Get(("Rotated"+EDName[i]+"NoisesGraph").c_str()) ;

    Outer1BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"OuterBLine").c_str()) ;
    Outer1TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"OuterTLine").c_str()) ;
    Outer1LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"OuterLLine").c_str()) ;
    Outer1RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"OuterRLine").c_str()) ;
    Inner1BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"InnerBLine").c_str()) ;
    Inner1TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"InnerTLine").c_str()) ;
    Inner1LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"InnerLLine").c_str()) ;
    Inner1RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT1Name[i]+"InnerRLine").c_str()) ;

    Outer2BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"OuterBLine").c_str()) ;
    Outer2TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"OuterTLine").c_str()) ;
    Outer2LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"OuterLLine").c_str()) ;
    Outer2RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"OuterRLine").c_str()) ;
    Inner2BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"InnerBLine").c_str()) ;
    Inner2TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"InnerTLine").c_str()) ;
    Inner2LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"InnerLLine").c_str()) ;
    Inner2RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT2Name[i]+"InnerRLine").c_str()) ;

    Outer3BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"OuterBLine").c_str()) ;
    Outer3TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"OuterTLine").c_str()) ;
    Outer3LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"OuterLLine").c_str()) ;
    Outer3RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"OuterRLine").c_str()) ;
    Inner3BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"InnerBLine").c_str()) ;
    Inner3TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"InnerTLine").c_str()) ;
    Inner3LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"InnerLLine").c_str()) ;
    Inner3RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT3Name[i]+"InnerRLine").c_str()) ;

    Outer4BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"OuterBLine").c_str()) ;
    Outer4TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"OuterTLine").c_str()) ;
    Outer4LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"OuterLLine").c_str()) ;
    Outer4RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"OuterRLine").c_str()) ;
    Inner4BLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"InnerBLine").c_str()) ;
    Inner4TLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"InnerTLine").c_str()) ;
    Inner4LLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"InnerLLine").c_str()) ;
    Inner4RLine[i] =(TLine*) GlobalInfo->Get((EDName[i]+"_"+EDSCT4Name[i]+"InnerRLine").c_str()) ;
  }
}

void PrintCORARS::BuildPaveText(int i)
{
  PtCORARS[i] = new TPaveText(0.1, 0.1, 0.9, 0.9, "nbNDC") ;
  PtCORARS[i]->SetShadowColor(0) ;
  PtCORARS[i]->SetTextAlign(11) ;
  PtCORARS[i]->SetFillColor(0) ;
  PtCORARS[i]->SetBorderSize(0) ;
  PtCORARS[i]->AddText(("Collection Time: "+conf->CORARSCollectionTime).c_str());
  PtCORARS[i]->AddText(("Temperature: "+conf->Temperature).c_str());
  PtCORARS[i]->AddText(TString::Format("NEvents: %i",(int)(*NEvents)[0]));
  PtCORARS[i]->AddText(TString::Format("SingleRate: %.2f kHz",(*SingleRate)[i]/1000));
  for(int n=0 ; n<NEDs ; n++)
    PtCORARS[i]->AddText((EDName[n]+": "+EDSCT1Name[n]+" "+EDSCT2Name[n]+" "+EDSCT3Name[n]+" "+EDSCT4Name[n]).c_str());

  PtED[i] = new TPaveText(0.05, 0.05, 0.95, 0.95, "nbNDC");
  PtED[i]->SetTextSize(0.03) ;
  PtED[i]->SetShadowColor(0) ;
  PtED[i]->SetTextAlign(11) ;
  PtED[i]->SetFillColor(0) ;
  PtED[i]->SetBorderSize(0) ;
  //PtED[i]->AddText((EDName[i]).c_str());
  //PtED[i]->AddText("") ;
  PtED[i]->AddText((EDName[i]+"-"+EDSCT1Name[i]+": ").c_str()) ;
  PtED[i]->AddText(TString::Format("   Detection Efficiency: %.2f%%",(*EDDetEffi)[4*i])) ;
  PtED[i]->AddText(TString::Format("   Time Resolution: %.2f ns",(*EDTimeRes)[4*i])) ;
  PtED[i]->AddText(TString::Format("   Energy Resolution: %.2f Counts",(*EDEnergyRes)[4*i])) ;
  PtED[i]->AddText(TString::Format("   Relative Energy Resolution: %.2f%%",(*EDReEnergyRes)[4*i]*100)) ;
  PtED[i]->AddText(TString::Format("   Single Particle Peak: %.2f Counts",(*EDSingleParticlePeak)[4*i])) ;
  PtED[i]->AddText("") ;
  PtED[i]->AddText((EDName[i]+"-"+EDSCT2Name[i]+": ").c_str()) ;
  PtED[i]->AddText(TString::Format("   Detection Efficiency: %.2f%%",(*EDDetEffi)[4*i+1])) ;
  PtED[i]->AddText(TString::Format("   Time Resolution: %.2f ns",(*EDTimeRes)[4*i+1])) ;
  PtED[i]->AddText(TString::Format("   Energy Resolution: %.2f Counts",(*EDEnergyRes)[4*i+1])) ;
  PtED[i]->AddText(TString::Format("   Relative Energy Resolution: %.2f%%",(*EDReEnergyRes)[4*i+1]*100)) ;
  PtED[i]->AddText(TString::Format("   Single Particle Peak: %.2f Counts",(*EDSingleParticlePeak)[4*i+1])) ;
  PtED[i]->AddText("") ;
  PtED[i]->AddText((EDName[i]+"-"+EDSCT3Name[i]+": ").c_str()) ;
  PtED[i]->AddText(TString::Format("   Detection Efficiency: %.2f%%",(*EDDetEffi)[4*i+2])) ;
  PtED[i]->AddText(TString::Format("   Time Resolution: %.2f ns",(*EDTimeRes)[4*i+2])) ;
  PtED[i]->AddText(TString::Format("   Energy Resolution: %.2f Counts",(*EDEnergyRes)[4*i+2])) ;
  PtED[i]->AddText(TString::Format("   Relative Energy Resolution: %.2f%%",(*EDReEnergyRes)[4*i+2]*100)) ;
  PtED[i]->AddText(TString::Format("   Single Particle Peak: %.2f Counts",(*EDSingleParticlePeak)[4*i+2])) ;
  PtED[i]->AddText("") ;
  PtED[i]->AddText((EDName[i]+"-"+EDSCT4Name[i]+": ").c_str()) ;
  PtED[i]->AddText(TString::Format("   Detection Efficiency: %.2f%%",(*EDDetEffi)[4*i+3])) ;
  PtED[i]->AddText(TString::Format("   Time Resolution: %.2f ns",(*EDTimeRes)[4*i+3])) ;
  PtED[i]->AddText(TString::Format("   Energy Resolution: %.2f Counts",(*EDEnergyRes)[4*i+3])) ;
  PtED[i]->AddText(TString::Format("   Relative Energy Resolution: %.2f%%",(*EDReEnergyRes)[4*i+3]*100)) ;
  PtED[i]->AddText(TString::Format("   Single Particle Peak: %.2f Counts",(*EDSingleParticlePeak)[4*i+3])) ;
}

void PrintCORARS::ExportPDF(int i)
{
  TCanvas *c = new TCanvas((EDName[i]+"Canvas").c_str(),"",800,600) ;
  string pdfname = outPath+EDName[i]+"_"+CORARSCollectionTime+".pdf" ;
  c->Print((pdfname+"[").c_str()) ;
  c->SetMargin(0.15,0.15,0.15,0.15) ;
  PtCORARS[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  FiredNStripsUpTGCOne->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNWiresUpTGCOne->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNStripsUpTGCTwo->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNWiresUpTGCTwo->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNStripsDownTGCOne->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNWiresDownTGCOne->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNStripsDownTGCTwo->Draw() ;
  c->Print(pdfname.c_str()) ;
  FiredNWiresDownTGCTwo->Draw() ;
  c->Print(pdfname.c_str()) ;
  UpTGCOneStripSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  UpTGCOneWireSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  UpTGCTwoStripSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  UpTGCTwoWireSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  DownTGCOneStripSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  DownTGCOneWireSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  DownTGCTwoStripSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  DownTGCTwoWireSignalWidth->Draw() ;
  c->Print(pdfname.c_str()) ;
  UpScinHitsGraph->Draw("ap") ;
  c->Print(pdfname.c_str()) ;
  DownScinHitsGraph->Draw("ap") ;
  c->Print(pdfname.c_str()) ;
  UpScinX->Draw() ;
  c->Print(pdfname.c_str()) ;
  DownScinX->Draw() ;
  c->Print(pdfname.c_str()) ;
  EventQuality->Scale(1./EventQuality->Integral()) ;
  EventQuality->SetTitle("Current") ;
  conf->NormalEventQuality->Scale(1./conf->NormalEventQuality->Integral()) ;
  conf->NormalEventQuality->SetLineColor(kRed) ;
  conf->NormalEventQuality->Draw() ;
  EventQuality->Draw("SAME") ;
  c->BuildLegend(0.6, 0.5, 0.8, 0.7) ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  DeltaTOF->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0) ;
  DeltaTimeUDScins->Draw("AP") ;
  c->Print(pdfname.c_str()) ;
  c->Clear() ;
  PtED[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  RotatedEDHitsGraphs[i]->Draw("AP") ;
  Outer1BLine[i]->Draw("SAME") ;
  Outer1TLine[i]->Draw("SAME") ;
  Outer1LLine[i]->Draw("SAME") ;
  Outer1RLine[i]->Draw("SAME") ;
  Inner1BLine[i]->Draw("SAME") ;
  Inner1TLine[i]->Draw("SAME") ;
  Inner1LLine[i]->Draw("SAME") ;
  Inner1RLine[i]->Draw("SAME") ;
  Outer2BLine[i]->Draw("SAME") ;
  Outer2TLine[i]->Draw("SAME") ;
  Outer2LLine[i]->Draw("SAME") ;
  Outer2RLine[i]->Draw("SAME") ;
  Inner2BLine[i]->Draw("SAME") ;
  Inner2TLine[i]->Draw("SAME") ;
  Inner2LLine[i]->Draw("SAME") ;
  Inner2RLine[i]->Draw("SAME") ;
  Outer3BLine[i]->Draw("SAME") ;
  Outer3TLine[i]->Draw("SAME") ;
  Outer3LLine[i]->Draw("SAME") ;
  Outer3RLine[i]->Draw("SAME") ;
  Inner3BLine[i]->Draw("SAME") ;
  Inner3TLine[i]->Draw("SAME") ;
  Inner3LLine[i]->Draw("SAME") ;
  Inner3RLine[i]->Draw("SAME") ;
  Outer4BLine[i]->Draw("SAME") ;
  Outer4TLine[i]->Draw("SAME") ;
  Outer4LLine[i]->Draw("SAME") ;
  Outer4RLine[i]->Draw("SAME") ;
  Inner4BLine[i]->Draw("SAME") ;
  Inner4TLine[i]->Draw("SAME") ;
  Inner4LLine[i]->Draw("SAME") ;
  Inner4RLine[i]->Draw("SAME") ;
  c->Print(pdfname.c_str()) ;
  RotatedEDNoisesGraphs[i]->Draw("AP") ;
  Outer1BLine[i]->Draw("SAME") ;
  Outer1TLine[i]->Draw("SAME") ;
  Outer1LLine[i]->Draw("SAME") ;
  Outer1RLine[i]->Draw("SAME") ;
  Inner1BLine[i]->Draw("SAME") ;
  Inner1TLine[i]->Draw("SAME") ;
  Inner1LLine[i]->Draw("SAME") ;
  Inner1RLine[i]->Draw("SAME") ;
  Outer2BLine[i]->Draw("SAME") ;
  Outer2TLine[i]->Draw("SAME") ;
  Outer2LLine[i]->Draw("SAME") ;
  Outer2RLine[i]->Draw("SAME") ;
  Inner2BLine[i]->Draw("SAME") ;
  Inner2TLine[i]->Draw("SAME") ;
  Inner2LLine[i]->Draw("SAME") ;
  Inner2RLine[i]->Draw("SAME") ;
  Outer3BLine[i]->Draw("SAME") ;
  Outer3TLine[i]->Draw("SAME") ;
  Outer3LLine[i]->Draw("SAME") ;
  Outer3RLine[i]->Draw("SAME") ;
  Inner3BLine[i]->Draw("SAME") ;
  Inner3TLine[i]->Draw("SAME") ;
  Inner3LLine[i]->Draw("SAME") ;
  Inner3RLine[i]->Draw("SAME") ;
  Outer4BLine[i]->Draw("SAME") ;
  Outer4TLine[i]->Draw("SAME") ;
  Outer4LLine[i]->Draw("SAME") ;
  Outer4RLine[i]->Draw("SAME") ;
  Inner4BLine[i]->Draw("SAME") ;
  Inner4TLine[i]->Draw("SAME") ;
  Inner4LLine[i]->Draw("SAME") ;
  Inner4RLine[i]->Draw("SAME") ;
  c->Print(pdfname.c_str()) ;
  EDPlaneDetEffi2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT1NFiredHits[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT1DetEffi2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT1DetEffi1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);
  EDSCT1CorrectedDeltaTime[i]->Draw() ;
  c->Print(pdfname.c_str()) ;

  gStyle->SetOptStat(0);
  cout<<"EDSCT1TimeRes2DScan-"<<endl ;
  EDSCT1TimeRes2DScan[i]->Draw("lego2z") ;
  cout<<"-EDSCT1TimeRes2DScan"<<endl ;
  c->Print(pdfname.c_str()) ;
  EDSCT1TimeRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;

  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);
  EDSCT1SignalCharge[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0);
  EDSCT1EnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT1EnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT1ReEnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT1ReEnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT1SingleParticlePeak2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT1SingleParticlePeak1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  
  EDSCT2NFiredHits[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT2DetEffi2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT2DetEffi1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  EDSCT2CorrectedDeltaTime[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  EDSCT2TimeRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT2TimeRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  EDSCT2SignalCharge[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  EDSCT2EnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT2EnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT2ReEnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT2ReEnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT2SingleParticlePeak2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT2SingleParticlePeak1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;

  EDSCT3NFiredHits[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT3DetEffi2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT3DetEffi1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  EDSCT3CorrectedDeltaTime[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  EDSCT3TimeRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT3TimeRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  EDSCT3SignalCharge[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  EDSCT3EnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT3EnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT3ReEnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT3ReEnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT3SingleParticlePeak2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT3SingleParticlePeak1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;

  EDSCT4NFiredHits[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT4DetEffi2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT4DetEffi1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  EDSCT4CorrectedDeltaTime[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  EDSCT4TimeRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT4TimeRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(1); 
  gStyle->SetOptFit(1); 
  EDSCT4SignalCharge[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  gStyle->SetOptStat(0); 
  EDSCT4EnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT4EnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT4ReEnergyRes2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT4ReEnergyRes1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;
  EDSCT4SingleParticlePeak2DScan[i]->Draw("lego2z") ;
  c->Print(pdfname.c_str()) ;
  EDSCT4SingleParticlePeak1DScan[i]->Draw() ;
  c->Print(pdfname.c_str()) ;

  c->Print((pdfname+"]").c_str()) ;
}

void PrintCORARS::ExportTXT(int i)
{
  ofstream OutFile ;
  OutFile.open((outPath+"/Result"+conf->CORARSCollectionTime+".txt").c_str(),ios_base::app) ; 
  OutFile<< setiosflags(ios::fixed) << setiosflags(ios::right) << setprecision(2);
  OutFile<<"| "<<"EDDetEffi"<<setw(15)<<" | "<<"EDTimeRes"<<setw(15)<<" | "<<"EDEnergyRes"<<setw(15)<<" | "<<"EDReEnergyRes"<<setw(15)<<" | "<<"EDSingleParticlePeak"<<" | "<<"\r\n" ;
  OutFile<<conf->EDName[i]<<" "<<conf->EDSCT1Name[i]<<":"<<"\r\n" ;
  OutFile<<"| "<<(*EDDetEffi)[4*i]<<setw(15)<<" | "<<(*EDTimeRes)[4*i]<<setw(15)<<" | "<<(*EDEnergyRes)[4*i]<<setw(15)<<" | "<<(*EDReEnergyRes)[4*i]<<setw(15)<<" | "<<(*EDSingleParticlePeak)[4*i]<<" | "<<"\r\n" ;
  OutFile<<conf->EDName[i]<<" "<<conf->EDSCT2Name[i]<<":"<<"\r\n" ;
  OutFile<<"| "<<(*EDDetEffi)[4*i+1]<<setw(15)<<" | "<<(*EDTimeRes)[4*i+1]<<setw(15)<<" | "<<(*EDEnergyRes)[4*i+1]<<setw(15)<<" | "<<(*EDReEnergyRes)[4*i+1]<<setw(15)<<" | "<<(*EDSingleParticlePeak)[4*i+1]<<" | "<<"\r\n" ;
  OutFile.close() ;
}

void PrintCORARS::Print()
{
  GetHists() ;
  GetGraphs() ;
  ofstream OutFile;   //create object for output
  OutFile.open((outPath+"/Result"+CORARSCollectionTime+".txt").c_str()); //associate with a file
  for(int i=0 ; i<conf->NEDs ; i++)
  {
    BuildPaveText(i) ;
    ExportPDF(i) ;
    ExportTXT(i) ;
  }
}

#endif
