/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Global Classes  for Quasi-Continuous Simulation
************************************************************************/

#define  MaxPlotPts      50
#define  True            1
#define  False           0

/***********************************************************************
	Clock Class
************************************************************************/


/*****  Clock Class   *****/

class Clock
{
 private:
     static  float NOW;
     static  float resolution;

 public:
     Clock(float Res)               {resolution = Res;}
     void  AnotherTick(void)        {NOW += resolution;}
     static  float Time(void)       {return NOW;}
     static  float Step(void)       {return resolution;}
};

/********************************************************************
	Monitor
*********************************************************************/

class Monitor
{
 private:
    float Length;

 public:
    Monitor(float L)  	{Length = L;}
    int MoreTime(void)  {return (Length-Clock::Time()) > 0 ? True : False;}
};

/********************************************************************
	Variable
*********************************************************************/

class Variable
{
 private:
    float level;
    float deriv;
 public:
    Variable(float L)   {level = L;}
    void Fcn(float D)   {deriv = D;}
    void Update(void);
    float Level(void)   {return level;}
};


/********************************************************************
	Statistics
*********************************************************************/


/*****  Graph  *****/

class Graph
{
 private:
    int maxXcoord,maxYcoord;
    int moveX,moveY;
    float xCoordUnit,yCoordUnit;
    float minXvalue,maxXvalue;
    float minYvalue,maxYvalue;
    char  grid[MaxPlotPts][MaxPlotPts];

 public:
    Graph(float minXobs, float maxXobs, float minYobs, float maxYobs, int maxXcoordP, int maxYcoordP);
    void Update(float xObs, float yObs, char symbol);
    void Show(void);
};

/********************************************************************
	Simulation Frame
*********************************************************************/


class SimFrame
{
 protected:
    virtual void Show(void);
    virtual void Update(void) = 0;

 public:
    Clock   TheClock;
    Monitor TheMonitor;
    Graph   TheGraph;

    SimFrame(float R, float L, float X1, float X2, float Y1, float Y2, float X, float Y) :
             TheClock(R), TheMonitor(L), TheGraph(X1, X2, Y1, Y2, X, Y) {};
    void Run(void);
};
