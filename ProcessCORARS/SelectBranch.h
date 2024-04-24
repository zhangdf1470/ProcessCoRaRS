#ifndef __SelectBranch__H
#define __SelectBranch__H

#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TSystem.h"
#include "TH1I.h"
#include "TNamed.h"
#include "TVectorD.h"

#include "ConfigureClass.h"

// Selcet the used Branch and rename them
class SelectBranch
{
  public:
    SelectBranch() ;
    ~SelectBranch() ;
    void Initialize() ;
    void Count() ;
    void Select() ;

  private:
    // global configuration variables
    ConfigureClass* conf;

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
    TTree* outTree ;

    UInt_t event_id ;
    UInt_t event_time ;
    UInt_t TGC_Leading_One[512] ;
    UInt_t TGC_Trailing_One[512] ;
    UInt_t TGC_Leading_Two[512] ;
    UInt_t TGC_Trailing_Two[512] ;
    //UInt_t Scin_ED_Time_Signal[16] ;
    //float ED_Charge_Signal[16] ; // chaged from UInt_t to float // charge collected by CORARS or White Rabbit
    float KM2ACharge[16] ; // charge measured by new electronics pad
    float KM2ATime[16] ; // time measured by new electronics pad
    int NEDNoiseHit[8] ; // Number of noise hits before trigger


    // N of fired strips or wires
    int Count_Up_Strip_One ; 
    int Count_Up_Wire_One  ;
    int Count_Down_Strip_One ;
    int Count_Down_Wire_One ;
    int Count_Up_Strip_Two ; 
    int Count_Up_Wire_Two  ;
    int Count_Down_Strip_Two ;
    int Count_Down_Wire_Two ;

    TH1I* FiredNStripsUpTGCOne ;
    TH1I* FiredNWiresUpTGCOne ;
    TH1I* FiredNStripsUpTGCTwo ;
    TH1I* FiredNWiresUpTGCTwo ;
    TH1I* FiredNStripsDownTGCOne ;
    TH1I* FiredNWiresDownTGCOne ;
    TH1I* FiredNStripsDownTGCTwo ;
    TH1I* FiredNWiresDownTGCTwo ;
};
#endif
