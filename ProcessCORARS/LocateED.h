#ifndef __LocateED__H
#define __LocateED__H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TLine.h"
#include "TEnv.h"
#include "TAxis.h"
#include "TPad.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TCut.h"
#include "ConfigureClass.h"

using namespace std;
class LocateED
{
  private:
    ConfigureClass* conf ;

  public:
    LocateED() ;
    ~LocateED() ;
    void Initialize() ;
    void GetGraphs() ;
    void GetRotatedGraphs() ;
    void GetSlope(int NEvents, TGraph* graph, float minslope, float maxslope, float slopestep, float minytruncation, float maxytruncation, float minxtruncation, float maxxtruncation, float truncationstep, float *Slope, float* XTrunc, float* YTrunc) ;
    void GetSlopes() ;
    void GetIntersectionPoints() ;
    void GetIntersectionPoint(TF1* func1, TF1* func2, float* pointx, float *pointy) ;
    void Tuning() ;
    void GetLines() ;
    void Write() ;
    void FindED() ;

  public:
    // informations of in files
    string inPath ; // path of thein data files
    string inFileName ; // in file name 
    TFile *inFile ;
    TTree *inTree ;

    // informations of out files
    string outPath ; // out path
    string outFileName ; // out file name
    TFile* outFile ; // out root file containing measured time and charge of Scintillor s and EDs

    int NEDs ; // totoal number of measured EDs
    string EDName[8] ; // ED name
    // name of scintillators in ED
    string EDSCT1Name[8] ;
    string EDSCT2Name[8] ;
    string EDSCT3Name[8] ;
    string EDSCT4Name[8] ;

    TGraph* EDHitsGraphs[8] ;
    TGraph* RotatedEDHitsGraphs[8] ;
    TGraph* EDNoisesGraphs[8] ;
    TGraph* RotatedEDNoisesGraphs[8] ;
    // Lines to highlight ED Positions
    // the left two scintillators
    TLine* Inner1BLine[8] ;
    TLine* Inner1TLine[8] ;
    TLine* Inner1LLine[8] ;
    TLine* Inner1RLine[8] ;
    TLine* Outer1BLine[8] ;
    TLine* Outer1TLine[8] ;
    TLine* Outer1LLine[8] ;
    TLine* Outer1RLine[8] ;
    TLine* Inner2BLine[8] ;
    TLine* Inner2TLine[8] ;
    TLine* Inner2LLine[8] ;
    TLine* Inner2RLine[8] ;
    TLine* Outer2BLine[8] ;
    TLine* Outer2TLine[8] ;
    TLine* Outer2LLine[8] ;
    TLine* Outer2RLine[8] ;

    // the right two scintillators
    TLine* Inner3BLine[8] ;
    TLine* Inner3TLine[8] ;
    TLine* Inner3LLine[8] ;
    TLine* Inner3RLine[8] ;
    TLine* Outer3BLine[8] ;
    TLine* Outer3TLine[8] ;
    TLine* Outer3LLine[8] ;
    TLine* Outer3RLine[8] ;
    TLine* Inner4BLine[8] ;
    TLine* Inner4TLine[8] ;
    TLine* Inner4LLine[8] ;
    TLine* Inner4RLine[8] ;
    TLine* Outer4BLine[8] ;
    TLine* Outer4TLine[8] ;
    TLine* Outer4LLine[8] ;
    TLine* Outer4RLine[8] ;

    TVectorD *FuncSlope ;
    TVectorD *XTruncation ;
    TVectorD *YTruncation ;
    TVectorD *SCT1BLPoint ; // the left  bottom  point (x, y) of SCT 1
    TVectorD *SCT2BLPoint ; // the left  bottom  point (x, y) of SCT 2
    TVectorD *SCT3BLPoint ; // the left  bottom  point (x, y) of SCT 3
    TVectorD *SCT4BLPoint ; // the left  bottom  point (x, y) of SCT 4
    TVectorD *SCT1BLRotatedPoint ; // the left  bottom point (x, y) after rotation of SCT 1
    TVectorD *SCT2BLRotatedPoint ; // the left  bottom point (x, y) after rotation of SCT 2
    TVectorD *SCT3BLRotatedPoint ; // the left  bottom point (x, y) after rotation of SCT 3
    TVectorD *SCT4BLRotatedPoint ; // the left  bottom point (x, y) after rotation of SCT 4
};
#endif
