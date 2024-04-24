#include <iostream>
#include <string>
#include <cmath>

#include "Rtypes.h"

#include "atlasstyle/AtlasUtils.h"
#include "atlasstyle/AtlasStyle.h"
#include "atlasstyle/AtlasStyle.C"
#include "atlasstyle/AtlasLabels.h"

#ifdef __CLING__
// these are not headers - do not treat them as such - needed for ROOT6
#include "atlasstyle/AtlasLabels.C"
#include "atlasstyle/AtlasUtils.C"
#endif

#include "TCanvas.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TRandom.h"
#include "TGraphErrors.h"

using namespace std;

void EDExample2D(string ID, string time) 
{ 

  #ifdef __CINT__
    gROOT->LoadMacro("atlasstyle/AtlasLabels.C");
    gROOT->LoadMacro("atlasstyle/AtlasUtils.C");
  #endif

  SetAtlasStyle();
  gStyle->SetPalette(55) ;

  TFile* f = new TFile(("../../outData/"+time+"/AnalysedResult_"+time+".root").c_str(),"read") ;
  TH2F* h_des[4] ;
  TH2F* h_trs[4] ;
  TH2F* h_ers[4] ;
  TH2F* h_spps[4] ;
  TH2F* h_rers[4] ;
  for(int i=0 ; i<4 ; i++)
  {
    h_des[i] = (TH2F*)f->Get(("ED"+ID+string(TString::Format("SCT%iDetEffi2DScan",i+1).Data())).c_str()) ;
    h_des[i]->SetXTitle("X Bin ID(*5cm)");
    h_des[i]->GetXaxis()->CenterTitle();
    h_des[i]->SetYTitle("Y Bin ID(*5cm)");
    h_des[i]->GetYaxis()->CenterTitle();
    h_des[i]->SetZTitle("Detection Efficiency(%)");
    h_des[i]->GetZaxis()->SetRangeUser(80,100) ;
    h_des[i]->GetXaxis()->SetNdivisions(4);
    h_des[i]->GetYaxis()->SetNdivisions(504);
    h_des[i]->GetZaxis()->SetNdivisions(508);
    h_trs[i] = (TH2F*)f->Get(("ED"+ID+string(TString::Format("SCT%iTimeRes2DScan",i+1).Data())).c_str()) ;
    h_trs[i]->SetXTitle("X Bin ID(*5cm)");
    h_trs[i]->GetXaxis()->CenterTitle();
    h_trs[i]->SetYTitle("Y Bin ID(*5cm)");
    h_trs[i]->GetYaxis()->CenterTitle();
    h_trs[i]->SetZTitle("Time Resolution(ns)");
    h_trs[i]->GetZaxis()->SetRangeUser(1.,2.) ;
    h_trs[i]->GetXaxis()->SetNdivisions(4);
    h_trs[i]->GetYaxis()->SetNdivisions(504);
    h_trs[i]->GetZaxis()->SetNdivisions(210);
    h_ers[i] = (TH2F*)f->Get(("ED"+ID+string(TString::Format("SCT%iEnergyRes2DScan",i+1).Data())).c_str()) ;
    h_ers[i]->SetXTitle("X Bin ID(*5cm)");
    h_ers[i]->GetXaxis()->CenterTitle();
    h_ers[i]->SetYTitle("Y Bin ID(*5cm)");
    h_ers[i]->GetYaxis()->CenterTitle();
    h_ers[i]->SetZTitle("Energy Resolution(Count)");
    h_ers[i]->GetZaxis()->SetRangeUser(10, 20) ;
    h_ers[i]->GetXaxis()->SetNdivisions(4);
    h_ers[i]->GetYaxis()->SetNdivisions(504);
    h_ers[i]->GetZaxis()->SetNdivisions(504);
    h_spps[i] = (TH2F*)f->Get(("ED"+ID+string(TString::Format("SCT%iSingleParticlePeak2DScan",i+1).Data())).c_str()) ;
    h_spps[i]->SetXTitle("X Bin ID(*5cm)");
    h_spps[i]->GetXaxis()->CenterTitle();
    h_spps[i]->SetYTitle("Y Bin ID(*5cm)");
    h_spps[i]->GetYaxis()->CenterTitle();
    h_spps[i]->SetZTitle("Single Particle Peak(Count)");
    h_spps[i]->GetZaxis()->SetRangeUser(20, 30) ;
    h_spps[i]->GetXaxis()->SetNdivisions(4);
    h_spps[i]->GetYaxis()->SetNdivisions(504);
    h_spps[i]->GetZaxis()->SetNdivisions(505);
    h_rers[i] = (TH2F*)f->Get(("ED"+ID+string(TString::Format("SCT%iReEnergyRes2DScan",i+1).Data())).c_str()) ;
    h_rers[i]->SetXTitle("X Bin ID(*5cm)");
    h_rers[i]->GetXaxis()->CenterTitle();
    h_rers[i]->SetYTitle("Y Bin ID(*5cm)");
    h_rers[i]->GetYaxis()->CenterTitle();
    h_rers[i]->SetZTitle("Relative Energy Resolution");
    h_rers[i]->GetZaxis()->SetRangeUser(0.5, 0.8) ;
    h_rers[i]->GetXaxis()->SetNdivisions(4);
    h_rers[i]->GetYaxis()->SetNdivisions(504);
    h_rers[i]->GetZaxis()->SetNdivisions(506);
  }
  for(int i=0 ; i<4 ; i++)
  {
    TCanvas* c1 = new TCanvas("c1","", 800, 600);
    h_des[i]->Draw("lego2z");
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iDetectionEfficiency.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iDetectionEfficiency.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iDetectionEfficiency.png",i+1).Data())).c_str());
    //delete c1 ;
    h_trs[i]->Draw("lego2z");
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iTimeResolution.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iTimeResolution.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iTimeResolution.png",i+1).Data())).c_str());
    h_ers[i]->Draw("lego2z");
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iEnergyResolution.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iEnergyResolution.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iEnergyResolution.png",i+1).Data())).c_str());
    h_rers[i]->Draw("lego2z");
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iReEnergyResolution.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iReEnergyResolution.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iReEnergyResolution.png",i+1).Data())).c_str());
    h_spps[i]->Draw("lego2z");
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iSingleParticlePeak.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iSingleParticlePeak.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iSingleParticlePeak.png",i+1).Data())).c_str());
    delete c1 ;
  }
}

