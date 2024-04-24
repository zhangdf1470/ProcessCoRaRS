// ##########################################
//          Author: Dengfeng Zhang
//          eMail: zhangdf1470@aliyun.com
// ##########################################
// std include
#ifndef __HistToolKit__H
#define __HistToolKit__H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// local include 
#include "SetAttributes.h"
// root includes
#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TROOT.h"
// if want to call following functions, do following
// from ROOT import *; gROOT.ProcessLine("#include \"HistToolKit.h\"")
// define some functions in head
using namespace std;
static int DrawLabels(float xstart, float ystart, std::string label)
{
  // (xstart, ystart) the starting coordinate of the latex
  TLatex m_latex;
  m_latex.SetTextSize(0.04) ;
  m_latex.SetTextColor(kBlack) ;
  m_latex.SetTextFont(72) ;
  // Draw ATLAS lebel
  m_latex.DrawLatex(xstart, ystart, label.c_str()) ;
  return 0 ;
}
static int DrawATLASLabels(float xstart, float ystart, int labeltype)
{
  // (xstart, ystart) the starting coordinate of the latex
  // define tlatex
  TLatex m_latex;
  m_latex.SetTextSize(0.03) ;
  m_latex.SetTextColor(kBlack) ;
  m_latex.SetTextFont(72) ;
  // Draw ATLAS lebel
  m_latex.DrawLatex(xstart, ystart, "ATLAS") ;

  m_latex.SetTextFont(42) ;
  float spacing = 0.15 ;
  switch(labeltype)
  {
    case 0:
          break ;
    case 1:
          m_latex.DrawLatex(xstart+spacing, ystart, "Preliminary") ;
          break ;
    case 2:
          m_latex.DrawLatex(xstart+spacing, ystart, "Internal") ;
          break ;
    case 3:
          m_latex.DrawLatex(xstart+spacing, ystart, "Simulation Preliminary") ;
          break ;
    case 4:
          m_latex.DrawLatex(xstart+spacing, ystart, "Work in Progress") ;
          break ;
   default: break ;
  }
  return 0;
}
static int DrawCMEAndLumi(float xstart, float ystart, std::string lumiInFb, std::string CME)
{
  // (xstart, ystart): coornidate of the leftbottom of the pavetext
  TPaveText *pavetext =new TPaveText() ;
  pavetext->SetOption("brNDC") ;
  pavetext->SetFillColor(0) ;
  pavetext->SetFillStyle(1001) ;
  pavetext->SetBorderSize(0) ;
  pavetext->SetTextAlign(11) ;
  pavetext->SetTextFont(42) ;
  pavetext->SetTextSize(0.03) ;
  pavetext->SetTextColor(kBlack) ;
  pavetext->SetX1NDC(xstart-0.01) ;
  pavetext->SetY1NDC(ystart-0.01) ;
  pavetext->SetX2NDC(xstart+0.25) ;
  pavetext->SetY2NDC(ystart+0.05) ;

  pavetext->AddText(0.04, 1./8., ("#sqrt{s}="+CME+", #intLdt="+lumiInFb+"fb^{-1}").c_str()) ;
  pavetext->Draw() ;

  return  0;
}
static bool sort_by_histmax(TH1F* hist1, TH1F* hist2)
{
  return hist1->GetMaximum()>hist2->GetMaximum() ;
}
static bool sort_by_vechistmax(std::pair<TH1F*, bool> p1, std::pair<TH1F*, bool> p2)
{
  return p1.first->GetMaximum()>p2.first->GetMaximum() ;
}

static TH1F* bookTH1F(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, double xlow, double xhigh,
               bool sumw2 = true)
{
  TH1F* tmp = new TH1F( name.c_str(), title.c_str(), xbins, xlow, xhigh);
  tmp->GetXaxis()->SetTitle(xlabel.c_str()) ;
  tmp->GetYaxis()->SetTitle(ylabel.c_str()) ;
  //if(sumw2) tmp->Sumw2() ;
  tmp->StatOverflows(false) ;
  return tmp ;
}
static TH2F* bookTH2F(std::string name, std::string title,
               std::string xlabel,std::string ylabel, std::string zlabel,
               int xbins, double xlow, double xhigh, int ybins, double ylow, double yhigh,
               bool sumw2 = true)
{
  TH2F* tmp = new TH2F( name.c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh);
  tmp->GetXaxis()->SetTitle(xlabel.c_str()) ;
  tmp->GetYaxis()->SetTitle(ylabel.c_str()) ;
  tmp->GetZaxis()->SetTitle(zlabel.c_str()) ;
  //if(sumw2) tmp->Sumw2() ;
  tmp->StatOverflows() ;
  return tmp ;
}
static TH1F* bookTH1F(string name, string title,
               string xlabel,string ylabel,
               int nbins, float *bininterval,
               bool sumw2 = true)
{
  TH1F* tmp = new TH1F( name.c_str(), title.c_str(), nbins, bininterval);
  tmp->GetXaxis()->SetTitle(xlabel.c_str()) ;
  tmp->GetYaxis()->SetTitle(ylabel.c_str()) ;
  //if(sumw2) tmp->Sumw2() ;
  tmp->StatOverflows() ;
  return tmp ;
}

