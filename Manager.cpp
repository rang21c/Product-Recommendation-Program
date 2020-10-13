#include "Manager.h"
#include <string>

void Manager::run(const char* command)
{
	fin.open(command);//read file open
	flog.open("log.txt", ios::app);//write file open
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}
	char cmd[100];
	while (!fin.eof())
	{
		fin.getline(cmd, 100);//read line
		char* tmp = strtok(cmd, " ");//word cutting
		if (strcmp(tmp, "LOAD") == 0)
		{
			flog << "========== " << tmp << " ==========" << endl;
			cout << "========== " << tmp << " ==========" << endl;
			if (LOAD())
				printSuccessCode();//LOAD success
			else
				printErrorCode(100);//LOAD fail
		}
		else if (strcmp(tmp, "BTLOAD") == 0)
		{
			flog << "========== " << tmp << " ==========" << endl;
			cout << "========== " << tmp << " ==========" << endl;
			if (BTLOAD())
				printSuccessCode();//BTLOAD success
			else
				printErrorCode(200);//BTLOAD fail
		}
		else if (strcmp(tmp, "PRINT_ITEMLIST") == 0)
		{
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (PRINT_ITEMLIST())
				continue;//PRINT_ITEMLIST success
			else
				printErrorCode(300);//PRINT_ITEMLIST fail
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	fstream temp;
	//market.txt
	temp.open("testcase1.txt");//read file open
	if (!temp || !fpgrowth->getHeaderTable()->getdataTable().empty() || fpgrowth->getTree()->getNext())//file open fail || queue is not empty
		return false;//LOAD fail
	string cmd;
	while (!temp.eof())
	{
		getline(temp, cmd);//read line
		char* name = strtok((char*)cmd.c_str(), "\t");
		if (name == NULL)
			break;
		while (1)
		{
			fpgrowth->createTable(name, 1);
			name = strtok(NULL, "\t");
			if (name == NULL)
				break;
		}
	}
	fpgrowth->getHeaderTable()->descendingIndexTable();
	temp.close();
	return true;//LOAD success
}

bool Manager::BTLOAD()
{
	
	return true;
}

bool Manager::PRINT_ITEMLIST()
{
	if(!fpgrowth->printList())
		return false;//headertable is empty
	return true;
}

bool Manager::PRINT_FPTREE()
{
	return false;
}

//bool Manager::PRINT_ITEMLIST(char* item, int start, int end) 
//{
//	
//}
//
//bool Manager::PRINT_FPTREE(char* item, int start, int end) 
//{
//	
//}

bool Manager::PRINT_MIN(char* item, int min_frequency) 
{
	return true;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) 
{
	return true;
}

bool Manager::PRINT_RANGE(char* item, int start, int end) 
{
	return true;
}

void Manager::printErrorCode(int n) 
{//ERROR CODE PRINT
	flog << "======== ERROR " << n << " ========" << endl;
	flog << "=======================" << endl << endl;
	cout << "======== ERROR " << n << " ========" << endl;
	cout << "=======================" << endl << endl;
}

void Manager::printSuccessCode() 
{//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
	cout << "Success" << endl;
	cout << "=======================" << endl << endl;
}

