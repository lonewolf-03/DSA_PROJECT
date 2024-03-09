#include <iostream>
#include <iostream>
#include <cstdlib> 
#include <ctime> 

#include "../UILayer/UI.h"
#include "../Processor/Processor.h"

using namespace std;

int randomNumber(int s, int e) {
	int x = rand() % (e - s + 1) + s;
	return x;
}

void createInputFile(string filename, int sjf, int fcfs, int rr, int processNum, int rrTimeSlice, int rtf, int maxW, int stl, int fp, int ct_limit, int n_limit) {
	fstream input_file;
	input_file.open(filename, ios::out);
	input_file << fcfs << "		" << sjf << "		" << rr << '\n';
	input_file << rrTimeSlice << '\n';
	input_file << rtf << " " << maxW << " " << stl << " " << fp << '\n';
	input_file << processNum << '\n';
	int at = randomNumber(1, 5);
	int prev_at = at;
	int ct;
	int n;
	for (int i = 0; i < processNum; i++) {
		at = randomNumber(prev_at, prev_at + 2);
		prev_at = at;

		ct = randomNumber(1, ct_limit);
		n = randomNumber(0, n_limit);
		input_file << at << " " << i + 1 << " " << ct << " " << n << " ";
		int io_r, prev_io_r, io_d;
		io_r = randomNumber(at, at + 2);
		prev_io_r = io_r;
		for (int j = 0; j < n - 1; j++) {
			io_r = randomNumber(prev_io_r, prev_io_r + 10);
			prev_io_r = io_r;
			int temp = (((io_r - prev_io_r) - 1) > 1) ? ((io_r - prev_io_r) - 1) : 1;
			io_d = randomNumber(1, temp);
			input_file << "(" << io_r << "," << io_d << "), ";
		}
		io_r = randomNumber(prev_io_r, prev_io_r + 50);
		prev_io_r = io_r;
		int temp = (((io_r - prev_io_r) - 1) > 1) ? ((io_r - prev_io_r) - 1) : 1;
		io_d = randomNumber(1, temp);
		input_file << "(" << io_r << "," << io_d << ")\n";
	}
}


int main() {
	srand(time(0));


	
	Scheduler* s = new Scheduler();
	UI u(s,"file2.txt", "Output_special.txt");
	u.Start(1);

	return 0; 



}