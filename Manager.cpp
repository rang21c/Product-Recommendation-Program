#include "Manager.h"

void Manager::run(const char* command)
{
	fin.open(command);
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}
	while (!fin.eof())
	{
		/* You must fill here */
	
	fin.close();
	return;
}

bool Manager::LOAD()
{
	return true;
}




bool Manager::BTLOAD()
{
	
	return true;
}

bool Manager::PRINT_ITEMLIST(char* item, int start, int end) {
	
}

bool Manager::PRINT_FPTREE(char* item, int start, int end) {
	
}

bool Manager::PRINT_MIN(char* item, int min_frequency) {
	
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "======== ERROR " << n << " ========" << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

