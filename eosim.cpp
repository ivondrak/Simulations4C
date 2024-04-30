/***********************************************************************
	Simulation - OOP C++
	<c> I.Vondrak 1991
	Event-Oriented Simulation Methods
************************************************************************/
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "eosim.hpp"
#include <limits> // Replace #include <values.h> with #include <limits>

/**** Static values ****/
float Clock::now = 0;
float Clock::timeOfNextEvent = std::numeric_limits<float>::max(); // Replace MAXFLOAT with std::numeric_limits<float>::max();

/************************************************************************
	Queueing Network Classes
*************************************************************************/

/**** Source and Sink Class ****/

Transaction* Source::Create(void)
{
    Transaction *thisOne;

    count++;
    if ((thisOne = new Transaction(Clock::NOW())) == NULL)
	    {
	     std::cout << "No memory for Transaction!\n";
	     exit(0);
	    }
    thisOne->nextInLine = NULL;
    return thisOne;
}

void Source::Report(void)
{
    std::cout << std::setprecision(2);
    std::cout << "Source report:\n";
    std::cout << HowMany() << " transactions ";
    std::cout << "were created by this source at " << std::setw(8) << Clock::NOW() << std::endl << std::endl;
}

void Sink::Remove(Transaction *t)
{
  delete t;
  count++;
}

void Sink::Report(void)
{
    std::cout << std::setprecision(2);
    std::cout << "Sink report:\n";
    std::cout << HowMany() << " transactions ";
    std::cout << "were removed through this sink at " << std::setw(8) << Clock::NOW() << std::endl << std::endl;
}

/***** Server *****/

Server::Server(void)
{
   startTime = Clock::NOW();
   state     = idle;
   timeOfLastChange = 0.0;
   useIntegral = 0.0;
}

float  Server::ServerUtilization(void)
{
    if (Clock::NOW() > 0)
	 return (useIntegral/(Clock::NOW()-startTime)*100);
    else
	 return (0.0);
}
		
void Server::Seize(void)
{
   if ( state == busy)
	std::cout << "The server is busy!\n";
   else
	{
	 state = busy;
	 timeOfLastChange = Clock::NOW();
	}
}

void Server::Release(void)
{
   if ( state == idle)
       std::cout << "The server is idle!\n";
   else
       {
            useIntegral += Clock::NOW()-timeOfLastChange;
            state = idle;
            timeOfLastChange = Clock::NOW();
       }
}

int  Server::IsAvailable(void)
{
   if ( state == idle)
       return true;
   else
       return false;
}

void Server::Report(void)
{
   std::cout << std::setprecision(2);
   std::cout << "Server report:\n";
   std::cout << "Utilization of the server was " << std::setw(5) << ServerUtilization() << " %.\n";
}

/***** Queue *****/

Queue::Queue(void)
{
    startTime        = Clock::NOW();
    first            = NULL;
    last             = NULL;
    length           = 0;
    maxLength        = 0;
    timeOfLastChange = 0.0;
    lengthIntegral   = 0.0;
    noOfEntries      = 0;
    noOfDepartures   = 0;
}


float Queue::MeanQlength(void)
{
    if (Clock::NOW() > 0)
	return lengthIntegral/(Clock::NOW()-startTime);
    else
	return (0.0);
}

float Queue::MeanQdelay(void)
{
    if (Clock::NOW() > 0)
	return lengthIntegral/(float) noOfDepartures;
    else
	return (0.0);
}

void  Queue::FileInto(Transaction *t)
{
    noOfEntries++;
    length++;
    lengthIntegral += (length-1)*(Clock::NOW()-timeOfLastChange);
    timeOfLastChange = Clock::NOW();
    if (length > maxLength)
	maxLength = length;
    if ((length-1) == 0)
	first = t;
    else
	last->nextInLine = t;
    t->nextInLine = NULL;
    last = t;
}

