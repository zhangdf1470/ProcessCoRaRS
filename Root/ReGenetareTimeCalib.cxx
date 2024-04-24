#ifndef ReGenerateTimeCalib_H
#define ReGenerateTimeCalib_H
#include "ProcessCORARS/ReGenerateTimeCalib.h"

using namespace std ;

ReGenerateTimeCalib::ReGenerateTimeCalib()
{
  inPath  = "";
  inFileName = "" ;
  inFile = 0 ;
  inTree = 0 ;
  outPath = "" ;
  outFileName = "" ;
  outFile = 0 ;
  outTree = 0 ;

  FitParas = 0 ;

  Initialize() ;
}
ReGenerateTimeCalib::~ReGenerateTimeCalib()
{
  delete inTree ; 
  delete inFile ;
  delete outTree ; 
  delete outFile ;
}

void ReGenerateTimeCalib::Initialize()
{
  static ConfigureClass* conf = ConfigureClass::Instance() ;

  TSCTXY = conf->GetTSCTXY() ; // Get the x and y of Trigger SCTs

  TEnv* config = new TEnv((conf->CalibrationConfFile).c_str()) ;
  if(!config)
  {
    cout<<"Cannot open configuration file.\n";
    return ;
  }

  inPath = config->GetValue("outPath","") ;
  inFileName = "SimulateTimeCalib.root" ;
  inFile = new TFile((inPath+"/"+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"inFile: "+inPath+"/"+inFileName+" doesn't exist!!!!"<<endl ;
    exit(0) ;
  }
  cout<<"inFile: "<<inFile->GetName()<<endl ;
  inTree = (TTree*)inFile->Get("TimeCalib") ;
  if(!inTree)
  {
    cout<<"inTree doesn't exist!!!!"<<endl ;
    exit(0) ;
  }
  inTree->SetBranchAddress("Up_Scin",&Up_Scin) ;
  inTree->SetBranchAddress("Down_Scin",&Down_Scin) ;

  FitParas = (TMatrixF* )inFile->Get("FitParas") ;
  if(!FitParas)
  {
    cout<<"FitParas doesn't exist!!!!"<<endl ;
    exit(0) ;
  }

  outPath = config->GetValue("outPath","") ;
  outFileName = "ReGenerateTimeCalib.root" ;
  outFile = new TFile((outPath+"/"+outFileName).c_str(),"RECREATE") ;
  outTree = new TTree("TimeCalib","Time Calib") ;
  outTree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F");
  outTree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/FF:TimeCalib/F");

}

void ReGenerateTimeCalib::CalcuScinCalib()
{
  float Distance = sqrt(pow(Down_Scin.Scin_Coor.x - Up_Scin.Scin_Coor.x, 2) + pow(Down_Scin.Scin_Coor.y - Up_Scin.Scin_Coor.y, 2) + pow(320.5, 2)) ;
  Up_Scin.TimeCalib = Up_Scin.Measure_Time+(Distance*1000/29.8)-Down_Scin.Measure_Time + 
  ((*FitParas)[Down_Scin.index][0] 
   +(*FitParas)[Down_Scin.index][1]*Down_Scin.DistanceToPMT 
   +(*FitParas)[Down_Scin.index][2]*pow(Down_Scin.DistanceToPMT,2)
   +(*FitParas)[Down_Scin.index][3]*pow(Down_Scin.DistanceToPMT,3) 
   +(*FitParas)[Down_Scin.index][4]*pow(Down_Scin.DistanceToPMT,4)
   +(*FitParas)[Down_Scin.index][5]*pow(Down_Scin.DistanceToPMT,5)
   +(*FitParas)[Down_Scin.index][6]*pow(Down_Scin.DistanceToPMT,6));
  
  Down_Scin.TimeCalib = Down_Scin.Measure_Time-(Distance*1000/29.8)-Up_Scin.Measure_Time +
  ((*FitParas)[Up_Scin.index][0]
   +(*FitParas)[Up_Scin.index][1]*Up_Scin.DistanceToPMT   
   +(*FitParas)[Up_Scin.index][2]*pow(Up_Scin.DistanceToPMT,2)
   +(*FitParas)[Up_Scin.index][3]*pow(Up_Scin.DistanceToPMT,3)   
   +(*FitParas)[Up_Scin.index][4]*pow(Up_Scin.DistanceToPMT,4)
   +(*FitParas)[Up_Scin.index][5]*pow(Up_Scin.DistanceToPMT,5)
   +(*FitParas)[Up_Scin.index][6]*pow(Up_Scin.DistanceToPMT,6)) ;

  int m = 0 ;
  int n = 0 ;
  m = 10*(Up_Scin.Scin_Coor.x-TSCTXY.at(Up_Scin.index).first)/25 ;
  n = 10*(Up_Scin.Scin_Coor.y-TSCTXY.at(Up_Scin.index).second)/25 ;
  if(m>=10) m=9 ;
  if(n>=48) n=47 ;
  CalibBin[Up_Scin.index][m][n]->Fill(Up_Scin.TimeCalib) ;

  m = 10*(Down_Scin.Scin_Coor.x-TSCTXY.at(Down_Scin.index).first)/25 ;
  n = 10*(Down_Scin.Scin_Coor.y-TSCTXY.at(Down_Scin.index).second)/25 ;
  if(m>=10) m=9 ;
  if(n>=48) n=47 ;
  CalibBin[Down_Scin.index][m][n]->Fill(Down_Scin.TimeCalib) ;
  return ;
}

void ReGenerateTimeCalib::ReGenerate()
{
  for(int i=0 ; i<12 ; i++)
  {
    for(int j=0 ; j<10 ; j++)
    {
      for(int k=0 ; k<48 ; k++)
      {
        CalibBin[i][j][k] = new TH1F(TString::Format("CalibBin%i_x%i_y%i",i,j,k),"",200,-100000,100000) ;
      }
    }
  }

  /*CalibMean[0] = new TH2F(TString::Format("CalibMean%i",0), "", 10, 0.2, 0.2+25, 48, 0., 120.) ;
  CalibMean[1] = new TH2F(TString::Format("CalibMean%i",1), "", 10, 28., 28+25, 48, 0., 120.) ;
  CalibMean[2] = new TH2F(TString::Format("CalibMean%i",2), "", 10, 55.9, 55.9+25, 48, 4.0, 4.+120.) ;
  CalibMean[3] = new TH2F(TString::Format("CalibMean%i",3), "", 10, 86., 86.+25, 48, 0.,120.) ;
  CalibMean[4] = new TH2F(TString::Format("CalibMean%i",4), "", 10, 114.2, 114.2+25, 48, 0.,120.) ;
  CalibMean[5] = new TH2F(TString::Format("CalibMean%i",5), "", 10, 142., 142.+25, 48,0.,120.) ;
  CalibMean[6] = new TH2F(TString::Format("CalibMean%i",6), "", 10, 0.2, 0.2+25, 48, 5., 5.+120.) ;
  CalibMean[7] = new TH2F(TString::Format("CalibMean%i",7), "", 10, 28., 28.+25., 48, 5., 5.+120.) ;
  CalibMean[8] = new TH2F(TString::Format("CalibMean%i",8), "", 10, 55.9, 55.9+25., 48, 5., 5.+120.) ;
  CalibMean[9] = new TH2F(TString::Format("CalibMean%i",9), "", 10, 84.4, 84.4+25, 48, 3., 3.+120.) ;
  CalibMean[10] = new TH2F(TString::Format("CalibMean%i",10), "", 10, 112.8, 112.8+25., 48, 2. , 2.+120.) ;
  CalibMean[11] = new TH2F(TString::Format("CalibMean%i",11), "", 10, 140.2, 140.2+25., 48, 2., 2.+120.) ;*/
  for(int i= 0 ; i<12 ; i++)
  {
    CalibMean[i] = new TH2F(TString::Format("CalibMean%i",i), "", 10, TSCTXY.at(i).first, TSCTXY.at(i).first+25, 48, TSCTXY.at(i).second, TSCTXY.at(i).second+120.) ;
  }

  Long64_t nEntries = inTree->GetEntries() ;
  cout<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
  {
    if(i%10000==0)
     cout<<i<<endl ;
    inTree->GetEntry(i) ;
    CalcuScinCalib() ;
    outTree->Fill() ;
  }

  for(int i=0 ; i<12 ; i++)
  {
    for(int j=0 ; j<10 ; j++)
    {
      for(int k=0 ; k<48 ; k++)
      {
        if(CalibBin[i][j][k]->GetEntries()>0)
        {
          TF1* func = new TF1("func", "gaus", CalibBin[i][j][k]->GetMean()-CalibBin[i][j][k]->GetRMS(),CalibBin[i][j][k]->GetMean()+CalibBin[i][j][k]->GetRMS()) ;
          CalibBin[i][j][k]->Fit("func", "RQ") ;
          //CalibMean[i]->SetBinContent(j+1,k+1,func->GetParameter(1)) ;
          CalibMean[i]->SetBinContent(j+1,k+1,CalibBin[i][j][k]->GetMean()) ;
          delete func ;
        }
        else
          CalibMean[i]->SetBinContent(j+1,k+1,0) ;
        CalibBin[i][j][k]->Write() ;
      }
    }
    CalibMean[i]->Write() ;
  }
}

#endif
