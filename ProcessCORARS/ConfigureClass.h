// Dengfeng Zhang dengfeng.zhang@cern.ch
// Singleton Class to configure the package:
//   set the in/out path
//   in/out file name
//   ED height
//   electronics channel connected with ED

#ifndef __ConfigureClass__H
#define __ConfigureClass__H

// c++ includes
#include <iostream>
#include <string>
#include <stdlib.h>  /* exit, EXIT_FAILURE */
// root includes
#include "TFile.h"
#include "TEnv.h"
#include "TString.h"
#include "TSystem.h"
#include "TH1I.h"

using namespace std;
class ConfigureClass
{
  private:
    ConfigureClass();

    //it exists in the full life time of the package
    static ConfigureClass* Singleton_instance; // the only class instance of the singleton class

  public:
    static ConfigureClass* Instance()
    {
      if( Singleton_instance==NULL) Singleton_instance=new ConfigureClass();
      return Singleton_instance;
    }

    void SetProcessStage(int n) ;

    string Temperature ;  // temperature in las
    bool ProcessAllStages ; //process all the stages or not, from the original collected data to final results
    string process_stages ; // specify the running substage
    int process_stage ; // specify the running substage, from 1 to 10

    string inPath ;  // inpath of the original datas collected by CORARS and White Rabbit
    string GetinPath() { return inPath ;}
    string outPath ;  // outpath of the processing stage x , it can also be the inpath of next step
    string GetoutPath() { return outPath ;}

    bool doTimeCalib ; // do time calibration or not
    string CalibrationConfFile ; // configuration for time calibration
    string TimeCalibrationFile ; // resulted calibration file used in time calibration

    bool doLocateED ; // locate ED position or not
    bool useNormalizedCharge ; // normalize charge to the united thickness of ED

    float FuncSlope[8] ; // slope of ED position
    float XTruncation[8] ;
    float YTruncation[8] ;

    string CORARSCollectionTime ; // collection time of CORARS
    string GetCORARSCollectionTime() { return CORARSCollectionTime ; }
    string WRCollectionTime ; // collection time of White Rabbit DAQ with new electronics
    string GetWRCollectionTime() { return WRCollectionTime ; }

    // Trigger SCT Position
    string TSCTFile ;  // file containing position of Trigger SCTs
    vector< std::pair<float, float> > TSCTXY ; // x and y of the bottom left of SCT
    void SetTSCTXY() ; // read the x and y of the bottom left of SCT from TSCTFile
    vector< std::pair<float, float> > GetTSCTXY() { return TSCTXY ; } // get the x and y of TSCT

    int NEDs ; // totoal number of measured EDs
    int GetNEDs() { return NEDs ; }
    string EDName[8] ; // ED name
    string GetEDName(int i) { return EDName[i] ; }
    // name of scintillators in ED
    string EDSCT1Name[8] ;
    string GetEDSCT1Name(int i) { return EDSCT1Name[i] ; }
    string EDSCT2Name[8] ;
    string GetEDSCT2Name(int i) { return EDSCT2Name[i] ; }
    string EDSCT3Name[8] ;
    string GetEDSCT3Name(int i) { return EDSCT3Name[i] ; }
    string EDSCT4Name[8] ;
    string GetEDSCT4Name(int i) { return EDSCT4Name[i] ; }

    float EDZ[8] ; // ED Height
    float GetEDZ(int i) { return EDZ[i] ; }
    int EDV1290NChannel[8] ; // ED V1290N Channel
    int GetEDV1290NChannel(int i) { return EDV1290NChannel[i] ; }
    int EDV792NChannel[8] ; // ED V792N Channel
    int GetEDV792NChannel(int i) { return EDV792NChannel[i] ; }

    //-----------------------------------------
    int UpScinV1290NChannel ; // V1290N Channel Number of Up Scintillator
    int GetUpScinV1290NChannel() { return UpScinV1290NChannel ; }
    int DownScinV1290NChannel ; // V1290N Channel Number of Down Scintillator
    int GetDownScinV1290NChannel() { return DownScinV1290NChannel ; }

    int TriggerID ; // Electrinics Pad ID of Trigger
    int GetTriggerID() { return TriggerID ; }
    int UpScinID ;  // Electrinics Pad ID of Up Scintillator
    int GetUpScinID() { return UpScinID ; }
    int DownScinID ; // Electrinics Pad ID of Down Scintillator
    int GetDownScinID() { return DownScinID ; }
    int EDID[8] ; // Electrinics Pad ID of EDs
    int GetEDID(int i) { return EDID[i] ; }

    string NormalEventQualityFileName ;
    TFile* NormalEventQualityFile ;
    TH1F* NormalEventQuality ;
} ;
#endif
