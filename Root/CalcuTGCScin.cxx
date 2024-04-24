#ifndef __CalcuTGCScin__CXX
#define __CalcuTGCScin__CXX

#include "ProcessCORARS/CalcuTGCScin.h"

CalcuTGCScin::CalcuTGCScin()
{
  inFile = 0 ;
  inFileName = "" ;
  inTree = 0 ;
  outFile = 0 ;
  outFileName = "" ;
  outTree = 0 ;

  UpScinV1290NChannel = -1 ;
  DownScinV1290NChannel = -1 ;

  UpTGCOneStripSignalWidth = 0 ;
  UpTGCOneWireSignalWidth = 0 ;
  UpTGCTwoStripSignalWidth = 0 ;
  UpTGCTwoWireSignalWidth = 0 ;
  DownTGCOneStripSignalWidth = 0 ;
  DownTGCOneWireSignalWidth = 0 ;
  DownTGCTwoStripSignalWidth = 0 ;
  DownTGCTwoWireSignalWidth = 0 ;

  EventQualityDis = 0 ;
  UpScinHitsGraph = 0 ;
  DownScinHitsGraph = 0 ;
  UpScinX = 0 ;
  DownScinX = 0 ;
  // initialize elements of class
  Initialize() ;
}

CalcuTGCScin::~CalcuTGCScin()
{
  delete inTree ;
  delete inFile ;
  delete outTree ;
  delete outFile ;
}

void CalcuTGCScin::Initialize()
{
  conf = ConfigureClass::Instance() ;

  TSCTXY = conf->GetTSCTXY() ; // Get the x and y of Trigger SCTs
  
  UpScinV1290NChannel = conf->UpScinV1290NChannel ;
  DownScinV1290NChannel = conf->DownScinV1290NChannel ;

  inPath = conf->outPath+"/"+conf->CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(inPath.c_str()))
  {
    cout<<"In Path: "<<inPath<<" doesn't exist!!! Exit!!"<<endl ;
    exit(0) ;
  }
  inFileName = "Stage_One_"+conf->CORARSCollectionTime+".root" ;
	inFile = new TFile((inPath+inFileName).c_str(),"READ") ;
  if(!inFile)
  {
    cout<<"In File: "<<inPath<<inFileName<<" doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
	cout<<"In File: "<<inFile->GetName()<<endl ;
	inTree = (TTree*)(inFile->Get("UsedVariables")) ; 
  if(!inTree)
  {
    cout<<"UsedVariables Tree doesn't exist!! Exit!!"<<endl ;
    exit(0) ;
  }
  // set branch address of in tree
  inTree->SetBranchAddress("event_id",&event_id) ;
  inTree->SetBranchAddress("event_time",&event_time) ;
  inTree->SetBranchAddress("Up_TGC_Strip_Leading_One",Up_TGC_Strip_Leading_One) ;
  inTree->SetBranchAddress("Up_TGC_Strip_Trailing_One",Up_TGC_Strip_Trailing_One) ;
  inTree->SetBranchAddress("Up_TGC_Wire_Leading_One",Up_TGC_Wire_Leading_One) ; 
  inTree->SetBranchAddress("Up_TGC_Wire_Trailing_One",Up_TGC_Wire_Trailing_One) ;
  inTree->SetBranchAddress("Down_TGC_Strip_Leading_One",Down_TGC_Strip_Leading_One) ;
  inTree->SetBranchAddress("Down_TGC_Strip_Trailing_One",Down_TGC_Strip_Trailing_One) ; 
  inTree->SetBranchAddress("Down_TGC_Wire_Leading_One",Down_TGC_Wire_Leading_One) ;
  inTree->SetBranchAddress("Down_TGC_Wire_Trailing_One",Down_TGC_Wire_Trailing_One) ;
  inTree->SetBranchAddress("Up_TGC_Strip_Leading_Two",Up_TGC_Strip_Leading_Two) ;
  inTree->SetBranchAddress("Up_TGC_Strip_Trailing_Two",Up_TGC_Strip_Trailing_Two) ;
  inTree->SetBranchAddress("Up_TGC_Wire_Leading_Two",Up_TGC_Wire_Leading_Two) ;
  inTree->SetBranchAddress("Up_TGC_Wire_Trailing_Two",Up_TGC_Wire_Trailing_Two) ;
  inTree->SetBranchAddress("Down_TGC_Strip_Leading_Two",Down_TGC_Strip_Leading_Two) ;
  inTree->SetBranchAddress("Down_TGC_Strip_Trailing_Two",Down_TGC_Strip_Trailing_Two) ;
  inTree->SetBranchAddress("Down_TGC_Wire_Leading_Two",Down_TGC_Wire_Leading_Two) ;
  inTree->SetBranchAddress("Down_TGC_Wire_Trailing_Two",Down_TGC_Wire_Trailing_Two) ;
  inTree->SetBranchAddress("KM2ATime",KM2ATime);
  inTree->SetBranchAddress("KM2ACharge",KM2ACharge);
  inTree->SetBranchAddress("NEDNoiseHit",NEDNoiseHit);
  inTree->SetBranchAddress("Count_Up_Strip_One",&Count_Up_Strip_One) ;
  inTree->SetBranchAddress("Count_Up_Wire_One",&Count_Up_Wire_One) ;
  inTree->SetBranchAddress("Count_Down_Strip_One",&Count_Down_Strip_One) ;
  inTree->SetBranchAddress("Count_Down_Wire_One",&Count_Down_Wire_One) ;
  inTree->SetBranchAddress("Count_Up_Strip_Two",&Count_Up_Strip_Two) ;
  inTree->SetBranchAddress("Count_Up_Wire_Two",&Count_Up_Wire_Two) ;
  inTree->SetBranchAddress("Count_Down_Strip_Two",&Count_Down_Strip_Two) ;
  inTree->SetBranchAddress("Count_Down_Wire_Two",&Count_Down_Wire_Two) ;

  // out file 
  outPath = conf->outPath+"/"+conf->CORARSCollectionTime+"/" ;
  if(gSystem->AccessPathName(outPath.c_str()))
    gSystem->Exec(("mkdir "+outPath).c_str()) ;
  outFileName = "TGCScin_"+conf->CORARSCollectionTime+".root" ;
	outFile = new TFile((outPath+outFileName).c_str(),"RECREATE") ;
	cout<<"Out File: "<<outFile->GetName()<<endl ;
	outTree = new TTree("CalcuedTGCScin","Calculated TGC and Scin") ;
  outTree->SetDirectory(outFile) ;
  // add branches to out tree
  outTree->Branch("event_id",&event_id,"event_id/i"); 
  outTree->Branch("EventQuality",&EventQuality,"EventQuality/I");
  outTree->Branch("nosignal",&nosignal,"nosignal/I"); 
  outTree->Branch("event_time",&event_time,"event_time/i"); 
  outTree->Branch("Up_TGC_One",&Up_TGC_One,"x/F:y/F:z/F:Valid_Strip_Number/I:Valid_Wire_Number/I"); 
  outTree->Branch("Down_TGC_One",&Down_TGC_One,"x/F:y/F:z/F:Valid_Strip_Number/I:Valid_Wire_Number/I"); 
  outTree->Branch("Up_TGC_Two",&Up_TGC_Two,"x/F:y/F:z/F:Valid_Strip_Number/I:Valid_Wire_Number/I"); 
  outTree->Branch("Down_TGC_Two",&Down_TGC_Two,"x/F:y/F:z/F:Valid_Strip_Number/I:Valid_Wire_Number/I"); 
  outTree->Branch("Up_TGC",&Up_TGC,"x/F:y/F:z/F"); 
  outTree->Branch("Down_TGC",&Down_TGC,"x/F:y/F:z/F"); 

  outTree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F"); 
  outTree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/F");

  outTree->Branch("KM2ATime",KM2ATime,"KM2ATime[16]/F");
  outTree->Branch("KM2ACharge",KM2ACharge,"KM2ACharge[16]/F");
  outTree->Branch("NEDNoiseHit",NEDNoiseHit,"NEDNoiseHit[8]/I");

  outTree->Branch("Up_TGC_Strip_Two_Cluster","vector<Cluster>",&Up_TGC_Strip_Two_Cluster); 
  outTree->Branch("Up_TGC_Wire_Two_Cluster","vector<Cluster>",&Up_TGC_Wire_Two_Cluster); 
  outTree->Branch("Down_TGC_Strip_Two_Cluster","vector<Cluster>",&Down_TGC_Strip_Two_Cluster); 
  outTree->Branch("Down_TGC_Wire_Two_Cluster","vector<Cluster>",&Down_TGC_Wire_Two_Cluster); 

  outTree->Branch("Count_Up_Strip_One",&Count_Up_Strip_One,"Count_Up_Strip_One/I");
  outTree->Branch("Count_Up_Wire_One",&Count_Up_Wire_One,"Count_Up_Wire_One/I");
  outTree->Branch("Count_Down_Strip_One",&Count_Down_Strip_One,"Count_Down_Strip_One/I");
  outTree->Branch("Count_Down_Wire_One",&Count_Down_Wire_One,"Count_Down_Wire_One/I");
  outTree->Branch("Count_Up_Strip_Two",&Count_Up_Strip_Two,"Count_Up_Strip_Two/I");
  outTree->Branch("Count_Up_Wire_Two",&Count_Up_Wire_Two,"Count_Up_Wire_Two/I");
  outTree->Branch("Count_Down_Strip_Two",&Count_Down_Strip_Two,"Count_Down_Strip_Two/I");
  outTree->Branch("Count_Down_Wire_Two",&Count_Down_Wire_Two,"Count_Down_Wire_Two/I");
  outTree->Branch("Delta_Up_Strip_ValidNumber",&Delta_Up_Strip_ValidNumber,"Delta_Up_Strip_ValidNumber/I");
  outTree->Branch("Delta_Up_Wire_ValidNumber",&Delta_Up_Wire_ValidNumber,"Delta_Up_Wire_ValidNumber/I");
  outTree->Branch("Delta_Down_Strip_ValidNumber",&Delta_Down_Strip_ValidNumber,"Delta_Down_Strip_ValidNumber/I");
  outTree->Branch("Delta_Down_Wire_ValidNumber",&Delta_Down_Wire_ValidNumber,"Delta_Down_Wire_ValidNumber/I");
	// These four branches are used to store clusters
	outTree->Branch("Up_TGC_Strip_One_Cluster","vector<Cluster>",&Up_TGC_Strip_One_Cluster); 
	outTree->Branch("Up_TGC_Wire_One_Cluster","vector<Cluster>",&Up_TGC_Wire_One_Cluster); 
	outTree->Branch("Down_TGC_Strip_One_Cluster","vector<Cluster>",&Down_TGC_Strip_One_Cluster); 
	outTree->Branch("Down_TGC_Wire_One_Cluster","vector<Cluster>",&Down_TGC_Wire_One_Cluster); 

  // get paremeter of strips and wire
	ifstream Input_params ;
	Input_params.open("ProcessCORARS/Config/params") ;
	if(!Input_params)
		cout<<"open file failed!"<<endl ;
	else
	{
		for(int i = 0 ; i<189 ; i++)
		{Input_params>>params[i] ; }
	}
	Input_params.close();

  // initialize EventQuality Histogram
  EventQualityDis = new TH1F("EventQuality", "EventQuality", 100, -50, 50) ;
  // initialize signal width histograms
  UpTGCOneStripSignalWidth = new TH1F("UpTGCOneStripSignalWidth", "Strip Signal Width of Up TGC One", 100, -5000, 5000);
  UpTGCOneWireSignalWidth = new TH1F("UpTGCOneWireSignalWidth", "Wire Signal Width of Up TGC One", 100, -5000, 5000);
  UpTGCTwoStripSignalWidth = new TH1F("UpTGCTwoStripSignalWidth", "Strip Signal Width of Up TGC Two", 100, -5000, 5000);
  UpTGCTwoWireSignalWidth = new TH1F("UpTGCTwoWireSignalWidth", "Wire Signal Width of Up TGC Two", 100, -5000, 5000);
  DownTGCOneStripSignalWidth = new TH1F("DownTGCOneStripSignalWidth", "Strip Signal Width of Down TGC One", 100, -5000, 5000);
  DownTGCOneWireSignalWidth = new TH1F("DownTGCOneWireSignalWidth", "Wire Signal Width of Down TGC One", 100, -5000, 5000);
  DownTGCTwoStripSignalWidth = new TH1F("DownTGCTwoStripSignalWidth", "Strip Signal Width of Down TGC Two", 100, -5000, 5000);
  DownTGCTwoWireSignalWidth = new TH1F("DownTGCTwoWireSignalWidth", "Wire Signal Width of Down TGC Two", 100, -5000, 5000);
  UpTGCOneStripSignalWidth->SetDirectory(0) ;
  UpTGCOneWireSignalWidth->SetDirectory(0) ;
  UpTGCTwoStripSignalWidth->SetDirectory(0) ;
  UpTGCTwoWireSignalWidth->SetDirectory(0) ;
  DownTGCOneStripSignalWidth->SetDirectory(0) ;
  DownTGCOneWireSignalWidth->SetDirectory(0) ;
  DownTGCTwoStripSignalWidth->SetDirectory(0) ;
  DownTGCTwoWireSignalWidth->SetDirectory(0) ;
  EventQualityDis->SetDirectory(0) ;
  //
  UpScinX = new TH1F("UpScinX", "Up Scintillator X", 700, 0, 180.) ;
  UpScinX->SetDirectory(0) ;
  DownScinX = new TH1F("DownScinX", "Down Scintillator X", 700, 0, 180.) ;
  DownScinX->SetDirectory(0) ;

}
void CalcuTGCScin::GetClusters(UInt_t Leading[32] , UInt_t Trailing[32] , vector<Cluster> *vectorcluster)
{
	for(int i=0 ; i<32 ;)
	{
		Cluster *cluster = new Cluster() ;
	       if(Leading[i]<=0||Trailing[i]<=0)
		{
			i++ ;
			delete cluster ;
			continue ;
		}
		int j = i ;
		for(; j<32 ; j++)
		{
			if(Leading[j]>0||Trailing[j]>0)
			{
				cluster->Number.push_back(j) ;
				cluster->Leading.push_back(Leading[j]) ;
				cluster->Trailing.push_back(Trailing[j]) ;
				if(Leading[j]>0)
				{
					if(Trailing[j]>0) 
					cluster->SignalWidth.push_back(int(Trailing[j])-int(Leading[j])) ;
					else 
					cluster->SignalWidth.push_back(-int(Leading[j])) ;
				}
				else 
				cluster->SignalWidth.push_back(int(Trailing[j])) ;
			}
			else 
			break ;
		}
    if(cluster->GetMaxSignalWidth()>200)
    {
      cluster->MaxSignal = cluster->GetMaxSignalWidth() ;
		  vectorcluster->push_back(*cluster) ;
    }
		delete cluster ;
		i = j ;
	}
}

