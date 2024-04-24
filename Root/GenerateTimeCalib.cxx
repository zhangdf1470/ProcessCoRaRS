#ifndef GenerateTimeCalib_CXX
#define GenerateTimeCalib_CXX

#include "ProcessCORARS/GenerateTimeCalib.h"
#include "TString.h"
#include "TEnv.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std ;

GenerateTimeCalib::GenerateTimeCalib()
{
  chain = 0 ;
  inPath = "" ;
  outPath = "" ;
  inFileName = "" ;
  outFile = 0 ;
  outTree = 0 ;
  SoutTree = 0 ;
  outFileName = "" ;
  Initialize() ;
}

GenerateTimeCalib::~GenerateTimeCalib()
{
  delete chain ;
  delete outFile ;
  delete outTree ;
  delete SoutTree ;
}

void GenerateTimeCalib::Initialize()
{
  chain = new TChain("CalcuedTGCScin","add needed root file to a chain") ;
  chain->SetBranchAddress("Up_Scin",&Up_Scin) ; 
  chain->SetBranchAddress("Down_Scin",&Down_Scin) ; 
  chain->SetBranchAddress("EventQuality",&EventQuality) ; 

  static ConfigureClass* conf = ConfigureClass::Instance() ;

  TSCTXY = conf->GetTSCTXY() ; // Get the x and y of Trigger SCTs

  TEnv* config = new TEnv((conf->CalibrationConfFile).c_str()) ;
  if(!config)
  {
    cout<<"Cannot open configuration file: "<<conf->CalibrationConfFile<<".\n";
    exit(0) ;
  }

  inPath = config->GetValue("inPath","") ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"inPath: "<<inPath<<" doesn't exist!"<<endl ;
    exit(0) ;
  }
  outPath = config->GetValue("outPath","") ;
  if(gSystem->AccessPathName(outPath.c_str()))
    gSystem->Exec(("mkdir -p "+outPath).c_str()) ;

  string datalistfilename = config->GetValue("DataListFile","") ;
  ifstream datalist(datalistfilename.c_str()) ;
  if(!datalist)
  {
    cout<<"Cannot open datalist file: "<<datalistfilename<<".\n";
    exit(0) ;
  }

  gSystem->Exec(("cp "+ datalistfilename +" "+ outPath).c_str()) ;
  std::string time ;
  while(getline(datalist, time))
  {
    if(time.find_first_of('#')==0)
      continue ;
    if(time != "")
    {
      chain->Add((inPath+"/"+time+"/TGCScin_"+time+".root").c_str()) ;
      cout<<"inFile: "<<inPath+"/"+time+"/TGCScin_"+time+".root"<<endl ;
    }
  }
  datalist.close() ;

  outFileName = outPath+"/GenerateTimeCalib.root" ;
  outFile = new TFile(outFileName.c_str(),"RECREATE") ;
  cout<<"outFile: "<<outFile->GetName()<<endl ;
  outTree = new TTree("TimeCalib","only part of down Time Calib") ;
  outTree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F");
  outTree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F");

  SoutTree = new TTree("STimeCalib","Time Calib used further simulation") ;
  SoutTree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F");
  SoutTree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F");
}

bool GenerateTimeCalib::GetDownCalib()
{
  Up_Scin.DistanceToPMT = sqrt(pow(Up_Scin.Scin_Coor.x-(TSCTXY.at(Up_Scin.index).first+12.5), 2)+pow(Up_Scin.Scin_Coor.y-TSCTXY.at(Up_Scin.index).second,2)) ;
  Down_Scin.DistanceToPMT = sqrt(pow(Down_Scin.Scin_Coor.x-(TSCTXY.at(Down_Scin.index).first+12.5), 2)+pow(Down_Scin.Scin_Coor.y-TSCTXY.at(Down_Scin.index).second,2)) ;

  if(acos(Up_Scin.Scin_Coor.y/Up_Scin.DistanceToPMT)>=0.9||acos(Down_Scin.Scin_Coor.y/Down_Scin.DistanceToPMT)>=0.9||Up_Scin.DistanceToPMT<=10||Down_Scin.DistanceToPMT<=10)
     return false ;
  SoutTree->Fill() ;

  //if(Up_Scin.Scin_Coor.x>=65&&Up_Scin.Scin_Coor.x<70&&Up_Scin.Scin_Coor.y>=49.&&Up_Scin.Scin_Coor.y<59.)
  //if(Up_Scin.Scin_Coor.x>=63&&Up_Scin.Scin_Coor.x<73&&Up_Scin.Scin_Coor.y>=49.&&Up_Scin.Scin_Coor.y<59.)
  if(Up_Scin.Scin_Coor.x>=53&&Up_Scin.Scin_Coor.x<63&&Up_Scin.Scin_Coor.y>=49.&&Up_Scin.Scin_Coor.y<59.)
  {
    int TOF = 1000*sqrt(pow(Up_Scin.Scin_Coor.x-Down_Scin.Scin_Coor.x,2)+pow(Up_Scin.Scin_Coor.y-Down_Scin.Scin_Coor.y,2)+pow(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z,2))/29.8 ;
    // here, we set offseet_dw is equal to 0
    Down_Scin.TimeCalib = Down_Scin.Measure_Time - Up_Scin.Measure_Time - TOF ;
    return true ;
  }
  else 
    return false ;
}

void GenerateTimeCalib::Generate()
{
  int nEntries = chain->GetEntries() ;
  cout<<"Number of Events: "<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
  {
    if(i%100000==0)
      cout<<i<<endl ;
    chain->GetEntry(i) ;
    // In principle, to get the exact calibration,
    // we should use events with eventqualit is 1,
    // but here due to lack of events, we will use events with eventquality=1/2/3
    if(EventQuality!=1) 
      continue ;
    if(!GetDownCalib()) 
      continue ;
    outTree->Fill() ;
  }
  outFile->Write() ;
  outFile->Close() ;
}
#endif
