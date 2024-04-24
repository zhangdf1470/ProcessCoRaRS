#ifndef Alignment_CXX
#define Alignment_CXX
// include the herder file
#include "ProcessCORARS/Alignment.h"
//#include "KM2AEvent.cc"
#include "ProcessCORARS/KM2AEvent.h"

using namespace std;

Alignment::Alignment()
{
  inPath = "" ;
  outPath = "" ;
  inrawfilename = "" ;
  indaqfilename = "" ;
  outfilename = "" ;
  inRawFile = 0 ;
  inRawTree = 0 ;
  inDAQChain = 0 ;
  outFile = 0 ;
  outTree = 0 ;

  UpScinV1290NChannel = -1 ;
  DownScinV1290NChannel = -1 ;
  TriggerID = -1 ;
  UpScinID = -1 ;
  DownScinID = -1 ;
  for(int i= 0 ;i<8 ; i++)
  {
    EDV1290NChannel[i] = -1 ;
    EDV792NChannel[i] = -1 ;
    EDID[i] = -1 ;
  }

  Initialize() ;
}

Alignment::~Alignment()
{
  delete inRawFile ;
  delete outTree ;
  delete outFile ;
}

int Alignment::Initialize()
{
  // get configurations
  conf = ConfigureClass::Instance();

  NEDs = conf->NEDs ;
  UpScinV1290NChannel = conf->UpScinV1290NChannel ;
  DownScinV1290NChannel = conf->DownScinV1290NChannel ;
  TriggerID = conf->TriggerID ;
  UpScinID = conf->UpScinID ;
  DownScinID = conf->DownScinID ;
  for(int i=0 ; i<NEDs ; i++)
  {
    EDV792NChannel[i] = conf->EDV792NChannel[i] ;
    EDV1290NChannel[i] = conf->EDV1290NChannel[i] ;
    EDID[i] = conf->EDID[i] ;
  }

  inPath = conf->inPath ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!!"<<endl ;
    exit(0) ;
  }
  inrawfilename = "Raw_"+conf->CORARSCollectionTime+".root" ;
  inRawFile = new TFile((inPath+inrawfilename).c_str(),"read") ;
  if(!inRawFile)
  {
    cout<<"Error: "<<inrawfilename<<" doesn't exist!!!"<<endl ;
    exit(0) ;
  }
  cout<<"In Raw File: "<<inRawFile->GetName()<<endl ;
  // CORARS RawData Tree
  inRawTree = (TTree*) inRawFile->Get("RawData");
  inRawTree->SetBranchAddress("event_id",&event_id) ;
  inRawTree->SetBranchAddress("event_time",&event_time) ;
  inRawTree->SetBranchAddress("V1190A_1_LeadingMeasure",V1190A_1_LeadingMeasure) ;
  inRawTree->SetBranchAddress("V1190A_1_TrailingMeasure",V1190A_1_TrailingMeasure) ;
  inRawTree->SetBranchAddress("V1190A_2_LeadingMeasure",V1190A_2_LeadingMeasure) ;
  inRawTree->SetBranchAddress("V1190A_2_TrailingMeasure",V1190A_2_TrailingMeasure) ;
  inRawTree->SetBranchAddress("V1290N_measure",V1290N_measure) ;
  inRawTree->SetBranchAddress("V792N_measure",V792N_measure) ;

  inDAQChain = new TChain("event","") ;
  // list of in daq files
  string WRTimeList = conf->WRCollectionTime ;
  std::size_t pos = 0 ;
  while(pos!=std::string::npos)
  {
    std::string subtime = WRTimeList.substr(0, 14) ;
    pos = WRTimeList.find("/");
    WRTimeList = WRTimeList.substr(pos+1);
    if(WRTimeList=="")
      break ;
    indaqfilename = "daq-triggered-"+subtime+".root" ;
    cout<<"In DAQ File: "<<inPath+indaqfilename<<endl ;
    inDAQChain->Add((inPath+indaqfilename).c_str()) ;
  }
  pevent  = new KM2AEvent() ;
  inDAQChain->SetBranchAddress("Event",&pevent) ;

  outPath = conf->inPath ;
  outfilename = "AlignedRaw_"+conf->CORARSCollectionTime+".root" ;
  outFile = new TFile((outPath+outfilename).c_str(), "recreate") ;
  cout<<"Out File: "<<outFile->GetName()<<endl ;
  // out Tree
  outTree = new TTree("RawData","RawData");
  outTree->SetDirectory(outFile) ;
  outTree->Branch("event_id",&event_id,"event_id/i") ;
  outTree->Branch("event_time",&event_time,"event_time/i") ;
  outTree->Branch("V1190A_1_LeadingMeasure",V1190A_1_LeadingMeasure,"V1190A_1_LeadingMeasure[512]/i") ;
  outTree->Branch("V1190A_1_TrailingMeasure",V1190A_1_TrailingMeasure,"V1190A_1_TrailingMeasure[512]/i") ;
  outTree->Branch("V1190A_2_LeadingMeasure",V1190A_2_LeadingMeasure,"V1190A_2_LeadingMeasure[512]/i") ;
  outTree->Branch("V1190A_2_TrailingMeasure",V1190A_2_TrailingMeasure,"V1190A_2_TrailingMeasure[512]/i") ;
  outTree->Branch("V1290N_measure",V1290N_measure,"V1290N_measure[16]/I") ;
  outTree->Branch("V792N_measure",V792N_measure,"V792N_measure[16]/I") ;
  outTree->Branch("KM2ATime",KM2ATime,"KM2ATime[16]/F") ;
  outTree->Branch("NEDNoiseHit",NEDNoiseHit,"NEDNoiseHit[8]/I") ;
  outTree->Branch("EDHitIndex",EDHitIndex,"EDHitIndex[8]/I") ;
  outTree->Branch("KM2ACharge",KM2ACharge,"KM2ACharge[16]/F") ;

  isCORARSAhead = true ;
  NMisalignment = -1 ;
  return 0 ;
}


