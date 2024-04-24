#ifndef ChargeCorrectTime_H
#define ChargeCorrectTime_H

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"

#include <iostream>
#include <fstream>
#include "StructDef.h"
#include "ConfigureClass.h"

using namespace std ;
class ChargeCorrectTime
{
  private:

    ConfigureClass *conf ;

  public:

    ChargeCorrectTime() ;
    ~ChargeCorrectTime() ;
    void Initialize() ;
    void Correct() ;

    // informations of in files
    string inPath ; // path of thein data files
    string inFileName ; // in file name 
    TFile *inFile ;
    TTree *inTree ;

    // informations of out files
    string outPath ; // out path
    string outFileName ; // out file name
    TFile* outFile ; // out root file containing measured time and charge of Scintillor s and ED

    int NEDs ;
    string EDName[8] ;

    ED ElecDet[8] ;

    TH1F *ChargeDeltaTime[8] ;
    TH1F *DeltaTime[8][200] ;
    //TH1F *DeltaTime[8][55] ;
};
#endif
