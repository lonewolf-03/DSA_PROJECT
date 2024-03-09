#pragma once
#include "../Defs.h"
#include "../Process/Process.h"
#include "../DataStructures/PQueue/PQueue.h"
#include "../DataStructures/Queue/Queue.h"
#include "../DataStructures/List/ListADT.h"
#include "../Scheduler/Scheduler.h"
class Processor
{
	
public:
	List<Process*> readyProcesses;
	int exepectedTimeToFinish;
	
	Process* runningProc;
	List<Process*>* blk;
	List<Process*>* kill;
	List<Process*>* trm;
	int BusyTime ;
	int IDLE_Time;
	float load;
	ProcessorType type;
	ProcessorState state;



	Processor() {
		IDLE_Time = 0;
		BusyTime = 0;
		load = 0;
		state = IDLE;
		exepectedTimeToFinish = 0;
	}

	virtual int getExpectedFinishTime() {
		return this->exepectedTimeToFinish;
	}

	virtual void addProcess(Process* process) = 0;
	// void finishProcess();

	virtual Process* scheduleAlgo(int timeStep)=0 ;
	float getLoad() {
		return load;
	}

	virtual void Remove_Top(Process* pr) {
		/*Process* p = readyProcesses.top();
		if (p == nullptr)
			return;*/

		exepectedTimeToFinish -= pr->getRemainingT();
		readyProcesses.Delete(pr);
	}

	float getUtilTime() {
		int IDLE_time = GetTotal_IDLE();
		int Busy_time = GetTotalBusy();
		return (100 * Busy_time / (BusyTime + IDLE_time));
	}
	
	
	

	int GetTotal_IDLE() {
		return IDLE_Time;
	}
	int GetTotalBusy() {
		return BusyTime;

	}
	void setTotalBusy(int Time) {
		BusyTime = Time;
	}

	void setTotalIDLE(int Time) {
		IDLE_Time = Time;
	}

	void setLoad(float Load) {
		load = Load;
	}

	virtual string getTypeString() = 0;


	Process* getRunningProcPtr() { return runningProc; }

	
	List<Process*>* getRDYList() { return &readyProcesses; }
};