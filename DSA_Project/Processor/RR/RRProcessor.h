#pragma once
#include "../../Scheduler/Scheduler.h"
class RRProcessor : public Processor {
private:
	int RTF;
	int CurrProcTime; // to track for how long has the process been in the processor
public:
	int timeSlice = 0;
	RRProcessor() {
		runningProc = nullptr;
		type = RR;
		
	}


	int variable = -1;
	pair<int, int> var;

	Process* scheduleAlgo(int timeStep) {
		if (var.first == timeStep && runningProc && runningProc->getReq() > 0 && runningProc->getState() != TERM) {
			blk->insert(runningProc);
			runningProc->setState(BLK);
			runningProc->setIO(runningProc->getReq() - 1); // Decrementing I / O Requests
			runningProc = nullptr;
			state = IDLE;
		}
		else if (!readyProcesses.isEmpty() && !runningProc ) // If processor ready queue is not empty and has no running process /
		{
			
				var.first = 0;
				var.second = 0;
				state = BUSY;
				runningProc = readyProcesses.deQueue();
				if (runningProc->CT == runningProc->rm_time) {
					runningProc->RT = timeStep - runningProc->AT;
				}
				CurrProcTime = runningProc->getRemainingT();
				runningProc->setState(RUN);
				variable = timeSlice + timeStep;
				pair<int, int>* vv = NULL;
				runningProc->IO_R_D.peakFront(vv);
				if (vv) {
					var.first = vv->first;
					var.second = vv->second;
				}
				var.first += timeStep;
			
		}
		else if (runningProc) // If there's a process in running state already/
		{
			BusyTime++;
			
			if (variable > timeStep) {
				runningProc->Dec_RemT();
				CurrProcTime = runningProc->getRemainingT();
				if (CurrProcTime == 0 ) { // Process finished /
					runningProc->setState(TERM);
					trm->insert(runningProc);
					runningProc->setTT(timeStep);
					runningProc->TRT_Set();
					runningProc->WT_Set();
					runningProc = nullptr;
					state = IDLE;
				}
		
			}
			else {
				runningProc->Dec_RemT();
				Process* tmp = runningProc;
				runningProc = nullptr;
				return tmp;

			}
		}
		else {
			state = IDLE;
			IDLE_Time++;
		}

			
		return null;
	}


	int getExpectedFinishTime() {
		return this->exepectedTimeToFinish;
	}

	int getNumOfProcesses() {
		return readyProcesses.count;
	}

	string getTypeString() {

		return "RR";

	}

	void addProcess(Process* process) {
		readyProcesses.insert(process);
		exepectedTimeToFinish += process->getRemainingT();
		state = BUSY;
	}

	int getTimeSlice() { return timeSlice; }

	int getRTF() { return RTF; }

	void setTimeSlice(int n) { this->timeSlice = n; }

	void setRTF(int n) { this->RTF = n; }

	void updateState() {
		if (readyProcesses.isEmpty()) { state = IDLE; }
		else { state = BUSY; }
	}


};