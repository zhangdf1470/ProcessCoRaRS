#ifndef __LocateED_CXX
#define __LocateED_CXX
#include "ProcessCORARS/LocateED.h"

using namespace std;
LocateED::LocateED()
{
  inPath = "" ;
  outPath = "" ;
  inFileName = "" ;
  outFileName = "" ;
  inFile = 0 ;
  inTree = 0 ;
  outFile = 0 ;

  NEDs = 0 ;
  for(int i=0 ; i<8 ; i++)
  {
    EDName[i] = "" ;
    EDSCT1Name[i] = "" ;
    EDSCT2Name[i] = "" ;
    EDSCT3Name[i] = "" ;
    EDSCT4Name[i] = "" ;

    EDHitsGraphs[i] = 0 ;
    RotatedEDHitsGraphs[i] = 0 ;
    EDNoisesGraphs[i]  = 0;
    RotatedEDNoisesGraphs[i] =0 ;
    Outer1BLine[i] = 0 ;
    Outer1TLine[i] = 0 ;
    Outer1LLine[i] = 0 ;
    Outer1RLine[i] = 0 ;
    Inner1BLine[i] = 0 ;
    Inner1TLine[i] = 0 ;
    Inner1LLine[i] = 0 ;
    Inner1RLine[i] = 0 ;
    Inner2BLine[i] = 0 ;
    Inner2TLine[i] = 0 ;
    Inner2LLine[i] = 0 ;
    Inner2RLine[i] = 0 ;
    Outer2BLine[i] = 0 ;
    Outer2TLine[i] = 0 ;
    Outer2LLine[i] = 0 ;
    Outer2RLine[i] = 0 ;
    Inner3BLine[i] = 0 ;
    Inner3TLine[i] = 0 ;
    Inner3LLine[i] = 0 ;
    Inner3RLine[i] = 0 ;
    Outer3BLine[i] = 0 ;
    Outer3TLine[i] = 0 ;
    Outer3LLine[i] = 0 ;
    Outer3RLine[i] = 0 ;
    Inner4BLine[i] = 0 ;
    Inner4TLine[i] = 0 ;
    Inner4LLine[i] = 0 ;
    Inner4RLine[i] = 0 ;
    Outer4BLine[i] = 0 ;
    Outer4TLine[i] = 0 ;
    Outer4LLine[i] = 0 ;
    Outer4RLine[i] = 0 ;
  }
  FuncSlope = 0 ;
  XTruncation = 0 ;
  YTruncation = 0 ;
  SCT1BLPoint = 0 ;
  SCT2BLPoint = 0 ;
  SCT3BLPoint = 0 ;
  SCT4BLPoint = 0 ;
  SCT1BLRotatedPoint = 0 ;
  SCT2BLRotatedPoint = 0 ;
  SCT3BLRotatedPoint = 0 ;
  SCT4BLRotatedPoint = 0 ;
  // initialize 
  Initialize() ;
}
LocateED::~LocateED() 
{
  delete inTree ;
  delete inFile ;
  delete outFile ;
}
void LocateED::Initialize()
{
  conf = ConfigureClass::Instance();

  NEDs = conf->NEDs ;
  for(int i=0 ; i<NEDs ; i++)
  {
    EDName[i] = conf->EDName[i] ;
    EDSCT1Name[i] = conf->EDSCT1Name[i] ;
    EDSCT2Name[i] = conf->EDSCT2Name[i] ;
    EDSCT3Name[i] = conf->EDSCT3Name[i] ;
    EDSCT4Name[i] = conf->EDSCT4Name[i] ;
  }

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

  outPath = conf->outPath+conf->CORARSCollectionTime+"/"  ;
  outFileName = "EDPositions_"+conf->CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;

  FuncSlope = new TVectorD(8) ;
  XTruncation = new TVectorD(8) ;
  YTruncation = new TVectorD(8) ;
  SCT1BLPoint = new TVectorD(16); 
  SCT2BLPoint = new TVectorD(16); 
  SCT3BLPoint = new TVectorD(16); 
  SCT4BLPoint = new TVectorD(16); 
  SCT1BLRotatedPoint = new TVectorD(16);
  SCT2BLRotatedPoint = new TVectorD(16);
  SCT3BLRotatedPoint = new TVectorD(16);
  SCT4BLRotatedPoint = new TVectorD(16);

  return ;
}

