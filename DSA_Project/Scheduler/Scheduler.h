#pragma once
#include "../Processor/FCFS/FCFSProcessor.h"
#include "../Processor/SJF/SJFProcessor.h"
#include "../Processor/RR/RRProcessor.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <process.h>
#include "../Process/Process.h"

using namespace std;
class Scheduler {
private:
	int available_ID;
	int curr_time_step;
	int IO_R;
	int IO_D;
	int rtf, maxW, stl;
	int rrTimeSlice;
	List<Processor*> processors;
	float getWorkStealPercentage() { return 0; }
	float getForkedProcessPercentage() { return 0; }
	float getKilledProcessPercentage() { return 0; }
	float getRTF() { return 0; }

	Processor* getShortestProcessor() {
		Processor* minProcessor = nullptr;
		int minTime = INT_MAX;
		int count = processors.count;
		for (int i = 0; i < count; i++) {
			if (processors.elementAt(i)->getExpectedFinishTime() < minTime) {
				minTime = processors.elementAt(i)->getExpectedFinishTime();
				minProcessor = processors.elementAt(i);
			}
		}
		return minProcessor;
	}

	Processor* getLongestProcessor() {
		Processor* maxProcessor = nullptr;
		int maxTime = 0;
		int count = processors.count;
		for (int i = 0; i < count; i++) {
			if (processors.elementAt(i)->getExpectedFinishTime() > maxTime) {
				maxTime = processors.elementAt(i)->getExpectedFinishTime();
				maxProcessor = processors.elementAt(i);
			}
		}
		return maxProcessor;
	}
public:
	int total_rtf_migration = 0;
	int total_maxW_migration;
	int forkProb;
	int index = 0;
	int numOfInsertedProcess = 0;
	int initial_length;
	int forked_count;
	int kill_count;
	int fcfsNum;
	int sjfNum;
	int rrNum;
	int stealCount = 0, migCount = 0;
	List<Process*> currentTimeProc;
	List<Process*> NEWList;
	List<Process*> TRMList;
	List<Process*> BLKList;
	List<Process*> KillList;

	List<pair<int,int>> SIGKILL;

	Scheduler() : curr_time_step(1) {
		total_maxW_migration = 0;
		total_rtf_migration = 0;
		forked_count = 0;
		kill_count = 0;
	}

	void setProcessorsLoads() {
		int totalTrt = getTotalTRT();
		if (totalTrt == 0) 
			totalTrt = 1;
		for (int i = 0; i < processors.count; i++) {
			Processor* p = processors.elementAt(i);
			p->setLoad((p->BusyTime * 100.0) / (totalTrt * 1.0));
		}
	}

	int getMaxW() {

		return maxW;

	}

	int getTimeStep() {
		return curr_time_step;
	}
	
	int getTotalTRT() {
		int totalTRT = 0;
		for (int i = 0; i < TRMList.count; i++) {
			Process* p = TRMList.elementAt(i);
			totalTRT += p->GetTRT();
		}
		return totalTRT;
	}
	int getTotalKilled() {
		int killed = 0;
		Processor* prr = NULL;
		for (int i = 0; i < processors.count; i++) {
			prr = processors.elementAt(i);

			if (prr->type == FCFS) {

				killed += dynamic_cast<FCFSProcessor*>(prr)->kill_count;

			}

		}
		return killed;
	}
	void loadProcess(string filename) {
		// and processors
		char parantheses, comma;
		fstream stream;
		stream.open(filename, ios::in);
		stream >> fcfsNum 
			>> sjfNum 
			>> rrNum 
			>> rrTimeSlice 
			>> rtf 
			>> maxW 
			>> stl 
			>> forkProb;
		int n;
		stream >> n;
		initial_length = n;
		forked_count = 0;
		available_ID = n + 1;
		for (int i = 0; i < n; i++) {
			Process* p = new Process();
			int reqTime = 0;
			stream >> p->AT >> p->PID >> p->CT >> reqTime;
			p->setRemainingT();
			for (int j = 0; j < reqTime; j++) {
				char br = ' ';
				pair<int, int>* pa = new pair<int,int>();
				stream >> br >> pa->first >> br >> pa->second >> br;
				p->IO_R_D.enqueue(pa);
				if (j != reqTime - 1) stream >> br;
			}
			NEWList.insert(p, -p->AT);
		}
		
		string uuuu = "";
		stream >> uuuu;
		int sigTimes = 0;
		stream >> sigTimes;
		for (int i = 0; i < sigTimes; i++) {
			pair<int, int> pa;
			stream >> pa.first >> pa.second;
			SIGKILL.insert(pa);
		}
		for (int i = 0; i < fcfsNum; i++) {
			FCFSProcessor* pp = new FCFSProcessor();
			pp->sigKill = &SIGKILL;
			pp->blk = &BLKList;
			pp->kill = &KillList;
			pp->trm = &TRMList;
			pp->setMaxW(maxW);
			processors.insert(pp);
		}

		for (int i = 0; i < sjfNum; i++) {
			SJFProcessor* pp = new SJFProcessor();
			pp->blk = &BLKList;
			pp->kill = &KillList;
			pp->trm = &TRMList;
			processors.insert(pp);
		}

		for (int i = 0; i < rrNum; i++) {
			RRProcessor* pp = new RRProcessor();
			pp->blk = &BLKList;
			pp->kill = &KillList;
			pp->trm = &TRMList;
			pp->timeSlice = rrTimeSlice;
			processors.insert(pp);
		}
		stream.close();
	}