void CalcuTGCScin::GetAllClusters()
{
	GetClusters(Up_TGC_Strip_Leading_One , Up_TGC_Strip_Trailing_One , &Up_TGC_Strip_One_Cluster) ;
	GetClusters(Up_TGC_Wire_Leading_One ,  Up_TGC_Wire_Trailing_One , &Up_TGC_Wire_One_Cluster) ;  
	GetClusters(Up_TGC_Strip_Leading_Two , Up_TGC_Strip_Trailing_Two , &Up_TGC_Strip_Two_Cluster) ;
	GetClusters(Up_TGC_Wire_Leading_Two ,  Up_TGC_Wire_Trailing_Two , &Up_TGC_Wire_Two_Cluster) ;
	GetClusters(Down_TGC_Strip_Leading_One , Down_TGC_Strip_Trailing_One , &Down_TGC_Strip_One_Cluster) ;
	GetClusters(Down_TGC_Wire_Leading_One , Down_TGC_Wire_Trailing_One , &Down_TGC_Wire_One_Cluster) ;
	GetClusters(Down_TGC_Strip_Leading_Two , Down_TGC_Strip_Trailing_Two , &Down_TGC_Strip_Two_Cluster) ;
	GetClusters(Down_TGC_Wire_Leading_Two , Down_TGC_Wire_Trailing_Two , &Down_TGC_Wire_Two_Cluster) ;
}

void CalcuTGCScin::GetClusterValidNumbers(vector<Cluster> vectorcluster, vector<int> *vectorvalidnumber)
{
	for(int n=0 ; n<(int)vectorcluster.size() ; n++)
	{
		if(vectorcluster[n].Number.size()>3)
		  vectorvalidnumber->push_back(vectorcluster[n].GetMiddleNumberM3()) ;
		else 
		  vectorvalidnumber->push_back(vectorcluster[n].GetMiddleNumber()) ;
	}
}

int CalcuTGCScin::GetValidNumber(vector<Cluster> valid)
{
	if(valid.size()==0) return -1 ;
	vector<Cluster>::iterator iter = valid.begin() ;
	int max = (*iter).GetMaxSignalWidth() ;
	int valid_number = (*iter).GetMiddleNumber() ;
	for( ; iter!=valid.end() ; iter++)
	if(max<(*iter).GetMaxSignalWidth())
	{max = (*iter).GetMaxSignalWidth() ; valid_number = (*iter).GetMiddleNumber() ;}
	return valid_number ;
}

bool CalcuTGCScin::GetValidNumbers()
{
	if((Up_TGC_Strip_One_Cluster.size()==0||Up_TGC_Wire_One_Cluster.size()==0)&&
			(Up_TGC_Strip_Two_Cluster.size()==0||Up_TGC_Wire_Two_Cluster.size()==0))
	return false ;
	if((Down_TGC_Strip_One_Cluster.size()==0||Down_TGC_Wire_One_Cluster.size()==0)&&
			(Down_TGC_Strip_Two_Cluster.size()==0||Down_TGC_Wire_Two_Cluster.size()==0))
	return false ;

	Up_TGC_One.Valid_Strip_Number = GetValidNumber(Up_TGC_Strip_One_Cluster) ;
	Up_TGC_One.Valid_Wire_Number = GetValidNumber(Up_TGC_Wire_One_Cluster) ;
	Down_TGC_One.Valid_Strip_Number = GetValidNumber(Down_TGC_Strip_One_Cluster) ;
	Down_TGC_One.Valid_Wire_Number = GetValidNumber(Down_TGC_Wire_One_Cluster) ;
	Up_TGC_Two.Valid_Strip_Number = GetValidNumber(Up_TGC_Strip_Two_Cluster) ;
	Up_TGC_Two.Valid_Wire_Number = GetValidNumber(Up_TGC_Wire_Two_Cluster) ;
	Down_TGC_Two.Valid_Strip_Number = GetValidNumber(Down_TGC_Strip_Two_Cluster) ;
	Down_TGC_Two.Valid_Wire_Number = GetValidNumber(Down_TGC_Wire_Two_Cluster) ;
	return true ;
}

