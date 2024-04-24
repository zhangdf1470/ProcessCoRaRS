#include "ProcessCORARS/PedestalDrift.h"

PedestalDrift::PedestalDrift(){}
PedestalDrift::~PedestalDrift(){}

void PedestalDrift::Initialize()
{
  conf = ConfigureClass::Instance() ;
  //InPut_File = new TFile((conf->InFileName).c_str(),"READ") ;
  InPut_Tree = (TTree*)InPut_File->Get("UsedVariables") ;
  //OutPut_File = new TFile((conf->OutFileName).c_str(),"recreate") ;
  cout<<"InFile Name: "<<InPut_File->GetName()<<endl ;
  cout<<"OutFile Name: "<<OutPut_File->GetName()<<endl ;
}
void PedestalDrift::GetDrift()
{
  Initialize() ; 
  InPut_Tree->SetBranchAddress("event_time",&event_time);
  InPut_Tree->SetBranchAddress("event_id",&event_id) ; 
  long int nEntries = InPut_Tree->GetEntries() ;
  InPut_Tree->GetEntry(0) ;
  long int initial_time = (long int)event_time ;
  InPut_Tree->GetEntry(nEntries-1) ;
  long int final_time = (long int)event_time ;
  NtimeBin = (final_time-initial_time)/1000+1 ;
  cout<<final_time-initial_time<<endl ;
  cout<<"*****Note: take down NtimeBin for next step to calibriate charge!*****"<<endl ;
  cout<<"Number of Pedestals: "<<NtimeBin<<endl ;
  TH1F *Pedestal_Drift[8] ;
  TH1F * Charge[8][NtimeBin] ;
  ofstream OutFile ;
  OutFile.open("./Config/pedestaldrift") ;
  for(int i=0 ; i<8 ; i++)
  {
    Pedestal_Drift[i] = new TH1F(TString::Format("Pedestal_Drift%i",i),"", NtimeBin, 0, NtimeBin) ;
    for(int j=0 ; j<NtimeBin ; j++)
      Charge[i][j] = new TH1F(TString::Format("Charge_Bin%i_%i",i,j),"",1000,0,1000) ;
  }
  for(int i=0 ; i<8 ; i++)
  {
    for(int j=0 ; j<NtimeBin ; j++)
    { 
      InPut_Tree->Project(TString::Format("Charge_Bin%i_%i",i,j),TString::Format("ED_Charge_Signal[%i]",i),TString::Format("Scin_ED_Time_Signal[%i]==0&&event_time>=(%li+%i*1000)&&event_time<(%li+%i*1000)",i+8,initial_time,j,initial_time,j+1)) ;
      //if(i==1)
      TF1* func = new TF1("func","gaus",Charge[i][j]->GetMean()+5, Charge[i][j]->GetMean()-5) ;
      //TF1* func = new TF1("func","gaus",Charge[i][j]->GetBinLowEdge(Charge[i][j]->GetMaximumBin())-10,Charge[i][j]->GetBinLowEdge(Charge[i][j]->GetMaximumBin())+10) ;
      //else
      //  TF1* func = new TF1("func","gaus",Charge[i][j]->GetBinLowEdge(Charge[i][j]->GetMaximumBin())-2*Charge[i][j]->GetRMS(),Charge[i][j]->GetBinLowEdge(Charge[i][j]->GetMaximumBin())+2*Charge[i][j]->GetRMS()) ;
      func->SetLineColor(kRed) ;
      Charge[i][j]->Fit(func,"R") ;
      Pedestal_Drift[i]->SetBinContent(j+1,func->GetParameter(1)) ;
      OutFile<<func->GetParameter(1)<<" " ;
      cout<<i<<"	"<<j<<endl ;
      Charge[i][j]->Write() ;
      delete func ;
    }
    Pedestal_Drift[i]->Write() ;
  }
  OutFile.close() ;
  OutPut_File->Write() ;
}
