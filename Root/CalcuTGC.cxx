#include "ProcessCORARS/CalcuTGC.h"

CalcuTGC::CalcuTGC()
{
  ifstream config ;
  config.open("./Config/rawdata") ;
  if(!config)
  {
    cout<<"Cannot open input file.\n";
  }
  TString time ;
  config>>time ;
  config.close() ;
  InPut_File_Name = "Stage_One_" + time + ".root" ;
  cout<<InPut_File_Name<<endl ;
  InPut_File = new TFile("./OutPut_Data/"+time+"/"+InPut_File_Name,"READ") ;
  InPut_Tree = (TTree*) InPut_File->Get("UsedVariables") ;
  OutPut_File_Name = "./OutPut_Data/"+time+"/TGC.root" ;
  cout<<OutPut_File_Name<<endl ;
  OutPut_File = new TFile(OutPut_File_Name,"RECREATE") ;
  OutPut_Tree = new TTree("TGC","Thin Gap Chamber") ;

  ifstream Input_params ;
  Input_params.open("./Config/params") ;
  if(!Input_params)
  {
    cout<<"open file failed!"<<endl ;
  }
  else
  {
  for(int i = 0 ; i<189 ; i++)
    {Input_params>>params[i] ; }
  }
  Input_params.close();
}

CalcuTGC::~CalcuTGC(){}

bool CalcuTGC::Remove_Null_Signal_Two()
{
  int Count_Up_Strip = 0 , Count_Up_Wire = 0 ,
      Count_Dw_Strip = 0 , Count_Dw_Wire = 0 ;

  for(int i=0 ; i<32 ; i++)
    {
      if(Up_TGC_Strip_Leading_Two[i]!=0&&Up_TGC_Strip_Trailing_Two[i]!=0)
        Count_Up_Strip++ ;
      if(Up_TGC_Wire_Leading_Two[i]!=0&&Up_TGC_Wire_Trailing_Two[i]!=0)
        Count_Up_Wire++ ;
      if(Down_TGC_Strip_Leading_Two[i]!=0&&Down_TGC_Strip_Trailing_Two[i]!=0)
        Count_Dw_Strip++ ;
      if(Down_TGC_Wire_Leading_Two[i]!=0&&Down_TGC_Wire_Trailing_Two[i]!=0)
        Count_Dw_Wire++ ;
    }
  if(Count_Up_Strip>0&&Count_Up_Strip<4&&
     Count_Up_Wire>0&&Count_Up_Strip<4&&
     Count_Dw_Wire>0&&Count_Dw_Wire<4&&
     Count_Dw_Strip>0&&Count_Dw_Strip<4)
    return true ;
  else
    return false ;
}

int CalcuTGC::Get_Number(UInt_t *Signal)
{
  int count = 0 ;
  int ContiNu[3] = {0,0,0} ;
  for(int i=0 ; i<32 ; i++)
    {
      if(Signal[i]>0)
        { count++ ;
          if(count==1) ContiNu[0] = i ;
          if(count==2) ContiNu[1] = i ;
          if(count==3) ContiNu[2] = i ;
        }
    }
  switch(count)
  {
    case 1: {return 2*ContiNu[0] ;}
/*    case 2: {
              if(ContiNu[1]-ContiNu[0]==1)  {return 2*ContiNu[0]+1 ; }
              else { return -1 ; }
            }
    case 3: {
              if((ContiNu[1]-ContiNu[0]==1)&&(ContiNu[2]-ContiNu[1]==1))
                { return 2*ContiNu[1] ;  }
              else
                { return -1 ; }
            }*/
   default: return -1 ;
  }
}

