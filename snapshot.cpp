#include <iostream>

#include "snapshot.hpp"
#include "pflayer/pf.h"

using namespace std;

snapshot::snapshot(char *file) : raidSystem(NUMBER_OF_DISKS) {
	strcpy(fileName, file);
	PF_CreateFile(fileName);
	cout<<"Snapshot created at "<<fileName<<endl;
}

void snapshot::processItem(workItem item) {
	if(item.type) {
		map<int, int>::iterator iter = pageNumbers.find(item.pageNumber);
		if(iter != pageNumbers.end()) {
			char *page;
			int fd = PF_OpenFile(fileName);
			PF_GetThisPage(fd, iter->second, &page);
			memcpy(item.buffer, page, PF_PAGE_SIZE);
			PF_UnfixPage(fd, iter->second, FALSE);
			PF_CloseFile(fd);
		} else {
			raidSystem.execute_workItem(item);
		}
	} else if(item.operationKind) {
		map<int, int>::iterator iter = pageNumbers.find(item.pageNumber);
		if(iter == pageNumbers.end()) {
			workItem readItem;
			char buf[PF_PAGE_SIZE];

			readItem.pageNumber = item.pageNumber;
			readItem.operationKind = READ;
			readItem.buffer = buf;

			raidSystem.execute_workItem(readItem);

			int pno;
			char *page;
			int fd = PF_OpenFile(fileName);
			PF_AllocPage(fd, &pno, &page);
			memcpy(page, buf, PF_PAGE_SIZE);
			pageNumbers[item.pageNumber] = pno;
			PF_UnfixPage(fd, pno, TRUE);
			PF_CloseFile(fd);
		}
		raidSystem.execute_workItem(item);
	} else {
		raidSystem.execute_workItem(item);
	}
}