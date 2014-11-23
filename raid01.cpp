#include "raid01.hpp"
#include "pflayer/pf.h"
#include "defs.hpp"

#include <iostream>
#include <cstdio>

using namespace std;

bool first=true;

raid01::raid01(int n) {
	n_disk = n;
	available = (bool *)malloc(2 * n * sizeof(bool));
	previous = (int *)malloc(2 * n * sizeof(int));
	PF_Init();

	for(int i=0; i<2*n; i++) {
		available[i] = true;
		previous[i] = 0;
		char fileName[10] = "disk";
		char i_string[5];
		sprintf(i_string, "%d", i);
		strcat(fileName, i_string);

		PF_DestroyFile(fileName);
		PF_CreateFile(fileName);
		int fd = PF_OpenFile(fileName);
		for(int j=0; j<DISK_CAPACITY; j++) {
			int pno;
			char *page;
			PF_AllocPage(fd, &pno, &page);
			PF_UnfixPage(fd, pno, TRUE);
		}
		PF_CloseFile(fd);
	}
	seek_num = 0;
	write_num = 0;
	read_num = 0;
	ttime=0;
}

raid01::~raid01() {
	free(available);
	free(previous);

	if(!myqueue.empty()) {
		seek_num++;
		ttime++;
	}
}

void raid01::execute_workItem(workItem w) {
	int disk_num = w.pageNumber % n_disk;
	int page_num = w.pageNumber / n_disk;

	char fileName[10] = "disk";
	char i_string[5];
	sprintf(i_string, "%d", disk_num);
	strcat(fileName, i_string);

	char fileName2[10] = "disk";
	char i_string2[5];
	sprintf(i_string2, "%d", disk_num + n_disk);
	strcat(fileName2, i_string2);

	if(!w.operationKind) {
		read_num += 1;

		char *page;
		int fd = PF_OpenFile(fileName);
		PF_GetThisPage(fd, page_num, &page);
		memcpy(w.buffer, page, PF_PAGE_SIZE);
		PF_UnfixPage(fd, page_num, FALSE);
		PF_CloseFile(fd);
	} else {
		write_num += 2;

		char *page;
		int fd = PF_OpenFile(fileName);
		cout<<"WGTP1"<<PF_GetThisPage(fd, page_num, &page);
		memcpy(page, w.buffer, PF_PAGE_SIZE);
		PF_UnfixPage(fd, page_num, TRUE);
		PF_CloseFile(fd);

		int fd2 = PF_OpenFile(fileName2);
		cout<<"WGTP2"<<PF_GetThisPage(fd2, page_num, &page);
		memcpy(page, w.buffer, PF_PAGE_SIZE);
		PF_UnfixPage(fd2, page_num, TRUE);
		PF_CloseFile(fd2);
	}
	cout<<"yo";
}

void raid01::add_workItem(workItem w) {
	execute_workItem(w);

	int disk_num = w.pageNumber % n_disk;
	int page_num = w.pageNumber / n_disk;
	if(!w.operationKind) {
		if(w.timestamp > ttime) {
			while (!myqueue.empty())
				myqueue.pop();
			for (int i = 0; i < 2*n_disk; ++i)
				available[i] = true;

			seek_num += 1;
			ttime = w.timestamp;
			myqueue.push(w);
			available[disk_num] = false;
			previous[disk_num] = page_num;

			return;
		}

		if(previous[disk_num]+1 == page_num) {
			previous[disk_num] = page_num;
			return;
		}
		else if(previous[disk_num+n_disk]+1 == page_num){
			previous[disk_num+n_disk] = page_num;
			return;
		}

		if(available[disk_num]) {
			myqueue.push(w);
			available[disk_num]=false;
			previous[disk_num] = page_num;
		} else if(available[disk_num+n_disk]) {
			myqueue.push(w);
			available[disk_num+n_disk]=false;
			previous[disk_num+n_disk]=page_num;
		} else {
			while (!myqueue.empty())
				myqueue.pop();
			for (int i = 0; i < 2*n_disk; ++i)
				available[i] = true;

			seek_num += 1;
			ttime += 1;
			myqueue.push(w);
			available[disk_num] = false;
			previous[disk_num] = page_num;
		}
	} else {
		if(w.timestamp > ttime) {
			while (!myqueue.empty())
				myqueue.pop();
			for (int i = 0; i < 2*n_disk; ++i)
				available[i] = true;

			seek_num += 1;
			myqueue.push(w);
			ttime = w.timestamp;
			available[disk_num] = available[disk_num+n_disk] = false;
			previous[disk_num+n_disk] = page_num;
			previous[disk_num] = page_num;

			return;
		}

		if(previous[disk_num]+1 == page_num && previous[disk_num+n_disk]+1 == page_num) {
			previous[disk_num+n_disk] = page_num;
			previous[disk_num] = page_num;
			return;
		}

		if(available[disk_num] && available[disk_num+n_disk]) {
			myqueue.push(w);
			available[w.pageNumber]=false;
			available[w.pageNumber+n_disk]=false;
			previous[disk_num] = page_num;
			previous[disk_num+n_disk] = page_num;
		} else {
			while (!myqueue.empty())
				myqueue.pop();
			for (int i = 0; i < 2*n_disk; ++i)
				available[i] = true;

			seek_num += 1;
			ttime += 1;
			myqueue.push(w);
			available[disk_num] = available[disk_num+n_disk] = false;
			previous[disk_num+n_disk] = page_num;
			previous[disk_num] = page_num;
		}
	}
}