int Alignment::GetNMisalignment(int CORARSSite, int WTSite, int MaxNMisAlignment, int NLast, bool CORARSAhead)
{
  TH1F* h_corarstof = new TH1F("corarstof","",1000,-100000,100000) ;
  TH1F* h_whiterabbittof = new TH1F("whiterabbittof","",1000,-100000,100000) ;
  TH1F* h_deltatof = new TH1F("deltatof","",1000,-50000,50000) ;
  h_corarstof->StatOverflows(true) ;
  h_whiterabbittof->StatOverflows(true) ;
  h_deltatof->StatOverflows(true) ;

  NMisalignment = -1 ;
  for(int n=0 ; n<MaxNMisAlignment ; n++)
  {
    for(int i=n ; i<NLast; i++)
    {
      if(CORARSAhead)
      {
        if((i+CORARSSite)>inRawTree->GetEntries()||(i-n+WTSite)>inDAQChain->GetEntries())
          continue ;
        inRawTree->GetEntry(i+CORARSSite) ;
        inDAQChain->GetEntry(i-n+WTSite) ;
      }
      else
      {
        if((i-n+CORARSSite)>inRawTree->GetEntries()||(i+WTSite)>inDAQChain->GetEntries())
          continue ;
        inRawTree->GetEntry(i-n+CORARSSite) ;
        inDAQChain->GetEntry(i+WTSite) ;
      }

      int corarsupscintime = 0 ;
      int corarsdownscintime = 0 ;
      int wrupscintime = 0 ;
      int wrdownscintime = 0 ;
      // scintillator time measured by CORARS
      corarsupscintime = V1290N_measure[UpScinV1290NChannel] ;
      corarsdownscintime = V1290N_measure[DownScinV1290NChannel];
      // scintillator time measure by White Rabbit
      int upscinhitindex  = GetHitIndex(pevent, UpScinID) ;
      int downscinhitindex  = GetHitIndex(pevent, DownScinID) ;
      TClonesArray* array = pevent->Hitlist() ;
      KM2AHit* uphit = (KM2AHit* )array->ConstructedAt(upscinhitindex) ;
      wrupscintime = (int)uphit->Time() ;
      KM2AHit* downhit = (KM2AHit* )array->ConstructedAt(downscinhitindex) ;
      wrdownscintime = (int)downhit->Time() ;

      int corarstof = (corarsdownscintime-corarsupscintime)*25 ;
      int wrtof = (wrdownscintime-wrupscintime)*1000 ;
      h_corarstof->Fill(corarstof) ;
      h_whiterabbittof->Fill(wrtof) ;
      h_deltatof->Fill(corarstof-wrtof) ;
    }
    int  MaxBin = h_deltatof->GetMaximumBin() ;
    int N = h_deltatof->Integral() ;
    int NOutCenter = h_deltatof->Integral(0, h_deltatof->FindBin(h_deltatof->GetBinCenter(MaxBin)-4000.))+h_deltatof->Integral(h_deltatof->FindBin(h_deltatof->GetBinCenter(MaxBin)+4000.), h_deltatof->GetNbinsX()+1) ;
    if(float(NOutCenter)/N<0.2)
    {
      NMisalignment = n ;
      isCORARSAhead = CORARSAhead ;
      if(CORARSSite==0 && WTSite==0)
      {
        TF1 *guasfunc = new TF1("guasfit","gaus",h_deltatof->GetBinCenter(MaxBin)-3000., h_deltatof->GetBinCenter(MaxBin)+3000.);
        h_deltatof->Fit("guasfit","RQ") ;
        cout<<"NMisalignment: "<<NMisalignment<<endl ;
        cout<<"Sigma: "<<guasfunc->GetParameter(2)<<endl ;
        cout<<"Mean: "<<guasfunc->GetParameter(1)<<endl ;
        DeltaTOFMean = guasfunc->GetParameter(1) ;
        DeltaTOFSigma = guasfunc->GetParameter(2) ;
        h_deltatof->SetDirectory(outFile) ;
        h_deltatof->Write() ;
      }
      break ;
    }
    //delete guasfunc ;
    h_corarstof->Reset() ;
    h_whiterabbittof->Reset() ;
    h_deltatof->Reset() ;
  }
  delete h_corarstof ;
  delete h_whiterabbittof ;
  delete h_deltatof ;

  return 0 ;
}

