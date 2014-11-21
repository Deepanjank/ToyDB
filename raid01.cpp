#include"workload.hpp"
#include"raid01.hpp"
#include"pflayer/pf.h"
#include<iostream>
using namespace std;
void raid01::raid01(int n)
{
	n_disk=n;
	available=new bool[2*n_disk];
	for(int i=0;i<=2*n;i++){
		char* fileName="disk";
		char* i_string=inttoString(i);
		strcat(fileName,i_string);
		PF_CreateFile(fileName);
		for(int j=0;j<30;j++){
			int fd = PF_OpenFile(fileName);
			int pno;
			char* page;
                	PF_AllocPage(fd, &pno, &page);
                	memcpy(page, buf, PF_PAGE_SIZE);
                	//pageNumbers[item.pageNumber] = pno;
                	PF_UnfixPage(fd, pno, TRUE);
                	PF_CloseFile(fd);
		}
	}
	seek_num=0;
	write_num=0;
	read_num=0;
}
void raid01::execute_workItem(workItem w){
	int disk_num=w.pageNumber%n_disk;
	int page_num=w.pageNumber/n_disk;
	char* fileName="disk";
        char* i_string=inttoString(disk_num);
        strcat(fileName,i_string);
	if(w.operationKind){
		read_num+=1;
		 char *page;
                 int fd = PF_OpenFile(fileName);
                 PF_GetThisPage(fd, page_num, &page);
                 memcpy(w.buffer, page, PF_PAGE_SIZE);
                 PF_UnfixPage(fd, page_num, FALSE);
                 PF_CloseFile(fd);	
	}	
	else{
		write_num+=2;
		
	}
}
void raid01::add_workItem(workItem w){
	execute_workItem(w);
	int disk_num=w.pageNumber%n_disk;
	if(w.operationKind){
		if(available[disk_num]){
			myqueue.push(w);
			available[disk_num]=false;
		}
		else if(available[disk_num+n_disk]){
			myqueue.push(w);
			available[disk_num+n_disk]=false;
		}
		else{
			 while (!myqueue.empty())
  			{
     				myqueue.pop();
  			}
			seek_num+=1;
			myqueue.push(w);
		}	
	else{
		if(available[w.pageNumber]){
                          myqueue.push(w);
                          available[w.pageNumber]=false;
                          available[w.pageNumber+n_disk]=false;
                  } 
                  else{
                          while (!myqueue.empty())
                          {
                                  myqueue.pop();
                          }       
                          seek_num+=1;
                          myqueue.push(w);
                  }
	}
}
