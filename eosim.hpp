
/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Global Classes for Event-Oriented Simulation
************************************************************************/

#include <limits>

class SimFrame;

/************************************************************************
	Clock
*************************************************************************/


/*****  Clock Class   *****/

class Clock
{
 protected:
    static float now;
    static float timeOfNextEvent;

 public:
    static void  NOW(float T) { now = T; }
    static float NOW(void)    { return now; }
    static void  TimeOfNextEvent(float T) { timeOfNextEvent = T; }
    static float TimeOfNextEvent(void)    { return timeOfNextEvent; }
};

/***********************************************************************
	Transactions
************************************************************************/

/**** Transaction class ****/

class Transaction
{
 protected:
   float       birthTime;

 public:
   Transaction *nextInLine;

   Transaction(float T) : birthTime(T) {}
   virtual ~Transaction(void) {}
   float FlowTime(void) {return Clock::NOW() - birthTime;};
   float BirthTime(void)    { return birthTime; }
};

/************************************************************************
	Queueing Network Classes
*************************************************************************/

/**** Source and Sink Class ****/

class SandSBase
{
 protected:
    int   count;

    int HowMany(void) {return count;};

 public:
    SandSBase(void) : count(0) {}
};


class Source : public SandSBase
{
 public:
    Transaction* Create(void);
    void Report(void);
};

class Sink : public SandSBase
{
 public:
    void Remove(Transaction *t);
    void Report(void);
};

/**** Server class ****/

class Server
{
 protected:
    float startTime;
    int inUse;
    enum {idle,busy} state;
    float timeOfLastChange;
    float useIntegral;

    float ServerUtilization(void);

 public:
    Server(void);
    void Seize(void);
    void Release(void);
    int  IsAvailable(void);
    void Report(void);
};

/**** Queue class ****/

class Queue
{
 protected:
    Transaction *first, *last;
    int   length, maxLength;
    float timeOfLastChange;
    float lengthIntegral;
    float startTime;
    int   noOfEntries;
    int   noOfDepartures;

    float MeanQlength(void);
    int   MaxQlength(void) {return maxLength;};
    float MeanQdelay(void);

 public:
    Queue(void);
    void FileInto(Transaction *t);
    Transaction* TakeFirst(void);
    int  Length(void) {return length;}
    void Report(void);
};

/********************************************************************
	Event Notices
*********************************************************************/

/**** Event Notice ****/

typedef int EventType;

class EventNotice
{
 protected:
   float          eventTime;
   EventType      event;
   Transaction    *trans;

 public:
   EventNotice    *nextEvent;

   EventNotice(float T, EventType E, Transaction *t = NULL) :
	       eventTime(Clock::NOW()+T) { event = E; trans = t; }
   virtual ~EventNotice(void) {}
   float EventTime(void)    { return eventTime; }
   Transaction* Trans(void) { return trans; }
   EventType Event(void) { return event; }
};



/**** Event list ****/

struct EventList
{
    EventNotice *firstEvent;
    int length;

    EventList(void) : length(0) { firstEvent = 0;}
};

/********************************************************************
	Agenda
*********************************************************************/

/**** Agenda ****/

class Agenda
{
 protected:
    EventList       agenda;
    EventNotice     *currentEvent;

    void Insert(EventNotice *x, EventNotice *before, EventNotice *after);

 public:
    void FlushOutEvent(void) {delete currentEvent;};
    void ScheduleEvent(EventNotice *EN);
    void GetNextEvent(void);
    EventType KindOfEvent(void) { return currentEvent->Event(); }
    Transaction* GetTransaction(void) { return currentEvent->Trans(); }
    int Empty(void) { if (agenda.length > 0) return false; else return true; }
 };

/**************************************************************************
	Monitor
***************************************************************************/

/**** Monitor ****/

class Monitor
{
 protected:
    int    sampleSize;
    int    samplesTaken;

 public:
    Agenda TheAgenda;

    Monitor(int samples) : sampleSize(samples), samplesTaken(0) {}
    int  HowManySamples(void) { return samplesTaken;}
    int  MoreEvents(void) { if (TheAgenda.Empty()) return false; else return true;}
    int  MoreSamples(void) { if (samplesTaken < sampleSize) return true; else return false; }
    void RecordAnotherSample(void) { samplesTaken++; }
    void Report(void);
};

/*********************************************************************
	Event-Oriented Shell
**********************************************************************/

class SimFrame
{
 protected:
    virtual void Show(void);
    virtual void Execute(void) = 0;

 public:
    Clock       TheClock;
    Monitor     TheMonitor;
    Source      TheSource;
    Sink        TheSink;
    Queue       TheQueue;
    Server      TheServer;

    SimFrame(int S) : TheMonitor(S) {};
    void Run(void);
};