static TH1F* bookTH1F(string name, string title,
               string xlabel,string ylabel,
               TH1F* hist,
               bool sumw2 = true)
{
  TH1F* tmp = new TH1F( name.c_str(), title.c_str(), hist->GetNbinsX(), hist->GetXaxis()->GetXbins()->GetArray());
  tmp->GetXaxis()->SetTitle(xlabel.c_str()) ;
  tmp->GetYaxis()->SetTitle(ylabel.c_str()) ;
  //if(sumw2) tmp->Sumw2() ;
  tmp->StatOverflows() ;
  return tmp ;
}

static TH1I* bookTH1I(std::string name, std::string title,
               std::string xlabel,std::string ylabel,
               int xbins, int xlow, int xhigh,
               bool sumw2 = true)
{
  TH1I* tmp = new TH1I( name.c_str(), title.c_str(), xbins, xlow, xhigh);
  tmp->GetXaxis()->SetTitle(xlabel.c_str()) ;
  tmp->GetYaxis()->SetTitle(ylabel.c_str()) ;
  //if(sumw2) tmp->Sumw2() ;
  tmp->StatOverflows() ;
  return tmp ;
}

// Get Effective Histogram
static TH1F* getEffectiveEntriesHistogram(TH1F* hist, string name = "hee")
{
  std::cout<<"Get Effective Entries."<<std::endl ;
  TH1F *hee = new TH1F(name.c_str(), name.c_str(), hist->GetXaxis()->GetNbins(), 
                       hist->GetXaxis()->GetXbins()->GetArray()) ;
  hee->SetDirectory(0) ;
  for(int bin=1 ; bin<hist->GetNbinsX()+1 ; bin++)
  {
    float nee ;
    if (hist->GetBinError(bin) != 0.)
        nee = pow(hist->GetBinContent(bin),2)/pow(hist->GetBinError(bin),2) ;
    else
        nee = 0. ;
    hee->SetBinContent(bin, nee);
    hee->SetBinError(bin, sqrt(nee)) ;
  }
  return hee ;
}
// Get DataLike Histogram
static TH1F* getDataLikeHist(TH1F* eff, TH1F* scaled, string name, int jobSeed = 10 )
{
  std::cout<<"Get DataLike Entries."<<std::endl ;
  TH1F *dataLike = new TH1F(name.c_str(), name.c_str(), eff->GetXaxis()->GetNbins(),
                            eff->GetXaxis()->GetXbins()->GetArray()) ;
  dataLike->SetDirectory(0) ;
  //random number generator
  TRandom3 rand3(1986) ; //1986 #ORIGINAL

  //loop over bins
  for( int bin=1 ; bin<eff->GetNbinsX()+1 ; bin++)
  {
    //enough effective entries?
    float nee = eff->GetBinContent(bin) ;
    if(nee>=scaled->GetBinContent(bin))
    {
      //set seed
      //NOTE the seed for each bin must be always the same
      int binSeed = int(eff->GetBinCenter(bin) + jobSeed*1e5 );
      rand3.SetSeed(binSeed);
      //get data-like bin content by drawing entries
      //NOTE weights are poissonian by construction
      for( int jj=1 ; jj<=int(nee) ; jj++)
        if(rand3.Uniform()<(float)scaled->GetBinContent(bin)/nee)
          dataLike->Fill(dataLike->GetBinCenter(bin)) ;
    }
  }
  return dataLike ;
}


// Get map from binary file
static int GetNumberOfEvents(std::string filename, std::map<string, int> *channelvsbumber)
{
  ifstream fileIn(filename.c_str());
  std::string line;
  std::string subStr;
  string channel ;
  int number ;
  while (getline(fileIn, line))
  {
    istringstream iss(line);
    iss >> channel ;
    if( channel.find_first_of('#')==0 )
      continue ;
    iss >> subStr;
    iss >> subStr;
    iss >> subStr;
    sscanf(subStr.c_str(), "%i", &number);
    (*channelvsbumber)[channel]=number ;
    cout << "channelvsbumber[" << channel << "]: " << (*channelvsbumber)[channel] << endl;
  }
  return 0 ;
}

