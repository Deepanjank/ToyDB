#include<queue>
#include "workload.hpp"
using namespace std;
#ifndef RAID01_HPP
#define RAID01_HPP
class raid01{
	int n_disk;
	queue<workItem> myqueue;
	bool* available;
	int seek_num;
	int write_num;
	int read_num;
	public:
	raid01(int n);
	void add_workItem(workItem w);
	void execute_workItem(workItem w);
}
#endif
