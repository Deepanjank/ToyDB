#include <iostream>

#include "snapshot.hpp"
#include "pflayer/pf.h"

using namespace std;

snapshot::snapshot(char *file) {
	strcpy(fileName, file);
	PF_CreateFile(fileName);
}

void snapshot::processItem(workItem item) {
	if(item.type) {
		pageNumbers::iterator iter = pageNumbers.find(item.pageNumber)
		if(iter != pageNumbers.end()) {
			char *page;
			int fd = PF_OpenFile(fileName);
			PF_GetThisPage(fd, iter->second, &page);
			memcpy(item.buffer, page, PF_PAGE_SIZE);
			PF_UnfixPage(fd, iter->second, FALSE);
			PF_CloseFile(fd);
		} else {
			// TODO - Forward to RAID
		}
	} else if(item.operationKind) {
		pageNumbers::iterator iter = pageNumbers.find(item.pageNumber)
		if(iter == pageNumbers.end()) {
			workItem readItem;
			char buf[PF_PAGE_SIZE];

			readItem.pageNumber = item.pageNumber;
			readItem.operationKind = READ;
			readItem.buffer = buf;

			// TODO - Forward to RAID

			int pno;
			char *page;
			int fd = PF_OpenFile(fileName);
			PF_AllocPage(fd, &pno, &page);
			memcpy(page, buf, PF_PAGE_SIZE);
			pageNumbers[item.pageNumber] = pno;
			PF_UnfixPage(fd, pno, TRUE);
			PF_CloseFile(fd);
		}
		// TODO - Forward to RAID
	} else {
		// TODO - Forward to RAID
	}
}