static int DrawSingleHistOnCanvas(string canvasname, TH1F* hist, bool DrawHistorNot=false, bool logx=false, bool logy=true, bool isrectangle=true)
{
  //gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  float height=600, width=600 ;
  if(isrectangle)
    width=800 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  //SetPadAttributes(outpad, 4000, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  outpad->SetFillStyle(4000) ;
  TPad *interpad = new TPad("interpad","interpad",0,0,1,1) ;// For main histo
  SetPadAttributes(interpad, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;

  interpad->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  //DrawATLASLabels(0.5, 0.8, 1) ;
  // draw center of mass energy and integrated luminosity
  //DrawCMEAndLumi(0.5, 0.7, "XX", "13 TeV") ;
  interpad->cd() ;
  // Set histogram attributes
  SetHistAttributes(hist, 1, 2, 1, 1001, 0, 20, 1, 1, 0.04, 1.2, 0.04, 0.04, 1.2, 0.04) ;
  // draw histogram using option "HIST" or not
  if(DrawHistorNot)
    hist->Draw("HIST") ;
  else
    hist->Draw("PE") ;
  c->Update() ;
  c->Print((canvasname+".eps").c_str()) ;

  return 0 ;
}

static int DrawSingle2DHistInOneCanvas(string canvasname, TH2F* hist, bool DrawLegoorNot=true, bool logx=false, bool logy=false, bool logz=false, bool isrectangle=true)
{
  //gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  hist->SetStats(0) ;
  // define Canvas
  float height=600, width=600 ;
  if(isrectangle)
    width=800 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;
  
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  //SetPadAttributes(outpad, 4000, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  outpad->SetFillStyle(4000) ;
  //TPad *interpad = new TPad("interpad","interpad",0,0,1,1) ;// For main histo
  //SetPadAttributes(interpad, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;

  //interpad->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  //DrawATLASLabels(0.5, 0.8, 1) ;
  // draw center of mass energy and integrated luminosity
  //DrawCMEAndLumi(0.5, 0.7, "XX", "13 TeV") ;
  //interpad->cd() ;
  // Set histogram attributes
  //SetHistAttributes(hist, 1, 2, 1, 1001, 0, 20, 1, 1, 0.04, 1.2, 0.04, 0.04, 1.2, 0.04) ;
  // draw histogram using option "HIST" or not
  if(DrawLegoorNot)
    hist->Draw("lego2z") ;
  else
    hist->Draw("col2z") ;
  c->Update() ;
  c->Print((canvasname+".eps").c_str()) ;

  return 0 ;
}

static int SetHistDrawOption(TH1F* hist, TLegend *leg, bool DrawHistorNot=false)
{
  if(DrawHistorNot)
  {
    hist->Draw("HISTSAME") ;
    leg->AddEntry(hist, hist->GetTitle(), "L") ;
  }
  else
  {
    hist->Draw("PESAME") ;
    leg->AddEntry(hist, hist->GetTitle(), "LPE") ;
  }
  leg->Draw("same") ;
  return 0 ;
}

static int SetHistDrawOption(std::vector<std::pair<TH1F*, bool> > histvec, TLegend *leg)
{
  string DrawOption1 = "HIST", DrawOption2 = "PE" ;
  for(std::vector< std::pair<TH1F*, bool> >::iterator itr=histvec.begin() ; itr!=histvec.end() ; itr++)
  {
    if(itr->second)
    { 
      itr->first->Draw(DrawOption1.c_str()) ;
      leg->AddEntry(itr->first, itr->first->GetTitle(), "L") ;
    }
    else
    {
      itr->first->Draw(DrawOption2.c_str()) ;
      leg->AddEntry(itr->first, itr->first->GetTitle(), "LPE") ;
    }
    if(DrawOption1.find("SAME")==string::npos)
      DrawOption1 += "SAME" ;
    if(DrawOption2.find("SAME")==string::npos)
      DrawOption2 += "SAME" ;
  }
  leg->Draw("same") ;
  return 0 ;
}
static int DrawTwoHistsInOneCanvas(string canvasname, TH1F* hist1, TH1F* hist2, bool FirstDrawHistorNot=false, bool SecondDrawHistorNot=true, bool logx=false, bool logy=true, bool isrectangle=true)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  float height=600, width=600 ;
  if(isrectangle)
    width=800 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  outpad->SetFillStyle(4000) ;//transparent
  TPad *interpad = new TPad("interpad","interpad",0,0,1,1) ;// For main histo
  SetPadAttributes(interpad, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;

  interpad->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  //DrawATLASLabels(0.5, 0.8, 4) ;
  // draw center of mass energy and integrated luminosity
//  DrawCMEAndLumi(0.5, 0.7, "XX", "13 TeV") ;

  interpad->cd() ;
  // Set histogram attributes
  SetHistAttributes(hist1, 1, 2, 1, 0, 1, 20, 1) ;
  SetHistAttributes(hist2, 1, 2, 2, 0, 1, 22, 2) ;

  // define legend
  TLegend *leg=new TLegend(0.65,0.75,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  // sort the hist by its maximum
  std::vector< std::pair<TH1F*, bool> > histvec ;
  histvec.push_back(std::make_pair(hist1, FirstDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist2, SecondDrawHistorNot)) ;
  std::sort(histvec.begin(), histvec.end(),sort_by_vechistmax) ;
  // draw histograms using option "HIST" or not
  SetHistDrawOption(histvec, leg) ;
  //hist1->Draw() ;
  //hist2->Draw("same") ;
  c->Print((canvasname+".eps").c_str()) ;
  return 0 ;
}

static int DrawThreeHistsInOneCanvas(string canvasname, TH1F* hist1, TH1F* hist2, TH1F* hist3, bool FirstDrawHistorNot=false, bool SecondDrawHistorNot=false, bool ThirdDrawHistorNot=false, bool logx=false, bool logy=true, bool isrectangle=true)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  outpad->SetFillStyle(4000) ;//transparent
  TPad *interpad = new TPad("interpad","interpad",0,0,1,1) ;// For main histo
  SetPadAttributes(interpad, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;

  interpad->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
//  DrawATLASLabels(0.5, 0.8, 4) ;
  // draw center of mass energy and integrated luminosity
//  DrawCMEAndLumi(0.5, 0.7, "XX", "13 TeV") ;

  interpad->cd() ;
  // Set histogram attributes
  SetHistAttributes(hist1, 1, 2, 1, 3004, 1, 20, 1, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist2, 1, 2, 2, 3005, 1, 22, 2, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist3, 1, 2, 3, 3006, 1, 23, 3, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;

  // define legend
  TLegend *leg=new TLegend(0.65,0.75,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  // sort the hist by its maximum
  std::vector< std::pair<TH1F*, bool> > histvec ;
  histvec.push_back(std::make_pair(hist1, FirstDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist2, SecondDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist3, ThirdDrawHistorNot)) ;
  std::sort(histvec.begin(), histvec.end(),sort_by_vechistmax) ;
  // draw histograms using option "HIST" or not
  SetHistDrawOption(histvec, leg) ;
  c->Print((canvasname+".eps").c_str()) ;
  return 0 ;
}

static int DrawFourHistsInOneCanvas(string canvasname, TH1F* hist1, TH1F* hist2, TH1F* hist3, TH1F* hist4, bool FirstDrawHistorNot=false, bool SecondDrawHistorNot=false, bool ThirdDrawHistorNot=false, bool FourthDrawHistorNot=false, bool logx=false, bool logy=true, bool isrectangle=true)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  outpad->SetFillStyle(4000) ;//transparent
  TPad *interpad = new TPad("interpad","interpad",0,0,1,1) ;// For main histo
  SetPadAttributes(interpad, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;

  interpad->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  //Draw ATLAS Label
  //DrawATLASLabels(0.5, 0.8, 4) ;
  //draw center of mass energy and integrated luminosity
  //DrawCMEAndLumi(0.5, 0.7, "XX", "13 TeV") ;

  interpad->cd() ;
  // Set histogram attributes
  SetHistAttributes(hist1, 1, 2, 1, 0, 1, 20, 1, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist2, 1, 2, 2, 0, 1, 22, 2, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist3, 1, 2, 3, 0, 1, 23, 3, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist4, 1, 2, 3, 0, 1, 24, 3, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;

  // define legend
  TLegend *leg=new TLegend(0.65,0.7,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  // sort the hist by its maximum
  std::vector< std::pair<TH1F*, bool> > histvec ;
  histvec.push_back(std::make_pair(hist1, FirstDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist2, SecondDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist3, ThirdDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist4, FourthDrawHistorNot)) ;
  std::sort(histvec.begin(), histvec.end(),sort_by_vechistmax) ;
  
  // draw histograms using option "HIST" or not
  SetHistDrawOption(histvec, leg) ;
  //TPaveText *pt = (TPaveText*)interpad->GetListOfPrimitives()->FindObject("title") ;
  //pt->Clear() ;
  //pt->Draw() ;
  c->Print((canvasname+".eps").c_str()) ;

  return 0 ;
}

static int DrawFiveHistsInOneCanvas(string canvasname, TH1F* hist1, TH1F* hist2, TH1F* hist3, TH1F* hist4, TH1F* hist5, bool FirstDrawHistorNot=false, bool SecondDrawHistorNot=false, bool ThirdDrawHistorNot=false, bool FourthDrawHistorNot=false, bool FifthDrawHistorNot=false, bool logx=false, bool logy=true, bool isrectangle=true)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  outpad->SetFillStyle(4000) ;//transparent
  TPad *interpad = new TPad("interpad","interpad",0,0,1,1) ;// For main histo
  SetPadAttributes(interpad, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;

  interpad->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
//  DrawATLASLabels(0.5, 0.8, 4) ;
  // draw center of mass energy and integrated luminosity
//  DrawCMEAndLumi(0.5, 0.7, "XX", "13 TeV") ;

  interpad->cd() ;
  // Set histogram attributes
  SetHistAttributes(hist1, 1, 2, 1, 0, 1, 20, 1, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist2, 1, 2, 2, 0, 1, 22, 2, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist3, 1, 2, 3, 0, 1, 23, 3, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist4, 1, 2, 4, 0, 1, 24, 4, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;
  SetHistAttributes(hist5, 1, 2, 5, 0, 1, 25, 5, 1, 0.04, 1., 0.04, 0.04, 1.2, 0.04, false) ;

  // define legend
  TLegend *leg=new TLegend(0.65,0.65,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  // sort the hist by its maximum
  std::vector< std::pair<TH1F*, bool> > histvec ;
  histvec.push_back(std::make_pair(hist1, FirstDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist2, SecondDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist3, ThirdDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist4, FourthDrawHistorNot)) ;
  histvec.push_back(std::make_pair(hist5, FifthDrawHistorNot)) ;
  std::sort(histvec.begin(), histvec.end(),sort_by_vechistmax) ;
  // draw histograms using option "HIST" or not
  SetHistDrawOption(histvec, leg) ;
  c->Print((canvasname+".eps").c_str()) ;
  return 0 ;
}

static int DrawTopTwoHistsAndBottomOneHist(string canvasname, TH1F *top1, TH1F *top2, 
    TH1F *bottom1, bool TFirstDrawHistorNot=false, bool TSecondDrawHistorNot=false, bool logx=false, bool toplogy=true, bool bottomlogy=false)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For residuals histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0, 0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  DrawATLASLabels(0.65, 0.75, 4) ;
  // draw center of mass energy and integrated luminosity
  DrawCMEAndLumi(0.65, 0.7, "XX", "13 TeV") ;
 
  pad1->cd() ;
  pad1->SetTickx() ;
  pad1->SetTicky() ;

  // set histogram attributes
  SetHistAttributes(top1, 1, 2, 1, 0, 1, 20, 1, 1, 0.05, 1., 0.05, 0.05, 1., 0.05, false) ;
  SetHistAttributes(top2, 1, 2, 2, 0, 1, 21, 2, 1, 0.05, 1., 0.05, 0.05, 1., 0.05, false) ;
  SetHistAttributes(bottom1, 1, 2, 1, 0, 1, 20, 1, 1, 0.1, 1.0, 0.1, 0.1, 0.5, 0.1, false) ;
  // draw legend
  TLegend *leg=new TLegend(0.65,0.7,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  // sort the top hists by its maximum
  std::vector< std::pair<TH1F*, bool> > histvec ;
  histvec.push_back(std::make_pair(top1, TFirstDrawHistorNot)) ;
  histvec.push_back(std::make_pair(top2, TSecondDrawHistorNot)) ;
  std::sort(histvec.begin(), histvec.end(),sort_by_vechistmax) ;
  // draw histograms using option "HIST" or not
  SetHistDrawOption(histvec, leg) ;

  pad2->cd() ;
  pad2->SetTicky() ;
  // Draw bottom histograms
  bottom1->Draw("EP0") ;
  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawTopTwoHistsAndBottomTwoHists(string canvasname, TH1F *top1, TH1F* top2, 
    TH1F* bottom1, TH1F* bottom2, 
    bool TFirstDrawHistorNot=true, bool TSecondDrawHistorNot=false,
//    bool BFirstDrawHistorNot=true, bool BSecondDrawHistorNot=false, 
    bool logx=false, bool toplogy=true, bool bottomlogy=false)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;

  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For residuals histo

  //outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 4000, 0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0, 0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  DrawATLASLabels(0.5, 0.85, 4) ;
  // draw center of mass energy and integrated luminosity
  DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  pad1->SetTickx() ;
  pad1->SetTicky() ;
  // set top histograms attributes
  SetHistAttributes(top1, 1, 2, 1, 0, 1, 20, 1, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top2, 1, 2, 2, 0, 1, 21, 2, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  // sort the top hists by its maximum
  std::vector< std::pair<TH1F*, bool> > histvec ;
  histvec.push_back(std::make_pair(top1, TFirstDrawHistorNot)) ;
  histvec.push_back(std::make_pair(top2, TSecondDrawHistorNot)) ;
  std::sort(histvec.begin(), histvec.end(),sort_by_vechistmax) ;
  // draw histograms using option "HIST" or not
  SetHistDrawOption(histvec, leg) ;

  pad2->cd() ;
  pad2->SetTicky() ;
  // set bottom histograms attributes
  SetHistAttributes(bottom1, 1, 2, 1, 3004, 1, 20, 1, 1, 0.1, 0.5, 0.1, 0.1, 0.5, 0.1, false) ;
  SetHistAttributes(bottom2, 1, 2, 2, 3005, 1, 21, 2, 1, 0.1, 0.5, 0.1, 0.1, 0.5, 0.1, false) ;
  // Draw bottom histograms
  bottom1->Draw("EP0") ;
  bottom2->Draw("EP0same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;
  return 0;
}

static int DrawTopFourHistsAndBottomTwoHists(string canvasname, TH1F *top1, TH1F* top2, TH1F* top3, TH1F* top4, TH1F* bottom1, TH1F* bottom2, 
    string tdrawopt1="HIST", string tdrawopt2="PESAME",string tdrawopt3="PESAME",string tdrawopt4="PEASAME", string bdrawopt1="HIST",string bdrawopt2="PESAME",
    bool logx=false, bool toplogy=true, bool bottomlogy=false)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For residuals histo
  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 4000, 0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0, 0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  //DrawATLASLabels(0.5, 0.85, 4) ;
  // draw center of mass energy and integrated luminosity
  //DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
  pad1->cd() ;
  pad1->SetTickx() ;
  pad1->SetTicky() ;
  // set top histograms attributes
  SetHistAttributes(top1, 1, 2, 1, 0, 1, 20, 1, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top2, 1, 2, 2, 0, 1, 21, 2, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top3, 1, 2, 3, 0, 1, 22, 3, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top4, 1, 2, 4, 0, 1, 23, 4, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  // draw top histograms
  top1->Draw(tdrawopt1.c_str()) ;
  top2->Draw(tdrawopt2.c_str()) ;
  top3->Draw(tdrawopt3.c_str()) ;
  top4->Draw(tdrawopt4.c_str()) ;
  // draw legend
  TLegend *leg=new TLegend(0.5,0.5,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  leg->SetFillStyle(0) ;
  leg->SetTextSize(0.03) ;
  if(tdrawopt1.find("HIST")!=string::npos)
    leg->AddEntry(top1, top1->GetTitle(), "L") ;
  else
    leg->AddEntry(top1, top1->GetTitle(), "LPE") ;
  if(tdrawopt2.find("HIST")!=string::npos)
    leg->AddEntry(top2, top2->GetTitle(), "L") ;
  else
    leg->AddEntry(top2, top2->GetTitle(), "LPE") ;
  if(tdrawopt3.find("HIST")!=string::npos)
    leg->AddEntry(top3, top3->GetTitle(), "L") ;
  else
    leg->AddEntry(top3, top3->GetTitle(), "LPE") ;
  if(tdrawopt4.find("HIST")!=string::npos)
    leg->AddEntry(top4, top4->GetTitle(), "L") ;
  else
    leg->AddEntry(top4, top4->GetTitle(), "LPE") ;
  leg->Draw("same") ;
  pad2->cd() ;
  pad2->SetTicky() ;
  pad2->SetTickx() ;
  // set bottom histograms attributes
  SetHistAttributes(bottom1, 1, 2, 1, 0, 1, 20, 1, 1, 0.1, 0.5, 0.1, 0.1, 0.5, 0.1, false) ;
  SetHistAttributes(bottom2, 1, 2, 2, 0, 1, 21, 2, 1, 0.1, 0.5, 0.1, 0.1, 0.5, 0.1, false) ;
  // Draw bottom histograms
  bottom1->Draw(bdrawopt1.c_str()) ;
  bottom2->Draw(bdrawopt2.c_str()) ;
  // draw bottom legend
  TLegend *bleg=new TLegend(0.5,0.5,0.89,0.89) ;
  bleg->SetBorderSize(0) ;
  bleg->SetFillStyle(0) ;
  bleg->SetTextSize(0.04) ;
  if(bdrawopt1.find("HIST")!=string::npos)
    bleg->AddEntry(bottom1, bottom1->GetTitle(), "L") ;
  else
    bleg->AddEntry(bottom1, bottom1->GetTitle(), "LPE") ;
  if(bdrawopt1.find("HIST")!=string::npos)
    bleg->AddEntry(bottom2, bottom2->GetTitle(), "L") ;
  else
    bleg->AddEntry(bottom2, bottom2->GetTitle(), "LPE") ;
  bleg->Draw("SAME") ;
  c->Print((canvasname+".eps").c_str()) ;
  return 0;
}

static int DrawTopFiveHistsAndBottomTwoHists(string canvasname, TH1F *top1, TH1F* top2, TH1F* top3, TH1F* top4, TH1F* top5,TH1F* bottom1, TH1F* bottom2, 
    string tdrawopt1="HIST", string tdrawopt2="PESAME",string tdrawopt3="PESAME",string tdrawopt4="PEASAME",string tdrawopt5="PEASAME", string bdrawopt1="HIST",string bdrawopt2="PESAME",
    bool logx=false, bool toplogy=true, bool bottomlogy=true)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For residuals histo
  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 4000, 0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0, 0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  //DrawATLASLabels(0.5, 0.85, 4) ;
  // draw center of mass energy and integrated luminosity
  //DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
  pad1->cd() ;
  pad1->SetTickx() ;
  pad1->SetTicky() ;
  // set top histograms attributes
  SetHistAttributes(top1, 1, 2, 1, 0, 1, 20, 1, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top2, 1, 2, 2, 0, 1, 21, 2, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top3, 1, 2, 3, 0, 1, 22, 3, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top4, 1, 2, 4, 0, 1, 23, 4, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  SetHistAttributes(top5, 1, 2, 5, 0, 1, 24, 5, 1, 0.04, 1., 0.04, 0.04, 1., 0.04, false) ;
  // draw top histograms
  top1->Draw(tdrawopt1.c_str()) ;
  top2->Draw(tdrawopt2.c_str()) ;
  top3->Draw(tdrawopt3.c_str()) ;
  top4->Draw(tdrawopt4.c_str()) ;
  top5->Draw(tdrawopt5.c_str()) ;
  // draw legend
  TLegend *leg=new TLegend(0.5,0.5,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  leg->SetFillStyle(0) ;
  leg->SetTextSize(0.03) ;
  if(tdrawopt1.find("HIST")!=string::npos)
    leg->AddEntry(top1, top1->GetTitle(), "L") ;
  else
    leg->AddEntry(top1, top1->GetTitle(), "LPE") ;
  if(tdrawopt2.find("HIST")!=string::npos)
    leg->AddEntry(top2, top2->GetTitle(), "L") ;
  else
    leg->AddEntry(top2, top2->GetTitle(), "LPE") ;
  if(tdrawopt3.find("HIST")!=string::npos)
    leg->AddEntry(top3, top3->GetTitle(), "L") ;
  else
    leg->AddEntry(top3, top3->GetTitle(), "LPE") ;
  if(tdrawopt4.find("HIST")!=string::npos)
    leg->AddEntry(top4, top4->GetTitle(), "L") ;
  else
    leg->AddEntry(top4, top4->GetTitle(), "LPE") ;
  if(tdrawopt5.find("HIST")!=string::npos)
    leg->AddEntry(top5, top5->GetTitle(), "L") ;
  else
    leg->AddEntry(top5, top5->GetTitle(), "LPE") ;
  leg->Draw("same") ;
  pad2->cd() ;
  pad2->SetTicky() ;
  pad2->SetTickx() ;
  // set bottom histograms attributes
  SetHistAttributes(bottom1, 1, 2, 1, 0, 1, 20, 1, 1, 0.1, 1., 0.1, 0.1, 0.5, 0.1, false) ;
  SetHistAttributes(bottom2, 1, 2, 2, 0, 1, 21, 2, 1, 0.1, 1., 0.1, 0.1, 0.5, 0.1, false) ;
  // Draw bottom histograms
  bottom1->Draw(bdrawopt1.c_str()) ;
  bottom2->Draw(bdrawopt2.c_str()) ;
  // draw bottom legend
  TLegend *bleg=new TLegend(0.5,0.5,0.89,0.8) ;
  bleg->SetBorderSize(0) ;
  bleg->SetFillStyle(0) ;
  bleg->SetTextSize(0.05) ;
  if(bdrawopt1.find("HIST")!=string::npos)
    bleg->AddEntry(bottom1, bottom1->GetTitle(), "L") ;
  else
    bleg->AddEntry(bottom1, bottom1->GetTitle(), "LPE") ;
  if(bdrawopt1.find("HIST")!=string::npos)
    bleg->AddEntry(bottom2, bottom2->GetTitle(), "L") ;
  else
    bleg->AddEntry(bottom2, bottom2->GetTitle(), "LPE") ;
  bleg->Draw("SAME") ;
  c->Print((canvasname+".eps").c_str()) ;
  return 0;
}

static int DrawTwoHistsinHStack(string canvasname, TH1F *top1, TH1F *top2, 
                  bool logx=false, bool logy=false, 
                  bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false, bool isrectangle=true)
{
  gStyle->SetOptTitle(0); //this will disable the title for all coming histograms
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  pad1->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.65, 0.7, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.65, 0.65, "XX", "13 TeV") ;
 
  pad1->cd() ;
  pad1->SetTickx() ;
  pad1->SetTicky() ;
  // draw stack
  stack->Draw("HIST") ;
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  gPad->Modified() ;
  // draw legend
  TLegend *leg=new TLegend(0.65,0.75,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;
  return 0;
}

static int DrawThreeHistsinHStack(string canvasname, TH1F *top1, TH1F *top2, TH1F* top3,
                  bool logx=false, bool logy=false, 
                  bool isrectangle=true, bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3) ;
  SetHistAttributes(top3, 1, 2, 4, 1001, 4) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  stack->Add(top3) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  pad1->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.65, 0.7, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.65, 0.65, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->Draw("HIST") ;
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  gPad->Modified() ;

  // draw legend
  TLegend *leg=new TLegend(0.65,0.75,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  leg->AddEntry(top3, top2->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawFourHistsinHStack(string canvasname, 
                  TH1F *top1, TH1F *top2, TH1F* top3, TH1F* top4,
                  bool logx=false, bool logy=false, 
                  bool isrectangle=true, bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3) ;
  SetHistAttributes(top3, 1, 2, 4, 1001, 4) ;
  SetHistAttributes(top4, 1, 2, 5, 1001, 5) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  stack->Add(top3) ;
  stack->Add(top4) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  pad1->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.65, 0.65, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.65, 0.6, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->Draw("HIST") ;
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  gPad->Modified() ;

  // draw legend
  TLegend *leg=new TLegend(0.65,0.7,0.89,0.89) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  leg->AddEntry(top3, top3->GetTitle(), "F") ;
  leg->AddEntry(top4, top4->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawTwoHistsinStackAndOneHist(string canvasname, TH1F *top1, TH1F *top2, TH1F *hist,
                  bool logx=false, bool logy=false, 
                  bool isrectangle=true, bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3) ;
  SetHistAttributes(hist, 1, 2, 1, 1001, 1, 20, 1, 1) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  pad1->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.5, 0.85, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->Draw("HIST") ;
  hist->Draw("PESAME") ;

  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  leg->AddEntry(hist, hist->GetTitle(), "APL") ;
  // draw legend
  leg->Draw("same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawThreeHistsinHStackAndOneHist(string canvasname, TH1F *top1, TH1F *top2, TH1F* top3,                   TH1F *hist,
                  bool logx=false, bool logy=false, 
                  bool isrectangle=true, bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3) ;
  SetHistAttributes(top3, 1, 2, 4, 1001, 4) ;
  SetHistAttributes(hist, 1, 2, 1, 1001, 1, 20, 1, 1) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  stack->Add(top3) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.1, 0.1, 0, logx, logy) ;
  pad1->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.5, 0.85, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  stack->Draw("HIST") ;
  hist->Draw("PESAME") ;

  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  leg->AddEntry(top3, top3->GetTitle(), "F") ;
  leg->AddEntry(hist, hist->GetTitle(), "APL") ;
  // draw legend
  leg->Draw("same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawFourHistsinHStackAndOneHist(string canvasname, 
                  TH1F *top1, TH1F *top2, TH1F* top3, TH1F *top4, TH1F *hist,
                  bool logx=false, bool logy=false, 
                  bool isrectangle=true, bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  float height=600 ;
  float width ;
  if(isrectangle)
    width=800 ;
  else
    width=600 ;
  TCanvas *c = new TCanvas(canvasname.c_str(), "", width, height) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3) ;
  SetHistAttributes(top3, 1, 2, 4, 1001, 4) ;
  SetHistAttributes(top4, 1, 2, 5, 1001, 5) ;
  SetHistAttributes(hist, 1, 2, 1, 1001, 1, 20, 1, 1) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  stack->Add(top3) ;
  stack->Add(top4) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.1, 0.1, logx, logy) ;
  pad1->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.5, 0.85, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  stack->Draw("HIST") ;
  hist->Draw("PESAME") ;

  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  leg->AddEntry(top3, top3->GetTitle(), "F") ;
  leg->AddEntry(top4, top4->GetTitle(), "F") ;
  leg->AddEntry(hist, hist->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawTopTwoHistsinHStackAndOneHistBottomOneHist(string canvasname, 
                  TH1F *top1, TH1F *top2, TH1F *hist, TH1F*bottom,
                  bool logx=false, bool toplogy=false, bool bottomlogy=false,
                  bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2, 21, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3, 21, 3) ;
  SetHistAttributes(hist, 1, 2, 1, 1001, 1, 20, 1) ;
  SetHistAttributes(bottom, 1, 2, 1, 1001, 1, 20, 1, 1,  0.1, 0.5, 0.1, 0.1, 0.5, 0.1) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0, 0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0, 0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.5, 0.85, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  stack->Draw("HIST") ;
  hist->Draw("PESAME") ;

  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;
  // draw bottom pad
  pad2->cd() ;
  bottom->Draw("PE") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawTopThreeHistsinHStackAndOneHistBottomOneHist(string canvasname, 
                  TH1F *top1, TH1F *top2, TH1F* top3, TH1F *hist, TH1F *bottom,
                  bool logx=false, bool toplogy=false, bool bottomlogy=false,
                  bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2, 21, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3, 21, 3) ;
  SetHistAttributes(top3, 1, 2, 4, 1001, 4, 21, 4) ;
  SetHistAttributes(hist, 1, 2, 1, 1001, 1, 20, 1) ;
  SetHistAttributes(bottom, 1, 2, 1, 1001, 1, 20, 1, 1, 0.1, 0.5, 0.1, 0.1, 0.5, 0.1) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  stack->Add(top3) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0,0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0,0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.5, 0.85, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  SetHStackAttributes(stack) ;
  stack->Draw("HIST") ;
  hist->Draw("PESAME") ;

  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;
  // draw bottom pad
  pad2->cd() ;
  bottom->Draw("PE") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

static int DrawTopFourHistsinHStacksAndOneHistBottomOneHist(string canvasname, 
                  TH1F *top1, TH1F *top2, TH1F* top3, TH1F* top4, TH1F *hist,TH1F* bottom,
                  bool logx=false, bool toplogy=false, bool bottomlogy=false,
                  bool drawlabel=true, string label="", 
                  bool drawcmeandlumi=false)
{
  // define Canvas
  TCanvas *c = new TCanvas(canvasname.c_str(), "", 800, 800) ;

  // set attributs of histograms
  SetHistAttributes(top1, 1, 2, 2, 1001, 2, 21, 2) ;
  SetHistAttributes(top2, 1, 2, 3, 1001, 3, 21, 3) ;
  SetHistAttributes(top3, 1, 2, 4, 1001, 4, 21, 4) ;
  SetHistAttributes(top4, 1, 2, 5, 1001, 5, 21, 5) ;
  SetHistAttributes(hist, 1, 2, 1, 1001, 1, 20, 1) ;
  SetHistAttributes(bottom, 1, 2, 1, 1001, 1, 20, 1, 1, 0.1, 0.5, 0.1, 0.1, 0.5, 0.1) ;

  // define stack 
  THStack *stack=new THStack("stack","") ;
  stack->Add(top1) ;
  stack->Add(top2) ;
  stack->Add(top3) ;
  stack->Add(top4) ;
  // Define pads
  TPad *outpad = new TPad("extpad","extpad",0,0,1,1) ;// For marking outermost dimensions
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1) ;// For main histo
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33) ;// For main histo

  outpad->SetFillStyle(4000) ;//transparent
  SetPadAttributes(pad1, 0,0, 0.1, 0.1, 0.00001, 0.1, 0, logx, toplogy) ;
  SetPadAttributes(pad2, 0,0, 0.1, 0.1, 0.3, 0.00001, 0, logx, bottomlogy) ;
  pad1->Draw() ;
  pad2->Draw() ;
  outpad->Draw() ;

  outpad->cd() ;
  // Draw ATLAS Label
  if(drawlabel)
    DrawLabels(0.5, 0.85, label.c_str()) ;
  // draw center of mass energy and integrated luminosity
  if(drawcmeandlumi)
    DrawCMEAndLumi(0.5, 0.8, "XX", "13 TeV") ;
 
  pad1->cd() ;
  // draw stack
  stack->GetXaxis()->SetTitle(top1->GetXaxis()->GetTitle()) ;
  stack->GetYaxis()->SetTitle(top1->GetYaxis()->GetTitle()) ;
  stack->Draw("HIST") ;
  hist->Draw("PESAME") ;

  // draw legend
  TLegend *leg=new TLegend(0.5,0.55,0.89,0.65) ;
  leg->SetBorderSize(0) ;
  leg->AddEntry(top1, top1->GetTitle(), "F") ;
  leg->AddEntry(top2, top2->GetTitle(), "F") ;
  // draw legend
  leg->Draw("same") ;
  // draw bottom pad
  pad2->cd() ;
  bottom->Draw("PE") ;

  c->Update() ; 
  c->Print((canvasname+".eps").c_str()) ;

  return 0;
}

//-----------------------------------------------------



// get the ratio histogram of two histrograms
static TH1F *GetRatioHistOfTwoHists(string name, string title, string xaxistitle, string yaxistitle, TH1F* numeratorhist, TH1F* denominatorhist)
{
  TH1F *ratiohist = (TH1F*)numeratorhist->Clone() ;
  ratiohist->SetName(name.c_str()) ;
  ratiohist->SetTitle(title.c_str()) ;
  ratiohist->GetXaxis()->SetTitle(xaxistitle.c_str()) ;
  ratiohist->GetYaxis()->SetTitle(yaxistitle.c_str()) ;
  ratiohist->Divide(denominatorhist) ;
  return ratiohist ;
}

// get the ratio histogram of errorbar of two histrograms
static TH1F *GetErrorBarRatioHistOfTwoHists(string name, string title, string xaxistitle, string yaxistitle, TH1F* numeratorhist, TH1F* denominatorhist)
{
  TH1F *ratiohist = new TH1F(name.c_str(), title.c_str(), numeratorhist->GetNbinsX(), numeratorhist->GetXaxis()->GetXbins()->GetArray()) ;
  ratiohist->Sumw2() ;
  ratiohist->GetYaxis()->SetTitle(yaxistitle.c_str()) ;
  ratiohist->GetXaxis()->SetTitle(xaxistitle.c_str()) ;
  for(int n=1 ; n<=ratiohist->GetNbinsX() ; n++)
  {
    if(denominatorhist->GetBinError(n)==0) 
      ratiohist->SetBinContent(n, 0.) ;
    else
    {
      float over = numeratorhist->GetBinError(n)/denominatorhist->GetBinError(n) ;
      ratiohist->SetBinContent(n, over) ;
    }
  }
  return ratiohist ;
}

#endif