void CalcuTGCScin::Calcu_TGC()
{
	//now that the two layers TGCs in one box is very close; the strip and wire number of two layers should be very close

	// now to judge to calcualte hit point of single layer TGC or double layers TGC
	if((Up_TGC_One.Valid_Strip_Number==-1||Up_TGC_One.Valid_Wire_Number==-1)||
			(Up_TGC_Two.Valid_Strip_Number==-1||Up_TGC_Two.Valid_Wire_Number==-1))
	{
		if((Up_TGC_One.Valid_Strip_Number==-1||Up_TGC_One.Valid_Wire_Number==-1)&&
				(Up_TGC_Two.Valid_Strip_Number!=-1||Up_TGC_Two.Valid_Wire_Number!=-1))
		{
			float a1 = 12.16, 
			b1 = params[Up_TGC_Two.Valid_Strip_Number*3], 
			c1 = params[Up_TGC_Two.Valid_Strip_Number*3+1], 
			c2 = params[Up_TGC_Two.Valid_Wire_Number*3+2]; 
			Up_TGC_Two.TGC_Coor.x = (c1 - b1*c2)/a1 +85. ;
			Up_TGC_Two.TGC_Coor.y = c2 ;
			Up_TGC_Two.TGC_Coor.z = 323.4 ;
			Up_TGC = Up_TGC_Two.TGC_Coor ;
		}
		else
		{
			float a1 = 12.16, 
			b1 = -params[188-Up_TGC_One.Valid_Strip_Number*3-2], 
			c1 = -params[188-Up_TGC_One.Valid_Strip_Number*3-1], 
			c2 = params[Up_TGC_One.Valid_Wire_Number*3+2]; 
			Up_TGC_One.TGC_Coor.x = (c1 - b1*c2)/a1 +85. ;
			Up_TGC_One.TGC_Coor.y = c2 ;
			Up_TGC_One.TGC_Coor.z = 325. ;
			Up_TGC = Up_TGC_One.TGC_Coor ;
		}
	}
	else
	{
		float a1 = 12.16, 
		b1 = -params[188-Up_TGC_One.Valid_Strip_Number*3-2], 
		c1 = -params[188-Up_TGC_One.Valid_Strip_Number*3-1], 
		c2 = params[Up_TGC_One.Valid_Wire_Number*3+2];
		Up_TGC_One.TGC_Coor.x = (c1 - b1*c2)/a1 +85. ;
		Up_TGC_One.TGC_Coor.y = c2 ;
		Up_TGC_One.TGC_Coor.z = 325. ;
		b1 = params[Up_TGC_Two.Valid_Strip_Number*3], 
		c1 = params[Up_TGC_Two.Valid_Strip_Number*3+1], 
		c2 = params[Up_TGC_Two.Valid_Wire_Number*3+2]; 
                
		Up_TGC_Two.TGC_Coor.x = (c1 - b1*c2)/a1 +85. ; 
		Up_TGC_Two.TGC_Coor.y = c2 ;
		Up_TGC_Two.TGC_Coor.z = 323.4 ;

		Up_TGC.x = (Up_TGC_One.TGC_Coor.x+Up_TGC_Two.TGC_Coor.x)/2 ;
		Up_TGC.y = (Up_TGC_One.TGC_Coor.y+Up_TGC_Two.TGC_Coor.y)/2 ;
		Up_TGC.z = (Up_TGC_One.TGC_Coor.z+Up_TGC_Two.TGC_Coor.z)/2 ;
		Delta_Up_Strip_ValidNumber = abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number) ;
		Delta_Up_Wire_ValidNumber = abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number) ;
	}
    if((Down_TGC_One.Valid_Strip_Number==-1||Down_TGC_One.Valid_Wire_Number==-1)||
       (Down_TGC_Two.Valid_Strip_Number==-1||Down_TGC_Two.Valid_Wire_Number==-1))
	{
	    if((Down_TGC_One.Valid_Strip_Number==-1||Down_TGC_One.Valid_Wire_Number==-1)&&
	       (Down_TGC_Two.Valid_Strip_Number!=-1||Down_TGC_Two.Valid_Wire_Number!=-1))
	     {
	        float a1 = 12.16, 
	        b1 = params[Down_TGC_Two.Valid_Strip_Number*3], 
	        c1 = params[Down_TGC_Two.Valid_Strip_Number*3+1], 
	        c2 = params[Down_TGC_Two.Valid_Wire_Number*3+2]; 
		Down_TGC_Two.TGC_Coor.x = (c1 - b1*c2)/a1 +85. ;
		Down_TGC_Two.TGC_Coor.y = c2 ;
		Down_TGC_Two.TGC_Coor.z = 43.6 ;
		Down_TGC = Down_TGC_Two.TGC_Coor ;
	     }
		else
		{
			float a1 = 12.16, 
			b1 = -params[188-Down_TGC_One.Valid_Strip_Number*3-2], 
			c1 = -params[188-Down_TGC_One.Valid_Strip_Number*3-1], 
			c2 = params[Down_TGC_One.Valid_Wire_Number*3+2]; 
			Down_TGC_One.TGC_Coor.x = (c1 - b1*c2)/a1 +85.;
			Down_TGC_One.TGC_Coor.y = c2 ;
			Down_TGC_One.TGC_Coor.z = 45. ;
			Down_TGC = Down_TGC_One.TGC_Coor ;
		}
	}
	else
	{
		float a1 = 12.16, 
		b1 = -params[188-Down_TGC_One.Valid_Strip_Number*3-2], 
		c1 = -params[188-Down_TGC_One.Valid_Strip_Number*3-1], 
		c2 =  params[Down_TGC_One.Valid_Wire_Number*3+2]; 
		Down_TGC_One.TGC_Coor.x = (c1 - b1*c2)/a1 +85.;
		Down_TGC_One.TGC_Coor.y = c2 ;
		Down_TGC_One.TGC_Coor.z = 45. ;
		b1 = params[Down_TGC_Two.Valid_Strip_Number*3], 
		c1 = params[Down_TGC_Two.Valid_Strip_Number*3+1], 
		c2 = params[Down_TGC_Two.Valid_Wire_Number*3+2]; 
		Down_TGC_Two.TGC_Coor.x = (c1 - b1*c2)/a1 +85. ;
		Down_TGC_Two.TGC_Coor.y = c2 ;
		Down_TGC_Two.TGC_Coor.z = 43.6 ;
		Down_TGC.x = (Down_TGC_One.TGC_Coor.x+Down_TGC_Two.TGC_Coor.x)/2 ;
		Down_TGC.y = (Down_TGC_One.TGC_Coor.y+Down_TGC_Two.TGC_Coor.y)/2 ;
		Down_TGC.z = (Down_TGC_One.TGC_Coor.z+Down_TGC_Two.TGC_Coor.z)/2 ;
		Delta_Down_Strip_ValidNumber = abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number) ;
		Delta_Down_Wire_ValidNumber = abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number) ;
	}
}
// calculate scin and compoung the muon track with scin
bool CalcuTGCScin::Calcu_Scin()
{
  // height of scintillators
	Up_Scin.Scin_Coor.z = 346.5 ;
	Down_Scin.Scin_Coor.z = 26. ;

  // calculate coordinate of hits on Scintillator
	float a =  (Up_TGC.x-Down_TGC.x)/(Up_TGC.z-Down_TGC.z) ;
	float b = (Up_TGC.z*Down_TGC.x-Up_TGC.x*Down_TGC.z)/(Up_TGC.z-Down_TGC.z) ;
	float c =  (Up_TGC.y-Down_TGC.y)/(Up_TGC.z-Down_TGC.z) ;
	float d = (Up_TGC.z*Down_TGC.y-Up_TGC.y*Down_TGC.z)/(Up_TGC.z-Down_TGC.z);
	Up_Scin.Scin_Coor.x = a*Up_Scin.Scin_Coor.z+b ;
	Up_Scin.Scin_Coor.y = c*Up_Scin.Scin_Coor.z+d ;
	Down_Scin.Scin_Coor.x = a*Down_Scin.Scin_Coor.z+b ;
	Down_Scin.Scin_Coor.y = c*Down_Scin.Scin_Coor.z+d ;
	
  // initialize index of scintillator
	Up_Scin.index=-1; 
  Down_Scin.index=-1;

  // on the top layer, the index of scintillator is: 0, 1, 2, 3, 4, 5
  // on the bottom layer, the index of scintillator is: 6, 7, 8, 9, 10, 11, 12
  // if the hit is not on Scin, set it on the boundary

  // position of up scin:(xmin, xmax) (ymin, ymax)
  // 0: (0.2, 25.2) (0, 120)
  // 1: (28, 53) (0, 120)
  // 2: (55.9, 80.9) (4, 124)
  // 3: (86, 111) (0, 120)
  // 4: (114.2, 139.2) (0, 120)
  // 5: (142, 167) (0, 120)
  if(Up_Scin.Scin_Coor.x<=(TSCTXY.at(0).first+TSCTXY.at(1).first+25.)/2.)
  {
    if(Up_Scin.Scin_Coor.x<TSCTXY.at(0).first)
      Up_Scin.Scin_Coor.x = TSCTXY.at(0).first ;
    if(Up_Scin.Scin_Coor.x>TSCTXY.at(0).first+25.)
      Up_Scin.Scin_Coor.x = TSCTXY.at(0).first+25. ;
    if(Up_Scin.Scin_Coor.y>TSCTXY.at(0).second+120.)
      Up_Scin.Scin_Coor.y = TSCTXY.at(0).second+120. ;
    if(Up_Scin.Scin_Coor.y<TSCTXY.at(0).second)
      Up_Scin.Scin_Coor.y = TSCTXY.at(0).second ;
    Up_Scin.index = 0 ;
  }
  if(Up_Scin.Scin_Coor.x>(TSCTXY.at(0).first+25.+TSCTXY.at(1).first)/2.
    &&Up_Scin.Scin_Coor.x<=(TSCTXY.at(1).first+25.+TSCTXY.at(2).first)/2.)
  {
    if(Up_Scin.Scin_Coor.x<TSCTXY.at(1).first)
      Up_Scin.Scin_Coor.x = TSCTXY.at(1).first ;
    if(Up_Scin.Scin_Coor.x>TSCTXY.at(1).first+25.)
      Up_Scin.Scin_Coor.x = TSCTXY.at(1).first+25. ;
    if(Up_Scin.Scin_Coor.y>TSCTXY.at(1).second+120.)
      Up_Scin.Scin_Coor.y = TSCTXY.at(1).second+120. ;
    if(Up_Scin.Scin_Coor.y<TSCTXY.at(1).second)
      Up_Scin.Scin_Coor.y = TSCTXY.at(1).second ;
    Up_Scin.index= 1 ;
  }
  if(Up_Scin.Scin_Coor.x>(TSCTXY.at(1).first+25.+TSCTXY.at(2).first)/2.
    &&Up_Scin.Scin_Coor.x<=(TSCTXY.at(2).first+25.+TSCTXY.at(3).first)/2.)
  {
    if(Up_Scin.Scin_Coor.x<TSCTXY.at(2).first)
      Up_Scin.Scin_Coor.x = TSCTXY.at(2).first ;
    if(Up_Scin.Scin_Coor.x>TSCTXY.at(2).first+25.)
      Up_Scin.Scin_Coor.x = TSCTXY.at(2).first+25. ;
    if(Up_Scin.Scin_Coor.y>TSCTXY.at(2).second+120.)
      Up_Scin.Scin_Coor.y = TSCTXY.at(2).second+120. ;
    if(Up_Scin.Scin_Coor.y<TSCTXY.at(2).second)
      Up_Scin.Scin_Coor.y = TSCTXY.at(2).second ;
    Up_Scin.index= 2 ;
  }
  if(Up_Scin.Scin_Coor.x>(TSCTXY.at(2).first+25.+TSCTXY.at(3).first)/2.
    &&Up_Scin.Scin_Coor.x<=(TSCTXY.at(3).first+25.+TSCTXY.at(4).first)/2.)
  {
    if(Up_Scin.Scin_Coor.x<TSCTXY.at(3).first)
      Up_Scin.Scin_Coor.x = TSCTXY.at(3).first ;
    if(Up_Scin.Scin_Coor.x>TSCTXY.at(3).first+25.)
      Up_Scin.Scin_Coor.x = TSCTXY.at(3).first+25. ;
    if(Up_Scin.Scin_Coor.y>TSCTXY.at(3).second+120.)
      Up_Scin.Scin_Coor.y = TSCTXY.at(3).second+120. ;
    if(Up_Scin.Scin_Coor.y<TSCTXY.at(3).second)
      Up_Scin.Scin_Coor.y = TSCTXY.at(3).second ;
    Up_Scin.index= 3 ;
  }
  if(Up_Scin.Scin_Coor.x>(TSCTXY.at(3).first+25.+TSCTXY.at(4).first)/2.
    &&Up_Scin.Scin_Coor.x<=(TSCTXY.at(4).first+25.+TSCTXY.at(5).first)/2.)
  {
    if(Up_Scin.Scin_Coor.x<TSCTXY.at(4).first)
      Up_Scin.Scin_Coor.x = TSCTXY.at(4).first ;
    if(Up_Scin.Scin_Coor.x>TSCTXY.at(4).first+25.)
      Up_Scin.Scin_Coor.x = TSCTXY.at(4).first+25. ;
    if(Up_Scin.Scin_Coor.y>TSCTXY.at(4).second+120.)
      Up_Scin.Scin_Coor.y = TSCTXY.at(4).second+120. ;
    if(Up_Scin.Scin_Coor.y<TSCTXY.at(4).second)
      Up_Scin.Scin_Coor.y = TSCTXY.at(4).second ;
    Up_Scin.index= 4 ;
  }
  if(Up_Scin.Scin_Coor.x>(TSCTXY.at(4).first+25.+TSCTXY.at(5).first)/2.)
  {
    if(Up_Scin.Scin_Coor.x<TSCTXY.at(5).first)
      Up_Scin.Scin_Coor.x = TSCTXY.at(5).first ;
    if(Up_Scin.Scin_Coor.x>TSCTXY.at(5).first+25.)
      Up_Scin.Scin_Coor.x = TSCTXY.at(5).first+25. ;
    if(Up_Scin.Scin_Coor.y>TSCTXY.at(5).second+120.)
      Up_Scin.Scin_Coor.y = TSCTXY.at(5).second+120. ;
    if(Up_Scin.Scin_Coor.y<TSCTXY.at(5).second)
      Up_Scin.Scin_Coor.y = TSCTXY.at(5).second ;
    Up_Scin.index= 5 ;
  }
  Up_Scin.Measure_Time = KM2ATime[UpScinV1290NChannel]*1000;

  // position of down scin:(xmin, xmax) (ymin, ymax)
  // 0: (0.2, 25.2) (5, 125)
  // 1: (28, 53) (5, 125)
  // 2: (55.9, 80.9) (5, 125)
  // 3: (84.4, 109.4) (3, 123)
  // 4: (112.8, 137.8) (2, 122)
  // 5: (140.2, 165.2) (2, 122)
  if(Down_Scin.Scin_Coor.x<=(TSCTXY.at(6).first+TSCTXY.at(7).first+25.)/2.)
  {
    if(Down_Scin.Scin_Coor.x<TSCTXY.at(6).first)
      Down_Scin.Scin_Coor.x = TSCTXY.at(6).first ;
    if(Down_Scin.Scin_Coor.x>TSCTXY.at(6).first+25.)
      Down_Scin.Scin_Coor.x = TSCTXY.at(6).first+25. ;
    if(Down_Scin.Scin_Coor.y>TSCTXY.at(6).second+120.)
      Down_Scin.Scin_Coor.y = TSCTXY.at(6).second+120. ;
    if(Down_Scin.Scin_Coor.y<TSCTXY.at(6).second)
      Down_Scin.Scin_Coor.y = TSCTXY.at(6).second ;
    Down_Scin.index = 6 ;
  }
  if(Down_Scin.Scin_Coor.x>(TSCTXY.at(6).first+25.+TSCTXY.at(7).first)/2.
     &&Down_Scin.Scin_Coor.x<=(TSCTXY.at(7).first+25.+TSCTXY.at(8).first)/2.)
  {
    if(Down_Scin.Scin_Coor.x<TSCTXY.at(7).first)
      Down_Scin.Scin_Coor.x = TSCTXY.at(7).first ;
    if(Down_Scin.Scin_Coor.x>TSCTXY.at(7).first+25.)
      Down_Scin.Scin_Coor.x = TSCTXY.at(7).first+25. ;
    if(Down_Scin.Scin_Coor.y>TSCTXY.at(7).second+120.)
      Down_Scin.Scin_Coor.y = TSCTXY.at(7).second+120. ;
    if(Down_Scin.Scin_Coor.y<TSCTXY.at(7).second)
      Down_Scin.Scin_Coor.y = TSCTXY.at(7).second ;
    Down_Scin.index= 7 ;
  }
  if(Down_Scin.Scin_Coor.x>(TSCTXY.at(7).first+25.+TSCTXY.at(8).first)/2.
     &&Down_Scin.Scin_Coor.x<=(TSCTXY.at(8).first+25.+TSCTXY.at(9).first)/2.)
  {
    if(Down_Scin.Scin_Coor.x<TSCTXY.at(8).first)
      Down_Scin.Scin_Coor.x = TSCTXY.at(8).first ;
    if(Down_Scin.Scin_Coor.x>TSCTXY.at(8).first+25.)
      Down_Scin.Scin_Coor.x = TSCTXY.at(8).first+25. ;
    if(Down_Scin.Scin_Coor.y>TSCTXY.at(8).second+120.)
      Down_Scin.Scin_Coor.y = TSCTXY.at(8).second+120. ;
    if(Down_Scin.Scin_Coor.y<TSCTXY.at(8).second)
      Down_Scin.Scin_Coor.y = TSCTXY.at(8).second ;
    Down_Scin.index= 8 ;
  }
  if(Down_Scin.Scin_Coor.x>(TSCTXY.at(8).first+25.+TSCTXY.at(9).first)/2.
     &&Down_Scin.Scin_Coor.x<=(TSCTXY.at(9).first+25.+TSCTXY.at(10).first)/2.)
  {
    if(Down_Scin.Scin_Coor.x<TSCTXY.at(9).first)
      Down_Scin.Scin_Coor.x = TSCTXY.at(9).first ;
    if(Down_Scin.Scin_Coor.x>TSCTXY.at(9).first+25.)
      Down_Scin.Scin_Coor.x = TSCTXY.at(9).first+25. ;
    if(Down_Scin.Scin_Coor.y>TSCTXY.at(9).second+120.)
      Down_Scin.Scin_Coor.y = TSCTXY.at(9).second+120. ;
    if(Down_Scin.Scin_Coor.y<TSCTXY.at(9).second)
      Down_Scin.Scin_Coor.y = TSCTXY.at(9).second ;
    Down_Scin.index= 9 ;
  }
  if(Down_Scin.Scin_Coor.x>(TSCTXY.at(9).first+25.+TSCTXY.at(10).first)/2.
     &&Down_Scin.Scin_Coor.x<=(TSCTXY.at(10).first+25.+TSCTXY.at(11).first)/2.)
  {
    if(Down_Scin.Scin_Coor.x<TSCTXY.at(10).first)
      Down_Scin.Scin_Coor.x = TSCTXY.at(10).first ;
    if(Down_Scin.Scin_Coor.x>TSCTXY.at(10).first+25.)
      Down_Scin.Scin_Coor.x = TSCTXY.at(10).first+25. ;
    if(Down_Scin.Scin_Coor.y>TSCTXY.at(10).second+120.)
      Down_Scin.Scin_Coor.y = TSCTXY.at(10).second+120. ;
    if(Down_Scin.Scin_Coor.y<TSCTXY.at(10).second)
      Down_Scin.Scin_Coor.y = TSCTXY.at(10).second ;
    Down_Scin.index= 10 ;
  }
  if(Down_Scin.Scin_Coor.x>(TSCTXY.at(10).first+25.+TSCTXY.at(11).first)/2.)
  {
    if(Down_Scin.Scin_Coor.x<TSCTXY.at(11).first)
      Down_Scin.Scin_Coor.x = TSCTXY.at(11).first ;
    if(Down_Scin.Scin_Coor.x>TSCTXY.at(11).first+25.)
      Down_Scin.Scin_Coor.x = TSCTXY.at(11).first+25. ;
    if(Down_Scin.Scin_Coor.y>TSCTXY.at(11).second+120.)
      Down_Scin.Scin_Coor.y = TSCTXY.at(11).second+120. ;
    if(Down_Scin.Scin_Coor.y<TSCTXY.at(11).second)
      Down_Scin.Scin_Coor.y = TSCTXY.at(11).second ;
    Down_Scin.index= 11 ;
  }
  Down_Scin.Measure_Time = KM2ATime[DownScinV1290NChannel]*1000 ;

	return true ;
}

