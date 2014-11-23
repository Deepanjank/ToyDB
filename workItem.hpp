#ifndef WORKITEM_HPP
#define WORKITEM_HPP

#define BACKUP true
#define WORKLOAD false

#define WRITE true
#define READ false

struct workItem
{
	bool type;
	bool operationKind;
	int pageNumber;
	char *buffer;
	int timestamp;
};

#endif // WORKITEM_HPP
