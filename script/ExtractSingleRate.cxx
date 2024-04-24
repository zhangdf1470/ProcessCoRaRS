#include <iostream>
#include <string>
//#include "/hep/home/zhangdf/ToolScript/GraphToolKit.h"

using namespace std;
int ExtractSingleRate()
{
  //string collectiontime = "20171029161741" ;
  //string collectiontime = "20171029204130" ;
  //string collectiontime = "20171030175218" ;
  //string collectiontime = "20171030082003" ;
  //string collectiontime = "20171031082112" ;
  //string collectiontime = "20171031143905" ;
  //string collectiontime = "20171031203139" ;
  //string collectiontime = "20171101090638" ;
  //string collectiontime = "20171101161808" ;
  //string collectiontime = "20171102083407" ;
  //string collectiontime = "20171102202853" ;
  //string collectiontime = "20171106203704" ;
  //string collectiontime = "20171107211537" ;
  //string collectiontime = "20171109101128" ;
  //string collectiontime = "20180109220624" ;
  //string collectiontime = "20180109220624" ;
  string collectiontime = "20180111150111" ;

  //string inDAQFile = "/hepdata/d3/dengfengz/CORARS/InPut_Data/PN-ReadoutED-1-"+collectiontime+".out" ;
  string inDAQFile = "/tmp/PN-ReadoutED-1-"+collectiontime+".out" ;
  string grepoutFile = "SingleRate-grep-"+collectiontime+".txt" ;
  string awkoutFile = "SingleRate-awk-"+collectiontime+".txt" ;
  string finaloutFile = "SingleRate-final-"+collectiontime+".txt" ;
  //string PadID = "218" ;
  string PadID = "49" ;
  //string command = "cat "+inDAQFile+" | grep \"HIT Rate\" "+" | grep "+PadID+">> "+outFile ;
  string grepcommand = "cat "+inDAQFile+" | grep \"HIT RATE\""+"| grep "+PadID +" | grep KHz | grep -v TS | grep -v DEBUG "+" >"+grepoutFile;
  gSystem->Exec(grepcommand.c_str()) ;
  string awkcommand = "awk \' $4=="+PadID+" { gsub(\"KHz,\",\"\",$3); print  $3 }\' "+ grepoutFile+"> "+awkoutFile;
  gSystem->Exec(awkcommand.c_str()) ;
  string finalcommand = "cat "+awkoutFile+" | grep -v HIT | grep -v RATE | grep -v , > "+finaloutFile ;
  gSystem->Exec(finalcommand.c_str()) ;

  string rootfile = "SingleRate-ED"+PadID+"-"+collectiontime+".root" ;
  TFile* f = new TFile(rootfile.c_str(), "recreate") ;
  cout<<"out File: "<<f->GetName()<<endl ;
  TTree* tree = new TTree("tree", "") ;
  tree->SetDirectory(0) ;
  tree->ReadFile(finaloutFile.c_str(), "singlerate") ;
  int id = 0 ;
  float singlerate = 0. ;
  tree->SetBranchAddress("singlerate", &singlerate) ;
  TTree* outtree = new TTree("outtree", "") ;
  outtree->Branch("id", &id, "id/I") ;
  outtree->Branch("singlerate", &singlerate, "singlerate/F") ;
  int N = tree->GetEntries() ;
  for(int n=0 ; n<N ; n++)
  {
    tree->GetEntry(n) ;
    id = n ;
    if(singlerate>10)
    {
      cout<<id<<endl ;
      continue ;
    }
    outtree->Fill() ;
  }
  outtree->Draw("singlerate:id") ;
  TGraph* gr = (TGraph*) ((TGraph*)gPad->GetPrimitive("Graph"))->Clone(); 
  gr->SetName("SingleRateGraph") ;
  gr->SetTitle(("SingleRate ED"+PadID).c_str()) ;
  gr->GetXaxis()->SetTitle("event id") ;
  gr->GetYaxis()->SetTitle("SingleRate (kHz)") ;
  gr->SetMarkerSize(4) ;
  TCanvas* c = new TCanvas("c", "", 800, 600) ;
  gr->Draw("ap") ;
  c->Print(("ED"+PadID+"SingleRate"+collectiontime+".eps").c_str()) ;
  c->Print(("ED"+PadID+"SingleRate"+collectiontime+".pdf").c_str()) ;
  gr->Write() ;
  f->Write() ;

  gSystem->Exec(("rm "+grepoutFile).c_str()) ;
  gSystem->Exec(("rm "+awkoutFile).c_str()) ;
  gSystem->Exec(("rm "+finaloutFile).c_str()) ;
  return 0 ;
}
