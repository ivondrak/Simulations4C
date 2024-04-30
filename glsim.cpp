/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Global Classes' Methods
************************************************************************/
#include "glsim.hpp"
#include <math.h>
#include <iostream>
#include <iomanip>

/********************************************************************
	Generator - methods
*********************************************************************/

/**** Binary Distribution ****/

int Binary::Generate(void)
{
  if (Generator::Generate() <= percentTRUE)
      return True;
  else
      return False;
}

/**** Discrete Uniform Distribution ****/

int Discrete::Generate(void)
{
  return  (int) ((high-low+1)*Generator::Generate())+low;
}

/**** Uniform  distribution ****/

float Uniform::Generate(void)
{
  return (high-low)*Generator::Generate()+low;
}

/**** Exponential  Distribution	****/

float Exponential::Generate(void)
{
  return (float) -log((double) Generator::Generate())/lambda;
}

/**** Normal Distribution ****/

float Normal::Generate(void)
{
  float temp;
  float firstRN,secondRN;

  do
    {
     firstRN = (float) -log((double) Generator::Generate());
     secondRN = (float) -log((double) Generator::Generate());
    }
  while ( 2*firstRN < (float) pow((double) (secondRN-1),2.0));
  if (Generator::Generate() < 0.5) secondRN = -secondRN;
  temp = sigma*secondRN+mean;
  return temp;
}

/**********************************************************
	Statistics
***********************************************************/

/**** Histogram ****/

Histogram::Histogram(float L, float H, int C)
{
  noOfClasses = C;
  low  = L;
  high = H;
  noOfObs = 0;
  classWidth = (high-low)/(float) noOfClasses;
  for (int i = 0; i < noOfClasses; i++)
      {
      counters[i] = 0;
      overflow    = 0;
      underflow   = 0;
      }
}

void Histogram::Update(float obs)
{
  int hist_class;
  if (obs < low)
      underflow++;
  else
      if (obs > high)
        overflow++;
      else
        {
          hist_class = (int) ((obs - low)/classWidth);
          if (hist_class > noOfClasses)
            hist_class = noOfClasses;
          counters[hist_class]++;
        }
    noOfObs++;
 }

void Histogram::Show(void)
{
  int lineLength = 50;    // number of char. in column
  int barWidth   = 3;     // width of column
  //char *space     = " ";
  char printSymbol = '*';
  const char *space = " "; 
  int maxCount = counters[0];
  for (int index = 1; index < noOfClasses; index++)
      if (counters[index] > maxCount)
      maxCount = counters[index];
  std::cout << std::setprecision(2) << std::endl;
  std::cout << "Overflow was " << overflow << std::endl;
  std::cout << "Underflow was " << underflow << std::endl << std::endl;

  // measure
  float stars = (float) lineLength / (float) maxCount;

  // first class
  float classLabel = low;
  std::cout << std::setw(9) << classLabel << "I\n";
  // print columns
  for (int index = 0; index < noOfClasses; index++)
  {
    float percent = (float) counters[index]/(float) noOfObs;
    for (int barLines = 0; barLines < barWidth; barLines++)
    {
      std::cout << std::setw(9) << space << "I ";
      for (int barStars = 0; barStars < (int) (stars*counters[index]+0.5); barStars++)
        std::cout.put(printSymbol);
      if (barLines == 1)
        std::cout << std::setw(5) << percent*100 << "%";
      std::cout << std::endl;
    }
    classLabel += classWidth;
    std::cout << std::setw(9) << classLabel << "I\n";
  }
}