int Alignment::Align()
{
  GetNMisalignment(0, 0, 200, 1000, true) ;
  if(NMisalignment<0)
    GetNMisalignment(0, 0, 200, 1000, false) ;

  int NCORARSEvents = inRawTree->GetEntries() ;
  int NWTEvents = inDAQChain->GetEntries() ;
  cout<<"Number of CORARS Events: "<<NCORARSEvents<<endl ;
  cout<<"Number of White Rabbit Events: "<<NWTEvents<<endl ;
  cout<<"Aligning: "<<endl ;
  
  int PreMisAlignmentID = -100 ;
  int iCORARS = 0 ;
  int iWhiteRabbit = 0 ;
  if(isCORARSAhead) 
    iCORARS = iCORARS+NMisalignment ;
  else
    iWhiteRabbit = iWhiteRabbit+NMisalignment ; 
  for( ; iCORARS<NCORARSEvents&&iWhiteRabbit<NWTEvents; iCORARS++, iWhiteRabbit++)
  {
    inRawTree->GetEntry(iCORARS) ;
    inDAQChain->GetEntry(iWhiteRabbit) ;
    if(iCORARS%10000==0)
      cout<<iCORARS<<endl ;
    //if(iCORARS>100000)
    //  break ;

    for(int channel=0 ; channel<16 ; channel++)
    {
       KM2ATime[channel] = 0 ;
       KM2ACharge[channel] = 0 ;
    }
    TClonesArray* array = pevent->Hitlist() ;
    int upscintime = 0 ; // Up Scintillator Time measured by White Rabbit
    UpScinHitIndex = GetHitIndex(pevent, UpScinID) ;
    KM2AHit* upscinhit = (KM2AHit* )array->ConstructedAt(UpScinHitIndex) ;
    upscintime = (int)upscinhit->Time() ;
    int downscintime = 0 ; // Down Scintillator Time measured by White Rabbit
    DownScinHitIndex = GetHitIndex(pevent, DownScinID) ;
    KM2AHit* downscinhit = (KM2AHit* )array->ConstructedAt(DownScinHitIndex) ;
    downscintime = (int)downscinhit->Time() ;

    int corarsupscintime = V1290N_measure[UpScinV1290NChannel] ;
    int corarsdownscintime = V1290N_measure[DownScinV1290NChannel];
    int corarstof = (corarsdownscintime-corarsupscintime)*25 ;
    int wrtof = ((int)downscintime-(int)upscintime)*1000 ;
    if((corarstof-wrtof)>(DeltaTOFMean+5*DeltaTOFSigma)||(corarstof-wrtof)<(DeltaTOFMean-5*DeltaTOFSigma))
    {
      if(PreMisAlignmentID<0)
      {
        PreMisAlignmentID = event_id ;
        continue ;
      }
      else
      {
        if(event_id-PreMisAlignmentID==1)
        {
          PreMisAlignmentID = -100 ;
          GetNMisalignment(iCORARS-1, iWhiteRabbit-1, 20, 100, true) ;
          if(NMisalignment<0)
            GetNMisalignment(iCORARS-1, iWhiteRabbit-1, 20, 100, false) ;
          if(NMisalignment<0)
          {
           cout<<isCORARSAhead<<" NMisalignment: "<<NMisalignment<<endl ;
           continue ;
          }
          if(NMisalignment>0)
           cout<<isCORARSAhead<<" NMisalignment: "<<NMisalignment<<endl ;
          if(isCORARSAhead) 
            iCORARS = iCORARS+NMisalignment ;
          else
            iWhiteRabbit = iWhiteRabbit+NMisalignment ;
          iCORARS = iCORARS-1 ;
          iWhiteRabbit = iWhiteRabbit-1 ;
          continue ;
        }
        else
          PreMisAlignmentID = event_id ;
      }
    }

    // asign scintillator time measure by White Rabbit to CORARS
    KM2ATime[UpScinV1290NChannel] = upscintime ;
    KM2ATime[DownScinV1290NChannel] = downscintime ;

    for(int n=0 ; n<NEDs ; n++)
    {
      NEDNoiseHit[n] = 0 ;
      EDHitIndex[n] = -1 ;
      EDHitIndex[n] = GetHitIndex(pevent, EDID[n]) ;
      int edtime = 0 ;
      float edcharge = 0 ;
      if(EDHitIndex[n]<0)
      {
        edtime = 0 ;
        edcharge = 0 ;
      }
      else
      {
        NEDNoiseHit[n] = GetEDNNoise(pevent, EDID[n]) ;
        KM2AHit* edhit = (KM2AHit* )array->ConstructedAt(EDHitIndex[n]) ;
        edtime = (int)edhit->Time() ; // in ns
        edcharge = (float)(edhit->Qa())-(float)(edhit->Peda())/8.;
        if(edcharge<=0)
          edcharge = 0 ;
      }
      KM2ACharge[EDV1290NChannel[n]] = edcharge ; // in ns
      KM2ATime[EDV792NChannel[n]] = edtime ;
    }
    outTree->Fill() ;
  }
  outFile->Write() ;

  return 0 ;
}

