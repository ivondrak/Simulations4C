/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Global Classes for Monte Carlo
************************************************************************/
#include "glsim.hpp"

/************************************************************************
	Monitor
*************************************************************************/

class Monitor
{
 protected:
    int samplesTaken;
    int toBeTaken;

 public:
    Monitor(int Samples);
    int AnotherSample(void);
};

/************************************************************************
	Monte Carlo Shell
*************************************************************************/

class SimFrame
{
 protected:
    float   Sample;

    virtual void Show(void);
    virtual void Generate(void) = 0;
    virtual void Record(void);

 public:
    Monitor     TheMonitor;
    Histogram   TheHistogram;

    SimFrame(float S, float L, float H, int C):
             TheMonitor(S), TheHistogram( L, H, C) {Sample = 0;};
    void Run(void);
};