void CalcuTGCScin::recalculate()
{
  Up_TGC.z=324.2;
  float a = (Up_Scin.Scin_Coor.x-Down_Scin.Scin_Coor.x)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z) ;
  float b = (Up_Scin.Scin_Coor.z*Down_Scin.Scin_Coor.x-Up_Scin.Scin_Coor.x*Down_Scin.Scin_Coor.z)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z) ;
  float  c = (Up_Scin.Scin_Coor.y-Down_Scin.Scin_Coor.y)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z) ;
  float d = (Up_Scin.Scin_Coor.z*Down_Scin.Scin_Coor.y-Up_Scin.Scin_Coor.y*Down_Scin.Scin_Coor.z)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z) ;
  Up_TGC.x = a*Up_TGC.z+b ;
  Up_TGC.y = c*Up_TGC.z+d ;
}

void CalcuTGCScin::recalculate1()
{
	Down_TGC.z=44.3;
	float   a = (Up_Scin.Scin_Coor.x-Down_Scin.Scin_Coor.x)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z),
	b = (Up_Scin.Scin_Coor.z*Down_Scin.Scin_Coor.x-Up_Scin.Scin_Coor.x*Down_Scin.Scin_Coor.z)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z),
	c = (Up_Scin.Scin_Coor.y-Down_Scin.Scin_Coor.y)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z),
	d = (Up_Scin.Scin_Coor.z*Down_Scin.Scin_Coor.y-Up_Scin.Scin_Coor.y*Down_Scin.Scin_Coor.z)/(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z);
	Down_TGC.x = a*Down_TGC.z+b ;
	Down_TGC.y = c*Down_TGC.z+d ;
}

void FillSignalWidth(vector<Cluster> ClusterVec, TH1F* Hist)
{
  for(vector<Cluster>::iterator cluster_iter = ClusterVec.begin(); cluster_iter != ClusterVec.end(); cluster_iter++)
  {
    for(vector<int>::iterator iter = cluster_iter->SignalWidth.begin(); iter != cluster_iter->SignalWidth.end(); iter++)
      Hist->Fill(*iter) ;
  }
}

void CalcuTGCScin::Calcu()
{
  int nEntries = inTree->GetEntries() ;
  cout<<"Total events: "<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
	{
    Clear() ;  // clear all the vectors

    if(i%10000==0)
      cout<<i<<endl ;

		inTree->GetEntry(i) ;
		GetAllClusters() ;
    
    // Fill Signal Width
    FillSignalWidth(Up_TGC_Strip_One_Cluster, UpTGCOneStripSignalWidth) ;
    FillSignalWidth(Up_TGC_Wire_One_Cluster, UpTGCOneWireSignalWidth) ;
    FillSignalWidth(Up_TGC_Strip_Two_Cluster, UpTGCTwoStripSignalWidth) ;
    FillSignalWidth(Up_TGC_Wire_Two_Cluster, UpTGCTwoWireSignalWidth) ;
    FillSignalWidth(Down_TGC_Strip_One_Cluster, DownTGCOneStripSignalWidth) ;
    FillSignalWidth(Down_TGC_Wire_One_Cluster, DownTGCOneWireSignalWidth) ;
    FillSignalWidth(Down_TGC_Strip_Two_Cluster, DownTGCTwoStripSignalWidth) ;
    FillSignalWidth(Down_TGC_Wire_Two_Cluster, DownTGCTwoWireSignalWidth) ;
		EventQuality = 0 ;
		//-------------------------------------
		// strip or wire number used to calculate coordinate of hits on TGC
		Up_TGC_One.Valid_Strip_Number = -1 ;
		Up_TGC_One.Valid_Wire_Number = -1 ;
		Up_TGC_Two.Valid_Strip_Number = -1 ;
		Up_TGC_Two.Valid_Wire_Number = -1 ;
		Down_TGC_One.Valid_Strip_Number = -1 ;
		Down_TGC_One.Valid_Wire_Number = -1 ;
		Down_TGC_Two.Valid_Strip_Number = -1 ;
		Down_TGC_Two.Valid_Wire_Number = -1 ;
		Delta_Up_Strip_ValidNumber = -1 ;
		Delta_Up_Wire_ValidNumber = -1 ;
		Delta_Down_Strip_ValidNumber = -1 ;
		Delta_Down_Wire_ValidNumber = -1 ;
    //------------------------------------
    eventAnalysis();
    outTree->Fill() ; // Fill outTree

    EventQualityDis->Fill(EventQuality) ;
    if(EventQuality>0)
    {
      UpScinX->Fill(Up_Scin.Scin_Coor.x) ;
      DownScinX->Fill(Down_Scin.Scin_Coor.x) ;
    }
    
  }
  outFile->Write() ;

  TFile* GlobalInfo = new TFile((outPath+"GlobalInfo_"+conf->CORARSCollectionTime+".root").c_str(), "update") ;
  cout<<"Global File: "<<GlobalInfo->GetName()<<endl ;
  EventQualityDis->Write() ;
  UpTGCOneStripSignalWidth->Write() ;
  UpTGCOneWireSignalWidth->Write() ;
  UpTGCTwoStripSignalWidth->Write() ;
  UpTGCTwoWireSignalWidth->Write() ;
  DownTGCOneStripSignalWidth->Write() ;
  DownTGCOneWireSignalWidth->Write() ;
  DownTGCTwoStripSignalWidth->Write() ;
  DownTGCTwoWireSignalWidth->Write() ;
  // get the scatter diagram of Scintillator
  UpScinX->GetXaxis()->SetTitle("x (cm)") ;
  DownScinX->GetXaxis()->SetTitle("x (cm)") ;
  UpScinX->Write() ;
  DownScinX->Write() ;

  outTree->Draw("Up_Scin.y:Up_Scin.x","EventQuality>0") ;
  UpScinHitsGraph = (TGraph*)((TGraph*)gPad->GetPrimitive("Graph"))->Clone();
  UpScinHitsGraph->SetName("UpScinHitsGraph") ;
  UpScinHitsGraph->SetTitle("Hits of Up Scintillator") ;
  UpScinHitsGraph->GetXaxis()->SetTitle("x (cm)") ;
  UpScinHitsGraph->GetYaxis()->SetTitle("y (cm)") ;
  outTree->Draw("Down_Scin.y:Down_Scin.x","EventQuality>0") ;
  DownScinHitsGraph = (TGraph*)((TGraph*)gPad->GetPrimitive("Graph"))->Clone();
  DownScinHitsGraph->SetName("DownScinHitsGraph") ;
  DownScinHitsGraph->SetTitle("Hits of Down Scintillator") ;
  DownScinHitsGraph->GetXaxis()->SetTitle("x (cm)") ;
  DownScinHitsGraph->GetYaxis()->SetTitle("y (cm)") ;
  UpScinHitsGraph->Write() ;
  DownScinHitsGraph->Write() ;
  GlobalInfo->Close() ;
  delete GlobalInfo ;
}

void GetClusterSize(vector<Cluster> TGC_Cluster, vector<int>* cluster_size, vector<int>* cluster_maxsignal, bool allClusterSizeLessThan4)
{
  for(int n=0 ; n<(int)TGC_Cluster.size() ; n++)
  { 
    cluster_size->push_back(TGC_Cluster[n].Number.size()) ;
    if( TGC_Cluster[n].Number.size() >4)
      allClusterSizeLessThan4 = false;
    cluster_maxsignal->push_back(TGC_Cluster[n].GetMaxSignalWidth()) ;
  }
}

