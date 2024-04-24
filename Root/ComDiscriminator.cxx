#include "ProcessCORARS/ComDiscriminator.h"

ComDiscriminator::ComDiscriminator() {}

ComDiscriminator::~ComDiscriminator(){}

void ComDiscriminator::Initialize()
{
  ifstream config ;
  config.open("./Config/timeconfig") ;
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
  InPut_Tree = (TTree*)(InPut_File->Get("UsedVariables")) ;
  OutPut_File_Name = "./OutPut_Data/"+time+"/Stage_Two_" + time + ".root" ;
  cout<<OutPut_File_Name<<endl ;
  OutPut_File = new TFile(OutPut_File_Name,"RECREATE") ;
  OutPut_Tree = new TTree("Calcued_Variables","Calculated Variables") ;

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
  //input the calib of time
  ifstream Input_calib ;
  Input_calib.open("./Config/timecalib") ;
  if(!Input_calib)
    cout<<"open file failed!"<<endl ;
  else
  {
    for(int i = 0 ; i<8 ; i++)
      for(int j=0 ; j<10 ; j++)
        for(int k=0 ; k<48 ; k++)
             Input_calib>>calib_mean[i][j][k] ;
  }
  Input_calib.close();
  ifstream Input_calib_charge ;
  Input_calib_charge.open("./Config/calib_charge") ;
  if(!Input_calib_charge)
    cout<<"open file failed!"<<endl ;
  else
  {
    for(int i = 0 ; i<8 ; i++)
      for(int j=0 ; j<55 ; j++)
             Input_calib_charge>>calib_charge[i][j] ;
  }
  Input_calib_charge.close();
}
int ComDiscriminator::Get_Number(UInt_t *Signal)
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
    case 2: {
              if(ContiNu[1]-ContiNu[0]==1)  {return 2*ContiNu[0]+1 ; }
              else { return -1 ; }
            }
    case 3: {
              if((ContiNu[1]-ContiNu[0]==1)&&(ContiNu[2]-ContiNu[1]==1))
                { return 2*ContiNu[1] ;  }
              else 
                { return -1 ; }
            }
   default: return -1 ;
  }
}

