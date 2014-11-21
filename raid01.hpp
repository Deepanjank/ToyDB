#include<queue>
#include "workload.hpp"
using namespace std;
#ifndef _RAID01_HPP
#define _RAID01_HPP
class raid01{
	queue<work_item> myqueue;
	int seek_num;
	int write_num;
	int read_num;
	public:
	void add_work_item(work_item w);
	void execute_work_item(work_item w);
}
