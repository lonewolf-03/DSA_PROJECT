#pragma once
#include "../Defs.h"
#include "../DataStructures/Queue/Queue.h"
/*Each process has the following information in the input file
1-AT
2-PID
3-CT
4-Number of times the process requests I/O
5-I/O_R and I/O_D
*/

class Process {

	
public:
	int PID=0;
	int AT = 0;
	int RT;
	int CT;
	int TT = 0;
	int TRT;
	int IO_REQ;
	int WT;
	int N; /*I/O Requests*/
	LinkedListQueue<pair<int,int>*> IO_R_D;
	
	int size;
	ProcessState state;
	/*Processor* processor_ptr;*/ // to be added
	int rm_time;
	bool RT_set;
	bool TT_set;
	bool PID_set;
	bool AT_set;
	bool CT_set;
	bool N_set;
	bool IO_RD_set;
	Process* parent;
	Process* child;
	bool forked;
	bool forking;
	bool orphan;
	int fork_random;


	int getTotalIOD() {
		int total = 0;
		while (!IO_R_D.isEmpty()) {
			pair<int, int>* aa;
			IO_R_D.dequeue(aa);
			total += aa->second;
		}
		return total;
	}


	Process() {
		RT_set = false;
		TT_set = false;
		PID_set = false;
		AT_set = false;
		CT_set = false;
		N_set = false;
		IO_RD_set = false;
		state = NEW;
		parent = nullptr;
		RT = PID = AT = CT = TT = TRT = WT = N = size = rm_time = 0;
	}

	Process(const Process& proc) { // copy constructor
		this->AT = proc.AT;
		this->AT_set = proc.AT_set;
		this->child = proc.child;
		this->CT = proc.CT;
		this->CT_set = proc.CT_set;
		this->forked = proc.forked;
		this->forking = proc.forking;
		this->fork_random = proc.fork_random;
		this->IO_RD_set = proc.IO_RD_set;
		this->IO_REQ = proc.IO_REQ;
		this->N = proc.N_set;
		this->orphan = proc.orphan;
		this->parent = proc.parent;
		this->PID = proc.PID;
		this->PID_set = proc.PID_set;
		this->rm_time = proc.rm_time;
		this->RT = proc.RT;
		this->RT_set = proc.RT_set;
		this->size = proc.size;
		this->state = proc.state;
		this->TT_set = proc.TT_set;
	}

	bool TRT_Set() {
		TRT = TT - AT;
		return true;
	}
	bool WT_Set(bool flag = true) {
		if (flag == true) {

			WT = TRT - CT;
		}
		else {

			WT = TRT - (CT - rm_time);
		}
	 
		return true;
	}

	void setParent(Process* parent_proc) {
		if (!forking) {
			parent = parent_proc;
			forking = true;
		}
	}


	bool isForked() { return forked; }

	bool isForking() { return forking; }


	void setChild(Process* child_proc) {
		if (!forked) {
			child = child_proc;
			forked = true;
		}
	}



	int GetPID() { return PID; }
	int GetAT() { return AT; }
	int GetCT() {return CT; }
	int GetTRT() { return (TT - AT); }
	int GetWT() { return (TRT - CT); }

	bool setPID(int PID1)
	{
		if (!PID_set) {
			PID_set = true;
			this->PID = PID1;
			return true;
		}

		return false;
	}



	bool setAT(int AT1)
	{
		if (!AT_set) {
			AT_set = true;
			this->AT = AT1;
			return true;
		}

		return false;
	}

	bool setIO(int N1)
	{
		if (!N_set) {
			N_set = true;
			this->N = N1;
			if (N > 0) {

			}
			return true;
		}

		return false;
	}

	/*bool setIO_RD(int IOR, int IOD) {
		if (N_set && (IO_R.getCount() <= N) && (IO_D.getCount() <= N)) {
			IO_R.enqueue(IOR);
			IO_D.enqueue(IOD);
			return true;
		}

		return false;
	}*/

	bool setRT(int time)
	{
		if (!RT_set) {
			RT_set = true;
			this->RT = time;
			return true;
		}

		return false;
	}

	void setTT(int time)
	{
		TT = time;
	}

	bool setReq(int Req)
	{
		if (!N_set) {
			N_set = true;
			this->N = Req;
			return true;
		}

		return false;
	}

	int getReq()
	{
		return IO_R_D.getCount();
	}

	ProcessState getState() { return state; }
	
	bool setState(ProcessState state) {
		if ((this->state == NEW) && (state != RDY)) {
			return false;
		}
		else if ((this->state == RDY) && (state != RUN)) {
			return false;
		}
		else if ((this->state == BLK) && (state != RDY)) {
			return false;
		}
		else if ((this->state == RUN) && (state == NEW)) {
			return false;
		}
		this->state = state;
		return true;
	}

	bool setCT(int CT1)
	{
		if (!CT_set) {
			CT_set = true;
			this->CT = CT1;
			return true;
		}

		return false;
	}

	/*int getTimeIO_Requests()
	{
		return IO_REQ;
	}*/


	void setRemainingT() { rm_time = GetCT();}

	int getRemainingT(){return rm_time;}

	void Dec_RemT()
	{
		if (rm_time >= 0) {
			rm_time--;
		}
		if (rm_time < 0) {

			rm_time = 0;
		}
	}

	Process* getParent() {
		return parent;
	}

	bool operator<=(const Process& proc2) {
		return (this->TT <= proc2.TT);
	}
	

};