bool CalcuTGC::Get_Valid_Strip_Wire_Number_One()
{
  if(Get_Number(Up_TGC_Strip_One)==-1||
     Get_Number(Up_TGC_Wire_One)==-1||
     Get_Number(Down_TGC_Strip_One)==-1||
     Get_Number(Down_TGC_Wire_One)==-1)
     return false ;
  else
  {
    Up_TGC_Strip_Number_One = Get_Number(Up_TGC_Strip_One) ;
    Up_TGC_Wire_Number_One = Get_Number(Up_TGC_Wire_One) ;
    Down_TGC_Strip_Number_One = Get_Number(Down_TGC_Strip_One) ;
    Down_TGC_Wire_Number_One = Get_Number(Down_TGC_Wire_One) ;
    if(Up_TGC_Strip_Number_One%2==1||Up_TGC_Wire_Number_One%2==1||Down_TGC_Strip_Number_One%2==1||Down_TGC_Wire_Number_One%2==1)
      return false ;
    else
      return true ;
  }

}
bool CalcuTGC::Get_Valid_Strip_Wire_Number_Two()
{
  if(Get_Number(Up_TGC_Strip_Leading_Two)==-1||Get_Number(Up_TGC_Strip_Leading_Two)%2==1||
     Get_Number(Up_TGC_Wire_Leading_Two)==-1||Get_Number(Up_TGC_Wire_Leading_Two)%2==1||
     Get_Number(Down_TGC_Strip_Leading_Two)==-1||Get_Number(Down_TGC_Strip_Leading_Two)%2==1||
     Get_Number(Down_TGC_Wire_Leading_Two)==-1||Get_Number(Down_TGC_Wire_Leading_Two)%2==1)
     return false ;
  else
  {
    Up_TGC_Strip_Number_Two = Get_Number(Up_TGC_Strip_Leading_Two) ;
    Up_TGC_Wire_Number_Two = Get_Number(Up_TGC_Wire_Leading_Two) ;
    Down_TGC_Strip_Number_Two = Get_Number(Down_TGC_Strip_Leading_Two) ;
    Down_TGC_Wire_Number_Two = Get_Number(Down_TGC_Wire_Leading_Two) ;
    if(Up_TGC_Strip_Number_Two%2==1||Up_TGC_Wire_Number_Two%2==1||Down_TGC_Strip_Number_Two%2==1||Down_TGC_Wire_Number_Two%2==1)
      return false ;
    else
      return true ; 
  }

}

void CalcuTGC::Calcu_TGC_Coor_Two(){
 float a1 = 12.16,
        b1 = *(params+Up_TGC_Strip_Number_One*3),
        c1 = *(params+Up_TGC_Strip_Number_One*3+1),
        c2 = *(params+Up_TGC_Wire_Number_One*3+2);
 Up_TGC_One.x = (c1 - b1*c2)/a1 +85. ;
 Up_TGC_One.y = c2 ;
 Up_TGC_One.z = 326. ;

 b1 = params[Down_TGC_Strip_Number_One*3];
 c1 = params[Down_TGC_Strip_Number_One*3 + 1];
 c2 = params[Down_TGC_Wire_Number_One*3 + 2];
 Down_TGC_One.x = (c1 - b1*c2)/a1 +85. ;
 Down_TGC_One.y = c2 ;
 Down_TGC_One.z = 46. ;

 Up_TGC_Two.z = 323.4 ;
 Down_TGC_Two.z = 43.4 ;
/*
  b1 = *(params+Up_TGC_Strip_Number_Two*3),
  c1 = *(params+Up_TGC_Strip_Number_Two*3+1),
  c2 = *(params+Up_TGC_Wire_Number_Two*3+2);
  Up_TGC_Two.x = (c1 - b1*c2)/a1 +85. ;
  Up_TGC_Two.y = c2 ;
  b1 = *(params+Down_TGC_Strip_Number_Two*3),
  c1 = *(params+Down_TGC_Strip_Number_Two*3+1),
  c2 = *(params+Down_TGC_Wire_Number_Two*3+2);
  Down_TGC_Two.x = (c1 - b1*c2)/a1 +85. ;
  Down_TGC_Two.y = c2 ;
*/
 float a = (Up_TGC_One.x-Down_TGC_One.x)/(Up_TGC_One.z-Down_TGC_One.z),
       b = (Up_TGC_One.z*Down_TGC_One.x-Up_TGC_One.x*Down_TGC_One.z)/(Up_TGC_One.z-Down_TGC_One.z),
       c = (Up_TGC_One.y-Down_TGC_One.y)/(Up_TGC_One.z-Down_TGC_One.z),
       d = (Up_TGC_One.z*Down_TGC_One.y-Up_TGC_One.y*Down_TGC_One.z)/(Up_TGC_One.z-Down_TGC_One.z);
  Up_TGC_Two.x = a*Up_TGC_Two.z+b ;
  Up_TGC_Two.y = c*Up_TGC_Two.z+d ;
  Down_TGC_Two.x = a*Down_TGC_Two.z+b ;
  Down_TGC_Two.y = c*Down_TGC_Two.z+d ;
}

