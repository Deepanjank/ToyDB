#include <iostream>
using namespace std;
int main()
{
	int t=0;
	srand(0);
	cout<<200<<endl;
	for(int i=0;i<200;i++)
	{
		cout<<0<<" "<<rand()%2<<" "<<rand()%200<<" "<<t<<endl;
		if(i<200) cout<<1<<" "<<0<<" "<<i<<" "<<t<<endl;
		t+=(rand()%3 > 1);
	}
}