void EDExample1D(string ID, string time) 
{ 

  #ifdef __CINT__
    gROOT->LoadMacro("atlasstyle/AtlasLabels.C");
    gROOT->LoadMacro("atlasstyle/AtlasUtils.C");
  #endif

  SetAtlasStyle();
  gStyle->SetPalette(55) ;

  TFile* f = new TFile(("../../outData/"+time+"/AnalysedResult_"+time+".root").c_str(),"read") ;
  TH1F* h_des[4] ;
  TH1F* h_trs[4] ;
  TH1F* h_ers[4] ;
  TH1F* h_spps[4] ;
  TH1F* h_rers[4] ;
  for(int i=0 ; i<4 ; i++)
  {
    h_des[i] = (TH1F*)f->Get(("ED"+ID+string(TString::Format("SCT%iDetEffi1DScan",i+1).Data())).c_str()) ;
    h_des[i]->SetYTitle("Detection Efficiency(%)");
    h_des[i]->GetYaxis()->SetRangeUser(80,100) ;
    h_des[i]->GetXaxis()->SetNdivisions(1904);
    h_des[i]->GetYaxis()->SetNdivisions(508);
    h_trs[i] = (TH1F*)f->Get(("ED"+ID+string(TString::Format("SCT%iTimeRes1DScan",i+1).Data())).c_str()) ;
    h_trs[i]->SetYTitle("Time Resolution(ns)");
    h_trs[i]->GetYaxis()->SetRangeUser(1.,2.) ;
    h_trs[i]->GetXaxis()->SetNdivisions(1904);
    h_trs[i]->GetYaxis()->SetNdivisions(210);
    h_ers[i] = (TH1F*)f->Get(("ED"+ID+string(TString::Format("SCT%iEnergyRes1DScan",i+1).Data())).c_str()) ;
    h_ers[i]->SetYTitle("Energy Resolution(Count)");
    h_ers[i]->GetYaxis()->SetRangeUser(10, 20) ;
    h_ers[i]->GetXaxis()->SetNdivisions(1904);
    h_ers[i]->GetYaxis()->SetNdivisions(504);
    h_spps[i] = (TH1F*)f->Get(("ED"+ID+string(TString::Format("SCT%iSingleParticlePeak1DScan",i+1).Data())).c_str()) ;
    h_spps[i]->SetYTitle("Single Particle Peak(Count)");
    h_spps[i]->GetYaxis()->SetRangeUser(20, 30) ;
    h_spps[i]->GetXaxis()->SetNdivisions(1904);
    h_spps[i]->GetYaxis()->SetNdivisions(505);
    h_rers[i] = (TH1F*)f->Get(("ED"+ID+string(TString::Format("SCT%iReEnergyRes1DScan",i+1).Data())).c_str()) ;
    h_rers[i]->SetYTitle("Relative Energy Resolution");
    h_rers[i]->GetZaxis()->SetRangeUser(0.5, 0.8) ;
    h_rers[i]->GetXaxis()->SetNdivisions(1904);
    h_rers[i]->GetYaxis()->SetNdivisions(506);
  }
  for(int i=0 ; i<4 ; i++)
  {
    TCanvas* c1 = new TCanvas("c1","", 800, 600);
    h_des[i]->Draw();
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iDetectionEfficiency1D.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iDetectionEfficiency1D.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iDetectionEfficiency1D.png",i+1).Data())).c_str());
    //delete c1 ;
    h_trs[i]->Draw();
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iTimeResolution1D.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iTimeResolution1D.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iTimeResolution1D.png",i+1).Data())).c_str());
    h_ers[i]->Draw();
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iEnergyResolution1D.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iEnergyResolution1D.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iEnergyResolution1D.png",i+1).Data())).c_str());
    h_rers[i]->Draw();
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iReEnergyResolution1D.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iReEnergyResolution1D.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iReEnergyResolution1D.png",i+1).Data())).c_str());
    h_spps[i]->Draw();
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iSingleParticlePeak1D.eps",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iSingleParticlePeak1D.pdf",i+1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iSingleParticlePeak1D.png",i+1).Data())).c_str());
    delete c1 ;
  }
}

void DrawDeltaTime(string ID , string time)
{
  #ifdef __CINT__
    gROOT->LoadMacro("atlasstyle/AtlasLabels.C");
    gROOT->LoadMacro("atlasstyle/AtlasUtils.C");
  #endif

  SetAtlasStyle();
  gStyle->SetPalette(55) ;

  TFile* f = new TFile(("../../outData/"+time+"/AnalysedResult_"+time+".root").c_str(),"read") ;
  TH1F* h_time[4] ;
  for(int i=0 ; i<4 ; i++)
  {
    h_time[i] = (TH1F*) f->Get(("ED"+ID+string(TString::Format("SCT%iCorrectedDeltaTime",i+1).Data())).c_str()) ;
    h_time[i]->GetXaxis()->SetRangeUser(-10000,10000);
    h_time[i]->GetXaxis()->SetNdivisions(508);
    TCanvas* c1 = new TCanvas("c1","", 800, 600);
    h_time[i]->Draw() ;
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iCorrectedDeltaTime.png",1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iCorrectedDeltaTime.eps",1).Data())).c_str());
    c1->Print(("../../outData/plots/ED"+ID+"-"+time+string(TString::Format("-SCT%iCorrectedDeltaTime.pdf",i+1).Data())).c_str());
  }
}