void CalcuTGC::Calcu_Two()
{
  InPut_Tree->SetBranchAddress("Up_TGC_Strip_Leading_One",Up_TGC_Strip_One) ;
  InPut_Tree->SetBranchAddress("Up_TGC_Wire_Leading_One",Up_TGC_Wire_One) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Strip_Leading_One",Down_TGC_Strip_One) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Wire_Leading_One",Down_TGC_Wire_One) ;

  InPut_Tree->SetBranchAddress("Up_TGC_Strip_Leading_Two",Up_TGC_Strip_Leading_Two) ;
  InPut_Tree->SetBranchAddress("Up_TGC_Strip_Trailing_Two",Up_TGC_Strip_Trailing_Two) ;
  InPut_Tree->SetBranchAddress("Up_TGC_Wire_Leading_Two",Up_TGC_Wire_Leading_Two) ;
  InPut_Tree->SetBranchAddress("Up_TGC_Wire_Trailing_Two",Up_TGC_Wire_Trailing_Two) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Strip_Leading_Two",Down_TGC_Strip_Leading_Two) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Strip_Trailing_Two",Down_TGC_Strip_Trailing_Two) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Wire_Leading_Two",Down_TGC_Wire_Leading_Two) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Wire_Trailing_Two",Down_TGC_Wire_Trailing_Two) ;

  OutPut_Tree->Branch("Up_TGC_One",&Up_TGC_One,"x/F:y/F:z/F");
  OutPut_Tree->Branch("Down_TGC_One",&Down_TGC_One,"x/F:y/F:z/F");
  OutPut_Tree->Branch("Up_TGC_Strip_Number_One",&Up_TGC_Strip_Number_One,"Up_TGC_Strip_Number_One/I");
  OutPut_Tree->Branch("Up_TGC_Wire_Number_One",&Up_TGC_Wire_Number_One,"Up_TGC_Wire_Number_One/I");
  OutPut_Tree->Branch("Down_TGC_Strip_Number_One",&Down_TGC_Strip_Number_One,"Down_TGC_Strip_Number_One/I");
  OutPut_Tree->Branch("Down_TGC_Wire_Number_One",&Down_TGC_Wire_Number_One,"Down_TGC_Wire_Number_One/I");
  OutPut_Tree->Branch("Up_TGC_Two",&Up_TGC_Two,"x/F:y/F:z/F");
  OutPut_Tree->Branch("Down_TGC_Two",&Down_TGC_Two,"x/F:y/F:z/F");
  OutPut_Tree->Branch("Up_TGC_Strip_Number_Two",&Up_TGC_Strip_Number_Two,"Up_TGC_Strip_Number_Two/I");
  OutPut_Tree->Branch("Up_TGC_Wire_Number_Two",&Up_TGC_Wire_Number_Two,"Up_TGC_Wire_Number_Two/I");
  OutPut_Tree->Branch("Down_TGC_Strip_Number_Two",&Down_TGC_Strip_Number_Two,"Down_TGC_Strip_Number_Two/I");
  OutPut_Tree->Branch("Down_TGC_Wire_Number_Two",&Down_TGC_Wire_Number_Two,"Down_TGC_Wire_Number_Two/I");

  Long64_t nEntries = InPut_Tree->GetEntries() ;
  cout<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
  {
    InPut_Tree->GetEntry(i) ;
    if(!Remove_Null_Signal_Two()) continue ;
    if(!Get_Valid_Strip_Wire_Number_One()) continue ;
    if(!Get_Valid_Strip_Wire_Number_Two()) continue ;
    Calcu_TGC_Coor_Two() ;
    OutPut_Tree->Fill() ;
  }
  OutPut_File->Write() ;
  OutPut_File->Close() ;
}
