/*** Test of Event Oriented Simulation ***/

#include "glsim.hpp"
#include "eosim.hpp" // Include the eosim.hpp header file

class Simulation : public SimFrame
{
   enum { hello, start, finish, bye };

   Exponential    Arrivals,Service;
   Histogram      SpentTime;

   void Show(void);
   void Execute(void);

 public:
   Simulation(void) : SimFrame(1000), Arrivals(0.8), Service(1.0),
		      SpentTime(0,25,5) {
		      TheMonitor.TheAgenda.ScheduleEvent(new EventNotice(0, hello));
		     }
   void Hello(void);
   void Start(void);
   void Finish(Transaction*);
   void Bye(Transaction*);
};

void Simulation::Show(void)
{
   SimFrame::Show();
   SpentTime.Show();
}

void Simulation::Execute(void)
{
   switch (TheMonitor.TheAgenda.KindOfEvent())
      {
       case hello : Hello(); break;
       case start : Start(); break;
       case finish: Finish( TheMonitor.TheAgenda.GetTransaction()); break;
       case bye   : Bye( TheMonitor.TheAgenda.GetTransaction()); break;
      }
}

void Simulation::Hello(void)
{
   Transaction *client;

   TheMonitor.TheAgenda.ScheduleEvent(new EventNotice(Arrivals.Generate(),hello));
   client = TheSource.Create();
   TheQueue.FileInto(client);
   TheMonitor.TheAgenda.ScheduleEvent(new EventNotice(0, start));
}

void Simulation::Start(void)
{
    Transaction *client;

    if (TheServer.IsAvailable() && TheQueue.Length() > 0)
   {
      TheServer.Seize();
      client = TheQueue.TakeFirst();
      TheMonitor.TheAgenda.ScheduleEvent(new EventNotice(Service.Generate(),finish,client));
   }
}

void Simulation::Finish(Transaction* client)
{
    TheServer.Release();
    TheMonitor.TheAgenda.ScheduleEvent(new EventNotice(0,bye,client));
    TheMonitor.TheAgenda.ScheduleEvent(new EventNotice(0,start));
}

void Simulation::Bye(Transaction* client)
{
    SpentTime.Update(client->FlowTime());
    TheSink.Remove(client);
    TheMonitor.RecordAnotherSample();
}

/***  Main  ***/

int main(void)
{
    Simulation Sim;
    Sim.Run();
    return 0;
}