Transaction* Queue::TakeFirst(void)
{
    Transaction *t;

    if (length <= 0)
	{
        std::cout << "Can't remove item from an empty queue!\n";
        return NULL;
	}
    else
	{
        noOfDepartures++;
        length--;
        lengthIntegral += (length+1)*(Clock::NOW()-timeOfLastChange);
        timeOfLastChange = Clock::NOW();
        t = first;
        first = first->nextInLine;
        t->nextInLine = NULL;
        if (length == 0)
            last = NULL;
        return t;
	}
}

void  Queue::Report(void)
{
   std::cout << std::setprecision(2);
   std::cout << "Queue report:\n";
   std::cout << noOfEntries << " items entered and\n";
   std::cout << noOfDepartures << " left the queue.\n";
   std::cout << "Its current length is " << std::setw(5) << length << std::endl;
   std::cout << "Queue's main characteristics were: \n";
   std::cout << "---------------------------------- \n";
   std::cout << "Mean length = " << std::setw(5) << MeanQlength() << std::endl;
   std::cout << "Max  length = " << std::setw(5) << MaxQlength() << std::endl;
   std::cout << "Mean delay  = " << std::setw(5) << MeanQdelay() << std::endl << std::endl;
}

/********************************************************************
	Agenda
*********************************************************************/

/**** Agenda ****/

void Agenda::Insert(EventNotice *x, EventNotice *before, EventNotice *after)
{
   before->nextEvent = x;
   x->nextEvent = after;
}

void Agenda::ScheduleEvent(EventNotice *EN)
{
   EventNotice *that,*prev,*next;

   that = EN;
   that->nextEvent = NULL;
   agenda.length++;
   if (agenda.firstEvent == NULL)
       agenda.firstEvent = that;
   else
      {
       if (that->EventTime() < agenda.firstEvent->EventTime())
	  {
	   that->nextEvent = agenda.firstEvent;
	   agenda.firstEvent = that;
	   Clock::TimeOfNextEvent(that->EventTime());
	  }
       else
	  {
	   next = agenda.firstEvent;
	   prev = NULL;
	   while (that->EventTime() >= next->EventTime()
		  && next->nextEvent != NULL)
	       {
		prev = next;
		next = next->nextEvent;
	       }
	   if (next->nextEvent == NULL
	       && that->EventTime() >= next->EventTime())
	       Insert(that,next,NULL);
	   else
	       Insert(that,prev,next);
	  }
       agenda.length++;
      }
}

void Agenda::GetNextEvent(void)
{
    int aLongNumber = INT_MAX;
    EventNotice *current;

    current = NULL;
    if (agenda.firstEvent == NULL)
	    std::cout << "ERROR! Can't remove any item from empty event list.\n";
    else
    {
        current = agenda.firstEvent;
        agenda.firstEvent = agenda.firstEvent->nextEvent;
        agenda.length--;
        Clock::NOW(current->EventTime());
        if (agenda.firstEvent == NULL)
            Clock::TimeOfNextEvent(aLongNumber);
        else
            Clock::TimeOfNextEvent(agenda.firstEvent->EventTime());
        currentEvent = current;
    }
}

/**************************************************************************
	Monitor
***************************************************************************/

/**** Monitor ****/

void Monitor::Report(void)
{
   std::cout << std::setprecision(2);
   std::cout << "Monitor report:\n";
   std::cout << HowManySamples() << " samples were taken.\n";
   std::cout << "The current clock time is: " << std::setw(8) << Clock::NOW() << std::endl << std::endl;
}

/*********************************************************************
	Event-Oriented Shell
**********************************************************************/

void SimFrame::Show(void)
{
    TheMonitor.Report();
    TheSource.Report();
    TheSink.Report();
    TheQueue.Report();
    TheServer.Report();
}

void SimFrame::Run(void)
{
    while (TheMonitor.MoreSamples() && TheMonitor.MoreEvents())
	{
	 TheMonitor.TheAgenda.GetNextEvent();
	 Execute();
	 TheMonitor.TheAgenda.FlushOutEvent();
	}
    Show();
}