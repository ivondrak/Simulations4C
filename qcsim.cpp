/***********************************************************************
	Quasi-Continuous Simulation - OOP C++
	<c> I.Vondrak 1991
************************************************************************/

#include "qcsim.hpp"
#include <iostream>
#include <iomanip>

float Clock::NOW = 0;
float Clock::resolution;

/********************************************************************
    Variable - methods
*********************************************************************/

void Variable::Update(void)
{
   level += deriv*Clock::Step();
}


/********************************************************************
	Statistics - methods
*********************************************************************/


/*****  Graph - methods  *****/

Graph::Graph(float minXobs, float maxXobs, float minYobs, float maxYobs, int maxXcoordP, int maxYcoordP)
{
 char space = ' ';
 int rows,cols;

 minXvalue = minXobs;
 minYvalue = minYobs;
 maxXvalue = maxXobs;
 maxYvalue = maxYobs;
 maxXcoord = maxXcoordP;
 maxYcoord = maxYcoordP;
 xCoordUnit = (maxXvalue-minXvalue)/(float) maxXcoord;
 yCoordUnit = (maxYvalue-minYvalue)/(float) maxYcoord;
 moveX = (int) (minXvalue/(float) xCoordUnit + (minXvalue < 0 ? -0.5 : 0.5)) * (-1);
 moveY = (int) (minYvalue/(float) yCoordUnit + (minYvalue < 0 ? -0.5 : 0.5)) * (-1);
 for (rows = 0; rows <= maxYcoord; rows++)
     for (cols = 0; cols <= maxXcoord; cols++)
	  grid[cols][rows] = space;
}

void Graph::Update(float xObs, float yObs, char symbol)
{
 char overprintSymbol = '#';
 char space           = ' ';
#include <iostream>

int x,y;
if (xObs < minXvalue || xObs > maxXvalue)
    std::cout << "X = " << xObs << " out of range for this graph!\n";
else
    if (yObs < minYvalue || yObs > maxYvalue)
        std::cout << "Y = " << yObs << " out of range for this graph!\n";
    else
    {
        x = (int) (xObs/(float) xCoordUnit+(xObs < 0 ? -0.5 : 0.5))+moveX;
        y = (int) (yObs/(float) yCoordUnit+(yObs < 0 ? -0.5 : 0.5))+moveY;
        if (grid[x][y] != space && grid[x][y] != symbol)
            grid[x][y] = overprintSymbol;
        else
            grid[x][y] = symbol;
    }
}

void Graph::Show(void)
{
#include <iostream> // Include the missing header file

const char *space = " ";
int rows,cols;
float yLable;

std::cout << std::setprecision(2) << std::endl; // Use std::cout instead of cout
std::cout << std::setw(12) << minXvalue;
std::cout << std::setw(maxXcoord-1) << space;
std::cout.setf(std::ios::left,std::ios::adjustfield);
std::cout << std::setw(8) << maxXvalue << std::endl;
std::cout.setf(std::ios::right,std::ios::adjustfield);
std::cout << std::setw(11) << "+";
for (cols = 0; cols <= maxXcoord; cols++)
    std::cout << "-";
std::cout << std::endl;
yLable = minYvalue;
 for (rows = 0; rows <=  maxYcoord; rows++)
     {
      if (rows%5 == 0)
	  std::cout << std::setw(9) << yLable << " I";
      else
	  std::cout << std::setw(9) << space << " I";
      for (cols = 0; cols <= maxXcoord; cols++)
	   std::cout << grid[cols][rows];
      std::cout << std::endl;
      yLable = yLable+yCoordUnit;
     }
 std::cout << std::endl;
}

/********************************************************************
	SimFrame - methods
*********************************************************************/

void SimFrame::Show(void)
{
 TheGraph.Show();
}

void SimFrame::Run(void)
{
 while (TheMonitor.MoreTime())
 {
  TheClock.AnotherTick();
  Update();
 }
 Show();
}