void LocateED::GetGraphs()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    TCut timecut = (EDName[i]+".Measure_Time>0").c_str() ;
    TCut qualitycut = "EventQuality==1" ;
    TString varexp = EDName[i]+".y:"+EDName[i]+".x" ;
    inTree->Draw(varexp, timecut&&qualitycut) ;
    EDHitsGraphs[i] = (TGraph*) ((TGraph*)gPad->GetPrimitive("Graph"))->Clone();
    EDHitsGraphs[i]->SetName((conf->EDName[i]+"HitsGraph").c_str()) ;
    EDHitsGraphs[i]->SetTitle((conf->EDName[i]+" Hits").c_str()) ;
    EDHitsGraphs[i]->GetXaxis()->SetTitle("x (cm)") ;
    EDHitsGraphs[i]->GetYaxis()->SetTitle("y (cm)") ;
    inTree->Draw(varexp,(EDName[i]+".Measure_Time==0").c_str()) ;
    EDNoisesGraphs[i] = (TGraph*) ((TGraph*)gPad->GetPrimitive("Graph"))->Clone();
    EDNoisesGraphs[i]->SetName((conf->EDName[i]+"NoisesGraph").c_str()) ;
    EDNoisesGraphs[i]->SetTitle((conf->EDName[i]+" Noises").c_str()) ;
    EDNoisesGraphs[i]->GetXaxis()->SetTitle("x (cm)") ;
    EDNoisesGraphs[i]->GetYaxis()->SetTitle("y (cm)") ;
  }
  return  ;
}

void LocateED::Tuning()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    TH1F* h_lx = new TH1F("h_lx","", 100, (*SCT1BLRotatedPoint)[i*2]+10, (*SCT1BLRotatedPoint)[i*2]+40) ;
    TCut timecut = (EDName[i]+".Measure_Time==0").c_str() ;
    TString str_ycut = TString::Format("((EDName.y*cos(atan(%f))-EDName.x*sin(atan(%f))))>%f&&((EDName.y*cos(atan(%f))-EDName.x*sin(atan(%f))))<%f",(*FuncSlope)[i], (*FuncSlope)[i], (*SCT1BLRotatedPoint)[i*2+1]+10 , (*FuncSlope)[i], (*FuncSlope)[i], (*SCT1BLRotatedPoint)[i*2+1]+90) ;
    TCut ycut = str_ycut.ReplaceAll("EDName", EDName[i].c_str()).Data() ;
    TString str_lxcut = TString::Format("(EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f)))>%f&&(EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f)))<%f", (*FuncSlope)[i], (*FuncSlope)[i], (*SCT1BLRotatedPoint)[i*2]+10 , (*FuncSlope)[i], (*FuncSlope)[i], (*SCT1BLRotatedPoint)[i*2]+40) ;
    TCut lxcut = str_lxcut.ReplaceAll("EDName", EDName[i].c_str()).Data() ;
    TString lvarexp = TString::Format("(EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f)))>>h_lx", (*FuncSlope)[i], (*FuncSlope)[i]) ;
    lvarexp.ReplaceAll("EDName", EDName[i].c_str()) ;
    inTree->Draw(lvarexp, timecut&&lxcut&&ycut) ;
    TF1* functotal = new TF1("functotal","gaus(0)+pol1(3)") ;
    functotal->SetRange((*SCT1BLRotatedPoint)[i*2]+10, (*SCT1BLRotatedPoint)[i*2]+40) ;
    TF1* funcgaus = new TF1("funcgaus","gaus") ;
    funcgaus->SetRange((*SCT1BLRotatedPoint)[i*2]+20, (*SCT1BLRotatedPoint)[i*2]+30) ;
    TF1* funcpol = new TF1("funcpol","pol1") ;
    funcpol->SetRange((*SCT1BLRotatedPoint)[i*2], (*SCT1BLRotatedPoint)[i*2]+15) ;
    h_lx->Fit("funcgaus","RQ") ;
    h_lx->Fit("funcpol","RQ") ;
    functotal->SetParameter(0,funcgaus->GetParameter(0)) ;
    functotal->SetParameter(1,funcgaus->GetParameter(1)) ;
    functotal->SetParameter(2,funcgaus->GetParameter(2)) ;
    functotal->SetParameter(3,funcpol->GetParameter(0)) ;
    functotal->SetParameter(4,funcpol->GetParameter(1)) ;
    h_lx->Fit(functotal, "RQ") ;
    cout<<"Before: "<<(*SCT1BLRotatedPoint)[i*2]<<endl ;
    (*SCT1BLRotatedPoint)[i*2] = functotal->GetParameter(1)-25.5 ;
    (*SCT2BLRotatedPoint)[i*2] = (*SCT1BLRotatedPoint)[i*2] + 26. ;
    cout<<"After: "<<(*SCT1BLRotatedPoint)[i*2]<<endl ;
    h_lx->Write(TString::Format("h%i_lx", i)) ;
    delete functotal ;
    delete funcgaus ;
    delete funcpol ;

    TH1F* h_rx = new TH1F("h_rx","", 100, (*SCT3BLRotatedPoint)[i*2]+10, (*SCT3BLRotatedPoint)[i*2]+40) ;
    TString str_rxcut = TString::Format("(EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f)))>%f&&(EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f)))<%f", (*FuncSlope)[i], (*FuncSlope)[i], (*SCT3BLRotatedPoint)[i*2]+10 , (*FuncSlope)[i], (*FuncSlope)[i], (*SCT3BLRotatedPoint)[i*2]+40) ;
    TCut rxcut = str_rxcut.ReplaceAll("EDName", EDName[i].c_str()).Data() ;
    TString rvarexp = TString::Format("(EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f)))>>h_rx", (*FuncSlope)[i], (*FuncSlope)[i]) ;
    rvarexp.ReplaceAll("EDName", EDName[i].c_str()) ;
    inTree->Draw(rvarexp, timecut&&rxcut&&ycut) ;
    functotal = new TF1("functotal","gaus(0)+pol1(3)") ;
    funcgaus = new TF1("funcgaus","gaus") ;
    funcpol = new TF1("funcpol","pol1") ;
    functotal->SetRange((*SCT3BLRotatedPoint)[i*2]+10, (*SCT3BLRotatedPoint)[i*2]+40) ;
    funcgaus->SetRange((*SCT3BLRotatedPoint)[i*2]+20, (*SCT3BLRotatedPoint)[i*2]+30) ;
    funcpol->SetRange((*SCT3BLRotatedPoint)[i*2]+10, (*SCT3BLRotatedPoint)[i*2]+20) ;
    h_rx->Fit("funcgaus","RQ") ;
    h_rx->Fit("funcpol","RQ") ;
    functotal->SetParameter(0,funcgaus->GetParameter(0)) ;
    functotal->SetParameter(1,funcgaus->GetParameter(1)) ;
    functotal->SetParameter(2,funcgaus->GetParameter(2)) ;
    functotal->SetParameter(3,funcpol->GetParameter(0)) ;
    functotal->SetParameter(4,funcpol->GetParameter(1)) ;
    h_rx->Fit(functotal, "RQ") ;
    h_rx->Write(TString::Format("h%i_rx", i)) ;
    (*SCT3BLRotatedPoint)[i*2] = functotal->GetParameter(1)-25.5 ;
    (*SCT4BLRotatedPoint)[i*2] = (*SCT3BLRotatedPoint)[i*2] + 26. ;
  }
  return  ;
}

