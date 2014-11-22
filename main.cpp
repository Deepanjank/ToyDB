#include <iostream>

#include "snapshot.hpp"

using namespace std;

int main() {
	snapshot ss("snapshot");
	workItem item;
	char buf[4096];
	cout<<sizeof(char);
	for (int i = 0; i < 4096; ++i)
	{
		buf[i] = '0' + (i%10);
		printf("%c", buf[i]);
	}

	item.buffer = buf;
	while(1) {
		cin>>item.type>>item.operationKind>>item.pageNumber;
		ss.processItem(item);
	}
	return 0;
}
