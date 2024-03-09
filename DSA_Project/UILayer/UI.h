#pragma once
#include "../Scheduler/Scheduler.h"
#include <iostream>
#include "../Process/Process.h"
#include <thread>
#include <chrono>

using namespace std;



class UI {
private:
	int mode;
	Scheduler* schedular;
	string filename;
	string output_filename;
	void getInputFile() {
	
		schedular->loadProcess(filename);
	}


	void RunInteractiveMode() {
		while (!schedular->isDone()) {
			std::cout << "Current Timestep: " << schedular->getTimeStep() << "\n";
			schedular->update();

			PrintRDYLists();
			PrintBLKList();
			PrintRUNProcesses();
			PrintTRMList();
			PrintKILLList();
			std::cout << "PRESS ANY KEY TO CONTINUE\n";

			cin.get();

			system("cls");

		}
	/*	schedular->setProcessorsLoads();*/
	}

	void RunSilentMode() {
	
		while (!schedular->isDone()) {
		
			schedular->update();
		

		}
	}

	void RunStpByStpMode() {
		while (!schedular->isDone()) {
			std::cout << "Current Timestep: " << schedular->getTimeStep() << "\n";
			schedular->update();

			PrintRDYLists();
			PrintBLKList();
			PrintRUNProcesses();
			PrintTRMList();
			PrintKILLList();
			this_thread::sleep_for(chrono::microseconds(100));
			system("cls");
		}
	}