void LocateED::GetRotatedGraphs()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    inTree->Draw(TString::Format("EDName.y*cos(atan(%f))-EDName.x*sin(atan(%f)):EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f))",(*FuncSlope)[i],(*FuncSlope)[i],(*FuncSlope)[i],(*FuncSlope)[i]).ReplaceAll("EDName",EDName[i].c_str()),(EDName[i]+".Measure_Time>0").c_str()) ;
    RotatedEDHitsGraphs[i] = (TGraph*) ((TGraph*)gPad->GetPrimitive("Graph"))->Clone();
    RotatedEDHitsGraphs[i]->SetName(("Rotated"+conf->EDName[i]+"HitsGraph").c_str()) ;
    RotatedEDHitsGraphs[i]->SetTitle(("Rotated "+conf->EDName[i]+" Hits").c_str()) ;
    RotatedEDHitsGraphs[i]->GetXaxis()->SetTitle("x (cm)") ;
    RotatedEDHitsGraphs[i]->GetYaxis()->SetTitle("y (cm)") ;
    inTree->Draw(TString::Format("EDName.y*cos(atan(%f))-EDName.x*sin(atan(%f)):EDName.x*cos(atan(%f))+EDName.y*sin(atan(%f))",(*FuncSlope)[i],(*FuncSlope)[i],(*FuncSlope)[i],(*FuncSlope)[i]).ReplaceAll("EDName",EDName[i].c_str()),(EDName[i]+".Measure_Time==0").c_str()) ;
    RotatedEDNoisesGraphs[i] = (TGraph*) ((TGraph*)gPad->GetPrimitive("Graph"))->Clone();
    RotatedEDNoisesGraphs[i]->SetName(("Rotated"+conf->EDName[i]+"NoisesGraph").c_str()) ;
    RotatedEDNoisesGraphs[i]->SetTitle(("Rotated "+conf->EDName[i]+" Noises").c_str()) ;
    RotatedEDNoisesGraphs[i]->GetXaxis()->SetTitle("x (cm)") ;
    RotatedEDNoisesGraphs[i]->GetYaxis()->SetTitle("y (cm)") ;
  }
  return  ;
}