// Get the number of hits of ID
int Alignment::GetNHits(KM2AEvent* pevent, int ID)
{
  TClonesArray* array = pevent->Hitlist() ;
  int nhits = 0 ;
  for(int j=0 ; j<pevent->NHit()+pevent->Nmd();j++)
  {
    KM2AHit* hit = (KM2AHit* )array->ConstructedAt(j) ;
    if((int)hit->ID()==ID)
      nhits++ ;
  }
  return nhits ;
}
// Get the index of the first hit of ID
int Alignment::GetHitIndex(KM2AEvent* pevent, int ID)
{
  TClonesArray* array = pevent->Hitlist() ;
  int index = -1 ;
  for(int j=0 ; j<pevent->NHit()+pevent->Nmd();j++)
  {
    KM2AHit* hit = (KM2AHit* )array->ConstructedAt(j) ;
    int id = (int) hit->ID() ;
    //float charge = (float)(hit->Qa())-(float)(hit->Peda())/8. ;
    if(id==ID)
    {
      index = j ;
      break ;
    }
  }
  return index ;
}

int Alignment::GetEDNNoise(KM2AEvent* pevent, int ID)
{
  int nnoise = 0 ;
  TClonesArray* array = pevent->Hitlist() ;
  for(int j=0 ; j<pevent->NHit()+pevent->Nmd();j++)
  {
    KM2AHit* hit = (KM2AHit* )array->ConstructedAt(j) ;
    if((int)hit->ID()==ID)
    {
      float time = (float)hit->Time() ;
      if(time<4700)
        nnoise++ ;
    }
  }
  return nnoise ;
}

#endif
