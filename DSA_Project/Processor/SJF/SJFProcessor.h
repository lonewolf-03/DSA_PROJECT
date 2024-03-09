#pragma once
#include "../Processor.h"

class SJFProcessor : public Processor {
private:
public:
	SJFProcessor() {
		runningProc = nullptr;
		type = SJF;
	}

	Process* FindShortestJob() {
		int min_time = INT_MAX;
		Process* shortest_proc = nullptr;
		for (int i = 0; i < readyProcesses.count; i++) {
			if (readyProcesses.elementAt(i)->getRemainingT() < min_time) {
				min_time = readyProcesses.elementAt(i)->getRemainingT();
				shortest_proc = readyProcesses.elementAt(i);
			}
		}

		return shortest_proc;
	}
	Process* getShortestProcess() {
		Process* minp = null;
		Process* p = null;
		int min = INT_MAX;

		for (int i = 0; i < readyProcesses.count; i++) {
			p = readyProcesses.elementAt(i);
			if (p->getRemainingT() < min) {
				min = p->getRemainingT();
				minp = p;
			}
		}
		return minp;
	}

	pair<int, int> var;

	Process* scheduleAlgo(int timeStep) {
		if (var.first == timeStep && runningProc && runningProc->getReq() > 0) {
			blk->insert(runningProc);
			runningProc->setState(BLK);
			runningProc->setIO(runningProc->getReq() - 1); // Decrementing I / O Requests
			runningProc = nullptr;
			state = IDLE;
		}else if (readyProcesses.count > 0 && !runningProc) {
			var.first = 0;
			var.second = 0;
			Process* temp;
			temp = getShortestProcess();
			runningProc = new Process(*(temp));
			if (runningProc->CT == runningProc->rm_time) {
				runningProc->RT = timeStep- runningProc->AT ;
			}
			readyProcesses.Delete(temp);
			runningProc->setState(RUN);
			pair<int, int>* vv = NULL;
			runningProc->IO_R_D.peakFront(vv);
			if (vv) {
				var.first = vv->first;
				var.second = vv->second;
			}
			var.first += timeStep;
		}
		else if (runningProc) {
			BusyTime++;
			if (runningProc->getRemainingT() > 0) {
				runningProc->Dec_RemT();
			}
			else {
				trm->insert(runningProc);
				runningProc->setTT(timeStep);
				runningProc->TRT_Set();
				runningProc->WT_Set();
				runningProc = nullptr;
			}
		}
		else { // the ready list is empty
			state = IDLE;
			IDLE_Time++;
		}
		return null;
	}

	int getExpectedFinishTime() {
		return this->exepectedTimeToFinish;
	}

	int getUtilTime() { return 0; }
	int getLoad() { return 0; }

	int getNumOfProcesses() {
		return readyProcesses.count;
	}

	string getTypeString() {
		return "SJF";

	}

	Process* getRunningProcPtr() { return runningProc; }


	void addProcess(Process* process) {
		readyProcesses.insert(process, process->GetAT());
		
		exepectedTimeToFinish += process->getRemainingT();
		state = BUSY;
	}

	void updateState() {
		if (readyProcesses.isEmpty()) state = IDLE;
		else state = BUSY;
	}
};