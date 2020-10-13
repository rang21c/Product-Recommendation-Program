#pragma once
#include "FPGrowth.h"
#include "BpTree.h"

class Manager
{
private:
	char* cmd;
	FPGrowth* fpgrowth;
	BpTree* bptree;
public:
	Manager(int threshold, int bpOrder)	//constructor
	{
		cmd = NULL;
		fpgrowth = new FPGrowth(&flog, threshold);
		bptree = new BpTree(&flog, bpOrder);
	}
	~Manager()//destructor
	{
		cmd = NULL;
		delete fpgrowth;
		delete bptree;
	}

	ifstream fin;
	ofstream flog;

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	bool PRINT_RANGE(char* item, int start, int end);
	bool PRINT_CONFIDENCE(char* item, double rate);
	bool PRINT_MIN(char* item, int min_frequency);


	void printErrorCode(int n);
	void printSuccessCode();
};

