/*** Test of Quasi-Continuous Simulation ***/

#include "qcsim.hpp"
#include <iostream>

/*** Global Data ***/

char  SpeedSymbol   = '+';
char  DeviaSymbol   = '*';
char  ForceSymbol   = 'o';


class Simulation : public SimFrame
{
 Variable dXt, Xt;
 void Show(void);
 void Update(void);

 public:
    Simulation(void) : SimFrame(0.01,2,0,2,-12,12,49,20),
           dXt(0), Xt(0) {};
};

#include <iostream>

void Simulation::Show(void)
{
    std::cout << "*********** Oscilating of the mass ************\n\n";
    std::cout << SpeedSymbol << " for the speed\n";
    std::cout << DeviaSymbol << " for the deviation\n";
    std::cout << ForceSymbol << " for the force\n\n";
    SimFrame::Show();
}

void Simulation::Update(void)
{
 float F;

 if (TheClock.Time() < 0.5) F = 0.0;
     else                   F = 100.0;
 dXt.Fcn(F-2*dXt.Level()-100*Xt.Level());
 Xt.Fcn(dXt.Level());

 dXt.Update();
 Xt.Update();
 TheGraph.Update(TheClock.Time(),dXt.Level(),SpeedSymbol);
 TheGraph.Update(TheClock.Time(),Xt.Level(),DeviaSymbol);
 TheGraph.Update(TheClock.Time(),F/20.0,ForceSymbol);
}

/*** Main ***/

int main()
{
    Simulation MySim;
    MySim.Run();
    return 0;
}
