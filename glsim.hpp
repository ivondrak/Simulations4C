/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Generator and Statistics
************************************************************************/
#include <stdlib.h>

#define  MaxClasses      50
#define  True            1
#define  False           0

/************************************************************************
	Monte Carlo
*************************************************************************/

/**** Generator ****/

class Generator
{
 public:
    float Generate(void) { return (float) rand() / RAND_MAX;}
};

/**** Binary Distribution ****/

class Binary : public Generator
{
 private:
    float percentTRUE;

 public:
    Binary(float P)   {percentTRUE = P;};
    int Generate(void);
};


/****  Discrete Uniform Distribution ****/

class Discrete :public Generator
{
 private:
    int low;
    int high;

 public:
    Discrete(int L, int H) { low = L; high = H;}
    int Generate(void);
};

/**** Uniform Distribution ****/

class Uniform : public Generator
{
 private:
    float low;
    float high;

 public:
    Uniform(float L, float H) {low = L; high = H;}
    float Generate(void);
};

/**** Exponential Distribution ****/

class Exponential : public Generator
{
 private:
    float lambda;

 public:
    Exponential(float L) {lambda = L;}
    float Generate(void);
};

/**** Normal Distribution ****/

class Normal : public Generator
{
 private:
    float mean;
    float sigma;

 public:
    Normal(float M, float S) {mean = M; sigma = S;}
    float Generate(void);
};

/************************************************************
	Statistics
*************************************************************/

/**** Histogram ****/

class Histogram
{
 private:
    int   noOfClasses;
    int   noOfObs;
    float classWidth;
    float low,high;
    int   counters[MaxClasses];
    int   overflow,underflow;

 public:
    Histogram( float L, float H, int C);
    void Update(float obs);
    void Show(void);
};
