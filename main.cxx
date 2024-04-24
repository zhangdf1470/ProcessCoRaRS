#include <iostream>
#include <string>
#include <sstream>
#include "./ProcessCORARS/ProcessCORARS/ConfigureClass.h"
#include "./ProcessCORARS/ProcessCORARS/Alignment.h"
#include "./ProcessCORARS/ProcessCORARS/SelectBranch.h"
#include "./ProcessCORARS/ProcessCORARS/SelectBranch.h"
#include "./ProcessCORARS/ProcessCORARS/CalcuVariables.h"
#include "./ProcessCORARS/ProcessCORARS/AnalyseED.h"
#include "./ProcessCORARS/ProcessCORARS/ChargeCorrectTime.h"
#include "./ProcessCORARS/ProcessCORARS/GenerateTimeCalib.h"
#include "./ProcessCORARS/ProcessCORARS/ReGenerateTimeCalib.h"
#include "./ProcessCORARS/ProcessCORARS/SimulateTimeCalib.h"
#include "./ProcessCORARS/ProcessCORARS/ComDiscriminator.h"
#include "./ProcessCORARS/ProcessCORARS/CalcuTGCScin.h"
#include "./ProcessCORARS/ProcessCORARS/LocateED.h"
#include "./ProcessCORARS/ProcessCORARS/PrintCORARS.h"
#include "./ProcessCORARS/ProcessCORARS/ReCalcuVariables.h"

using namespace std;
int main()
{
  // count running cpu time
  TStopwatch t  ;
  t.Start() ;

  int stage  ; 
  cout<<"The process of analysing ED consists of nine stages:"<<endl ;
  cout<<"Stage 0: Extract useful variables ;"<<endl ;
  cout<<"Stage 1: Select the branches we will use ;"<<endl ;
  cout<<"Stage 2: Calculate hitting point on TGC and Scintillation ;"<<endl ;
  cout<<"Stage 3: Get pedestal drift ;"<<endl ;
  //cout<<"Needn't to run calibration stages 4/5/6 every time, run one time after some time:"<<endl ;
  //cout<<"Stage : Get the calibration of scintillators ;"<<endl ;
  //cout<<"Stage : Simulate the relationship between calibration and distance from hit point to PMT by a poly5 function;"<<endl ;
  //cout<<"Stage : Get the new calibration using the results from Stage 3&4 ;"<<endl ;
  cout<<"Stage 4: Calculate all the variables that we will use in next stages and name them by the characterics we can understand ;"<<endl ;
  cout<<"Stage 5: Get ED Position ;"<<endl ;
  cout<<"Stage 6: Correct the time difference between measured time and reconstructed time ;"<<endl ;
  cout<<"Stage 7: ReCalculate Variables considering all corrections ;"<<endl ;
  cout<<"Stage 8: Fill histograms and get the results by fitting or other methods, finally output PDF files ;"<<endl ;

  // create the instance of configuration class
  static ConfigureClass*  conf=ConfigureClass::Instance();

  if(conf->doTimeCalib)
  {
    GenerateTimeCalib *MyGTC = new GenerateTimeCalib() ;
    MyGTC->Generate() ;
    SimulateTimeCalib *MySTC = new SimulateTimeCalib() ;
    MySTC->Simulate() ;
    ReGenerateTimeCalib *MyRGTC = new ReGenerateTimeCalib() ;
    MyRGTC->ReGenerate() ;
    return 0 ;
  }

  string ProcessStages ;
  if(conf->ProcessAllStages)
    ProcessStages = "012345678" ;
  else
  {
    ProcessStages = conf->process_stages ;
  }
  for(int i=0 ; i<ProcessStages.size() ; i++)
  {
    int stage=0 ;
    istringstream ss(ProcessStages.substr(i,1)) ;
    ss  >> stage ;
    conf->SetProcessStage(stage) ;
    switch(conf->process_stage)
    {
      case 0:
      {
        Alignment* MyAlign = new Alignment() ;
        MyAlign->Align() ;
        delete MyAlign ;
        break ;
      }
      case 1:
      {
        SelectBranch *MySelect = new SelectBranch() ;
        MySelect->Select() ;
        delete MySelect;
        break ;
      }
      case 2:
      {
        CalcuTGCScin *MyCTS = new CalcuTGCScin() ;
        MyCTS->Calcu() ;
        delete MyCTS ;
        break ;
      }
      case 3:
      {
        CalcuVariables *MyCV = new CalcuVariables() ;
        MyCV->Calcu() ;
        delete MyCV ;
        break ;
      }
      case 4:
      {
        LocateED *MyLocateED = new LocateED() ;
        MyLocateED->FindED() ;
        delete MyLocateED ;
        break ;
      }
      case 5:
      {
        ChargeCorrectTime *MyCCT = new ChargeCorrectTime() ;
        MyCCT->Correct() ;
        delete MyCCT ;
        break ;
      }
      case 6:
      {
        ReCalcuVariables *MyReCalcu = new ReCalcuVariables() ;
        MyReCalcu->ReCalcu() ;
        delete MyReCalcu ;
        break ;
      }
      case 7:
      {
        AnalyseED *MyAna = new AnalyseED() ;
        MyAna->Analyse() ;
        delete MyAna ;
        break ;
      }
      case 8:
      {
        PrintCORARS *MyPrint = new PrintCORARS() ;
        MyPrint->Print() ;
        delete MyPrint ;
        break ;
      }
      default:
        break ;
    }
  }
  t.Stop() ;
  t.Print() ;

  return 0;
}