void LocateED::GetSlope(int NEvents, TGraph* graph, float minslope, float maxslope, float slopestep, float minytruncation, float maxytruncation, float minxtruncation, float maxxtruncation, float truncationstep, float *Slope, float* XTrunc, float* YTrunc)
{
  TF1* bfunc = new TF1("bfunc", "[0]*x+[1]", 0., 100) ;
  TF1* tfunc = new TF1("tfunc", "[0]*x+[1]", 0., 100) ;
  TF1* lfunc  = new TF1("lfunc", "[0]*x+[1]", 0., 100) ;
  TF1* rfunc  = new TF1("rfunc", "[0]*x+[1]", 0., 100) ;
  double *x = graph->GetX() ;
  double *y = graph->GetY() ;
  int MaxCount = -1 ;
  for(float slope=minslope ; slope<=maxslope ; slope+=slopestep)
  {
    for(float ytruncation=minytruncation ; ytruncation<=maxytruncation ; ytruncation+=truncationstep)
    {
      for(float xtruncation=minxtruncation ; xtruncation<=maxxtruncation ; xtruncation+=truncationstep)
      {
        bfunc->SetParameters(slope, ytruncation) ;
        tfunc->SetParameters(slope, ytruncation+100./cos(atan(abs(slope)))) ;
        if(slope==0)
        {
          lfunc->SetParameters(0., 0.) ;
          rfunc->SetParameters(0., 0.) ;
        }
        else
        {
          lfunc->SetParameters(-1./slope, xtruncation/slope) ;
          rfunc->SetParameters(-1./slope, (xtruncation+51./cos(atan(abs(slope))))/slope) ;
        }
        int count = 0 ;
        for(int n=0 ; n<NEvents ; n++)
        {
          if(slope==0)
          {
            if(y[n]>=ytruncation&&y[n]<=(ytruncation+100.)&&x[n]>=xtruncation&&x[n]<=(xtruncation+51.))
              count++ ;
          }
          else
          {
            if(y[n]>=bfunc->Eval(x[n])&&y[n]<=tfunc->Eval(x[n])&&y[n]>=lfunc->Eval(x[n])&&y[n]<=rfunc->Eval(x[n]))
              count++ ;
          }
        }
        if(count>MaxCount)
        {
          MaxCount = count ;
          *XTrunc = xtruncation ;
          *YTrunc = ytruncation ;
          *Slope = slope ;
        }
      }
    }
  }
}

