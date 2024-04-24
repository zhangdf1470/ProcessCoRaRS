#ifndef __CalcuTGCScin__H
#define __CalcuTGCScin__H

// c++/c includes
#include <istream>
#include <fstream>
#include <iostream>
#include <vector>
// root includes
#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TString.h"
#include "TMath.h"
#include "TGraph.h"
#include "TPad.h"
#include "TBenchmark.h"
#include "TNamed.h"
// local includes
#include "Cluster.h"
#include "StructDef.h"
#include "ConfigureClass.h"

using namespace std ;
class CalcuTGCScin
{
  public:

    CalcuTGCScin() ;
    ~CalcuTGCScin() ;
    void Initialize() ;
    void GetClusters(UInt_t Leading[32] , UInt_t Trailing[32] , vector<Cluster> *vectorcluster) ;
    void GetAllClusters() ;
    void GetClusterValidNumbers(vector<Cluster> vectorcluster, vector<int> *vectorvalidnumber) ;
    int GetValidNumber(vector<Cluster> valid) ;
    bool GetValidNumbers() ;
    void Calcu_TGC() ;
    bool Calcu_Scin() ;
    void Calcu() ;
    void recalculate();
    void recalculate1();
    void Clear() ;
    int getValidLineNumber( vector<int> vec1, vector<int> vec2 );
    void eventAnalysis();

  private:
    // global variables
    ConfigureClass* conf;

  public:

    string inPath ;
    string inFileName ;
    TFile *inFile ;
    TTree *inTree ;

    string outPath ;
    string outFileName ;
    TFile *outFile ;
    TTree *outTree ;

    UInt_t Up_TGC_Strip_Leading_One[32] ;
    UInt_t Up_TGC_Strip_Trailing_One[32] ;
    UInt_t Up_TGC_Wire_Leading_One[32] ;
    UInt_t Up_TGC_Wire_Trailing_One[32] ;
    UInt_t Down_TGC_Strip_Leading_One[32] ;
    UInt_t Down_TGC_Strip_Trailing_One[32] ;
    UInt_t Down_TGC_Wire_Leading_One[32] ;
    UInt_t Down_TGC_Wire_Trailing_One[32] ;

    UInt_t Up_TGC_Strip_Leading_Two[32] ;
    UInt_t Up_TGC_Strip_Trailing_Two[32] ;
    UInt_t Up_TGC_Wire_Leading_Two[32] ;
    UInt_t Up_TGC_Wire_Trailing_Two[32] ;
    UInt_t Down_TGC_Strip_Leading_Two[32] ;
    UInt_t Down_TGC_Strip_Trailing_Two[32] ;
    UInt_t Down_TGC_Wire_Leading_Two[32] ;
    UInt_t Down_TGC_Wire_Trailing_Two[32] ;

    vector< std::pair<float, float> > TSCTXY ; // x and y of the bottom left of SCT

    int UpScinV1290NChannel ; // V1290N Channel of Up SCT
    int DownScinV1290NChannel ; // V1290N Channel of Down SCT

    float KM2ACharge[16] ; // charge measured by new electronics pad
    float KM2ATime[16] ; // time measured by new electronics pad
    int NEDNoiseHit[8] ; // Number of noise hits before trigger

    int Count_Up_Strip_One ;
    int Count_Up_Wire_One  ;
    int Count_Down_Strip_One ;
    int Count_Down_Wire_One ;
    int Count_Up_Strip_Two ;
    int Count_Up_Wire_Two  ;
    int Count_Down_Strip_Two ;
    int Count_Down_Wire_Two ;
    float params[189] ;

    UInt_t event_id ;
    UInt_t event_time ;
    TGC Up_TGC_One ;
    TGC Down_TGC_One ;
    TGC Up_TGC_Two ;
    TGC Down_TGC_Two ;
    Point Up_TGC ;
    Point Down_TGC ;
    Scin Up_Scin ;
    Scin Down_Scin ;

    vector<Cluster> Up_TGC_Strip_One_Cluster ;
    vector<Cluster> Up_TGC_Wire_One_Cluster ;
    vector<Cluster> Down_TGC_Strip_One_Cluster ;
    vector<Cluster> Down_TGC_Wire_One_Cluster ;
    vector<Cluster> Up_TGC_Strip_Two_Cluster ;
    vector<Cluster> Up_TGC_Wire_Two_Cluster ;
    vector<Cluster> Down_TGC_Strip_Two_Cluster ;
    vector<Cluster> Down_TGC_Wire_Two_Cluster ;
    //store size of every cluster
    vector<int> up_one_strip_cluster_size ;
    vector<int> up_one_wire_cluster_size ;
    vector<int> up_two_strip_cluster_size ;
    vector<int> up_two_wire_cluster_size ;
    vector<int> down_one_strip_cluster_size ;
    vector<int> down_one_wire_cluster_size ;
    vector<int> down_two_strip_cluster_size ;
    vector<int> down_two_wire_cluster_size ;
    //store valid number every cluster
    vector<int> up_one_strip_cluster_validnumber ;
    vector<int> up_one_wire_cluster_validnumber ;
    vector<int> up_two_strip_cluster_validnumber ;
    vector<int> up_two_wire_cluster_validnumber ;
    vector<int> down_one_strip_cluster_validnumber ;
    vector<int> down_one_wire_cluster_validnumber ;
    vector<int> down_two_strip_cluster_validnumber ;
    vector<int> down_two_wire_cluster_validnumber ;
    //store the max signal of every cluster
    vector<int> up_one_strip_cluster_maxsignal ;
    vector<int> up_one_wire_cluster_maxsignal ;
    vector<int> up_two_strip_cluster_maxsignal ;
    vector<int> up_two_wire_cluster_maxsignal ;
    vector<int> down_one_strip_cluster_maxsignal ;
    vector<int> down_one_wire_cluster_maxsignal ;
    vector<int> down_two_strip_cluster_maxsignal ;
    vector<int> down_two_wire_cluster_maxsignal ;

    int Delta_Up_Strip_ValidNumber ;
    int Delta_Up_Wire_ValidNumber ;
    int Delta_Down_Strip_ValidNumber ;
    int Delta_Down_Wire_ValidNumber ;
    //label the quality of evnet 
    int EventQuality ;
    int nosignal;

    // histogram to store EventQuality
    TH1F* EventQualityDis ;
    // signal width
    TH1F* UpTGCOneStripSignalWidth ;
    TH1F* UpTGCOneWireSignalWidth ;
    TH1F* UpTGCTwoStripSignalWidth ;
    TH1F* UpTGCTwoWireSignalWidth ;
    TH1F* DownTGCOneStripSignalWidth ;
    TH1F* DownTGCOneWireSignalWidth ;
    TH1F* DownTGCTwoStripSignalWidth ;
    TH1F* DownTGCTwoWireSignalWidth ;
    // Scatter graph of Scintilltor
    TGraph* UpScinHitsGraph ;
    TGraph* DownScinHitsGraph ;
    TH1F* UpScinX ;
    TH1F* DownScinX ;
};
#endif
