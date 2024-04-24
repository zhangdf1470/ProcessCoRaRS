#ifndef SelectBranch_CXX
#define SelectBranch_CXX

#include "ProcessCORARS/SelectBranch.h"

using namespace std;
SelectBranch::SelectBranch()
{
  inPath = "" ;
  outPath = "" ;
  inFileName = "" ;
  outFileName = "" ;
  inFile = 0 ;
  inTree = 0 ;
  outFile = 0 ;
  outTree = 0 ;

  Initialize() ;
}
SelectBranch::~SelectBranch()
{
  delete inTree ;
  delete inFile ;
  delete outTree ;
  delete outFile ;
}

void SelectBranch::Initialize()
{
  // get configurations
  conf = ConfigureClass::Instance();

  inPath = conf->inPath ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  inFileName = "AlignedRaw_"+conf->CORARSCollectionTime+".root" ;
  inFile = new TFile((inPath+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"In File: "<<inPath<<inFileName<<" doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  cout<<"Input File: "<<inPath+inFileName<<endl ;
  inTree = (TTree*)(inFile->Get("RawData")) ;
  if(!inTree)
  {
    cout<<"RawData Tree doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  inTree->SetBranchAddress("event_id",&event_id) ;
  inTree->SetBranchAddress("event_time",&event_time) ;
  inTree->SetBranchAddress("V1190A_1_LeadingMeasure",TGC_Leading_One) ;
  inTree->SetBranchAddress("V1190A_1_TrailingMeasure",TGC_Trailing_One) ;
  inTree->SetBranchAddress("V1190A_2_LeadingMeasure",TGC_Leading_Two) ;
  inTree->SetBranchAddress("V1190A_2_TrailingMeasure",TGC_Trailing_Two) ;
  inTree->SetBranchAddress("KM2ATime",KM2ATime);
  inTree->SetBranchAddress("KM2ACharge",KM2ACharge);
  inTree->SetBranchAddress("NEDNoiseHit",NEDNoiseHit);

  outPath = conf->outPath+"/"+conf->CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(outPath.c_str()))
    gSystem->Exec(("mkdir "+outPath).c_str()) ;
  gSystem->Exec(("cp ProcessCORARS/Config/steering.env "+ outPath).c_str()) ;
  outFileName = "Stage_One_"+conf->CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
  cout<<"Output File: "<<outFile->GetName() <<endl ;
  outTree = new TTree("UsedVariables","UsedVariables") ;
  outTree->SetDirectory(outFile) ;
  //
  outTree->Branch("event_id",&event_id,"event_id/i") ;
  outTree->Branch("event_time",&event_time,"event_time/i") ;
//  exchange leading and trailing signal of strip 
//  Do Not Change
  outTree->Branch("Up_TGC_Strip_Leading_One",TGC_Trailing_One,
                                                 "Up_TGC_Strip_Leading_One[32]/i");
  outTree->Branch("Up_TGC_Strip_Trailing_One",TGC_Leading_One,
                                                 "Up_TGC_Strip_Trailing_One[32]/i");
  outTree->Branch("Up_TGC_Wire_Leading_One",&TGC_Leading_One[32],
                                                 "Up_TGC_Wire_Leading_One[32]/i");
  outTree->Branch("Up_TGC_Wire_Trailing_One",&TGC_Trailing_One[32],
                                                 "Up_TGC_Wire_Trailing_One[32]/i");
  outTree->Branch("Down_TGC_Strip_Leading_One",&TGC_Trailing_One[64],
                                                  "Down_TGC_Strip_Leading_One[32]/i");
  outTree->Branch("Down_TGC_Strip_Trailing_One",&TGC_Leading_One[64],
                                                  "Down_TGC_Strip_Trailing_One[32]/i");
  outTree->Branch("Down_TGC_Wire_Leading_One",&TGC_Leading_One[96],
                                                  "Down_TGC_Wire_Leading_One[32]/i");
  outTree->Branch("Down_TGC_Wire_Trailing_One",&TGC_Trailing_One[96],
                                                  "Down_TGC_Wire_Trailing_One[32]/i");
//--------------------------Two-----------------------------------------------------
  outTree->Branch("Up_TGC_Strip_Leading_Two",TGC_Trailing_Two,
                                                 "Up_TGC_Strip_Leading_Two[32]/i");
  outTree->Branch("Up_TGC_Strip_Trailing_Two",TGC_Leading_Two,
                                                 "Up_TGC_Strip_Trailing_Two[32]/i");
  outTree->Branch("Up_TGC_Wire_Leading_Two",&TGC_Leading_Two[32],
                                                 "Up_TGC_Wire_Leading_Two[32]/i");
  outTree->Branch("Up_TGC_Wire_Trailing_Two",&TGC_Trailing_Two[32],
                                                 "Up_TGC_Wire_Trailing_Two[32]/i");
  outTree->Branch("Down_TGC_Strip_Leading_Two",&TGC_Trailing_Two[64],
                                                  "Down_TGC_Strip_Leading_Two[32]/i");
  outTree->Branch("Down_TGC_Strip_Trailing_Two",&TGC_Leading_Two[64],
                                                  "Down_TGC_Strip_Trailing_Two[32]/i");
  outTree->Branch("Down_TGC_Wire_Leading_Two",&TGC_Leading_Two[96],
                                                  "Down_TGC_Wire_Leading_Two[32]/i");
  outTree->Branch("Down_TGC_Wire_Trailing_Two",&TGC_Trailing_Two[96],
                                                  "Down_TGC_Wire_Trailing_Two[32]/i");
  outTree->Branch("KM2ATime",KM2ATime,"KM2ATime[16]/F");
  outTree->Branch("KM2ACharge",KM2ACharge,"KM2ACharge[16]/F");
  outTree->Branch("NEDNoiseHit",NEDNoiseHit,"NEDNoiseHit[8]/I");

  outTree->Branch("Count_Up_Strip_One",&Count_Up_Strip_One,"Count_Up_Strip_One/I");
  outTree->Branch("Count_Up_Wire_One",&Count_Up_Wire_One,"Count_Up_Wire_One/I");
  outTree->Branch("Count_Down_Strip_One",&Count_Down_Strip_One,"Count_Down_Strip_One/I");
  outTree->Branch("Count_Down_Wire_One",&Count_Down_Wire_One,"Count_Down_Wire_One/I");
  outTree->Branch("Count_Up_Strip_Two",&Count_Up_Strip_Two,"Count_Up_Strip_Two/I");
  outTree->Branch("Count_Up_Wire_Two",&Count_Up_Wire_Two,"Count_Up_Wire_Two/I");
  outTree->Branch("Count_Down_Strip_Two",&Count_Down_Strip_Two,"Count_Down_Strip_Two/I");
  outTree->Branch("Count_Down_Wire_Two",&Count_Down_Wire_Two,"Count_Down_Wire_Two/I");

  //initialize hitograms
  FiredNStripsUpTGCOne = new TH1I("FiredNStripsUpTGCOne", "N Fired Strips of Up TGC One", 32, 0, 32) ;
  FiredNWiresUpTGCOne = new TH1I("FiredNWiresUpTGCOne", "N Fired Wires of Up TGC One", 32, 0, 32) ;
  FiredNStripsUpTGCTwo = new TH1I("FiredNStripsUpTGCTwo", "N Fired Strips of Up TGC Two", 32, 0, 32) ;
  FiredNWiresUpTGCTwo = new TH1I("FiredNWiresUpTGCTwo", "N Fired Wires of Up TGC Two", 32, 0, 32) ;
  FiredNStripsDownTGCOne = new TH1I("FiredNStripsDownTGCOne", "N Fired Strips of Down TGC One", 32, 0, 32) ;
  FiredNWiresDownTGCOne = new TH1I("FiredNWiresDownTGCOne", "N Fired Wires of Down TGC One", 32, 0, 32) ;
  FiredNStripsDownTGCTwo = new TH1I("FiredNStripsDownTGCTwo", "N Fired Strips of Down TGC Two", 32, 0, 32) ;
  FiredNWiresDownTGCTwo = new TH1I("FiredNWiresDownTGCTwo", "N Fired Wires of Down TGC Two", 32, 0, 32) ;
  FiredNStripsUpTGCOne->SetDirectory(0) ;
  FiredNWiresUpTGCOne->SetDirectory(0) ;
  FiredNStripsUpTGCTwo->SetDirectory(0) ;
  FiredNWiresUpTGCTwo->SetDirectory(0) ;
  FiredNStripsDownTGCOne->SetDirectory(0) ;
  FiredNWiresDownTGCOne->SetDirectory(0) ;
  FiredNStripsDownTGCTwo->SetDirectory(0) ;
  FiredNWiresDownTGCTwo->SetDirectory(0) ;
}
void SelectBranch::Count()
{
  Count_Up_Strip_One = 0 ; Count_Up_Wire_One = 0 ;
  Count_Down_Strip_One = 0 ; Count_Down_Wire_One = 0 ;
  Count_Up_Strip_Two = 0 ; Count_Up_Wire_Two = 0 ;
  Count_Down_Strip_Two = 0 ; Count_Down_Wire_Two = 0 ;
  
  for(int i=0 ; i<32 ; i++)
    {
      if(TGC_Leading_One[i]!=0&&TGC_Trailing_One[i]!=0)
        Count_Up_Strip_One++ ;
      if(TGC_Leading_Two[i]!=0&&TGC_Trailing_Two[i]!=0)
        Count_Up_Strip_Two++ ;
    }
  for(int i=32 ; i<64 ; i++)
    {
      if(TGC_Leading_One[i]!=0&&TGC_Trailing_One[i]!=0)
        Count_Up_Wire_One++ ;
      if(TGC_Leading_Two[i]!=0&&TGC_Trailing_Two[i]!=0)
        Count_Up_Wire_Two++ ;
    }
  for(int i=64 ; i<96 ; i++)
    {
      if(TGC_Leading_One[i]!=0&&TGC_Trailing_One[i]!=0)
        Count_Down_Strip_One++ ;
      if(TGC_Leading_Two[i]!=0&&TGC_Trailing_Two[i]!=0)
        Count_Down_Strip_Two++ ;
    }
  for(int i=96 ; i<128 ; i++)
    {
      if(TGC_Leading_One[i]!=0&&TGC_Trailing_One[i]!=0)
        Count_Down_Wire_One++ ;
      if(TGC_Leading_Two[i]!=0&&TGC_Trailing_Two[i]!=0)
        Count_Down_Wire_Two++ ;
    }
}
void SelectBranch::Select()
{
//-------------------------------------------------------------------

  int nEntries = inTree->GetEntries() ;
  cout<<"Total events: "<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
  {
    if(i%10000==0)
      cout<<i<<endl ;
    inTree->GetEntry(i) ;
    Count() ;
    outTree->Fill() ;
    FiredNStripsUpTGCOne->Fill(Count_Up_Strip_One) ;
    FiredNWiresUpTGCOne->Fill(Count_Up_Wire_One) ;
    FiredNStripsUpTGCTwo->Fill(Count_Up_Strip_Two) ;
    FiredNWiresUpTGCTwo->Fill(Count_Up_Wire_Two) ;
    FiredNStripsDownTGCOne->Fill(Count_Down_Strip_One) ;
    FiredNWiresDownTGCOne->Fill(Count_Down_Wire_One) ;
    FiredNStripsDownTGCTwo->Fill(Count_Down_Strip_Two) ;
    FiredNWiresDownTGCTwo->Fill(Count_Down_Wire_Two) ;
  }
  outFile->Write() ;

  // store global informations to a pavetext
  TFile* GlobalInfo = new TFile((outPath+"/GlobalInfo_"+conf->CORARSCollectionTime+".root").c_str(), "recreate") ;
  cout<<"Global File: "<<GlobalInfo->GetName()<<endl ;
  TVectorD NEvents(1) ;
  NEvents[0] = nEntries ;
  TNamed CORARSCollectionTime("CORARSCollectionTime", conf->CORARSCollectionTime.c_str()) ;
  NEvents.Write("NEvents") ;
  CORARSCollectionTime.Write() ;
  FiredNStripsUpTGCOne->Write() ;
  FiredNWiresUpTGCOne->Write() ;
  FiredNStripsUpTGCTwo->Write() ;
  FiredNWiresUpTGCTwo->Write() ;
  FiredNStripsDownTGCOne->Write() ;
  FiredNWiresDownTGCOne->Write() ;
  FiredNStripsDownTGCTwo->Write() ;
  FiredNWiresDownTGCTwo->Write() ;
  GlobalInfo->Close() ;
  delete GlobalInfo ;
}

#endif