void CalcuTGCScin::eventAnalysis()
{
  int counttrack = 0 ;
  int max=-10000 ;
  int max1=-10000 ;
  int up_one_valid_strip_number = -1 ;
  int up_one_valid_wire_number = -1 ;
  int up_two_valid_strip_number = -1 ; 
  int up_two_valid_wire_number = -1 ;
  int down_one_valid_strip_number = -1 ; 
  int down_one_valid_wire_number = -1 ; 
  int down_two_valid_strip_number = -1 ; 
  int down_two_valid_wire_number = -1 ;

  int up_one_strip_vectorcluster_size = Up_TGC_Strip_One_Cluster.size() ;
  int up_one_wire_vectorcluster_size = Up_TGC_Wire_One_Cluster.size() ;
  int up_two_strip_vectorcluster_size = Up_TGC_Strip_Two_Cluster.size() ;
  int up_two_wire_vectorcluster_size = Up_TGC_Wire_Two_Cluster.size() ;
  int down_one_strip_vectorcluster_size = Down_TGC_Strip_One_Cluster.size() ;
  int down_one_wire_vectorcluster_size = Down_TGC_Wire_One_Cluster.size() ;
  int down_two_strip_vectorcluster_size = Down_TGC_Strip_Two_Cluster.size() ;
  int down_two_wire_vectorcluster_size = Down_TGC_Wire_Two_Cluster.size() ;

  bool allClusterSizeLessThan4 = true;

  GetClusterSize(Up_TGC_Strip_One_Cluster, &up_one_strip_cluster_size, &up_one_strip_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Up_TGC_Wire_One_Cluster, &up_one_wire_cluster_size, &up_one_wire_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Down_TGC_Strip_One_Cluster, &down_one_strip_cluster_size, &down_one_strip_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Down_TGC_Wire_One_Cluster, &down_one_wire_cluster_size, &down_one_wire_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Up_TGC_Strip_Two_Cluster, &up_two_strip_cluster_size, &up_two_strip_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Up_TGC_Wire_Two_Cluster, &up_two_wire_cluster_size, &up_two_wire_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Down_TGC_Strip_Two_Cluster, &down_two_strip_cluster_size, &down_two_strip_cluster_maxsignal, allClusterSizeLessThan4) ;
  GetClusterSize(Down_TGC_Wire_Two_Cluster, &down_two_wire_cluster_size, &down_two_wire_cluster_maxsignal, allClusterSizeLessThan4) ;
  /*
  for(int n=0 ; n<(int)Up_TGC_Strip_One_Cluster.size() ; n++)
  {  
    up_one_strip_cluster_size.push_back(Up_TGC_Strip_One_Cluster[n].Number.size()) ;
    if( Up_TGC_Strip_One_Cluster[n].Number.size() >4) 
      allClusterSizeLessThan4 = false;
    up_one_strip_cluster_maxsignal.push_back(Up_TGC_Strip_One_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Up_TGC_Wire_One_Cluster.size() ; n++)
  { 
    up_one_wire_cluster_size.push_back(Up_TGC_Wire_One_Cluster[n].Number.size()) ;
    if( Up_TGC_Wire_One_Cluster[n].Number.size() >4) 
      allClusterSizeLessThan4 = false;
    up_one_wire_cluster_maxsignal.push_back(Up_TGC_Wire_One_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Up_TGC_Strip_Two_Cluster.size() ; n++)
  { 
    up_two_strip_cluster_size.push_back(Up_TGC_Strip_Two_Cluster[n].Number.size()) ;
    if( Up_TGC_Strip_Two_Cluster[n].Number.size() >4) 
      allClusterSizeLessThan4 = false;
	  up_two_strip_cluster_maxsignal.push_back(Up_TGC_Strip_Two_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Up_TGC_Wire_Two_Cluster.size() ; n++)
  { 
    up_two_wire_cluster_size.push_back(Up_TGC_Wire_Two_Cluster[n].Number.size()) ;
    if( Up_TGC_Wire_Two_Cluster[n].Number.size()>4) 
      allClusterSizeLessThan4 = false;
	  up_two_wire_cluster_maxsignal.push_back(Up_TGC_Wire_Two_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Down_TGC_Strip_One_Cluster.size() ; n++)
  { 
    down_one_strip_cluster_size.push_back(Down_TGC_Strip_One_Cluster[n].Number.size()) ;
    if( Down_TGC_Strip_One_Cluster[n].Number.size()>4) 
      allClusterSizeLessThan4 = false;
	  down_one_strip_cluster_maxsignal.push_back(Down_TGC_Strip_One_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Down_TGC_Wire_One_Cluster.size() ; n++)
  { 
    down_one_wire_cluster_size.push_back(Down_TGC_Wire_One_Cluster[n].Number.size()) ;
    if( Down_TGC_Wire_One_Cluster[n].Number.size()>4) 
      allClusterSizeLessThan4 = false;
    down_one_wire_cluster_maxsignal.push_back(Down_TGC_Wire_One_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Down_TGC_Strip_Two_Cluster.size() ; n++)
  { 
    down_two_strip_cluster_size.push_back(Down_TGC_Strip_Two_Cluster[n].Number.size()) ;
    if( Down_TGC_Strip_Two_Cluster[n].Number.size()>4) 
      allClusterSizeLessThan4 = false;
	  down_two_strip_cluster_maxsignal.push_back(Down_TGC_Strip_Two_Cluster[n].GetMaxSignalWidth()) ;
  }
  for(int n=0 ; n<(int)Down_TGC_Wire_Two_Cluster.size() ; n++)
  { 
    down_two_wire_cluster_size.push_back(Down_TGC_Wire_Two_Cluster[n].Number.size()) ;
    if(Down_TGC_Wire_Two_Cluster[n].Number.size() >4) 
      allClusterSizeLessThan4 = false;
    down_two_wire_cluster_maxsignal.push_back(Down_TGC_Wire_Two_Cluster[n].GetMaxSignalWidth()) ;
  }
  */
  GetClusterValidNumbers(Up_TGC_Strip_One_Cluster,&up_one_strip_cluster_validnumber);
  GetClusterValidNumbers(Up_TGC_Wire_One_Cluster,&up_one_wire_cluster_validnumber);
  GetClusterValidNumbers(Up_TGC_Strip_Two_Cluster,&up_two_strip_cluster_validnumber);
  GetClusterValidNumbers(Up_TGC_Wire_Two_Cluster,&up_two_wire_cluster_validnumber);
  GetClusterValidNumbers(Down_TGC_Strip_One_Cluster,&down_one_strip_cluster_validnumber);
  GetClusterValidNumbers(Down_TGC_Wire_One_Cluster,&down_one_wire_cluster_validnumber);
  GetClusterValidNumbers(Down_TGC_Strip_Two_Cluster,&down_two_strip_cluster_validnumber);
  GetClusterValidNumbers(Down_TGC_Wire_Two_Cluster,&down_two_wire_cluster_validnumber);

  //  no hits on top two TGCs or bottom two TGCs
  // absolutely unuseful events, eventquality = -1
  if(((Up_TGC_Strip_One_Cluster.size()==0||Up_TGC_Wire_One_Cluster.size()==0)
	   &&(Up_TGC_Strip_Two_Cluster.size()==0||Up_TGC_Wire_Two_Cluster.size()==0))
	   ||((Down_TGC_Strip_One_Cluster.size()==0||Down_TGC_Wire_One_Cluster.size()==0)
	   &&(Down_TGC_Strip_Two_Cluster.size()==0||Down_TGC_Wire_Two_Cluster.size()==0)))
  { 
	  EventQuality = -1 ;
	  return; 
  }

	int firedN=  up_one_strip_vectorcluster_size 
	+ up_one_wire_vectorcluster_size
	+ up_two_strip_vectorcluster_size  
	+ up_two_wire_vectorcluster_size
	+ down_one_strip_vectorcluster_size
	+ down_one_wire_vectorcluster_size
	+ down_two_strip_vectorcluster_size 
	+ down_two_wire_vectorcluster_size;
  
  if(firedN==8&&(up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
  { 
    Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
    Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0]; 
    Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
    Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
    Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
    Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
    Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
    Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
    if(allClusterSizeLessThan4)
	  { 
	    if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<3&&
	       (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<3&&
	       (abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
	       (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
	    { 
	      Calcu_TGC() ;
	      if(Calcu_Scin()) 
	      { 
		      EventQuality = 1 ;
		      return ;           
		    }
	      else
		    {
		      EventQuality=-4;
		      EventQuality=-1;
		      return ;
		    }
	    }
   	////insert quality= 6 ---------------------------------
	  else
	  {
	    if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
	       (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
	       ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
	       (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
	    {
		    if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
		      (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
		    { 
		      Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		      Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];                      
		      Calcu_TGC();
		      if(Calcu_Scin())
		      {
            Up_TGC_Two.Valid_Strip_Number = -1 ; 
            Up_TGC_Two.Valid_Wire_Number = -1 ;
          }
		      else
		      {      
		        Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
			      Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
			      Calcu_TGC();
			      if(Calcu_Scin())
			      {
              Up_TGC_One.Valid_Strip_Number = -1 ; 
              Up_TGC_One.Valid_Wire_Number = -1 ;
            }
		      }
		    }
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				 	 (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{ 
          Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
				  Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
				  Calcu_TGC() ;
				  if(Calcu_Scin())
				  {
            Down_TGC_Two.Valid_Strip_Number=-1 ; 
            Down_TGC_Two.Valid_Wire_Number = -1 ;
          }
					else
					{  
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{
              Down_TGC_One.Valid_Strip_Number=-1 ; 
              Down_TGC_One.Valid_Wire_Number=-1 ;
            }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=6;
					return;
				} 
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				} 
			}
			if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)&&
					((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
			{
				Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
				Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
				Calcu_TGC();
				if(Calcu_Scin())
				{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
				else
				{
					Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
					Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
				}
					
				Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
				Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
				Calcu_TGC() ;
				if(Calcu_Scin())
				{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
				else
				{
					Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
					Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
				}
				
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=7;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
			////----------end of quality=6 he 7 ---------------------
	}
		///////////insert quality=2 , 8 , 9-----------------------------------

		else 
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=2;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-2;
					return ;
				}
			}
			/////insert quality= 8 ---------------------------------------
			else 
			{  
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{ 
						Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
						Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{ 
							Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
							Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];       
							Calcu_TGC() ;
							if(Calcu_Scin()) 
							{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3)
					{ 
						Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
						Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{  
							Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
							Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
							Calcu_TGC() ; 
							if(Calcu_Scin())
							{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}  
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality= 8;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-8;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)&&
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3))
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC();
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}

					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
					}
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality= 9;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-9;
						return ;
					}
				}
			}
		}
		
	}


	//event quality =3
	int firedNstrip  =   up_one_strip_vectorcluster_size 
	+ up_two_strip_vectorcluster_size  
	+ down_one_strip_vectorcluster_size
	+ down_two_strip_vectorcluster_size ;
	int firedNwire=  up_one_wire_vectorcluster_size
	+ up_two_wire_vectorcluster_size
	+ down_one_wire_vectorcluster_size
	+ down_two_wire_vectorcluster_size;

	if( up_one_strip_vectorcluster_size!=0 &&
			up_one_wire_vectorcluster_size!=0 &&
			up_two_strip_vectorcluster_size!=0&&
			up_two_wire_vectorcluster_size!=0&&
			down_one_strip_vectorcluster_size!=0&&
			down_one_wire_vectorcluster_size!=0 &&
			down_two_strip_vectorcluster_size!=0 &&
			down_two_wire_vectorcluster_size!=0)
	{
		if( allClusterSizeLessThan4 )
		{
			if((firedNstrip==4)&&(firedNwire==5)) 
			{ 
				Up_TGC_One.Valid_Strip_Number=getValidLineNumber(up_one_strip_cluster_validnumber,up_two_strip_cluster_validnumber);
				Up_TGC_One.Valid_Wire_Number=getValidLineNumber(up_one_wire_cluster_validnumber,up_two_wire_cluster_validnumber );
				Up_TGC_Two.Valid_Strip_Number=getValidLineNumber(up_two_strip_cluster_validnumber,up_one_strip_cluster_validnumber);
				Up_TGC_Two.Valid_Wire_Number=getValidLineNumber(up_two_wire_cluster_validnumber,up_one_wire_cluster_validnumber );
				Down_TGC_One.Valid_Strip_Number=getValidLineNumber(down_one_strip_cluster_validnumber,down_two_strip_cluster_validnumber);
				Down_TGC_One.Valid_Wire_Number=getValidLineNumber(down_one_wire_cluster_validnumber,down_two_wire_cluster_validnumber );
				Down_TGC_Two.Valid_Strip_Number=getValidLineNumber(down_two_strip_cluster_validnumber,down_one_strip_cluster_validnumber);
				Down_TGC_Two.Valid_Wire_Number=getValidLineNumber(down_two_wire_cluster_validnumber,down_one_wire_cluster_validnumber );
				
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<3&&
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<3&&
						(abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<3&&
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<3)  
				{ 
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=3;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-3;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)||
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)
					{
						Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
						Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
							Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3)
					{
						Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
						Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
							Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=6;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-6;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
				    (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)&&
				   ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
				    (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3))
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC();
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}

					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
					}
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality= 7;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-7;
						return ;
					}
				} 
			}
			
			if((firedNstrip==5)&&(firedNwire==4))
			{
				Up_TGC_One.Valid_Strip_Number=getValidLineNumber(up_one_strip_cluster_validnumber, up_two_strip_cluster_validnumber );            
				Up_TGC_One.Valid_Wire_Number=getValidLineNumber(up_one_wire_cluster_validnumber,up_two_wire_cluster_validnumber);
				Up_TGC_Two.Valid_Strip_Number=getValidLineNumber(up_two_strip_cluster_validnumber, up_one_strip_cluster_validnumber );
				Up_TGC_Two.Valid_Wire_Number=getValidLineNumber(up_two_wire_cluster_validnumber,up_one_wire_cluster_validnumber);
				Down_TGC_One.Valid_Strip_Number=getValidLineNumber(down_one_strip_cluster_validnumber,down_two_strip_cluster_validnumber );
				Down_TGC_One.Valid_Wire_Number=getValidLineNumber(down_one_wire_cluster_validnumber,down_two_wire_cluster_validnumber);
				Down_TGC_Two.Valid_Strip_Number=getValidLineNumber(down_two_strip_cluster_validnumber,down_one_strip_cluster_validnumber );
				Down_TGC_Two.Valid_Wire_Number=getValidLineNumber(down_two_wire_cluster_validnumber,down_one_wire_cluster_validnumber);   
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<3&&
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<3&&
						(abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<3&&
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=4;
						return;
					}
					else
					{
						EventQuality=-4;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)||
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)
					{
						Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
						Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
							Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3)
					{
						Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
						Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
							Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=6;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-6;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)&&
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3))
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC();
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}

					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 7;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
			if((firedNstrip==5)&&(firedNwire==5))
			{
				Up_TGC_One.Valid_Strip_Number=getValidLineNumber(up_one_strip_cluster_validnumber, up_two_strip_cluster_validnumber );            
				Up_TGC_One.Valid_Wire_Number=getValidLineNumber(up_one_wire_cluster_validnumber,up_two_wire_cluster_validnumber );
				Up_TGC_Two.Valid_Strip_Number=getValidLineNumber(up_two_strip_cluster_validnumber, up_one_strip_cluster_validnumber );
				Up_TGC_Two.Valid_Wire_Number=getValidLineNumber(up_two_wire_cluster_validnumber,up_one_wire_cluster_validnumber );
				Down_TGC_One.Valid_Strip_Number=getValidLineNumber(down_one_strip_cluster_validnumber,down_two_strip_cluster_validnumber );
				Down_TGC_One.Valid_Wire_Number=getValidLineNumber(down_one_wire_cluster_validnumber,down_two_wire_cluster_validnumber );
				Down_TGC_Two.Valid_Strip_Number=getValidLineNumber(down_two_strip_cluster_validnumber,down_one_strip_cluster_validnumber );
				Down_TGC_Two.Valid_Wire_Number=getValidLineNumber(down_two_wire_cluster_validnumber,down_one_wire_cluster_validnumber );
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<3&&
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<3&&
						(abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<3&&
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<3)
				{
					Calcu_TGC() ;

					if(Calcu_Scin())
					{
						EventQuality=5;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-5;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)||
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)
					{
						Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
						Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
							Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3)
					{
						Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
						Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
							Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=6;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-6;
						return ;
					}
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>=3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>=3)&&
				   ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>=3))
				 {
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC();
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}

					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
					}
					
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality= 7;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-7;
						return ;
					}
				}
			}
		}
		
	}

	if(((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0)))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		
               if( allClusterSizeLessThan4 )
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=6;
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else  /////insert quality=7---------------------------------- 
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{ 
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0]; 
					Calcu_TGC() ;
					if(Calcu_Scin())                 
					{  Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0]; 
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=7;    /////insert quality=7----------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		else 
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=8;   //////insert quality=8--------------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-8;
					return ;
				}
			}
			else 
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{ 
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];    
					Calcu_TGC() ;
					if(Calcu_Scin())
					{  Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=9;   //////insert quality=9---------------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}
			}
		}
	}

	if(((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))) 
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];

		if( allClusterSizeLessThan4 )
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=6;
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else 
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{ 
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())           
					{  Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=7;    /////insert quality=7----------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		else
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=8;  ////////insert quality=8-------------------------------- 
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else 
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{  
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=9;  ////////insert quality=9--------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}  

			}
		}
	}

	if(((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1)))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];

		if( allClusterSizeLessThan4 )
		{ 
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3 )
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=6;
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else 
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0]; 
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}  
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=7;    /////insert quality=7-------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		
		else 
		{
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3 )
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=8;      ////////insert quality=8--------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else
			{ 
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}
					
					Calcu_TGC() ;     
					if(Calcu_Scin())
					{
						EventQuality=9;    /////insert quality= 9 ----------------------------------
						nosignal=1;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-9;
						return ;
					}      
				}
			}

		}     
	}

	if(((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1)))
	{
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];

		if( allClusterSizeLessThan4 )
		{  
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3 )
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=6;
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 7 ;    /////insert quality= 7 ----------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}  
		} 
		
		else 
		{
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3 )
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=8;  ////////insert quality=8--------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())                   
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 9 ;  ////////insert quality= 9 --------------------------------
					nosignal=1;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}
			}

		}
	}  
	//////end of EQ=6 ,7,8,9 event
	//------------------------------------------------------------------------------------------
	///event quality = 7 

	if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];   
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0]; 

		if( allClusterSizeLessThan4)
		{
			Calcu_TGC() ;
			if(Calcu_Scin()) 
			{
				EventQuality=7;
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-7;
				return ;
			} 
		}
		else
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				EventQuality=9;  
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-9;
				return ;
			}
		}
	}

	if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
		
		if( allClusterSizeLessThan4)
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				EventQuality=7;
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-7;
				return ;
			}
		}
		else
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{   
				EventQuality=9; 
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-9;
				return ;
			}
		}
	}

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		
		if( allClusterSizeLessThan4)
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				EventQuality=7;
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-7;
				return ;
			}
		}
		else
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				EventQuality=9;
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-9;
				return ;
			}
		}
	}

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
	{ 
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
		
		if( allClusterSizeLessThan4)
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				EventQuality=7;
				nosignal=1; 
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-7;
				return ;
			}
		}
		else
		{
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				EventQuality=9;
				nosignal=1;
				return;
			}
			else
			{
				EventQuality=-4;
				EventQuality=-9;
				return ;
			}
		}
	}
	///end of event quality=7 ,  9 --------------------------------------------------------------------

 
	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<up_two_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<up_two_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_one_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_one_wire_vectorcluster_size ; q++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[n] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[o] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[p] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{  
				if(up_one_strip_cluster_maxsignal[l]>max)
				{
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[n];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[o];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[p];
					down_one_valid_wire_number=down_one_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality =-40;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18;
							return;
						}
						else
						{
							EventQuality =-18;
							return;
						}
					}
					else 
          { 
            EventQuality = -18 ;
            return; 
          } 
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number =-1;Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19;
						return;
					}
					else
					{
						EventQuality =-19;
						return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality =20 ;
							return;
						}
						else
						{
							EventQuality =-20;
							return;
						}
					}
					else { EventQuality = -20 ; return; }  
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ; }
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number =-1;Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 21;
						return;
					}
					else
					{
						EventQuality =-21;
						return;
					}
				}
				else { EventQuality = -21 ; return; }
			}
		} 
	}	 

	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<up_two_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<up_two_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_two_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_two_wire_vectorcluster_size ; q++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[n] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[o] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[p] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_one_strip_cluster_maxsignal[l]>max)
				{  
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[n];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[o];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[p];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		} 
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;

			if(allClusterSizeLessThan4)
			{ 
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18 ;
							return;
						}
						else
						{
							EventQuality = -18 ;
							return;
						}
					}
					else { EventQuality = -18 ; return; }
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
					Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ; }
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number =-1;Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19 ;
						return;
					} 
					else
					{
						EventQuality = -19 ;
						return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 20 ;
							return;
						}
						else
						{
							EventQuality = -20 ;
							return;
						}
					}
					else { EventQuality = -20 ; return; }
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
					Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ;  Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number =-1;Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 21 ;
						return;
					}
					else
					{
						EventQuality = -21 ;
						return;
					}
				}
				else { EventQuality = -21 ; return; }
			}
		}
	}

	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<down_one_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_two_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_two_wire_vectorcluster_size ; q++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[o] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[p] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(down_one_strip_cluster_maxsignal[n]>max1)
				{
					max1=down_one_strip_cluster_maxsignal[n] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
					down_one_valid_wire_number = down_one_wire_cluster_validnumber[o];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[p];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;

			if(allClusterSizeLessThan4)
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18 ;
							return;
						}
						else
						{
							EventQuality = -18;
							return;
						}
					}
					else { EventQuality = -18 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1; Down_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19 ;
						return;
					}
					else
					{
						EventQuality = -19;
						return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 20 ;
							return;
						}
						else
						{
							EventQuality = -20;
							return;
						}
					}
					else { EventQuality = -20 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1; Down_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 21 ;
						return;
					}
					else
					{
						EventQuality = -21;
						return;
					}
				}
				else { EventQuality = -21 ; return; }
			}
		}

	}

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))  
	{
		for(int l=0 ; l<up_two_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_two_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<down_one_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_two_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_two_wire_vectorcluster_size ; q++)
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[l] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[m] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[o] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[p] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(down_one_strip_cluster_maxsignal[n]>max1)
				{  
					max1=down_one_strip_cluster_maxsignal[n] ;
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[l];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[m];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
					down_one_valid_wire_number = down_one_wire_cluster_validnumber[o];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[p];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[q]; 
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{ 
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18 ;
							return;
						}
						else
						{
							EventQuality = -18 ;
							return;
						}
					}
					else { EventQuality = -18 ; return; }       
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
					Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ; }
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1; Down_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19 ;
						return;
					}
					else
					{
						EventQuality = -19 ;
						return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 20 ;
							return;
						}
						else
						{
							EventQuality = -20 ;
							return;
						}
					}
					else { EventQuality = -20 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
					Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ; }
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1; Down_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 21 ;
						return;
					}
					else
					{
						EventQuality = -21 ;
						return;
					}
				}
				else { EventQuality = -21 ; return; } 
			}
		}
		
	}


	//////end of event quality=18 , 19 , 20 , 21  ----------------------------------------------------------------


	/////enent quality=22 , 23   -------------------------------------------------------------------------


	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
		for(int k=0 ; k<down_one_wire_vectorcluster_size ; k++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[k] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_one_strip_cluster_maxsignal[l]>max) 
				{
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number = up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
					down_one_valid_wire_number=down_one_wire_cluster_validnumber[k];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Calcu_TGC() ;
			if(allClusterSizeLessThan4)
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 22 ;
						return;
					}
					else
					{ 
						EventQuality = -22 ;
						return;
					}
				}
				else { EventQuality = -22 ; return; }
			}
			else
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 23 ;
						return;
					}
					else
					{
						EventQuality = -23 ;
						return;
					}
				}
				else { EventQuality = -23 ; return; }
			}
		}
	}

	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_two_strip_vectorcluster_size ; n++)
		for(int k=0 ; k<down_two_wire_vectorcluster_size ; k++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[n] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[k] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_one_strip_cluster_maxsignal[l]>max) 
				{  
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number = up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[n];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[k];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			Calcu_TGC() ;
			if(allClusterSizeLessThan4)
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 22 ;
						return;
					}
					else 
					{
						EventQuality = -22;
						return;
					}
				}
				else { EventQuality = -22 ; return; }
			}
			else
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 23 ;
						return;
					}
					else
					{
						EventQuality = -23 ;
						return;
					}
				}
				else { EventQuality = -23 ; return; }
			}
		}
	}   

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		for(int l=0 ; l<up_two_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_two_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
		for(int k=0 ; k<down_one_wire_vectorcluster_size ; k++)
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[l] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[m] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[k] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_two_strip_cluster_maxsignal[l]>max) 
				{  
					max=up_two_strip_cluster_maxsignal[l] ; 
					up_two_valid_strip_number = up_two_strip_cluster_validnumber[l];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[m];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
					down_one_valid_wire_number=down_one_wire_cluster_validnumber[k];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Calcu_TGC() ;
			if(allClusterSizeLessThan4)
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 22 ;
						return;
					}
					else 
					{
						EventQuality = -22 ;
						return;
					}
				}
				else { EventQuality = -22 ; return; }
			}
			else
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 23 ;
						return;
					}
					else
					{
						EventQuality = -23 ;
						return;
					}
				}
				else { EventQuality = -23 ; return; }
			}
		}
	}  

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_two_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_two_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_two_strip_vectorcluster_size ; n++)
		for(int k=0 ; k<down_two_wire_vectorcluster_size ; k++)
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[l] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[m] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[n] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[k] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_two_strip_cluster_maxsignal[l]>max) 
				{  
					max=up_two_strip_cluster_maxsignal[l] ;
					up_two_valid_strip_number = up_two_strip_cluster_validnumber[l];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[m];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[n];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[k];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			Calcu_TGC() ;
			if(allClusterSizeLessThan4)
			{ 
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 22 ;
						return;
					}
					else 
					{
						EventQuality = -22 ;
						return;
					}
				}
				else { EventQuality = -22 ; return; }
			}
			else
			{
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 23 ;
						return;
					}
					else
					{
						EventQuality = -23 ;
						return;
					}
				}
				else { EventQuality = -23 ; return; }
			}
		}
	}

	//////end of event quality=22 , 23   -------------------------------------------------------------------------------------------

	if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
		if(allClusterSizeLessThan4)
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<3&&
			   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<3&&
			   (abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<3&&
			   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{ 
					EventQuality=1; /////--event quality=1---------------------------------------------------
					return ;  
				}
				else
				{
					EventQuality=-4;
					EventQuality=-1;
					return ;
				}
			}
			else
			{
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
				  ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				  (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
						Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
							Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
						Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())           
						{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
							Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
						} 
					}
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=6;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-6;
						return ;
					}
				}
				
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)&&
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)) 
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC();
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}

					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=7;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				} 
			}
		}
		else
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
			   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3&&
			   (abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
			   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=2;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-2;
					return ;
				}
			}
			else
			{
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
						Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin()) 
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
							Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3)
					{
						Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
						Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
							Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
							Calcu_TGC() ;
							if(Calcu_Scin())
							{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
						}
					}

					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=8;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-8;
						return ;
					} 
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)&&
						((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3))
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC();
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC();
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}

					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Down_TGC_Two.Valid_Strip_Number=-1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number=-1 ; Down_TGC_One.Valid_Wire_Number=-1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=9;
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}
			}      
		}   
	}

	if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		if(allClusterSizeLessThan4)
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{  
					EventQuality= 6 ;
					return ;        
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else  
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{  Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{ 
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					} 
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 7 ;    
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		else
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 8 ;   
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 9 ;   
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}
			}
		}
	}

	if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
		if(allClusterSizeLessThan4)
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 6 ;
					return ;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 7 ;    
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		else
		{
			if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 8 ;  
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
					Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{  Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
						Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_One.Valid_Strip_Number = -1 ; Up_TGC_One.Valid_Wire_Number = -1 ;}
					} 
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 9 ;  
					return;
				} 
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}
			}
		} 
		
	}

	if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
	{
		Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
		Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
		if(allClusterSizeLessThan4)
		{
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 6 ;
					return ;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0]; 
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 7 ;    
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		else
		{
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3 )
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 8 ;      
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}            
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=9;    
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				}

			}
		}
	}

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
	{
		Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
		Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
		Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
		Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
		Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
		Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
		if(allClusterSizeLessThan4)
		{
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=6;
					return ;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-6;
					return ;
				}
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin()) 
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					} 
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=7;    
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-7;
					return ;
				}
			}
		}
		else
		{
			if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
					(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3 )
			{
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality=8;  
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-8;
					return ;
				}
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
						(abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
					Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
						Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
						Calcu_TGC() ;
						if(Calcu_Scin()) 
						{ Down_TGC_One.Valid_Strip_Number = -1 ; Down_TGC_One.Valid_Wire_Number = -1 ;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					EventQuality= 9 ;  
					return;
				}
				else
				{
					EventQuality=-4;
					EventQuality=-9;
					return ;
				} 

			}

		}
	}

	if(((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
			||((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
			||((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
			||(((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))))
	{

		if(((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
				||((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
				||((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
				||(((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))))
		{
			if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
			{
				Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
				Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
				Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
				Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0];   
				if(allClusterSizeLessThan4)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=7;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-7;
						return ;
					}  
				}
				else
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=9;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-9;
						return ;
					}
				}
			}

			if((up_one_strip_vectorcluster_size==1&&up_one_wire_vectorcluster_size==1)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
			{
				Up_TGC_One.Valid_Strip_Number=up_one_strip_cluster_validnumber[0];
				Up_TGC_One.Valid_Wire_Number=up_one_wire_cluster_validnumber[0];
				Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
				Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
				if(allClusterSizeLessThan4)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=7;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-7;
						return ;
					}
				}
				else
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=9;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-9;
						return ;
					}
				} 
			}

			if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==1&&down_one_wire_vectorcluster_size==1)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
			{
				Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
				Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0];
				Down_TGC_One.Valid_Strip_Number=down_one_strip_cluster_validnumber[0];
				Down_TGC_One.Valid_Wire_Number=down_one_wire_cluster_validnumber[0]; 
				if(allClusterSizeLessThan4)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=7;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-7;
						return ;
					}
				}
				else
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=9;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-9;
						return ;
					}
				}

			}

			if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size==1&&up_two_wire_vectorcluster_size==1)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size==1&&down_two_wire_vectorcluster_size==1))
			{
				Up_TGC_Two.Valid_Strip_Number=up_two_strip_cluster_validnumber[0];
				Up_TGC_Two.Valid_Wire_Number=up_two_wire_cluster_validnumber[0]; 
				Down_TGC_Two.Valid_Strip_Number=down_two_strip_cluster_validnumber[0];
				Down_TGC_Two.Valid_Wire_Number=down_two_wire_cluster_validnumber[0];
				if(allClusterSizeLessThan4)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=7;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-7;
						return ;
					}
				}
				else
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						EventQuality=9;
						return;
					}
					else
					{
						EventQuality=-4;
						EventQuality=-9;
						return ;
					}
				}
			}
		}
		///------------------------------------------------------------------------////////////////////////////-------------------------------------------------
		else{

			if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
			{
				for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
				for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
				for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
				for(int k=0 ; k<down_one_wire_vectorcluster_size ; k++)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
					Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
					Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
					Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[k] ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(up_one_strip_cluster_maxsignal[l]>max) 
						{
							max=up_one_strip_cluster_maxsignal[l] ;
							up_one_valid_strip_number = up_one_strip_cluster_validnumber[l];
							up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
							down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
							down_one_valid_wire_number=down_one_wire_cluster_validnumber[k];
						}
						counttrack++ ;
					}
				}
				if(counttrack<1)
				{
					EventQuality = -40 ;
					return;
				}
				else
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(allClusterSizeLessThan4)
					{
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 22 ;
								return;
							}
							else
							{
								EventQuality = -22 ;
								return;
							}
						}
						else { EventQuality = -22 ;  return; }
					}
					else 
					{
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 23 ;
								return;
							}
							else
							{  
								EventQuality = -23 ;
								return;
							}
						}
						else { EventQuality = -23 ; return; }
					}
				}
			}

			if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
			{
				for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
				for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
				for(int n=0 ; n<down_two_strip_vectorcluster_size ; n++)
				for(int k=0 ; k<down_two_wire_vectorcluster_size ; k++)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
					Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
					Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[n] ;
					Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[k] ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(up_one_strip_cluster_maxsignal[l]>max) 
						{  
							max=up_one_strip_cluster_maxsignal[l] ;
							up_one_valid_strip_number = up_one_strip_cluster_validnumber[l];
							up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
							down_two_valid_strip_number=down_two_strip_cluster_validnumber[n];
							down_two_valid_wire_number=down_two_wire_cluster_validnumber[k];
						}
						counttrack++ ;
					}
				}
				if(counttrack<1)
				{
					EventQuality = -40 ;
					return;
				}
				else
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
					Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
					Calcu_TGC() ;
					if(allClusterSizeLessThan4)
					{
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 22 ;
								return;
							}
							else
							{
								EventQuality = -22 ;
								return;
							}
						}
						else { EventQuality = -22 ; return; }
					}
					else
					{
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 23 ;
								return;
							}
							else
							{
								EventQuality = -23 ;
								return;
							}
						}
						else { EventQuality = -23 ; return; }
					}
				}
			}

			if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
			{
				for(int l=0 ; l<up_two_strip_vectorcluster_size ; l++)
				for(int m=0 ; m<up_two_wire_vectorcluster_size ; m++)
				for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
				for(int k=0 ; k<down_one_wire_vectorcluster_size ; k++)
				{
					Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[l] ;
					Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[m] ;
					Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
					Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[k] ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(up_two_strip_cluster_maxsignal[l]>max) 
						{  
							max=up_two_strip_cluster_maxsignal[l] ; 
							up_two_valid_strip_number = up_two_strip_cluster_validnumber[l];
							up_two_valid_wire_number = up_two_wire_cluster_validnumber[m];
							down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
							down_one_valid_wire_number=down_one_wire_cluster_validnumber[k];
						}
						counttrack++ ;
					}
				}
				if(counttrack<1) { EventQuality = -40 ; return; }
				else
				{
					Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
					Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					if(allClusterSizeLessThan4)
					{
						Calcu_TGC() ;
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{ 
								EventQuality = 22 ;
								return;
							}
							else 
							{ 
								EventQuality = -22 ;
								return;
							}
						}
						else { EventQuality = -22 ; return; }
					}
					else
					{
						Calcu_TGC() ;
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 23 ;
								return;
							}
							else
							{
								EventQuality = -23 ;
								return;
							}    
						}
						else { EventQuality = -23 ; return; }
					} 
				}
			}

			if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
			{
				for(int l=0 ; l<up_two_strip_vectorcluster_size ; l++)
				for(int m=0 ; m<up_two_wire_vectorcluster_size ; m++)
				for(int n=0 ; n<down_two_strip_vectorcluster_size ; n++)
				for(int k=0 ; k<down_two_wire_vectorcluster_size ; k++)
				{
					Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[l] ;
					Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[m] ;
					Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[n] ;
					Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[k] ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(up_two_strip_cluster_maxsignal[l]>max) 
						{  
							max=up_two_strip_cluster_maxsignal[l] ;
							up_two_valid_strip_number = up_two_strip_cluster_validnumber[l];
							up_two_valid_wire_number = up_two_wire_cluster_validnumber[m];
							down_two_valid_strip_number=down_two_strip_cluster_validnumber[n];
							down_two_valid_wire_number=down_two_wire_cluster_validnumber[k];
						}
						counttrack++ ;
					}
				}
				if(counttrack<1)
				{
					EventQuality = -40 ;
					return;
				}
				else
				{
					Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
					Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
					Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
					Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
					Calcu_TGC() ;
					if(allClusterSizeLessThan4)
					{
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 22 ;
								return;
							}
							else
							{
								EventQuality = -22 ;
								return;
							}
						}
						else { EventQuality = -22 ; return; }
					}
					else
					{
						if(Calcu_Scin())
						{
							if(counttrack==1)
							{
								EventQuality = 23 ;
								return;
							}
							else
							{
								EventQuality = -23 ;
								return;
							}
						}
						else { EventQuality = -23 ; return; }
					}
				}
			}

		}
	}

	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<up_two_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<up_two_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_one_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_one_wire_vectorcluster_size ; q++)
		for(int r=0 ; r<down_two_strip_vectorcluster_size ; r++)
		for(int s=0 ; s<down_two_wire_vectorcluster_size ; s++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[n] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[o] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[p] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[q] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[r] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[s] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_one_strip_cluster_maxsignal[l]>max)
				{ 
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[n];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[o];
				}
				if(down_one_strip_cluster_maxsignal[p]>max1)
				{
					max1=down_one_strip_cluster_maxsignal[p] ;
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[p];
					down_one_valid_wire_number=down_one_wire_cluster_validnumber[q];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[r];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[s]; 
				}
				counttrack++ ;
			}
		}
		if(counttrack<1) { EventQuality = -40 ;return; }
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			if(allClusterSizeLessThan4) 
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 14 ;
							return;
						}
						else
						{
							EventQuality =-14 ;
							return;
						}
					}
					else { EventQuality = -14 ;  return; }
				} 
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				    (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
				  ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
					   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ; }
						else
						{
							Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
							Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{  Up_TGC_Two.Valid_Strip_Number =-1; Up_TGC_Two.Valid_Wire_Number =-1;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				           (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
							Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Down_TGC_Two.Valid_Strip_Number =-1; Down_TGC_Two.Valid_Wire_Number =-1; }
						}
					}
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 15 ;
						    return;
						}
						else
						{ 
						    EventQuality =-15 ;
						    return;
						}
					}
					else { EventQuality = -15 ;  return; }
				}
			}
			else
			{
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)&&
				  ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3))
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 16 ;
						    return;
						}
						else
						{
						    EventQuality =-16 ;
						    return;
						}
					}
					else { EventQuality = -16 ;  return; }
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
				  ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
					   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ; }
						else
						{
							Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
							Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{  Up_TGC_Two.Valid_Strip_Number =-1; Up_TGC_Two.Valid_Wire_Number =-1;}
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
					   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{
							Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
							Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Down_TGC_Two.Valid_Strip_Number =-1; Down_TGC_Two.Valid_Wire_Number =-1; }
						}
					}
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						   EventQuality = 17 ;
						   return;
						}
						else
						{
						   EventQuality =-17 ;
						   return;
						}
					}
					else { EventQuality = -17 ;  return; }
				}
			}
		}
	}

		if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<up_two_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<up_two_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_one_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_one_wire_vectorcluster_size ; q++)
		for(int r=0 ; r<down_two_strip_vectorcluster_size ; r++)
		for(int s=0 ; s<down_two_wire_vectorcluster_size ; s++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[n] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[o] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[p] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[q] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[r] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[s] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_one_strip_cluster_maxsignal[l]>max)
				{ 
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[n];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[o];
				}
				if(down_one_strip_cluster_maxsignal[p]>max1)
				{
					max1=down_one_strip_cluster_maxsignal[p] ;
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[p];
					down_one_valid_wire_number=down_one_wire_cluster_validnumber[q];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[r];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[s];
				}
				counttrack++;
			}
		}
		if(counttrack<1) { EventQuality = -40 ; return; }
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				    (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)&&
				   ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				    (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3))
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 10 ;
						    return;
						}
						else
						{
						    EventQuality =-10 ;
						    return;
						}
					}
					else { EventQuality = -10 ;  return; }
				}
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				    (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
				   ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				    (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
					   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_Two.Valid_Strip_Number =-1; Up_TGC_Two.Valid_Wire_Number =-1;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
							Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Up_TGC_One.Valid_Strip_Number =-1; Up_TGC_One.Valid_Wire_Number =-1;  }
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
					   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{ 
							Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
							Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Down_TGC_Two.Valid_Strip_Number =-1; Down_TGC_Two.Valid_Wire_Number =-1; }
						}
					}
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 11 ;
						    return;
						} 
						else
						{
						    EventQuality =-11 ;
						    return;
						} 
					}
					else { EventQuality = -11 ;  return; }
				}
			}
			else
			{
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				    (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)&&
				   ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				    (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3))
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						   EventQuality = 12 ;
						   return;
						}
						else
						{
						    EventQuality =-12 ;
						    return;
						}
					}
					else { EventQuality = -12 ;  return; }
				}
				
				if(((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				    (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)||
				  ((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3))
				{
					if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
							(abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_Two.Valid_Strip_Number =-1; Up_TGC_Two.Valid_Wire_Number =-1;}
						else
						{
							Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
							Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Up_TGC_One.Valid_Strip_Number =-1; Up_TGC_One.Valid_Wire_Number =-1;  }
						}
					}
					if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
					   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
					{
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
						else
						{  
							Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
							Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
							Calcu_TGC() ;
							if(Calcu_Scin())
							{Down_TGC_Two.Valid_Strip_Number =-1; Down_TGC_Two.Valid_Wire_Number =-1; } 
						}
					}
					Calcu_TGC() ;   
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 13 ;
						    return;
						}
						else
						{
						    EventQuality =-13 ;
						    return;
						}
					}
					else { EventQuality = -13 ; return; }
				}
			}
		}
	}
	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size==0||down_two_wire_vectorcluster_size==0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<up_two_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<up_two_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_one_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_one_wire_vectorcluster_size ; q++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[n] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[o] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[p] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(up_one_strip_cluster_maxsignal[l]>max)
				{  
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[n];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[o];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[p];
					down_one_valid_wire_number=down_one_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{ 
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18 ;
							return;
						}
						else{ EventQuality = -18 ;  return; }
					}
					else { EventQuality = -18 ; return; }
				}  
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else 
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number =-1;  Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19 ;
						return;
					}
					else{ EventQuality = -19 ;  return; }
				}
				else { EventQuality = -19 ;  return; }
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 20 ;
							return;
						}
						else{ EventQuality = -20 ;  return; }
					}
					else { EventQuality = -20 ;  return; }
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Up_TGC_Two.Valid_Strip_Number = -1 ; Up_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
						Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number =-1;  Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 21 ;
						return;
					}
					else { EventQuality = -21 ; return; }   
				}
				else { EventQuality = -21 ; return; }
			}  
		}
	}


	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size==0||down_one_wire_vectorcluster_size==0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<up_two_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<up_two_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_two_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_two_wire_vectorcluster_size ; q++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[n] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[o] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[p] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{ 
				if(up_one_strip_cluster_maxsignal[l]>max)
				{  
					max=up_one_strip_cluster_maxsignal[l] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[n];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[o];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[p];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
			EventQuality = -40 ;
			return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{ 
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18 ;
							return;
						}
						else { EventQuality = -18 ;  return; }
					} 
					else { EventQuality = -18 ; return; }
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
					Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number =-1; Up_TGC_Two.Valid_Wire_Number =-1; }
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1; Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19 ;
						return;
					}
					else
					{
						EventQuality = -19 ;
						return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 20 ;
							return;
						}
						else
						{
							EventQuality = -20 ;
							return;
						}
					}
					else { EventQuality = -20 ; return; } 
				}
				if((abs(Up_TGC_One.Valid_Strip_Number-Up_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Up_TGC_One.Valid_Wire_Number-Up_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
					Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{Up_TGC_Two.Valid_Strip_Number =-1; Up_TGC_Two.Valid_Wire_Number =-1; }
					else
					{
						Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
						Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Up_TGC_One.Valid_Strip_Number = -1; Up_TGC_One.Valid_Wire_Number =-1; }
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
					    EventQuality = 21 ;
					    return;
					}
					else { EventQuality = -21 ; return; }
				}
				else { EventQuality = -21 ; return; }
			}

		}
	}


	if((up_one_strip_vectorcluster_size>0&&up_one_wire_vectorcluster_size>0)&&(up_two_strip_vectorcluster_size==0||up_two_wire_vectorcluster_size==0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_one_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_one_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<down_one_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_two_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_two_wire_vectorcluster_size ; q++)
		{
			Up_TGC_One.Valid_Strip_Number = up_one_strip_cluster_validnumber[l] ;
			Up_TGC_One.Valid_Wire_Number = up_one_wire_cluster_validnumber[m] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[o] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[p] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(down_one_strip_cluster_maxsignal[n]>max1)
				{
					max1=down_one_strip_cluster_maxsignal[n] ;
					up_one_valid_strip_number=up_one_strip_cluster_validnumber[l];
					up_one_valid_wire_number = up_one_wire_cluster_validnumber[m];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
					down_one_valid_wire_number = down_one_wire_cluster_validnumber[o];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[p];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
		    EventQuality = -40 ;
		    return;
		}
		else
		{
			Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
			Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{ 
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
							EventQuality = 18 ;
							return;
						}
						else
						{
							EventQuality = -18;
							return;
						}
					}
					else { EventQuality = -18 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1; Down_TGC_One.Valid_Wire_Number =-1; }                
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
						EventQuality = 19 ;
						return;
					}
					else
					{
						EventQuality = -19;
						return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{ 
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 20 ;
						    return;
						}
						else
						{
						    EventQuality = -20;
						    return;
						}
					}
					else { EventQuality = -20 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
					Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{ Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Up_TGC_One.Valid_Strip_Number = up_one_valid_strip_number ;
						Up_TGC_One.Valid_Wire_Number = up_one_valid_wire_number ;
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1; Down_TGC_One.Valid_Wire_Number =-1; }
					}   
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
					    EventQuality = 21 ;
					    return;
					}
					else
					{
					    EventQuality = -21;
					    return;
					} 
				}
				else { EventQuality = -21 ; return; }
			}
		}
	}

	if((up_one_strip_vectorcluster_size==0||up_one_wire_vectorcluster_size==0)&&(up_two_strip_vectorcluster_size>0&&up_two_wire_vectorcluster_size>0)&&(down_one_strip_vectorcluster_size>0&&down_one_wire_vectorcluster_size>0)&&(down_two_strip_vectorcluster_size>0&&down_two_wire_vectorcluster_size>0))
	{
		for(int l=0 ; l<up_two_strip_vectorcluster_size ; l++)
		for(int m=0 ; m<up_two_wire_vectorcluster_size ; m++)
		for(int n=0 ; n<down_one_strip_vectorcluster_size ; n++)
		for(int o=0 ; o<down_one_wire_vectorcluster_size ; o++)
		for(int p=0 ; p<down_two_strip_vectorcluster_size ; p++)
		for(int q=0 ; q<down_two_wire_vectorcluster_size ; q++)
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_strip_cluster_validnumber[l] ;
			Up_TGC_Two.Valid_Wire_Number = up_two_wire_cluster_validnumber[m] ;
			Down_TGC_One.Valid_Strip_Number = down_one_strip_cluster_validnumber[n] ;
			Down_TGC_One.Valid_Wire_Number = down_one_wire_cluster_validnumber[o] ;
			Down_TGC_Two.Valid_Strip_Number = down_two_strip_cluster_validnumber[p] ;
			Down_TGC_Two.Valid_Wire_Number = down_two_wire_cluster_validnumber[q] ;
			Calcu_TGC() ;
			if(Calcu_Scin())
			{
				if(down_one_strip_cluster_maxsignal[n]>max1)
				{
					max1=down_one_strip_cluster_maxsignal[n] ;
					up_two_valid_strip_number=up_two_strip_cluster_validnumber[l];
					up_two_valid_wire_number = up_two_wire_cluster_validnumber[m];
					down_one_valid_strip_number=down_one_strip_cluster_validnumber[n];
					down_one_valid_wire_number = down_one_wire_cluster_validnumber[o];
					down_two_valid_strip_number=down_two_strip_cluster_validnumber[p];
					down_two_valid_wire_number=down_two_wire_cluster_validnumber[q];
				}
				counttrack++ ;
			}
		}
		if(counttrack<1)
		{
		    EventQuality = -40 ;
		    return;
		}
		else
		{
			Up_TGC_Two.Valid_Strip_Number = up_two_valid_strip_number ;
			Up_TGC_Two.Valid_Wire_Number = up_two_valid_wire_number ;
			Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
			Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
			Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
			Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
			if(allClusterSizeLessThan4)
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 18 ;
						    return;
						}
						else
						{
						   EventQuality = -18 ;
						   return;
						}
					}
					else { EventQuality = -18 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{  Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1;Down_TGC_One.Valid_Wire_Number =-1;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
					    EventQuality = 19 ;
					    return;
					}
					else
					{
					    EventQuality = -19 ;
					    return;
					}
				}
				else { EventQuality = -19 ; return; }
			}
			else
			{
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)<=3&&
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)<=3)
				{
					Calcu_TGC() ;
					if(Calcu_Scin())
					{
						if(counttrack==1)
						{
						    EventQuality = 20 ;
						    return;
						}
						else
						{
						    EventQuality = -20 ;
						    return;
						}
					}
					else { EventQuality = -20 ; return; }
				}
				if((abs(Down_TGC_One.Valid_Strip_Number-Down_TGC_Two.Valid_Strip_Number)-1)>3||
				   (abs(Down_TGC_One.Valid_Wire_Number-Down_TGC_Two.Valid_Wire_Number)-1)>3)
				{
					Down_TGC_One.Valid_Strip_Number = down_one_valid_strip_number ;
					Down_TGC_One.Valid_Wire_Number = down_one_valid_wire_number ;
					Calcu_TGC() ;
					if(Calcu_Scin())
					{  Down_TGC_Two.Valid_Strip_Number = -1 ; Down_TGC_Two.Valid_Wire_Number = -1 ;}
					else
					{
						Down_TGC_Two.Valid_Strip_Number = down_two_valid_strip_number ;
						Down_TGC_Two.Valid_Wire_Number = down_two_valid_wire_number ;
						Calcu_TGC() ;
						if(Calcu_Scin())
						{Down_TGC_One.Valid_Strip_Number =-1;Down_TGC_One.Valid_Wire_Number =-1;}
					}
				}
				Calcu_TGC() ;
				if(Calcu_Scin())
				{
					if(counttrack==1)
					{
					    EventQuality = 21 ;
					    return;
					}
					else
					{
					    EventQuality = -21 ;
					    return;
					}
				}
				else { EventQuality = -21 ; return; }
			}
		}  
	} 


	if(((up_one_strip_vectorcluster_size>0||up_one_wire_vectorcluster_size>0)||(up_two_strip_vectorcluster_size>0||up_two_wire_vectorcluster_size>0)||(down_one_strip_vectorcluster_size>0||down_one_wire_vectorcluster_size>0)||(down_two_strip_vectorcluster_size>0||down_two_wire_vectorcluster_size>0)))
	{
           EventQuality = -41 ;
           return;
	}

}

