#ifndef SNAPSHOT_HPP
#define SNAPSHOT_HPP

#include <map>

#include "workItem.hpp"

class snapshot {
	snapshot(char *file);
	void processItem(workItem item);
private:
	// TODO - Reference to RAID system
	char fileName[25];
	std::map<int, int> pageNumbers;
};

#endif // SNAPSHOT_HPP
