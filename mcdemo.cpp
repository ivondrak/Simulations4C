/*** Test of Monte Carlo Simulation ***/

#include "mcsim.hpp"
#include <math.h>
#include <iostream>

const float Pi=3.14159;

class MonteCarlo : public SimFrame
{
 private:
  Uniform CoordF;
  Uniform CoordZ;
  Uniform CoordY;

  Normal  InaccF;
  Normal  InaccZ;
  Normal  InaccY;

 public:
  MonteCarlo(void) : SimFrame(100,0,20,5),
       CoordF(0,3*Pi/2), CoordZ(1,1.6), CoordY(0.5,1.0),
       InaccF(0,Pi/500),  InaccZ(0,0.005),InaccY(0.0,0.002) {};
  void Generate(void);
  void Show(void);
};

void MonteCarlo::Generate(void)
{
  // Relative (general) coordinates
  float F  = CoordF.Generate();
  float Z  = CoordZ.Generate();
  float Y  = CoordY.Generate();

  float DF = InaccF.Generate();
  float DZ = InaccZ.Generate();
  float DY = InaccY.Generate();

  // Absolute coordinates
  float x0 = -sin(F) * Y;
  float y0 =  cos(F) * Y;
  float z0 =  Z;
  float x  = -sin(F + DF) * (Y + DY);
  float y  =  cos(F + DF) * (Y + DY);
  float z  =  Z + DZ;

  // Compute inaccuracy
  Sample = (float) sqrt(pow(x0-x,2) + pow(y0-y,2) + pow(z0-z,2)) * 1000;
}

#include <iostream>

void MonteCarlo::Show(void)
{
  std::cout << "Inaccuracy of the Robot in [mm]\n";
  SimFrame::Show();
}

int main()
{
 MonteCarlo MySim;
 MySim.Run();
 return 0;
}