	int randomNumber(int s, int e) {
		int x = rand() % (e - s + 1) + s;

		return x;
	}
	Processor* getShortestRRProcessor() {
		Processor* minProcessor = nullptr;
		int minTime = INT_MAX;
		int count = processors.count;
		for (int i = 0; i < count; i++) {
			Processor* pr = processors.elementAt(i);
			if (pr->getExpectedFinishTime() < minTime && pr->type == RR) {
				minTime = processors.elementAt(i)->getExpectedFinishTime();
				minProcessor = processors.elementAt(i);
			}
		}
		return minProcessor;
	}
	void update() {
	
		NEWToRDY();
		Processor* p;
		Process* running_proc = nullptr;

		for (int i = 0; i < processors.count; i++) {
			p = processors.elementAt(i);
			if ((p->type == FCFS) && (p->runningProc)) {
				fork(p, p->runningProc);
			}
			Process* pr = p->scheduleAlgo(curr_time_step);
			running_proc = p->runningProc;
			if (running_proc) {
				running_proc->fork_random = randomNumber(1, 100);
			}
			if (pr && p->type == RR) {
				getShortestProcessor()->readyProcesses.insert(pr);
			}
			else if (pr && p->type == FCFS && pr->getState() != TERM) {
					Processor* rrProc = getShortestRRProcessor();
					if (rrProc != NULL) {
						rrProc->readyProcesses.insert(pr);
						total_maxW_migration++;
					}
			}
		

		}
		
		RRtoSJF();
		if (getTimeStep() % stl == 0) {
			WorkSTeal();
		}
		
		//random();

		//fcfsRdyList();

		currentTimeProc.clear();
		curr_time_step++;
	}




	void NEWToRDY() {
		int timeStep = getTimeStep();
		while (!NEWList.isEmpty() && NEWList.elementAt(0)->GetAT() == timeStep) {
			// finding the process with the  earliest arrival time
			//int min = INT_MAX;
			Process* minProc = NEWList.elementAt(0);
			//min = minProc->GetAT();
			Processor* prr = getShortestProcessor();
			prr->addProcess(minProc);
			minProc->setState(RDY);
			NEWList.deQueue();
		}
	}

	void readyToRun() {
		for (int i = 0; i < processors.count; i++) {
			Processor* p = processors.elementAt(i);
			if (p->state == IDLE && !(p->getRDYList()->isEmpty())) {
				Process* proc = p->getRDYList()->top();
				p->getRDYList()->deQueue();
				p->runningProc = proc;
				proc->setState(RUN);
				p->state = BUSY;
			}
		}
	}



	void RUNtoBLK(Process* p) {
		// Call UI To Get Input
		BLKList.insert(p);
		p->setState(BLK);
	}



	void BLKtoRDY(Process* p){
		Processor* proc = getShortestProcessor();
		proc->addProcess(p);
		p->setState(RDY);
		BLKList.Delete(p);
	}



	void toTRM(Process* p) {
		if (p->isForking() && p->child) { // handling the case of forking processes
			p->child->state = ORPH;
			toTRM(p->child);
		}

		TRMList.insert(p);
		p->setState(TERM);
	}

	Processor* getShortestRR() {
		Processor* minProcessor = nullptr;
		int minTime = INT_MAX;
		int count = processors.count;
		for (int i = 0; i < count; i++) {

			if ((processors.elementAt(i)->type == RR) && (processors.elementAt(i)->getExpectedFinishTime() < minTime)) {
				minTime = processors.elementAt(i)->getExpectedFinishTime();
				minProcessor = processors.elementAt(i);

			}
		}
		return minProcessor;
	}