void CalcuTGCScin::Clear()
{
	Up_TGC_Strip_One_Cluster.clear() ;
	Up_TGC_Wire_One_Cluster.clear() ;
	Down_TGC_Strip_One_Cluster.clear() ;
	Down_TGC_Wire_One_Cluster.clear() ;
	Up_TGC_Strip_Two_Cluster.clear() ;
	Up_TGC_Wire_Two_Cluster.clear() ;
	Down_TGC_Strip_Two_Cluster.clear() ;
	Down_TGC_Wire_Two_Cluster.clear() ;
	up_one_strip_cluster_size.clear() ;
	up_one_wire_cluster_size.clear() ;
	up_two_strip_cluster_size.clear() ;
	up_two_wire_cluster_size.clear() ;
	down_one_strip_cluster_size.clear() ;
	down_one_wire_cluster_size.clear() ;
	down_two_strip_cluster_size.clear() ;
	down_two_wire_cluster_size.clear() ;
	up_one_strip_cluster_validnumber.clear() ;
	up_one_wire_cluster_validnumber.clear() ;
	up_two_strip_cluster_validnumber.clear() ;
	up_two_wire_cluster_validnumber.clear() ;
	down_one_strip_cluster_validnumber.clear() ;
	down_one_wire_cluster_validnumber.clear() ;
	down_two_strip_cluster_validnumber.clear() ;
	down_two_wire_cluster_validnumber.clear() ;
	up_one_strip_cluster_maxsignal.clear() ;
	up_one_wire_cluster_maxsignal.clear() ;
	up_two_strip_cluster_maxsignal.clear() ;
	up_two_wire_cluster_maxsignal.clear() ;
	down_one_strip_cluster_maxsignal.clear() ;
	down_one_wire_cluster_maxsignal.clear() ;
	down_two_strip_cluster_maxsignal.clear() ;
	down_two_wire_cluster_maxsignal.clear() ;
}


int CalcuTGCScin::getValidLineNumber( vector<int> vec1, vector<int> vec2 )
{

	int validN=-1;
	if( vec1.size()==1) validN=vec1[0];
	if((vec1.size()==2)&&(vec2.size()==1))
	{
		if((abs(vec1[0]-vec2[0])-1)<3)
		{ 
			validN= vec1[0]; 
		}
		else
		{  
			if((abs(vec1[1]-vec2[0])-1)<3)
			validN= vec1[1]; 
		}
	}
	return validN;
}
#endif