void LocateED::GetSlopes()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    int N = EDHitsGraphs[i]->GetN() ; // number of hits on the graph
    if(N>100000)
      N=100000 ; // speed up the locating
    float XTrunc = 0. ;
    float YTrunc = 0. ;
    float Slope = 1. ;

    cout<<"minytruncation:maxytruncation:minxtruncation:maxxtruncation"<<endl ;
    float minslope = -0.2 ;
    float maxslope = 0.2 ;
    float slopestep = 0.01 ;
    float minytruncation = 10. ;
    float maxytruncation = 30. ;
    float minxtruncation = 10. ;
    float maxxtruncation = 30. ;
    float truncationstep = 1. ;
    cout<<minslope<<":"<<maxslope<<endl ;
    cout<<minytruncation<<":"<<maxytruncation<<":"<<minxtruncation<<":"<<maxxtruncation<<endl ;
    GetSlope(N, EDHitsGraphs[i], minslope, maxslope, slopestep,  minytruncation,  maxytruncation, minxtruncation,  maxxtruncation,  truncationstep, &Slope, &XTrunc, &YTrunc) ;
    minslope = Slope-2*slopestep ;
    maxslope = Slope+2*slopestep ;
    slopestep = slopestep/2. ;
    minytruncation = YTrunc-2*truncationstep ;
    maxytruncation = YTrunc+2*truncationstep ;
    minxtruncation = XTrunc-2*truncationstep ;
    maxxtruncation = XTrunc+2*truncationstep ;
    truncationstep = truncationstep/2. ;
    cout<<minslope<<":"<<maxslope<<endl ;
    cout<<minytruncation<<":"<<maxytruncation<<":"<<minxtruncation<<":"<<maxxtruncation<<endl ;
    GetSlope(N, EDHitsGraphs[i], minslope, maxslope, slopestep,  minytruncation,  maxytruncation, minxtruncation,  maxxtruncation,  truncationstep,  &Slope, &XTrunc, &YTrunc) ;
    minslope = Slope-5*slopestep ;
    maxslope = Slope+5*slopestep ;
    slopestep = slopestep/5. ;
    minytruncation = YTrunc-5*truncationstep ;
    maxytruncation = YTrunc+5*truncationstep ;
    minxtruncation = XTrunc-5*truncationstep ;
    maxxtruncation = XTrunc+5*truncationstep ;
    truncationstep = truncationstep/5 ;
    cout<<minslope<<":"<<maxslope<<endl ;
    cout<<minytruncation<<":"<<maxytruncation<<":"<<minxtruncation<<":"<<maxxtruncation<<endl ;
    GetSlope(N, EDHitsGraphs[i], minslope, maxslope, slopestep,  minytruncation,  maxytruncation, minxtruncation,  maxxtruncation,  truncationstep,  &Slope, &XTrunc, &YTrunc) ;
    cout<<conf->EDName[i]<<": "<<endl ;
    cout<<"Slope: "<<Slope<<" XTrunc: "<<XTrunc<<" YTrunc: "<<YTrunc<<endl ;
    (*FuncSlope)[i] = Slope ;
    (*XTruncation)[i] = XTrunc ;
    (*YTruncation)[i] = YTrunc ;
  }
}
void LocateED::GetIntersectionPoints()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    TF1* bfunc = new TF1("bfunc", "[0]*x+[1]", 0., 100.) ;
    TF1* tfunc = new TF1("tfunc", "[0]*x+[1]", 0., 100.) ;
    TF1* lfunc = new TF1("lfunc", "[0]*x+[1]", 0., 100.) ;
    TF1* rfunc = new TF1("rfunc", "[0]*x+[1]", 0., 100.) ;
    bfunc->SetParameters((*FuncSlope)[i], (*YTruncation)[i]) ;
    tfunc->SetParameters((*FuncSlope)[i], (*YTruncation)[i]+100./cos(atan(abs((*FuncSlope)[i])))) ;
    if((*FuncSlope)[i]==0)
    {
      (*SCT1BLPoint)[i*2] = (*XTruncation)[i] ;
      (*SCT1BLPoint)[i*2+1] =  (*YTruncation)[i];
      (*SCT1BLRotatedPoint)[i*2] = (*XTruncation)[i] ;
      (*SCT1BLRotatedPoint)[i*2+1] = (*YTruncation)[i] ;
    }
    else
    {
      lfunc->SetParameters(-1./(*FuncSlope)[i], (*XTruncation)[i]/(*FuncSlope)[i]) ;
      rfunc->SetParameters(-1./(*FuncSlope)[i], ((*XTruncation)[i]+51./cos(atan(abs((*FuncSlope)[i]))))/(*FuncSlope)[i]) ;
      float x=0., y=0. ;
      GetIntersectionPoint(bfunc, lfunc, &x, &y) ;
      (*SCT1BLPoint)[i*2] = x ;
      (*SCT1BLPoint)[i*2+1] = y ;
      (*SCT1BLRotatedPoint)[i*2] = x*cos(atan(abs((*FuncSlope)[i])))+y*sin(atan(abs((*FuncSlope)[i]))) ;
      (*SCT1BLRotatedPoint)[i*2+1] = y*cos(atan(abs((*FuncSlope)[i])))-x*sin(atan(abs((*FuncSlope)[i]))) ;
      delete bfunc ;
      delete tfunc ;
      delete lfunc ;
      delete rfunc ;
    }
    (*SCT2BLRotatedPoint)[i*2] = (*SCT1BLRotatedPoint)[i*2] + 26. ;
    (*SCT2BLRotatedPoint)[i*2+1] = (*SCT1BLRotatedPoint)[i*2+1] ;
    (*SCT3BLRotatedPoint)[i*2] = (*SCT2BLRotatedPoint)[i*2] + 25. + 37. ;
    (*SCT3BLRotatedPoint)[i*2+1] = (*SCT2BLRotatedPoint)[i*2+1] ;
    (*SCT4BLRotatedPoint)[i*2] = (*SCT3BLRotatedPoint)[i*2] + 26. ;
    (*SCT4BLRotatedPoint)[i*2+1] = (*SCT3BLRotatedPoint)[i*2+1] ;
  }
}

void LocateED::GetIntersectionPoint(TF1* func1, TF1* func2, float* pointx, float *pointy)
{
  TF1 *fint = new TF1("fint",("abs("+string(func1->GetName())+"-"+string(func2->GetName())+")").c_str(),0., 100.);
  float xmin = fint->GetMinimumX() ;
  float ymin = func1->Eval(xmin)  ;
  *pointx = xmin ;
  *pointy = ymin ;
  delete fint ;
}

