#ifndef __PedestalDrift__H
#define __PedestalDrift__H

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include "ConfigureClass.h"

using namespace std ;
class PedestalDrift 
{
private:
  // global variables
  ConfigureClass* conf;
public :
  PedestalDrift() ;
  ~PedestalDrift() ;
  void Initialize() ;
  void GetDrift() ;

  TFile *InPut_File ;
  TTree *InPut_Tree ;
  TFile *OutPut_File ;
  UInt_t event_id ;
  UInt_t event_time ;

  int NtimeBin ;
};
#endif