	void PrintRDYLists() {
		List<Processor*>* processors_list = schedular->getProcessorsPtr();
		Processor* curr_processor;
		std::cout << "--------------------  RDY Processes  --------------------\n";
		for (int i = 0; i < processors_list->count; i++) {
			curr_processor = processors_list->elementAt(i);
			std::cout << "Processor " << i + 1 << " [ " << curr_processor->getTypeString() << " ]: " << curr_processor->getRDYList()->count << " RDY: ";
			for (int j = 0; j < curr_processor->getRDYList()->count; j++) {
				std::cout << curr_processor->getRDYList()->elementAt(j)->GetPID() << " ";
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}

	
	void PrintBLKList() {
		
		List<Process*>* blk_list = schedular->getBLKPtr();
		std::cout << "--------------------  BLK Processes  --------------------\n";
		if (!blk_list->isEmpty()) {
			cout << blk_list->count << " BLK: ";
			/*  Count of processes in BLK lists*/
			pair<int, int>* Duration = new pair<int, int>();
			for (int i = 0; i < blk_list->count; i++) {
				Process* p = blk_list->elementAt(i);
				p->IO_R_D.peakFront(Duration);
				if (Duration->second != 0) {
					/* Input or Output should be added here */
					cout << blk_list->elementAt(i)->GetPID() << ", ";
					Duration->second--; /* Duration decreaments every timestep */
				}
				else { // Duration is finished //
					p->IO_R_D.dequeue(Duration);
					schedular->BLKtoRDY(p);
					blk_list->Delete(p);
					



				}
			}
		}
		else {
			cout << "BLK: There are currently no processes in BLK list";
		}
		cout << endl;
	}

	void PrintRUNProcesses() {
		List<Processor*>* processors_list = schedular->getProcessorsPtr();
		cout << "--------------------  RUN Processes  --------------------\n";
		int c = 0;

		for (int i = 0; i < processors_list->count; i++) {
			Processor* proc = processors_list->elementAt(i);
			if (proc->runningProc)
				c++;
		}
		cout << c << " RUN: ";
		for (int i = 0; i < processors_list->count; i++) {
			Processor* proc = processors_list->elementAt(i);
			if (proc->runningProc) {
				cout << proc->runningProc->PID << "(P" << i + 1 << "), ";
			}
		}
		
		cout << '\n';

	}

	void PrintTRMList() {
		List<Process*>* trm_list = schedular->getTRMPtr();
		std::cout << "--------------------  TRM Processes  --------------------\n";
		if (trm_list) {
			cout << trm_list->count << " TRM: ";
			for (int i = 0; i < trm_list->count; i++) {
				std::cout << trm_list->elementAt(i)->GetPID() << ", ";
			}
		}
		std::cout << '\n';
	}


	void PrintKILLList() {
		List<Process*>* KillList = schedular->getKILLPtr();
		std::cout << "--------------------  KILLED Processes  --------------------\n";
		if (KillList) {
			cout << KillList->count << " KILLED: ";
			int kill_count = KillList->count;
			for (int i = 0; i < kill_count; i++) {
				std::cout << KillList->elementAt(i)->GetPID() << ", ";
			}
		}
		std::cout << '\n';
	}
	
	void generateOutput(string filename) {
		fstream output;
		output.open(filename, ios::out);
		int totalProcNum = schedular->initial_length + schedular->forked_count;
		int avgWT = 0;
		int avgRT = 0;
		int avgTRT = 0;
		float forkedPercent = ((schedular->forked_count * 1.0) / (totalProcNum * 1.0)) * 100;
		float killPercent = ((schedular->getTotalKilled() * 1.0) / (totalProcNum * 1.0)) * 100;

		output << "TT		  PID		  AT		  CT		  IO_D		  WT		RT			TRT\n";
		
		Process* curr_proc;
		while (!schedular->TRMList.isEmpty()) {
			curr_proc = schedular->TRMList.deQueue();
			if (curr_proc) {
				avgWT += (curr_proc->WT);
				avgRT += (curr_proc->RT);
				avgTRT += (curr_proc->TRT);
				output << curr_proc->TT << "		  " << curr_proc->PID << "		  " << (curr_proc->AT) << "		  " << (curr_proc->CT) << "		  " << curr_proc->getTotalIOD() << "		  " << curr_proc->WT << "		" << curr_proc->RT << "			 " << curr_proc->TRT << '\n';
			}
		}

		avgRT /= totalProcNum;
		avgWT /= totalProcNum;
	    avgTRT /= totalProcNum;

		output << '\n';
		output << "Processes: " << totalProcNum << '\n';
		output << "AvgWT = " << avgWT << "  AvgRT = " << avgRT << "  AvgTRT = " << avgTRT << '\n';
		output << "migration: " << (((schedular->total_maxW_migration + schedular->total_rtf_migration) * 1.0) / (totalProcNum * 1.0)) * 100.0<<"%\n";
		output << "work steal: " << ((schedular->stealCount * 1.0) / (totalProcNum*1.0))*100.0<<"%\n";
		output << "Forked processes : " << forkedPercent << "% \n";
		output << "Killed processes : " << killPercent << "% \n\n";

		output << "Processors: " << schedular->getProcessorsPtr()->count << "[ " << schedular->fcfsNum << " FCFS, " << schedular->sjfNum << " SJF, " << schedular->rrNum << " RR]\n";
		output << "Processors Load\n";
		for (int i = 0; i < schedular->getProcessorsPtr()->count; i++) {
			output << "p" << i + 1 << ": " << schedular->getProcessorsPtr()->elementAt(i)->getLoad() << "%  ";
		}

		output << '\n';
		output << "Processors utilize\n";
		float avgUtil = 0;
		for (int i = 0; i < schedular->getProcessorsPtr()->count; i++) {
			output << "p" << i + 1 << ": " << schedular->getProcessorsPtr()->elementAt(i)->getUtilTime() << "%  ";
			avgUtil += schedular->getProcessorsPtr()->elementAt(i)->getUtilTime();
		}

		avgUtil /= schedular->getProcessorsPtr()->count;
		output << "\nAvg utilization = " << avgUtil << "%\n";
	}

public:
	
	UI(Scheduler* s , string filename, string output_filename) : schedular(s) {
	
		this->filename = filename;
		this->output_filename = output_filename;
	}

	void Start(int mode) {
		this->mode = mode;
		std::cout << "Choose one of the three modes (Interactive(1)\\ step-by-step(2)\\ silent(3)): ";
		std::cin >> mode;
		switch (mode) {
		case(1):
			getInputFile();
			RunInteractiveMode();
			//generateOutput("output1");
			break;
		case(2):
			getInputFile();
			RunStpByStpMode();
			break;
		case(3):
			getInputFile();
			RunSilentMode();
			break;
		default:
			std::cout << "Invalid Choice \n";
			break;
		}
		schedular->setProcessorsLoads();
		generateOutput(output_filename);
	}
};