bool ComDiscriminator::Get_Valid_Strip_Wire_Number()
{
  if(Get_Number(Up_TGC_Strip_One)==-1||
     Get_Number(Up_TGC_Wire_One)==-1||
     Get_Number(Down_TGC_Strip_One)==-1||
     Get_Number(Down_TGC_Wire_One)==-1)
     return false ;
  else 
  {
    Up_TGC_Strip_Number = Get_Number(Up_TGC_Strip_One) ;
    Up_TGC_Wire_Number = Get_Number(Up_TGC_Wire_One) ;
    Down_TGC_Strip_Number = Get_Number(Down_TGC_Strip_One) ;
    Down_TGC_Wire_Number = Get_Number(Down_TGC_Wire_One) ;
    return true ;
  }

}
void ComDiscriminator::Calcu_TGC_Coor(){
 float a1 = 12.16, 
        b1 = *(params+Up_TGC_Strip_Number*3), 
        c1 = *(params+Up_TGC_Strip_Number*3+1), 
        c2 = *(params+Up_TGC_Wire_Number*3+2); 
 Up_TGC.x = (c1 - b1*c2)/a1 +85. ;
 Up_TGC.y = c2 ;
 Up_TGC.z = 326. ;
 
 b1 = params[Down_TGC_Strip_Number*3];
 c1 = params[Down_TGC_Strip_Number*3 + 1];
 c2 = params[Down_TGC_Wire_Number*3 + 2];
 Down_TGC.x = (c1 - b1*c2)/a1 +85. ;
 Down_TGC.y = c2 ;
 Down_TGC.z = 46. ;
} 
bool ComDiscriminator::Calcu_Scin(){
  Up_Scin.Scin_Coor.z = 346.5 ;
  Down_Scin.Scin_Coor.z = 26. ;
 
  float a =  (Up_TGC.x-Down_TGC.x)/(Up_TGC.z-Down_TGC.z),
           b = (Up_TGC.z*Down_TGC.x-Up_TGC.x*Down_TGC.z)/(Up_TGC.z-Down_TGC.z),
           c =  (Up_TGC.y-Down_TGC.y)/(Up_TGC.z-Down_TGC.z),
           d = (Up_TGC.z*Down_TGC.y-Up_TGC.y*Down_TGC.z)/(Up_TGC.z-Down_TGC.z);
  Up_Scin.Scin_Coor.x = a*Up_Scin.Scin_Coor.z+b ;
  Up_Scin.Scin_Coor.y = c*Up_Scin.Scin_Coor.z+d ;
  Down_Scin.Scin_Coor.x = a*Down_Scin.Scin_Coor.z+b ;
  Down_Scin.Scin_Coor.y = c*Down_Scin.Scin_Coor.z+d ;

  if(Up_Scin.Scin_Coor.y<0||Up_Scin.Scin_Coor.y>120||
     Down_Scin.Scin_Coor.y<0||Down_Scin.Scin_Coor.y>120)
    return false ;

  Up_Scin.index = -1 ; Down_Scin.index = -1 ;
  if(Up_Scin.Scin_Coor.x>25 && Up_Scin.Scin_Coor.x<50)   Up_Scin.index = 0; 
  if(Up_Scin.Scin_Coor.x>57 && Up_Scin.Scin_Coor.x<82)   Up_Scin.index = 1; 
  if(Up_Scin.Scin_Coor.x>88 && Up_Scin.Scin_Coor.x<113)   Up_Scin.index = 2; 
  if(Up_Scin.Scin_Coor.x>120 && Up_Scin.Scin_Coor.x<145)   Up_Scin.index = 3;
  if(Up_Scin.index==-1) return false ; 
  if(Down_Scin.Scin_Coor.x>25 && Down_Scin.Scin_Coor.x<50)   Down_Scin.index = 4; 
  if(Down_Scin.Scin_Coor.x>57 && Down_Scin.Scin_Coor.x<82)   Down_Scin.index = 5; 
  if(Down_Scin.Scin_Coor.x>88 && Down_Scin.Scin_Coor.x<113)   Down_Scin.index = 6; 
  if(Down_Scin.Scin_Coor.x>120 && Down_Scin.Scin_Coor.x<145)   Down_Scin.index = 7;
  if(Down_Scin.index==-1) return false ;
  //the unit of time is ps 
  Up_Scin.Measure_Time = (int)ScinED_Time_Signal[Up_Scin.index]*25 ;
  Down_Scin.Measure_Time =(int)ScinED_Time_Signal[Down_Scin.index]*25 ;
  if(Up_Scin.Measure_Time<=0||Down_Scin.Measure_Time<=0)
    return false ;
  //cut the points near the PMT
  //dis_up the distance between point and PMT
  float dis_up = 0 , dis_down = 0 ;
  switch(Up_Scin.index)
  {
    case 0:
         dis_up = sqrt(pow(Up_Scin.Scin_Coor.x-37.5, 2)+pow(Up_Scin.Scin_Coor.y,2));
         break ;
    case 1:
         dis_up = sqrt(pow(Up_Scin.Scin_Coor.x-69.5, 2)+pow(Up_Scin.Scin_Coor.y,2));
         break ;
    case 2:
         dis_up = sqrt(pow(Up_Scin.Scin_Coor.x-100.5, 2)+pow(Up_Scin.Scin_Coor.y,2));
         break ;
    case 3:
         dis_up = sqrt(pow(Up_Scin.Scin_Coor.x-132.5, 2)+pow(Up_Scin.Scin_Coor.y,2));
         break ;
  }
  switch(Down_Scin.index)
  {
    case 4:
         dis_down = sqrt(pow(Down_Scin.Scin_Coor.x-37.5, 2)+pow(Down_Scin.Scin_Coor.y,2));
         break ;
    case 5:
         dis_down = sqrt(pow(Down_Scin.Scin_Coor.x-69.5, 2)+pow(Down_Scin.Scin_Coor.y,2));
         break ;
    case 6:
         dis_down = sqrt(pow(Down_Scin.Scin_Coor.x-100.5, 2)+pow(Down_Scin.Scin_Coor.y,2));
         break ;
    case 7:
         dis_down = sqrt(pow(Down_Scin.Scin_Coor.x-132.5, 2)+pow(Down_Scin.Scin_Coor.y,2));
         break ;
  }
  if(acos(Up_Scin.Scin_Coor.y/dis_up)>=0.3||acos(Down_Scin.Scin_Coor.y/dis_down)>=0.3) 
    return false;
  //Calibed_Time is the time after calibration
  switch(Up_Scin.index)
  {
    case 0 :
    {
      int m , n ;
      m = 10*(Up_Scin.Scin_Coor.x-25)/25 ;
      n = 10*Up_Scin.Scin_Coor.y/25 ;
      Up_Scin.Calibed_Time = Up_Scin.Measure_Time - calib_mean[0][m][n] ;
      break ;
    }
    case 1 :
    {
      int m , n ;
      m = 10*(Up_Scin.Scin_Coor.x-57)/25 ;
      n = 10*Up_Scin.Scin_Coor.y/25 ;
      Up_Scin.Calibed_Time = Up_Scin.Measure_Time - calib_mean[1][m][n] ;
      break ;
    }
    case 2 :
    {
      int m , n ;
      m = 10*(Up_Scin.Scin_Coor.x-88)/25 ;
      n = 10*Up_Scin.Scin_Coor.y/25 ;
      Up_Scin.Calibed_Time = Up_Scin.Measure_Time - calib_mean[2][m][n] ;
      break ;
    }
    case 3 :
    {
      int m , n ;
      m = 10*(Up_Scin.Scin_Coor.x-120)/25 ;
      n = 10*Up_Scin.Scin_Coor.y/25 ;
      Up_Scin.Calibed_Time = Up_Scin.Measure_Time - calib_mean[3][m][n] ;
      break ;
    }
  }
  switch(Down_Scin.index)
  {
    case 4 :
    {
      int m , n ;
      m = 10*(Down_Scin.Scin_Coor.x-25)/25 ;
      n = 10*Down_Scin.Scin_Coor.y/25 ;
      Down_Scin.Calibed_Time = Down_Scin.Measure_Time - calib_mean[4][m][n] ;
      break ;
    }
    case 5 :
    {
      int m , n ;
      m = 10*(Down_Scin.Scin_Coor.x-57)/25 ;
      n = 10*Down_Scin.Scin_Coor.y/25 ;
      Down_Scin.Calibed_Time = Down_Scin.Measure_Time - calib_mean[5][m][n] ;
      break ;
    }
    case 6 :
    {
      int m , n ;
      m = 10*(Down_Scin.Scin_Coor.x-88)/25 ;
      n = 10*Down_Scin.Scin_Coor.y/25 ;
      Down_Scin.Calibed_Time = Down_Scin.Measure_Time - calib_mean[6][m][n] ;
      break ;
    }
    case 7 :
    {
      int m , n ;
      m = 10*(Down_Scin.Scin_Coor.x-120)/25 ;
      n = 10*Down_Scin.Scin_Coor.y/25 ;
      Down_Scin.Calibed_Time = Down_Scin.Measure_Time - calib_mean[7][m][n] ;
      break ;
    }
  }
  // Calculate flying of time 
  Muon_TOF.Real_TOF = (int)(1000*sqrt(pow(Up_Scin.Scin_Coor.x-Down_Scin.Scin_Coor.x,2)
                                     +pow(Up_Scin.Scin_Coor.y-Down_Scin.Scin_Coor.y,2)
                                     +pow(Up_Scin.Scin_Coor.z-Down_Scin.Scin_Coor.z,2))/29.8) ;
  Muon_TOF.Measure_TOF = Down_Scin.Calibed_Time - Up_Scin.Calibed_Time ;
  Muon_TOF.Delta_TOF = Muon_TOF.Measure_TOF - Muon_TOF.Real_TOF ;
  
  return true ;
}
void ComDiscriminator::Calcu_ED5(){
  float a =  (Up_TGC.x-Down_TGC.x)/(Up_TGC.z-Down_TGC.z),
        b = (Up_TGC.z*Down_TGC.x-Up_TGC.x*Down_TGC.z)/(Up_TGC.z-Down_TGC.z),
        c =  (Up_TGC.y-Down_TGC.y)/(Up_TGC.z-Down_TGC.z),
        d = (Up_TGC.z*Down_TGC.y-Up_TGC.y*Down_TGC.z)/(Up_TGC.z-Down_TGC.z);
  ElecDet5.ED_Coor.z = 304.5-26*5 ;
  ElecDet5.ED_Coor.x = a*ElecDet5.ED_Coor.z+b ;
  ElecDet5.ED_Coor.y = c*ElecDet5.ED_Coor.z+d ;
  ElecDet5.N841_Measure_Time = (int)ScinED_Time_Signal[8]*25 ;
  ElecDet5.ReCon_Time = Up_Scin.Calibed_Time + ((Down_Scin.Calibed_Time-Up_Scin.Calibed_Time)*(346.5-ElecDet5.ED_Coor.z)/320.5);
  ElecDet5.N841_Delta_Time = ElecDet5.N841_Measure_Time - ElecDet5.ReCon_Time ;
  ElecDet5.N844_Measure_Time = (int)ScinED_Time_Signal[9]*25 ;
  ElecDet5.N844_Delta_Time = ElecDet5.N844_Measure_Time - ElecDet5.ReCon_Time ;
  ElecDet5.Charge = ED_Charge_Signal[5] ;
  ElecDet5.Calibed_Charge = ED_Charge_Signal[5]-calib_charge[5][event_id/10000] ;
}
void ComDiscriminator::Calcu()
{
  Initialize() ;
  InPut_Tree->SetBranchAddress("event_id",&event_id) ;
  InPut_Tree->SetBranchAddress("Up_TGC_Strip_Leading_One",Up_TGC_Strip_One) ;
  InPut_Tree->SetBranchAddress("Up_TGC_Wire_Leading_One",Up_TGC_Wire_One) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Strip_Leading_One",Down_TGC_Strip_One) ;
  InPut_Tree->SetBranchAddress("Down_TGC_Wire_Leading_One",Down_TGC_Wire_One) ;
  InPut_Tree->SetBranchAddress("Scin_ED_Time_Signal",ScinED_Time_Signal) ;
  InPut_Tree->SetBranchAddress("ED_Charge_Signal",ED_Charge_Signal) ;

  OutPut_Tree->Branch("event_id",&event_id,"event_id/i"); 
  OutPut_Tree->Branch("Up_TGC",&Up_TGC,"x/F:y/F:z/F"); 
  OutPut_Tree->Branch("Down_TGC",&Down_TGC,"x/F:y/F:z/F"); 
   
  OutPut_Tree->Branch("Up_Scin",&Up_Scin,"x/F:y/F:z/F:index/I:Measure_Time/I:Calibed_Time/I"); 
  OutPut_Tree->Branch("Down_Scin",&Down_Scin,"x/F:y/F:z/F:index/I:Measure_Time/I:Calibed_Time/I"); 

  OutPut_Tree->Branch("Muon_TOF",&Muon_TOF,"Real_TOF/I:Measure_TOF/I:Delta_TOF/I");
 
  OutPut_Tree->Branch("ED5",&ElecDet5,"x/F:y/F:z/F:N841_Measure_Time/I:N844_Measure_Time/I:ReCon_Time/I:N841_Delta_Time/I:N841_Delta_Time/I:Charge/I:Calibed_Charge/F"); 
  
  Long64_t nEntries = InPut_Tree->GetEntries() ;
  cout<<nEntries<<endl ;
  for(int i=0 ; i<nEntries ; i++)
  {
    InPut_Tree->GetEntry(i) ;
    if(!Get_Valid_Strip_Wire_Number()) continue ;
    Calcu_TGC_Coor() ;
    if(!Calcu_Scin()) continue ;
    Calcu_ED5() ;
    OutPut_Tree->Fill() ;
  }
  OutPut_File->Write() ;
  OutPut_File->Close() ;
}
