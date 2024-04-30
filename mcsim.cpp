/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Monte Carlo Methods
************************************************************************/
#include "mcsim.hpp"

/********************************************************************
	Monitor
*********************************************************************/

Monitor::Monitor(int Samples)
{
  samplesTaken = 0;
  toBeTaken = Samples;
}

int Monitor::AnotherSample(void)
{
  samplesTaken++;
  if (samplesTaken <= toBeTaken)
      return True;
  else
      return False;
}


/*********************************************************
	Monte Carlo Frame
**********************************************************/

void SimFrame::Show(void)
{
   TheHistogram.Show();
}

void SimFrame::Record(void)
{
   TheHistogram.Update(Sample);
}

void SimFrame::Run(void)
{
   while (TheMonitor.AnotherSample())
   {
      Generate();
      Record();
   }
   Show();
}