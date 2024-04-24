#ifndef CalcuVariables_CXX
#define CalcuVariables_CXX

#include "ProcessCORARS/CalcuVariables.h"

CalcuVariables::CalcuVariables()
{
  inPath = "" ;
  inFileName = "" ;
  inFile = 0 ;
  inTree = 0 ;
  outPath = "" ;
  outFileName = "" ;
  outFile = 0 ;
  outTree = 0 ;
  NEDs = 0 ;
  for(int i=0 ; i<8 ; i++)
    EDName[i] = "" ;
  Initialize() ;
}

CalcuVariables::~CalcuVariables()
{
  delete inTree ;
  delete inFile ;
  delete outTree ;
  delete outFile ;
}

void CalcuVariables::Initialize()
{
  conf = ConfigureClass::Instance() ;

  TSCTXY = conf->GetTSCTXY() ; // Get the x and y of Trigger SCTs

  NEDs = conf->NEDs ;
  for(int i=0 ; i<NEDs ; i++)
    EDName[i] = conf->EDName[i] ;

  inPath = conf->outPath+conf->CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  inFileName = "TGCScin_"+conf->CORARSCollectionTime+".root" ;
  inFile = new TFile((inPath+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"In File: "<<inPath<<inFileName<<" doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  cout<<"In File: "<<inFile->GetName()<<endl ;
  inTree = (TTree*)(inFile->Get("CalcuedTGCScin")) ;
  if(!inTree)
  {
    cout<<"inTree doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  // branches of intree
  inTree->SetBranchAddress("event_id",&event_id) ;
  inTree->SetBranchAddress("EventQuality",&EventQuality) ;
  inTree->SetBranchAddress("Up_TGC",&Up_TGC) ;
  inTree->SetBranchAddress("Down_TGC",&Down_TGC) ;
  inTree->SetBranchAddress("Up_Scin",&Up_Scin_ScinED) ;
  inTree->SetBranchAddress("Down_Scin",&Down_Scin_ScinED) ;
  inTree->SetBranchAddress("KM2ATime",KM2ATime) ;
  inTree->SetBranchAddress("KM2ACharge",KM2ACharge) ;
  inTree->SetBranchAddress("NEDNoiseHit",NEDNoiseHit);

  string timecalibfilename  = conf->TimeCalibrationFile ;
  TFile *timecalibfile = new TFile(timecalibfilename.c_str() ,"READ") ;
  if(!timecalibfile)
  {
    cout<<"Time Calibration File: "<<timecalibfilename<<" doesn't exist!!!"<<endl ;
    exit(0) ;
  }
  for(int i=0 ; i<12 ; i++)
  {
    CalibMean[i] = (TH2F*) timecalibfile->Get(TString::Format("CalibMean%i",i)) ;
    if(!CalibMean[i])
    {
      cout<<"Can't Get Time Calibration of the  "<<i<<"th SCT!!!"<<endl ;
      exit(0) ;
    }
  }

  //
  outPath = conf->outPath+conf->CORARSCollectionTime+"/"  ;
  outFileName = "Stage_Two_"+conf->CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;
  outTree = new TTree("Calcued_Variables","Calculated Variables") ;
  outTree->SetDirectory(outFile) ;
  // branched of ou tree
  outTree->Branch("event_id",&event_id,"event_id/i"); 
  outTree->Branch("EventQuality",&EventQuality,"EventQuality/I"); 
  outTree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F:Calibed_Time/F"); 
  outTree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F:DistanceToPMT/F:TimeCalib/F:Calibed_Time/F"); 
  outTree->Branch("Muon_TOF",&Muon_TOF,"Real_TOF/F:Measure_TOF/F:Delta_TOF/F");
  outTree->Branch("TrackLength",&TrackLength,"TrackLength/F"); 
  outTree->Branch("NEDNoiseHit",&NEDNoiseHit,"NEDNoiseHit[8]/I"); 
  for(int i=0 ; i<NEDs ; i++)
    outTree->Branch(EDName[i].c_str(),ElecDet+i,"x/F:y/F:z/F:Measure_Time/F:Charge/F:ReCon_Time/F:Delta_Time"); 

  for (int i=0 ; i<4 ; i++)
  {
     DeltaTimeUDScins[i] = new TH1F(TString::Format("DeltaTimeUDScins%i",i),"", 100, -10000, 10000) ;
     DeltaTimeUDScins[i]->GetXaxis()->SetTitle("#DeltaT (ps)") ;
     DeltaTimeUDScins[i]->Sumw2() ;
     DeltaTimeUDScins[i]->SetDirectory(0) ;
  }
}
bool CalcuVariables::Calcu_Scin()
{
  Up_Scin.Scin_Coor = Up_Scin_ScinED.Scin_Coor ;
  Up_Scin.index = Up_Scin_ScinED.index ;
  Up_Scin.Measure_Time = Up_Scin_ScinED.Measure_Time ;
  Down_Scin.Scin_Coor = Down_Scin_ScinED.Scin_Coor ;
  Down_Scin.index = Down_Scin_ScinED.index ;
  Down_Scin.Measure_Time = Down_Scin_ScinED.Measure_Time ;
  //Calibed_Time is the time after calibration
  int upxbin = CalibMean[Up_Scin.index]->GetXaxis()->FindBin(Up_Scin.Scin_Coor.x) ;
  int upybin = CalibMean[Up_Scin.index]->GetYaxis()->FindBin(Up_Scin.Scin_Coor.y) ;
  Up_Scin.Calibed_Time = Up_Scin.Measure_Time-CalibMean[Up_Scin.index]->GetBinContent(upxbin, upybin) ;

  int downxbin = CalibMean[Down_Scin.index]->GetXaxis()->FindBin(Down_Scin.Scin_Coor.x) ;
  int downybin = CalibMean[Down_Scin.index]->GetYaxis()->FindBin(Down_Scin.Scin_Coor.y) ;
  Down_Scin.Calibed_Time=Down_Scin.Measure_Time-CalibMean[Down_Scin.index]->GetBinContent(downxbin, downybin) ;
  // Calculate flying of time 
  Muon_TOF.Real_TOF = (1000*sqrt(pow(Up_Scin.Scin_Coor.x-Down_Scin.Scin_Coor.x,2)
                            +pow(Up_Scin.Scin_Coor.y-Down_Scin.Scin_Coor.y,2)
                            +pow(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z,2))/29.8) ;
  Muon_TOF.Measure_TOF = Down_Scin.Calibed_Time - Up_Scin.Calibed_Time ;
  Muon_TOF.Delta_TOF = Muon_TOF.Measure_TOF - Muon_TOF.Real_TOF ;

  Up_Scin.DistanceToPMT = sqrt(pow(Up_Scin.Scin_Coor.x-(TSCTXY.at(Up_Scin.index).first+12.5), 2)+pow(Up_Scin.Scin_Coor.y-TSCTXY.at(Up_Scin.index).second,2)) ;
  Down_Scin.DistanceToPMT = sqrt(pow(Down_Scin.Scin_Coor.x-(TSCTXY.at(Down_Scin.index).first+12.5), 2)+pow(Down_Scin.Scin_Coor.y-TSCTXY.at(Down_Scin.index).second,2)) ;

  if(acos(Up_Scin.Scin_Coor.y/Up_Scin.DistanceToPMT)>=0.9||acos(Down_Scin.Scin_Coor.y/Down_Scin.DistanceToPMT)>=0.9||Up_Scin.DistanceToPMT<=10||Down_Scin.DistanceToPMT<=10)
     return false ;
  return true ;
}

void CalcuVariables::Calcu_ED()
{
  float a =  (Up_TGC.x-Down_TGC.x)/(Up_TGC.z-Down_TGC.z),
           b = (Up_TGC.z*Down_TGC.x-Up_TGC.x*Down_TGC.z)/(Up_TGC.z-Down_TGC.z),
           c =  (Up_TGC.y-Down_TGC.y)/(Up_TGC.z-Down_TGC.z),
           d = (Up_TGC.z*Down_TGC.y-Up_TGC.y*Down_TGC.z)/(Up_TGC.z-Down_TGC.z);
  for(int i=0 ; i<NEDs ; i++)
  {
    ElecDet[i].ED_Coor.z = conf->EDZ[i] ;
    //ElecDet[i].ED_Coor.z = 304.5-26*i ;
    //if(i==0) ElecDet[i].ED_Coor.z = 304.5-26*6;
    //if(i==1) ElecDet[i].ED_Coor.z = 304.5-26*7;
    //if(i==2) ElecDet[i].ED_Coor.z = 304.5-26*2+10;
    //if(i==3) ElecDet[i].ED_Coor.z = 304.5-26*2;
    ElecDet[i].ED_Coor.x = a*ElecDet[i].ED_Coor.z+b ;
    ElecDet[i].ED_Coor.y = c*ElecDet[i].ED_Coor.z+d ;
    ElecDet[i].Measure_Time = KM2ATime[conf->EDV1290NChannel[i]]*1000 ;
    ElecDet[i].Charge = KM2ACharge[conf->EDV792NChannel[i]] ;
    //ElecDet[i].Measure_Time = (int)KM2ATime[8+i]*25 ;
    //ElecDet[i].Charge = KM2ACharge[i] ;
    ElecDet[i].ReCon_Time = Up_Scin.Calibed_Time + ((Down_Scin.Calibed_Time-Up_Scin.Calibed_Time)*(346.5-ElecDet[i].ED_Coor.z)/320.5);
    ElecDet[i].Delta_Time = ElecDet[i].Measure_Time - ElecDet[i].ReCon_Time ;
  }
}
void CalcuVariables::Calcu()
{
  Long64_t nEntries = inTree->GetEntries() ;
  cout<<"Total Events: "<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
  {
    if(i%10000==0)
      cout<<i<<endl ;
    //if(i>100000) break ;
    inTree->GetEntry(i) ;
    if(EventQuality<=0) continue ;
    if(!Calcu_Scin()) continue ;
    TrackLength = sqrt(pow(Up_Scin.Scin_Coor.x-Down_Scin.Scin_Coor.x,2)
                       +pow(Up_Scin.Scin_Coor.y-Down_Scin.Scin_Coor.y,2)
                       +pow(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z,2)) ;
    //if(Down_Scin.Scin_Coor.x>67.5&&Down_Scin.Scin_Coor.x<72.5&&Down_Scin.Scin_Coor.y>65&&Down_Scin.Scin_Coor.y<75)
    //  DeltaTimeUDScins[Up_Scin.index]->Fill(Down_Scin.Calibed_Time-Up_Scin.Calibed_Time-Muon_TOF.Real_TOF) ;
    Calcu_ED() ;
    outTree->Fill() ;
  }
  outFile->Write() ;

  TFile* GlobalInfo = new TFile((outPath+"/GlobalInfo_"+conf->CORARSCollectionTime+".root").c_str(), "Update") ;
  float UpScinIndexs[4] = {0., 1., 2., 3.} ;
  float UpScinIndexErrors[4] = {0., 0., 0., 0.} ;
  float DeltaTimes[4] ;
  float DeltaTimeErrors[4] ;
  for(int i=0 ; i<4 ; i++)
  {
    DeltaTimes[i] = DeltaTimeUDScins[i]->GetMean() ;
    DeltaTimeErrors[i] = DeltaTimeUDScins[i]->GetRMS() ;
    DeltaTimeUDScins[i]->Write() ;
  }
  TGraphErrors* ger = new TGraphErrors(4, UpScinIndexs, DeltaTimes, UpScinIndexErrors, DeltaTimeErrors) ;
  ger->SetMarkerStyle(20) ;
  ger->SetMarkerSize(2) ;
  ger->SetLineWidth(2) ;
  ger->GetXaxis()->SetTitle("Up Scin Index") ;
  ger->GetYaxis()->SetTitle("#DeltaT (ps)") ;
  ger->SetName("DeltaTimeUDScins") ;
  ger->SetTitle("") ;
  ger->Write() ;
  GlobalInfo->Close() ;
}

#endif
