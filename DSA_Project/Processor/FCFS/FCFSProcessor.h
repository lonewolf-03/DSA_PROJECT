#pragma once
#include "../Processor.h"
class FCFSProcessor : public Processor {
private:
	int MaxW;
	int fork_probability;

public:
	int kill_count;
	List<pair<int, int>>* sigKill;

	FCFSProcessor() {
		runningProc = nullptr;
		type = FCFS;
		kill_count = 0;
		
	}

	pair<int, int> var;
	Process* scheduleAlgo(int timeStep) {
		if (runningProc != NULL) {
			if ((runningProc->getRemainingT() > MaxW) && !runningProc->isForked()) {
				Process* tmp = runningProc;
				runningProc = null;
				return tmp;
			}
		}
		Process* p = NULL;
		for (int i = 0; i < readyProcesses.count; i++) {
			Process* pr = readyProcesses.elementAt(i);
			for (int j = 0; j < sigKill->count; j++) {
				pair<int, int> pa = sigKill->elementAt(j);
				if (pr->PID == pa.second && timeStep == pa.first) {
				
					pr->setTT(timeStep);
					pr->TRT_Set();
					pr->WT_Set(false);

					
					pr->setState(TERM);
					Process* pep = new Process(*(pr));
					trm->insert(pep);
					readyProcesses.Delete(pr);
					kill_count++;

					
					
					
					
				}
				else if (runningProc && runningProc->PID == pa.second && timeStep == pa.first) {
					trm->insert(runningProc);
					runningProc->setTT(timeStep);
					runningProc->TRT_Set();
					runningProc->WT_Set(false);
					runningProc->setState(TERM);
					runningProc = null;
				}
			}
		}
		if (var.first == timeStep && runningProc && runningProc->getReq() > 0) {
			blk->insert(runningProc);
			runningProc->setState(BLK);
			runningProc->setIO(runningProc->getReq() - 1); // Decrementing I / O Requests
			runningProc = nullptr;
			state = IDLE;
		}
		else if ((!readyProcesses.isEmpty()) && !runningProc) {
			var.first = 0;
			var.second = 0;
			runningProc = readyProcesses.deQueue();
			if (runningProc->CT == runningProc->rm_time) {
				runningProc->RT = timeStep - runningProc->AT ;
				
			}
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
				if (!readyProcesses.isEmpty())
					p = readyProcesses.deQueue();
				if (p) {
					p->setState(RUN);
					runningProc = p;
				}
				else { 
					state = IDLE;
				}
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

	int getUtilTime() { return 0; }
	int getLoad() { return 0; }

	int getNumOfProcesses() {
		return readyProcesses.count;
	}

	string getTypeString() {

		return "FCFS";

	}

	void addProcess(Process* process) {
		readyProcesses.insert(process);
		exepectedTimeToFinish += process->getRemainingT();
		state = BUSY;
	}

	//Processor* getShortestSJF() {
	//	Processor* minProcessor = nullptr;
	//	int minTime = INT_MAX;
	//	int count = processors.count;
	//	for (int i = 0; i < count; i++) {

	//		if ((processors.elementAt(i)->type == SJF) && (processors.elementAt(i)->getExpectedFinishTime() < minTime)) {
	//			minTime = processors.elementAt(i)->getExpectedFinishTime();
	//			minProcessor = processors.elementAt(i);

	//		}
	//	}
	//	return minProcessor;
	//}

	int getMaxW() {
		return MaxW;
	}

	int getForkProb() {
		return fork_probability;
	}

	void updateState() {
		if (readyProcesses.isEmpty()) state = IDLE;
		else state = BUSY;
	}

	void setMaxW(int m) { this->MaxW = m; }

	void setForkProb(int m) { this->fork_probability = m; }

	Process* getRunningProcess() { return runningProc; }


};