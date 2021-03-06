#ifndef SNAPSHOT_HPP
#define SNAPSHOT_HPP

#include <map>

#include "raid01.hpp"
#include "workItem.hpp"

#define NUMBER_OF_DISKS 4

class snapshot {
public:
	bool enable;
	snapshot(char *file);
	void processItem(workItem item);
private:
	int previous;
	raid01 raidSystem;
	char fileName[25];
	std::map<int, int> pageNumbers;
};

#endif // SNAPSHOT_HPP
