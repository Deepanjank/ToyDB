#include <iostream>
using namespace std;
#define WORKLOAD 100
#define RATE 1
int main()
{
	int t=0;
	srand(0);
	cout<<300<<endl;
	for(int i=0;i<200;i++)
	{
		if(i<WORKLOAD) cout<<0<<" "<<rand()%2<<" "<<rand()%200<<" "<<t<<endl;
		if(i<200) cout<<1<<" "<<0<<" "<<i<<" "<<t<<endl;
		t+=(rand()%3 > 1)*5;
	}
}