	void migrateToRR(Process* s) {

		Processor* Proc=getShortestRR();
		Proc->readyProcesses.insert(s);
	}


	Processor* getShortestSJF() {
		Processor* minProcessor = nullptr;
		int minTime = INT_MAX;
		int count = processors.count;
		for (int i = 0; i < count; i++) {

			if ((processors.elementAt(i)->type == SJF) && (processors.elementAt(i)->getExpectedFinishTime() < minTime)) {
				minTime = processors.elementAt(i)->getExpectedFinishTime();
				minProcessor = processors.elementAt(i);

			}
		}
		return minProcessor;
	}
	Processor* getShortestSJFProcessor() {
		Processor* minProcessor = nullptr;
		int minTime = INT_MAX;
		int count = processors.count;
		for (int i = 0; i < count; i++) {
			Processor* pr = processors.elementAt(i);
			if (pr->getExpectedFinishTime() < minTime && pr->type == SJF) {
				minTime = processors.elementAt(i)->getExpectedFinishTime();
				minProcessor = processors.elementAt(i);
			}
		}
		return minProcessor;
	}
	void RRtoSJF() {
		Processor* ShortestSJFProcessor = nullptr;
		List<Process*>* rdy_list = nullptr;
		Process* currProc = nullptr;
		for (int j = 0; j < processors.count; j++) {
			Processor* pp = processors.elementAt(j);
			if (pp->type == RR) {

				if (pp->runningProc && pp->runningProc->getRemainingT()<rtf) {

					Processor* sjf = getShortestSJFProcessor();
					if (sjf != NULL) {
						sjf->addProcess(pp->runningProc);
						pp->runningProc = nullptr;
					}

				}
			
			}
		}
	}


	bool fork(Processor* processor, Process* forking_proc) {
		if ((forking_proc->state != RUN) || (processor->type != FCFS) || (forking_proc->isForking()) || forking_proc->fork_random <= forkProb) {
			return false;
		}
		Process* forked_proc = new Process;
		forked_proc->setPID(this->available_ID);
		available_ID++;
		forked_proc->setAT(this->curr_time_step);
		forked_proc->setCT(forking_proc->rm_time);
		forked_proc->setRemainingT();
		forked_proc->setParent(forking_proc);
		forking_proc->setChild(forked_proc);
		Processor* minFCFSProcessor = nullptr;
		int minTime = INT_MAX;
		for (int i = 0; i < processors.count; i++) {
			if (processors.elementAt(i)->type == FCFS && (processors.elementAt(i)->exepectedTimeToFinish < minTime)) {
				minTime = processors.elementAt(i)->getExpectedFinishTime();
				minFCFSProcessor = processors.elementAt(i);
			}
		}
		forked_count++;
		minFCFSProcessor->addProcess(forked_proc);
		return true;
	}


	void WorkSTeal() {
		Processor* longest=getLongestProcessor();
		Processor* shortest = getShortestProcessor();
		if (longest == nullptr || shortest == nullptr)
			return;
		float StealLimit;
		StealLimit = (static_cast<float>(longest->getExpectedFinishTime() - shortest->getExpectedFinishTime()) / (longest->getExpectedFinishTime())) * 100;
		int c = 0;
		Process* tmp_proc = nullptr;
		while (StealLimit > 40 && c < longest->getRDYList()->count) {
			tmp_proc= longest->getRDYList()->elementAt(c);
			if (tmp_proc->isForked()) {
				c++;
				continue;
			}
			longest->Remove_Top(tmp_proc);
			shortest->getRDYList()->insert(tmp_proc);
			StealLimit = (static_cast<float>(longest->getExpectedFinishTime() - shortest->getExpectedFinishTime()) / (longest->getExpectedFinishTime())) * 100;
			c++;
			stealCount++;
		}
	}

	
	// methods used for statistics needed for output file
	bool isDone() {
		return (TRMList.count == initial_length + forked_count);
	}

	List<Process*>* getTRMPtr() {
		return &TRMList;
	}

	List<Process*>* getKILLPtr() {
		List<Process*>* ptr = &KillList;
		return ptr;
	}


	List<Process*>* getBLKPtr() {
		List<Process*>* ptr = &BLKList;
		return ptr;
	}


	List<Processor*>* getProcessorsPtr() {
		return &processors;
	}
};