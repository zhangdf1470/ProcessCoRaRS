#ifndef __PrintCORARS__H
#define __PrintCORARS__H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "TString.h"
#include "TFile.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLine.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TVectorD.h"
#include "TMatrixF.h"
#include "TCanvas.h"
#include "ConfigureClass.h"

using namespace std;
class PrintCORARS
{
  private:
    ConfigureClass* conf ;

  public:
    PrintCORARS() ;
    ~PrintCORARS() ;

    void Initialize() ;
    void GetHists() ;
    void GetGraphs() ;
    void BuildPaveText(int i) ;
    void ExportPDF(int i) ; 
    void ExportTXT(int i) ; 
    void Print() ;

  public:
    string CORARSCollectionTime ;
    string inPath ; // path of thein data files
    string inFileName ; // in file name 
    TFile *inFile ;
    TFile* GlobalInfo ;

    string outPath ; // out path
    string outFileName ; // out file name
    TFile* outFile ; // out root file containing measured time and charge of Scintillor s and ED

    int NEDs ; // totoal number of measured EDs
    string EDName[8] ; // ED name
    string EDSCT1Name[8] ;
    string EDSCT2Name[8] ;
    string EDSCT3Name[8] ;
    string EDSCT4Name[8] ;

    TPaveText* PtCORARS[8] ;
    TPaveText* PtED[8] ;

    TVectorD* SingleRate ;
    TVectorD* NEvents ;
    TVectorD* EDDetEffi ;
    TVectorD* EDTimeRes ;
    TVectorD* EDEnergyRes ;
    TVectorD* EDReEnergyRes ;
    TVectorD* EDSingleParticlePeak ;

    TH1F* EventQuality ;

    TH1I* FiredNStripsUpTGCOne ;
    TH1I* FiredNWiresUpTGCOne ;
    TH1I* FiredNStripsUpTGCTwo ;
    TH1I* FiredNWiresUpTGCTwo ;
    TH1I* FiredNStripsDownTGCOne ;
    TH1I* FiredNWiresDownTGCOne ;
    TH1I* FiredNStripsDownTGCTwo ;
    TH1I* FiredNWiresDownTGCTwo ;

    TH1F* UpTGCOneStripSignalWidth ;
    TH1F* UpTGCOneWireSignalWidth ;
    TH1F* UpTGCTwoStripSignalWidth ;
    TH1F* UpTGCTwoWireSignalWidth ;
    TH1F* DownTGCOneStripSignalWidth ;
    TH1F* DownTGCOneWireSignalWidth ;
    TH1F* DownTGCTwoStripSignalWidth ;
    TH1F* DownTGCTwoWireSignalWidth ;

    TGraph* UpScinHitsGraph ;
    TGraph* DownScinHitsGraph ;
    TH1F* UpScinX ;
    TH1F* DownScinX ;

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

    // Hits distribution
    TH2F *EDPlaneNHits[8] ; //Count all the hits crossing ED Plane within whole ED plane
    TH2F *EDPlaneNFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED Plane within whole ED plane
    TH2F *EDSCT1NHits[8] ; //Count all the hits crossing ED SCT1
    TH2F *EDSCT2NHits[8] ; //Count all the hits crossing ED SCT2
    TH2F *EDSCT3NHits[8] ; //Count all the hits crossing ED SCT3
    TH2F *EDSCT4NHits[8] ; //Count all the hits crossing ED SCT4
    TH2F *EDSCT1NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED SCT1
    TH2F *EDSCT2NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED SCT2
    TH2F *EDSCT3NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED SCT3
    TH2F *EDSCT4NFiredHits[8] ; //Count all the hits above ED threashold (Firred Hits) crossing ED SCT4

    // Scan of detection efficiency
    TH2F *EDPlaneDetEffi2DScan[8] ;
    TH2F *EDSCT1DetEffi2DScan[8] ;
    TH2F *EDSCT2DetEffi2DScan[8] ;
    TH2F *EDSCT3DetEffi2DScan[8] ;
    TH2F *EDSCT4DetEffi2DScan[8] ;
    TH1F *EDSCT1DetEffi1DScan[8] ;
    TH1F *EDSCT2DetEffi1DScan[8] ;
    TH1F *EDSCT3DetEffi1DScan[8] ;
    TH1F *EDSCT4DetEffi1DScan[8] ;

    // Muon Delta TOF
    TH1F *DeltaTOF ;
 
    TGraphErrors* DeltaTimeUDScins ;

    TH1F *EDSCT1CorrectedDeltaTime[8] ;
    TH1F *EDSCT2CorrectedDeltaTime[8] ;
    TH1F *EDSCT3CorrectedDeltaTime[8] ;
    TH1F *EDSCT4CorrectedDeltaTime[8] ;
    TH2F *EDSCT1TimeRes2DScan[8] ;
    TH2F *EDSCT2TimeRes2DScan[8] ;
    TH2F *EDSCT3TimeRes2DScan[8] ;
    TH2F *EDSCT4TimeRes2DScan[8] ;
    TH1F *EDSCT1TimeRes1DScan[8] ;
    TH1F *EDSCT2TimeRes1DScan[8] ;
    TH1F *EDSCT3TimeRes1DScan[8] ;
    TH1F *EDSCT4TimeRes1DScan[8] ;

    TH1F *EDSCT1SignalCharge[8] ;
    TH1F *EDSCT2SignalCharge[8] ;
    TH1F *EDSCT3SignalCharge[8] ;
    TH1F *EDSCT4SignalCharge[8] ;

    TH2F *EDSCT1EnergyRes2DScan[8] ;
    TH2F *EDSCT2EnergyRes2DScan[8] ;
    TH2F *EDSCT3EnergyRes2DScan[8] ;
    TH2F *EDSCT4EnergyRes2DScan[8] ;
    TH1F *EDSCT1EnergyRes1DScan[8] ;
    TH1F *EDSCT2EnergyRes1DScan[8] ;
    TH1F *EDSCT3EnergyRes1DScan[8] ;
    TH1F *EDSCT4EnergyRes1DScan[8] ;
    TH2F *EDSCT1ReEnergyRes2DScan[8] ;
    TH2F *EDSCT2ReEnergyRes2DScan[8] ;
    TH2F *EDSCT3ReEnergyRes2DScan[8] ;
    TH2F *EDSCT4ReEnergyRes2DScan[8] ;
    TH1F *EDSCT1ReEnergyRes1DScan[8] ;
    TH1F *EDSCT2ReEnergyRes1DScan[8] ;
    TH1F *EDSCT3ReEnergyRes1DScan[8] ;
    TH1F *EDSCT4ReEnergyRes1DScan[8] ;
    TH2F *EDSCT1SingleParticlePeak2DScan[8] ;
    TH2F *EDSCT2SingleParticlePeak2DScan[8] ;
    TH2F *EDSCT3SingleParticlePeak2DScan[8] ;
    TH2F *EDSCT4SingleParticlePeak2DScan[8] ;
    TH1F *EDSCT1SingleParticlePeak1DScan[8] ;
    TH1F *EDSCT2SingleParticlePeak1DScan[8] ;
    TH1F *EDSCT3SingleParticlePeak1DScan[8] ;
    TH1F *EDSCT4SingleParticlePeak1DScan[8] ;

    TVectorD* SysTimeRes ;
    TVectorD* ScinTimeRes ;
};

#endif