void LocateED::GetLines()
{
  for(int i=0 ; i<NEDs ; i++)
  {
    Outer1BLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i],(*SCT1BLRotatedPoint)[2*i+1],(*SCT1BLRotatedPoint)[2*i]+25,(*SCT1BLRotatedPoint)[2*i+1]) ;
    Outer1TLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i],(*SCT1BLRotatedPoint)[2*i+1]+100,(*SCT1BLRotatedPoint)[2*i]+25,(*SCT1BLRotatedPoint)[2*i+1]+100) ;
    Outer1LLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i],(*SCT1BLRotatedPoint)[2*i+1],(*SCT1BLRotatedPoint)[2*i],(*SCT1BLRotatedPoint)[2*i+1]+100) ;
    Outer1RLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i]+25,(*SCT1BLRotatedPoint)[2*i+1],(*SCT1BLRotatedPoint)[2*i]+25,(*SCT1BLRotatedPoint)[2*i+1]+100) ;
    Inner1BLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i]+2.5,(*SCT1BLRotatedPoint)[2*i+1]+2.5,(*SCT1BLRotatedPoint)[2*i]+25-2.5,(*SCT1BLRotatedPoint)[2*i+1]+2.5) ;
    Inner1TLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i]+2.5,(*SCT1BLRotatedPoint)[2*i+1]+100-2.5,(*SCT1BLRotatedPoint)[2*i]+25-2.5,(*SCT1BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner1LLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i]+2.5,(*SCT1BLRotatedPoint)[2*i+1]+2.5,(*SCT1BLRotatedPoint)[2*i]+2.5,(*SCT1BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner1RLine[i] = new TLine((*SCT1BLRotatedPoint)[2*i]+25-2.5,(*SCT1BLRotatedPoint)[2*i+1]+2.5,(*SCT1BLRotatedPoint)[2*i]+25-2.5,(*SCT1BLRotatedPoint)[2*i+1]+100-2.5) ;
    Outer1BLine[i]->SetLineColor(kRed) ;
    Outer1BLine[i]->SetLineWidth(2) ;
    Outer1TLine[i]->SetLineColor(kRed) ;
    Outer1TLine[i]->SetLineWidth(2) ;
    Outer1LLine[i]->SetLineColor(kRed) ;
    Outer1LLine[i]->SetLineWidth(2) ;
    Outer1RLine[i]->SetLineColor(kRed) ;
    Outer1RLine[i]->SetLineWidth(2) ;
    Outer2BLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i],(*SCT2BLRotatedPoint)[2*i+1],(*SCT2BLRotatedPoint)[2*i]+25.,(*SCT2BLRotatedPoint)[2*i+1]) ;
    Outer2TLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i],(*SCT2BLRotatedPoint)[2*i+1]+100,(*SCT2BLRotatedPoint)[2*i]+25.,(*SCT2BLRotatedPoint)[2*i+1]+100) ;
    Outer2LLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i],(*SCT2BLRotatedPoint)[2*i+1],(*SCT2BLRotatedPoint)[2*i],(*SCT2BLRotatedPoint)[2*i+1]+100) ;
    Outer2RLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i]+25,(*SCT2BLRotatedPoint)[2*i+1],(*SCT2BLRotatedPoint)[2*i]+25.,(*SCT2BLRotatedPoint)[2*i+1]+100) ;
    Inner2BLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i]+2.5,(*SCT2BLRotatedPoint)[2*i+1]+2.5,(*SCT2BLRotatedPoint)[2*i]+25-2.5,(*SCT2BLRotatedPoint)[2*i+1]+2.5) ;
    Inner2TLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i]+2.5,(*SCT2BLRotatedPoint)[2*i+1]+100-2.5,(*SCT2BLRotatedPoint)[2*i]+25.-2.5,(*SCT2BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner2LLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i]+2.5,(*SCT2BLRotatedPoint)[2*i+1]+2.5,(*SCT2BLRotatedPoint)[2*i]+2.5,(*SCT2BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner2RLine[i] = new TLine((*SCT2BLRotatedPoint)[2*i]+25.-2.5,(*SCT2BLRotatedPoint)[2*i+1]+2.5,(*SCT2BLRotatedPoint)[2*i]+25.-2.5,(*SCT2BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner1BLine[i]->SetLineColor(kBlue) ;
    Inner1BLine[i]->SetLineWidth(2) ;
    Inner1TLine[i]->SetLineColor(kBlue) ;
    Inner1TLine[i]->SetLineWidth(2) ;
    Inner1LLine[i]->SetLineColor(kBlue) ;
    Inner1LLine[i]->SetLineWidth(2) ;
    Inner1RLine[i]->SetLineColor(kBlue) ;
    Inner1RLine[i]->SetLineWidth(2) ;
    Inner2BLine[i]->SetLineColor(kBlue) ;
    Inner2BLine[i]->SetLineWidth(2) ;
    Inner2TLine[i]->SetLineColor(kBlue) ;
    Inner2TLine[i]->SetLineWidth(2) ;
    Inner2LLine[i]->SetLineColor(kBlue) ;
    Inner2LLine[i]->SetLineWidth(2) ;
    Inner2RLine[i]->SetLineColor(kBlue) ;
    Inner2RLine[i]->SetLineWidth(2) ;
    Outer2BLine[i]->SetLineColor(kRed) ;
    Outer2BLine[i]->SetLineWidth(2) ;
    Outer2TLine[i]->SetLineColor(kRed) ;
    Outer2TLine[i]->SetLineWidth(2) ;
    Outer2LLine[i]->SetLineColor(kRed) ;
    Outer2LLine[i]->SetLineWidth(2) ;
    Outer2RLine[i]->SetLineColor(kRed) ;
    Outer2RLine[i]->SetLineWidth(2) ;
    Outer3BLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i],(*SCT3BLRotatedPoint)[2*i+1],(*SCT3BLRotatedPoint)[2*i]+25,(*SCT3BLRotatedPoint)[2*i+1]) ;
    Outer3TLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i],(*SCT3BLRotatedPoint)[2*i+1]+100,(*SCT3BLRotatedPoint)[2*i]+25,(*SCT3BLRotatedPoint)[2*i+1]+100) ;
    Outer3LLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i],(*SCT3BLRotatedPoint)[2*i+1],(*SCT3BLRotatedPoint)[2*i],(*SCT3BLRotatedPoint)[2*i+1]+100) ;
    Outer3RLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i]+25,(*SCT3BLRotatedPoint)[2*i+1],(*SCT3BLRotatedPoint)[2*i]+25,(*SCT3BLRotatedPoint)[2*i+1]+100) ;
    Inner3BLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i]+2.5,(*SCT3BLRotatedPoint)[2*i+1]+2.5,(*SCT3BLRotatedPoint)[2*i]+25-2.5,(*SCT3BLRotatedPoint)[2*i+1]+2.5) ;
    Inner3TLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i]+2.5,(*SCT3BLRotatedPoint)[2*i+1]+100-2.5,(*SCT3BLRotatedPoint)[2*i]+25-2.5,(*SCT3BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner3LLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i]+2.5,(*SCT3BLRotatedPoint)[2*i+1]+2.5,(*SCT3BLRotatedPoint)[2*i]+2.5,(*SCT3BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner3RLine[i] = new TLine((*SCT3BLRotatedPoint)[2*i]+25-2.5,(*SCT3BLRotatedPoint)[2*i+1]+2.5,(*SCT3BLRotatedPoint)[2*i]+25-2.5,(*SCT3BLRotatedPoint)[2*i+1]+100-2.5) ;
    Outer4BLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i],(*SCT4BLRotatedPoint)[2*i+1],(*SCT4BLRotatedPoint)[2*i]+25.,(*SCT4BLRotatedPoint)[2*i+1]) ;
    Outer4TLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i],(*SCT4BLRotatedPoint)[2*i+1]+100,(*SCT4BLRotatedPoint)[2*i]+25.,(*SCT4BLRotatedPoint)[2*i+1]+100) ;
    Outer4LLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i],(*SCT4BLRotatedPoint)[2*i+1],(*SCT4BLRotatedPoint)[2*i],(*SCT4BLRotatedPoint)[2*i+1]+100) ;
    Outer4RLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i]+25.,(*SCT4BLRotatedPoint)[2*i+1],(*SCT4BLRotatedPoint)[2*i]+25.,(*SCT4BLRotatedPoint)[2*i+1]+100) ;
    Inner4BLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i]+2.5,(*SCT4BLRotatedPoint)[2*i+1]+2.5,(*SCT4BLRotatedPoint)[2*i]+25.-2.5,(*SCT4BLRotatedPoint)[2*i+1]+2.5) ;
    Inner4TLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i]+2.5,(*SCT4BLRotatedPoint)[2*i+1]+100-2.5,(*SCT4BLRotatedPoint)[2*i]+25.-2.5,(*SCT4BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner4LLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i]+2.5,(*SCT4BLRotatedPoint)[2*i+1]+2.5,(*SCT4BLRotatedPoint)[2*i]+2.5,(*SCT4BLRotatedPoint)[2*i+1]+100-2.5) ;
    Inner4RLine[i] = new TLine((*SCT4BLRotatedPoint)[2*i]+25.-2.5,(*SCT4BLRotatedPoint)[2*i+1]+2.5,(*SCT4BLRotatedPoint)[2*i]+25.-2.5,(*SCT4BLRotatedPoint)[2*i+1]+100-2.5) ;
    Outer3BLine[i]->SetLineColor(kRed) ;
    Outer3BLine[i]->SetLineWidth(2) ;
    Outer3TLine[i]->SetLineColor(kRed) ;
    Outer3TLine[i]->SetLineWidth(2) ;
    Outer3LLine[i]->SetLineColor(kRed) ;
    Outer3LLine[i]->SetLineWidth(2) ;
    Outer3RLine[i]->SetLineColor(kRed) ;
    Outer3RLine[i]->SetLineWidth(2) ;
    Inner3BLine[i]->SetLineColor(kBlue) ;
    Inner3BLine[i]->SetLineWidth(2) ;
    Inner3TLine[i]->SetLineColor(kBlue) ;
    Inner3TLine[i]->SetLineWidth(2) ;
    Inner3LLine[i]->SetLineColor(kBlue) ;
    Inner3LLine[i]->SetLineWidth(2) ;
    Inner3RLine[i]->SetLineColor(kBlue) ;
    Inner3RLine[i]->SetLineWidth(2) ;
    Outer4BLine[i]->SetLineColor(kRed) ;
    Outer4BLine[i]->SetLineWidth(2) ;
    Outer4TLine[i]->SetLineColor(kRed) ;
    Outer4TLine[i]->SetLineWidth(2) ;
    Outer4LLine[i]->SetLineColor(kRed) ;
    Outer4LLine[i]->SetLineWidth(2) ;
    Outer4RLine[i]->SetLineColor(kRed) ;
    Outer4RLine[i]->SetLineWidth(2) ;
    Inner4BLine[i]->SetLineColor(kBlue) ;
    Inner4BLine[i]->SetLineWidth(2) ;
    Inner4TLine[i]->SetLineColor(kBlue) ;
    Inner4TLine[i]->SetLineWidth(2) ;
    Inner4LLine[i]->SetLineColor(kBlue) ;
    Inner4LLine[i]->SetLineWidth(2) ;
    Inner4RLine[i]->SetLineColor(kBlue) ;
    Inner4RLine[i]->SetLineWidth(2) ;
  }
}

