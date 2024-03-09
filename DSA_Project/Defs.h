#pragma once


enum ProcessState {
	NEW, RDY, RUN, BLK, TERM, ORPH
};

enum ProcessType {
	USR, SYS
};

enum ProcessorState {
	BUSY,
	IDLE,
};

enum ProcessorType {
	FCFS,
	SJF,
	RR,
};