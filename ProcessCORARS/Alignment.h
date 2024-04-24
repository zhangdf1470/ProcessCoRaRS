//a Author: Dengfeng Zhang dengfeng.zhang@cern.ch
// Extract time amd charge of scintilltor and EDs collected by DAQ with new electronics

#ifndef Alignment_H
#define Alignment_H

// c++ includes
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>  /* exit, EXIT_FAILURE */

// root includes
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TROOT.h"
#include "TEnv.h"
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TClonesArray.h"

// local includes
#include "ConfigureClass.h"
#include "KM2AEvent.h"

using namespace std;

class Alignment
{
  private: 
    ConfigureClass* conf;

    // informations of in files
    string inPath ; // path of thein data files
    string inrawfilename ; // in file name collected by White Babbit DAQ
    string indaqfilename ; // in file name collected by DAQ with new electronics 
    TFile* inRawFile ; // in root file of CORARS DAQ
    TTree* inRawTree ;
    //TFile* inDAQFile ; // in root file of White Babbit DAQ
    TChain* inDAQChain ;

    // informations of out files
    string outPath ; // out path
    string outfilename ; // out file name
    TFile* outFile ; // out root file containing measured time and charge of Scintillor s and EDs
    TTree* outTree ;

    UInt_t event_id ; // event id 
    UInt_t event_time ;// event time
    UInt_t V1190A_1_LeadingMeasure[512] ; // leading and trailing signal measured by V1190A
    UInt_t V1190A_1_TrailingMeasure[512] ;
    UInt_t V1190A_2_LeadingMeasure[512] ;
    UInt_t V1190A_2_TrailingMeasure[512] ;
    UInt_t V1290N_measure[16] ; // time  measured by V1290N
    UInt_t V792N_measure[16] ;  // charge measured by V792N
    float KM2ACharge[16] ; // charge measured by new electronics pad
    float KM2ATime[16] ; // time measured by new electronics pad

    KM2AEvent* pevent ; // event of White Rabbit

    int UpScinV1290NChannel ; // V1290N Channel Number of Up Scintillator
    int DownScinV1290NChannel ; // V1290N Channel Number of Down Scintillator
    int EDV1290NChannel[8] ; // ED V1290N Channel
    int EDV792NChannel[8] ; // ED V792N Channel

    int NEDs ; // Number of measured EDs
    int TriggerID ; // Electrinics Pad ID of Trigger
    int UpScinID ;  // Electrinics Pad ID of Up Scintillator 
    int UpScinHitIndex ;
    int DownScinID ; // Electrinics Pad ID of Down Scintillator
    int DownScinHitIndex ;
    int EDID[8] ; // Electrinics Pad ID of EDs
    int NEDNoiseHit[8] ; // Number of noise hits before trigger
    int EDHitIndex[8] ;

    bool isCORARSAhead ; // true: CORARS records event first, false: White Rabbit records event first
    int NMisalignment ; // N of event of misalignment between CORARAS and White Babbit
    // there can be many misalignments suring the collection
    float DeltaTOFMean ; 
    float DeltaTOFSigma ; 

  public:
    Alignment() ;
    ~Alignment() ;

    int Initialize() ;
    int GetNMisalignment(int CORARSSite, int WTSite, int MaxNMisAlignment, int NLast, bool CORARSAhead) ; // get the N of event of misalignment between CORARAS and White Babbit
    /*
    int GetEDHitIndex(KM2AEvent* pevent, int ID)
    {
      TClonesArray* array = pevent->Hitlist() ;
      int index = -1 ;
      float MaxCharge = -10000 ;
      for(int j=0 ; j<pevent->NHit()+pevent->Nmd();j++)
      {
        KM2AHit* edhit = (KM2AHit* )array->ConstructedAt(j) ;
        if((int)edhit->ID()==ID)
        {
          if(((float)(edhit->Qa())-(float)(edhit->Peda()))>MaxCharge)
          {
            MaxCharge = (float)(edhit->Qa())-(float)(edhit->Peda()) ;
            index = j ;
          }
        }
      }
      return index ; 
    }
    int GetSCTHitIndex(KM2AEvent* pevent, int ID)
    {
      TClonesArray* array = pevent->Hitlist() ;
      int index = -1 ;
      float MaxDeltaTime = 10000. ;
      for(int j=0 ; j<pevent->NHit()+pevent->Nmd();j++)
      {
        KM2AHit* upscinhit = (KM2AHit* )array->ConstructedAt(j) ;
        if((int)upscinhit->ID()==ID)
        {
          NUpScinHit++ ;
          if(abs((float)(upscinhit->Time())-5000.)<MaxDeltaTime)
          {
            MaxDeltaTime = abs((float)(upscinhit->Time())-5000.) ;
            index = j ;
          }
        }
      }
      return index ;
    }*/
    // Get the index of the first hit of ID
    int GetHitIndex(KM2AEvent* pevent, int ID) ;
    // Get the number of hits of ID
    int GetNHits(KM2AEvent* pevent, int ID) ;
    // Get the time vector of hits of ID
    int GetEDNNoise(KM2AEvent* pevent, int ID) ;

    int Align() ;

};

#endif