void LocateED::Write()
{
  outFile->cd() ;
  FuncSlope->Write("FuncSlope") ;
  XTruncation->Write("XTruncation") ;
  YTruncation->Write("YTruncation") ;
  SCT1BLPoint->Write("SCT1BLPoint") ;
  SCT2BLPoint->Write("SCT2BLPoint") ;
  SCT3BLPoint->Write("SCT3BLPoint") ;
  SCT4BLPoint->Write("SCT4BLPoint") ;
  SCT1BLRotatedPoint->Write("SCT1BLRotatedPoint") ;
  SCT2BLRotatedPoint->Write("SCT2BLRotatedPoint") ;
  SCT3BLRotatedPoint->Write("SCT3BLRotatedPoint") ;
  SCT4BLRotatedPoint->Write("SCT4BLRotatedPoint") ;
  outFile->Close() ;
  // store global informations to a pavetext
  TFile* GlobalInfo = new TFile((outPath+"/GlobalInfo_"+conf->CORARSCollectionTime+".root").c_str(), "Update") ;
  cout<<"Clobal File: "<<GlobalInfo->GetName()<<endl ;
  for(int i=0 ; i<NEDs ; i++)
  {
    EDHitsGraphs[i]->Write() ;
    RotatedEDHitsGraphs[i]->Write() ;
    EDNoisesGraphs[i]->Write();
    RotatedEDNoisesGraphs[i]->Write() ;
    Outer1BLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"OuterBLine").c_str()) ;
    Outer1TLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"OuterTLine").c_str()) ;
    Outer1LLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"OuterLLine").c_str()) ;
    Outer1RLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"OuterRLine").c_str()) ;
    Inner1BLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"InnerBLine").c_str()) ;
    Inner1TLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"InnerTLine").c_str()) ;
    Inner1LLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"InnerLLine").c_str()) ;
    Inner1RLine[i]->Write((EDName[i]+"_"+EDSCT1Name[i]+"InnerRLine").c_str()) ;
    Outer2BLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"OuterBLine").c_str()) ;
    Outer2TLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"OuterTLine").c_str()) ;
    Outer2LLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"OuterLLine").c_str()) ;
    Outer2RLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"OuterRLine").c_str()) ;
    Inner2BLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"InnerBLine").c_str()) ;
    Inner2TLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"InnerTLine").c_str()) ;
    Inner2LLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"InnerLLine").c_str()) ;
    Inner2RLine[i]->Write((EDName[i]+"_"+EDSCT2Name[i]+"InnerRLine").c_str()) ;
    Outer3BLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"OuterBLine").c_str()) ;
    Outer3TLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"OuterTLine").c_str()) ;
    Outer3LLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"OuterLLine").c_str()) ;
    Outer3RLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"OuterRLine").c_str()) ;
    Inner3BLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"InnerBLine").c_str()) ;
    Inner3TLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"InnerTLine").c_str()) ;
    Inner3LLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"InnerLLine").c_str()) ;
    Inner3RLine[i]->Write((EDName[i]+"_"+EDSCT3Name[i]+"InnerRLine").c_str()) ;
    Outer4BLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"OuterBLine").c_str()) ;
    Outer4TLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"OuterTLine").c_str()) ;
    Outer4LLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"OuterLLine").c_str()) ;
    Outer4RLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"OuterRLine").c_str()) ;
    Inner4BLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"InnerBLine").c_str()) ;
    Inner4TLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"InnerTLine").c_str()) ;
    Inner4LLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"InnerLLine").c_str()) ;
    Inner4RLine[i]->Write((EDName[i]+"_"+EDSCT4Name[i]+"InnerRLine").c_str()) ;
  }
  GlobalInfo->Close() ;
}
void LocateED::FindED()
{
  GetGraphs() ;
  if(conf->doLocateED)
    GetSlopes() ;
  else
  {
    for(int i=0 ; i<NEDs ; i++)
    {
      (*FuncSlope)[i] = conf->FuncSlope[i] ;
      (*XTruncation)[i] = conf->XTruncation[i] ;
      (*YTruncation)[i] = conf->YTruncation[i] ;
    }
  }
  GetIntersectionPoints() ;
  Tuning() ;
  GetRotatedGraphs() ;
  GetLines() ;
  Write() ;
  return  ;
}

#endif
