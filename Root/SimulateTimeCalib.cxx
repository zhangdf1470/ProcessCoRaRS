#ifndef SimulateTimeCalib_CXX
#define SimulateTimeCalib_CXX

#include "ProcessCORARS/SimulateTimeCalib.h"

using namespace std ;
SimulateTimeCalib::SimulateTimeCalib()
{
  inPath = "" ;
  inFileName = "" ;
  inFile = 0 ;
  inTree = 0 ;
  SinTree = 0 ;

  outPath = "" ;
  outFile = 0 ;
  outTree = 0 ;

  DownFitParas = 0 ;
  FitParas = 0 ;

  Initialize() ;
}

SimulateTimeCalib::~SimulateTimeCalib()
{
  delete inTree ;
  delete SinTree ;
  delete inFile ;
  delete outFile ;
  delete outTree ;
} 

void SimulateTimeCalib::Initialize()
{
  static ConfigureClass* conf = ConfigureClass::Instance() ;

  TEnv* config = new TEnv((conf->CalibrationConfFile).c_str()) ;
  if(!config)
  {
    cout<<"Cannot open configuration file.\n";
    return ;
  }

  inPath = config->GetValue("outPath","") ;

  inFileName = "GenerateTimeCalib.root" ;
  inFile = new TFile((inPath+"/"+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"inFile: "<<inPath+"/"+inFileName<<" doesn't exist!!!!"<<endl ;
    exit(0) ;
  }
  cout<<"inFile: "<<inFile->GetName()<<endl ;
  inTree = (TTree*)inFile->Get("TimeCalib") ;
  inTree->SetBranchAddress("Up_Scin",&Up_Scin) ;
  inTree->SetBranchAddress("Down_Scin",&Down_Scin) ;
  SinTree = (TTree*)(inFile->Get("STimeCalib")) ;
  SinTree->SetBranchAddress("Up_Scin",&Up_Scin) ;
  SinTree->SetBranchAddress("Down_Scin",&Down_Scin) ;

  outPath = config->GetValue("outPath","") ;
  outFileName = "SimulateTimeCalib.root" ;
  outFile = new TFile((outPath+"/"+outFileName).c_str(),"RECREATE") ;
  cout<<"outFile: "<<outFile->GetName()<<endl ;
  outTree = new TTree("TimeCalib","time calib information") ;
  outTree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/I:DistanceToPMT/F:TimeCalib/F");
  outTree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/I:DistanceToPMT/F:TimeCalib/F");

  DownFitParas = new TMatrixF(6, 7) ;
  FitParas = new TMatrixF(12, 7) ;

  for(int i=0 ; i<6 ; i++)
  {
    DownCalibToDistance[i] = new TH1F(TString::Format("DownCalibToDistance%i",i),"",62,0,124) ;
    for(int j=0 ; j<62 ; j++)
      DownCalinBin[i][j] = new TH1F(TString::Format("DownCalibBin%i_%i",i,j),"",200,-100000,100000) ;
  }
  for(int i=0 ; i<12 ; i++)
  {
    CalibToDistance[i] = new TH1F(TString::Format("CalibToDistance%i",i),"",62,0,124) ;
    for(int j=0 ; j<62 ; j++)
      CalinBin[i][j] = new TH1F(TString::Format("CalibBin%i_%i",i,j),"",200,-100000,100000) ;
  }
}
void SimulateTimeCalib::GetDownFitParas()
{
  cout<<"Entries of inTree: "<<inTree->GetEntries()<<endl ;
  for(int n=0 ; n<inTree->GetEntries() ; n++)
  {
    inTree->GetEntry(n) ;
    int index = int(Down_Scin.DistanceToPMT/2.) ;
    DownCalinBin[Down_Scin.index-6][index]->Fill(Down_Scin.TimeCalib) ;
  }
  for(int i=6 ; i<12 ; i++)
  {
    for(int j=0 ; j<62 ; j++)
    {
      TF1* gausfunc = new TF1("gausfunc", "gaus", DownCalinBin[i-6][j]->GetMean()-4*DownCalinBin[i-6][j]->GetRMS(),DownCalinBin[i-6][j]->GetMean()+4*DownCalinBin[i-6][j]->GetRMS()) ;
      //if(DownCalinBin[i-6][j]->GetEntries()>=15)
      //{
      //  DownCalinBin[i-6][j]->Fit("gausfunc", "RQ") ;
        //DownCalibToDistance[i-6]->SetBinContent(j+1,DownCalinBin[i-6][j]->GetMean()) ;
      //  DownCalibToDistance[i-6]->SetBinContent(j+1,gausfunc->GetParameter(1)) ;
      //}
      //else
        DownCalibToDistance[i-6]->SetBinContent(j+1,DownCalinBin[i-6][j]->GetMean()) ;
      DownCalinBin[i-6][j]->Write() ;
      delete gausfunc ;
    }
    DownCalibToDistance[i-6]->Write() ;
  }
  TF1 *function = new TF1("function","pol6",10.,124.) ;
  for(int i=0 ; i<6 ; i++)
  {
    DownCalibToDistance[i]->Fit(function,"RQ") ;
    (*DownFitParas)[i][0] = function->GetParameter(0) ;
    (*DownFitParas)[i][1] = function->GetParameter(1) ;
    (*DownFitParas)[i][2] = function->GetParameter(2) ;
    (*DownFitParas)[i][3] = function->GetParameter(3) ;
    (*DownFitParas)[i][4] = function->GetParameter(4) ;
    (*DownFitParas)[i][5] = function->GetParameter(5) ;
    (*DownFitParas)[i][6] = function->GetParameter(6) ;
    DownCalibToDistance[i]->Write() ;
  }
  
  delete function ;
}
void SimulateTimeCalib::GetCalib()
{
  float Distance = sqrt(pow(Down_Scin.Scin_Coor.x - Up_Scin.Scin_Coor.x, 2) + pow(Down_Scin.Scin_Coor.y - Up_Scin.Scin_Coor.y, 2) + pow(320.5, 2)) ;
  int  tof = (int)1000*Distance/29.8 ;
  Up_Scin.TimeCalib = Up_Scin.Measure_Time + tof - Down_Scin.Measure_Time 
  + ((*DownFitParas)[Down_Scin.index-6][0] 
  + (*DownFitParas)[Down_Scin.index-6][1]*Down_Scin.DistanceToPMT
  + (*DownFitParas)[Down_Scin.index-6][2]*pow(Down_Scin.DistanceToPMT,2)
  + (*DownFitParas)[Down_Scin.index-6][3]*pow(Down_Scin.DistanceToPMT,3)
  + (*DownFitParas)[Down_Scin.index-6][4]*pow(Down_Scin.DistanceToPMT,4)
  + (*DownFitParas)[Down_Scin.index-6][5]*pow(Down_Scin.DistanceToPMT,5)
  + (*DownFitParas)[Down_Scin.index-6][6]*pow(Down_Scin.DistanceToPMT,6)) ;

  Down_Scin.TimeCalib = Down_Scin.Measure_Time - Up_Scin.Measure_Time - tof + Up_Scin.TimeCalib ;
}
void SimulateTimeCalib::Simulate()
{
  GetDownFitParas() ;
  int nEntries = SinTree->GetEntries() ;
  for(int i=0 ; i<nEntries; i++)
  {
    if(i%100000==0)
      cout<<i<<endl ;
    SinTree->GetEntry(i) ;
    GetCalib() ;
    outTree->Fill() ;
  }

  for(int n=0 ; n<outTree->GetEntries() ; n++)
  {
    if(n%100000==0)
      cout<<n<<endl ;
    outTree->GetEntry(n) ;
    int upindex = int(Up_Scin.DistanceToPMT/2.) ;
    CalinBin[Up_Scin.index][upindex]->Fill(Up_Scin.TimeCalib) ;
    int downindex = int(Down_Scin.DistanceToPMT/2.) ;
    CalinBin[Down_Scin.index][downindex]->Fill(Down_Scin.TimeCalib) ;
  }

  for(int i=0 ; i<6 ; i++)
  {
    for(int j=0 ; j<62 ; j++)
    {
      //outTree->Draw(TString::Format("Up_Scin.TimeCalib>>CalibBin%i_%i",i,j), TString::Format("Up_Scin.index==%i && Up_Scin.DistanceToPMT>%i*2 && Up_Scin.DistanceToPMT<=%i*2",i,j,j+1)) ;
      //outTree->Project(TString::Format("CalibBin%i_%i",i,j),TString::Format("Up_Scin.TimeCalib"),TString::Format("Up_Scin.index==%i && Up_Scin.DistanceToPMT>%i*2 && Up_Scin.DistanceToPMT<=%i*2",i,j,j+1)) ;
      if(CalinBin[i][j]->GetEntries()>0)
      {
        TF1* gausfunc = new TF1("gausfunc", "gaus", CalinBin[i][j]->GetMean()-CalinBin[i][j]->GetRMS(),CalinBin[i][j]->GetMean()+CalinBin[i][j]->GetRMS()) ;
        CalinBin[i][j]->Fit("gausfunc", "RQ") ;
        CalibToDistance[i]->SetBinContent(j+1,gausfunc->GetParameter(1)) ;
      }
      else
        CalibToDistance[i]->SetBinContent(j+1,CalinBin[i][j]->GetMean()) ;
      CalinBin[i][j]->Write() ;
    }
    CalibToDistance[i]->Write() ;
  }
  for(int i=6 ; i<12 ; i++)
  {
    for(int j=0 ; j<62 ; j++)
    {
      CalibToDistance[i]->SetBinContent(j+1,CalinBin[i][j]->GetMean()) ;
      CalinBin[i][j]->Write() ;
    }
    CalibToDistance[i]->Write() ;
  }

  /*for(int i=6 ; i<12 ; i++)
  {
    for(int j=0 ; j<62 ; j++)
    {
      //outTree->Draw(TString::Format("Down_Scin.TimeCalib>>CalibBin%i_%i",i,j),TString::Format("Down_Scin.index==%i && Down_Scin.DistanceToPMT>%i*2 && Down_Scin.DistanceToPMT<=%i*2",i,j,j+1));
      //outTree->Project(TString::Format("CalibBin%i_%i",i,j),TString::Format("Down_Scin.TimeCalib"), TString::Format("Down_Scin.index==%i && Down_Scin.DistanceToPMT>%i*2 && Down_Scin.DistanceToPMT<=%i*2",i,j,j+1));
      //TF1* gausfunc = new TF1("gausfunc", "gaus", CalinBin[i][j]->GetMean()-4*CalinBin[i][j]->GetRMS(),CalinBin[i][j]->GetMean()+4*CalinBin[i][j]->GetRMS()) ;
      //CalinBin[i][j]->Fit("gausfunc", "R") ;
      //CalibToDistance[i]->SetBinContent(j+1,gausfunc->GetParameter(2)) ;
      CalibToDistance[i]->SetBinContent(j+1,CalinBin[i][j]->GetMean()) ;
      CalinBin[i][j]->Write() ;
      //delete gausfunc ;
    }
    CalibToDistance[i]->Write() ;
  }*/

  ofstream OutFile ;
  OutFile.open((inPath+"/simutimecalib").c_str()) ;

  TF1 *function = new TF1("function","pol6",10.,124.) ;
  for(int i=0 ; i<12 ; i++)
  {
    CalibToDistance[i]->Fit(function,"RQ") ;
    //cout<<function->GetParameter(0)<<" "<<function->GetParameter(1)<<"	"<<function->GetParameter(2)<<"	"<<function->GetParameter(3)<<"	"<<function->GetParameter(4)<<"	"<<function->GetParameter(5)<<" "<<function->GetParameter(6)<<endl ;
    OutFile<<function->GetParameter(0)<<"	"<<function->GetParameter(1)<<"	"<<function->GetParameter(2)<<"	"<<function->GetParameter(3)<<"	"<<function->GetParameter(4)<<"	"<<function->GetParameter(5)<<"	"<<function->GetParameter(6)<<" " ;
    (*FitParas)[i][0] = function->GetParameter(0) ;
    (*FitParas)[i][1] = function->GetParameter(1) ;
    (*FitParas)[i][2] = function->GetParameter(2) ;
    (*FitParas)[i][3] = function->GetParameter(3) ;
    (*FitParas)[i][4] = function->GetParameter(4) ;
    (*FitParas)[i][5] = function->GetParameter(5) ;
    (*FitParas)[i][6] = function->GetParameter(6) ;
    CalibToDistance[i]->Write() ;
  }
  FitParas->Write("FitParas") ;
  delete function ;

  outFile->Write() ;
  outFile->Close() ;
}
#endif
