#include <iostream>
#include <cstdlib>
#include <cstring>

#include "snapshot.hpp"
#include "defs.hpp"
#include "pflayer/pf.h"

using namespace std;

snapshot::snapshot(char *file) : raidSystem(NUMBER_OF_DISKS) {
	strcpy(fileName, file);
	previous=0;
	PF_DestroyFile(fileName);
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

			read_num2 += 1;
			if(iter->second != previous + 1) {
				seek_num2 += 1;
			}
			previous = iter->second;
		} else {
			raidSystem.add_workItem(item);
		}
	} else if(item.operationKind) {
		map<int, int>::iterator iter = pageNumbers.find(item.pageNumber);
		if(iter == pageNumbers.end()) {
			workItem readItem;
			char buf[PF_PAGE_SIZE];

			readItem.pageNumber = item.pageNumber;
			readItem.operationKind = READ;
			readItem.buffer = buf;

			raidSystem.add_workItem(readItem);

			int pno;
			char *page;
			int fd = PF_OpenFile(fileName);
			PF_AllocPage(fd, &pno, &page);
			memcpy(page, buf, PF_PAGE_SIZE);
			pageNumbers[item.pageNumber] = pno;
			PF_UnfixPage(fd, pno, TRUE);
			PF_CloseFile(fd);

			write_num2 += 1;
			if(pno != previous + 1) {
				seek_num2 += 1;
			}
			previous = pno;
		}
		raidSystem.add_workItem(item);
	} else {
		raidSystem.add_workItem(item);
	}
}