#ifndef __ConfigureClass__CXX
#define __ConfigureClass__CXX
#include "ProcessCORARS/ConfigureClass.h"

ConfigureClass* ConfigureClass::Singleton_instance=NULL ;

using namespace std;

ConfigureClass::ConfigureClass()
{
  // all env variables in steering file
  TEnv* env = new TEnv("ProcessCORARS/Config/steering.env") ;
  // collection time
  CORARSCollectionTime = env->GetValue("CORARSCollectionTime","") ; // Collection Time by CORARS DAQ
  WRCollectionTime = env->GetValue("WRCollectionTime","");//Collection Time by White Rabbit DAQ
  // inpath of raw data file and main outpath of output
  inPath = env->GetValue("inPath","") ;
  outPath = env->GetValue("outPath","") ;

  Temperature = env->GetValue("Temperature", "") ;

  ProcessAllStages = env->GetValue("ProcessAllStages", false) ;

  // get the specified sub stage number
  if(!ProcessAllStages)
    process_stages =  env->GetValue("ProcessStages", "") ;

  UpScinV1290NChannel = env->GetValue("UpScinV1290NChannel", -1) ;
  DownScinV1290NChannel = env->GetValue("DownScinV1290NChannel", -1) ;
  if(UpScinV1290NChannel==-1||DownScinV1290NChannel==-1)
  {
    cout<<"No V1290N Channel is assigned to Scintillator!! Exit."<<endl ;
    exit(0) ;
  }

  TriggerID = env->GetValue("TriggerID", -1) ;
  UpScinID = env->GetValue("UpScinID", -1) ;
  DownScinID = env->GetValue("DownScinID", -1) ;
  if(TriggerID==-1||UpScinID==-1||DownScinID==-1)
  {
    cout<<"No Electrinucs Pad is assigned to Scintillator!! Exit."<<endl ;
    exit(0) ;
  }

  doLocateED = env->GetValue("doLocateED", false) ;
  NEDs = env->GetValue("NEDs", 8) ; // N of meausred EDs
  for(int i=0 ; i<NEDs ; i++)
  {
    // get ED name, height, position,  and channel number connected to ED
    EDName[i] = env->GetValue(TString::Format("ED%i",i),"") ;
    EDSCT1Name[i] = env->GetValue(TString::Format("ED%iSCT1",i),"") ;
    EDSCT2Name[i] = env->GetValue(TString::Format("ED%iSCT2",i),"") ;
    EDSCT3Name[i] = env->GetValue(TString::Format("ED%iSCT3",i),"") ;
    EDSCT4Name[i] = env->GetValue(TString::Format("ED%iSCT4",i),"") ;

    EDZ[i] = env->GetValue(TString::Format("ED%iZ",i), 0.) ;

    EDV1290NChannel[i] = env->GetValue(TString::Format("ED%iV1290NChannel",i), -1) ;
    EDV792NChannel[i] = env->GetValue(TString::Format("ED%iV792NChannel",i), -1) ;
    EDID[i] = env->GetValue(TString::Format("ED%iID",i), -1) ; // Electronics Pad ID of ED

    if(!doLocateED)
    {
      FuncSlope[i] = env->GetValue(TString::Format("FuncSlope%i",i), 0.) ;
      XTruncation[i] = env->GetValue(TString::Format("XTruncation%i",i), 0.) ;
      YTruncation[i] = env->GetValue(TString::Format("YTruncation%i",i), 0.) ;
    }
  }

  NormalEventQualityFileName = env->GetValue("NormalEventQualityFileName", "") ;
  NormalEventQualityFile = new TFile(NormalEventQualityFileName.c_str(), "read") ;
  NormalEventQuality = (TH1F*) NormalEventQualityFile->Get("EventQuality") ;
  NormalEventQuality->SetTitle("Normal") ;

  doTimeCalib = env->GetValue("doTimeCalib", false) ;

  useNormalizedCharge = env->GetValue("useNormalizedCharge", false) ;

  if(doTimeCalib) 
  {
    process_stage = 0 ;
    CalibrationConfFile = env->GetValue("CalibrationConfFile","") ;
  }
  else
    TimeCalibrationFile = env->GetValue("TimeCalibrationFile", "") ;

  TSCTFile = env->GetValue("TSCTFile", "") ;
  if(TSCTFile=="")
  {
    cout<<"No Trigger SCT File !!!!"<<endl ;
    exit(0) ;
  }
  else
    SetTSCTXY() ;
}

void ConfigureClass::SetTSCTXY()
{
  TEnv* TSCTConfig = new TEnv(TSCTFile.c_str()) ;
  if(!TSCTConfig)
  {
    cout<<"Cannot open Trigger SCT configuration file: "<<TSCTFile<<".\n";
    exit(0) ;
  }
  else
  {
    for(int index=0 ; index<12 ; index++)
    {
      float x = TSCTConfig->GetValue(TString::Format("TSCT%iX", index), 0.) ;
      float y = TSCTConfig->GetValue(TString::Format("TSCT%iY", index), 0.) ;
      TSCTXY.push_back(std::make_pair(x, y)) ;
    }
  }
  return ;
}

void ConfigureClass::SetProcessStage(int n)
{
  process_stage = n ;
}

#endif
