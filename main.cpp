#include <iostream>

#include "snapshot.hpp"
#include "defs.hpp"

using namespace std;

void exec()
{
	char filename[] = "snapshot";
	snapshot ss(filename);
	ss.enable = false;
	workItem item;
	char buf[4096];
	int N;
	item.buffer = buf;
	cin>>N;
	for(int i=0; i<N; i++) {
		cin>>item.type>>item.operationKind>>item.pageNumber>>item.timestamp;
		ss.processItem(item);
	}
}

int main() {
	exec();
	cout<<"RAID system : "<<seek_num<<" seeks "<<read_num<<" reads "<<write_num<<" writes"<<endl;
	cout<<"Snapshot : "<<seek_num2<<" seeks "<<read_num2<<" reads "<<write_num2<<" writes"<<endl;
	cout<<"Time : "<<ttime<<endl;
	return 0;
}
