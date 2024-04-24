#ifndef __Cluster__H
#define __Cluster__H

#include <iostream>
#include <vector>

using namespace std ;
class Cluster
{
public:
  Cluster() {}; 
  ~Cluster() {}
int GetMiddleNumber() 
{
//size<4
  int size = Number.size() ;
  return Number[0] + Number[size-1] ;
}
int GetMiddleNumberM3() 
{
//size>3
  int size = Number.size() ;
  int maxsignal = SignalWidth[0] ;
  int middlenumber = Number[0] ;
//if more than three signalwidth are negative, here we can't geve correct result
  for(int i=0 ; i<size ; i++)
  {
    if(SignalWidth[i]>maxsignal)
    {
       maxsignal = SignalWidth[i] ;
       middlenumber = Number[i] ; 
    }
  }
  if(middlenumber==Number[0])
    return 2*middlenumber+1 ;
  else
  {
    if(middlenumber==Number[size-1])
      return 2*middlenumber-1 ;
    else
      return 2*middlenumber ;
  }
}
int GetMaxSignalWidth()
{
  vector<int>::iterator iter = SignalWidth.begin();
  int max = *iter ;
  for(; iter != SignalWidth.end() ; iter++)
    if(max<*iter)
      max = *iter ; 
  return max ;
}
  std::vector<int> Number ;
  std::vector<unsigned int> Leading ;
  std::vector<unsigned int> Trailing ;
  std::vector<int> SignalWidth ;
  int MaxSignal ; 
  
  ClassDef(Cluster,1) ;
};
#endif
