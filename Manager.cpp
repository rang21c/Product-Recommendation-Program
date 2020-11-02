#include "Manager.h"
#include <string>
#include <algorithm>

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
		if (tmp == NULL) break;
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
		else if (strcmp(tmp, "PRINT_FPTREE") == 0)
		{
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (PRINT_FPTREE())
				continue;//PRINT_ITEMLIST success
			else
				printErrorCode(400);//PRINT_ITEMLIST fail
		}
		else if (strcmp(tmp, "PRINT_MIN") == 0)
		{
			char* item = strtok(NULL, " ");//word cutting
			char* min_frequency = strtok(NULL, " ");//word cutting
			char* check = strtok(NULL, " ");
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (PRINT_MIN(item, atoi(min_frequency)) && check == NULL)
				continue;//PRINT_ITEMLIST success
			else
				printErrorCode(500);//PRINT_ITEMLIST fail
		}
		else if (strcmp(tmp, "PRINT_RANGE") == 0)
		{
			char* item = strtok(NULL, " ");//word cutting
			char* min_frequency = strtok(NULL, " ");//word cutting
			char* max_frequency = strtok(NULL, " ");//word cutting
			char* check = strtok(NULL, " ");
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (PRINT_RANGE(item, atoi(min_frequency), atoi(max_frequency)) && check == NULL)
				continue;//PRINT_ITEMLIST success
			else
				printErrorCode(700);//PRINT_ITEMLIST fail
		}
		else if (strcmp(tmp, "EXIT") == 0)
		{
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			printSuccessCode();
			return;
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	FPNode* root = this->fpgrowth->getTree();
	fstream temp;
	temp.open("market.txt");//read file open
	if (!temp || !fpgrowth->getHeaderTable()->getdataTable().empty() || fpgrowth->getTree()->getNext())//file open fail || already inserted
		return false;//LOAD fail
	string cmd;
	vector<list<string>> item_vector;//vector of item_array
	while (!temp.eof())
	{
		list<string> item_array;//list of items bought together
		getline(temp, cmd);//read line
		char* name = strtok((char*)cmd.c_str(), "\t");
		if (name == NULL)
			break;
		item_array.push_back(name);
		while (1)
		{
			fpgrowth->createTable(name, 1);//Make HeaderTable
			name = strtok(NULL, "\t");
			if (name == NULL)
				break;
			item_array.push_back(name);
		}
		item_vector.push_back(item_array);
	}
	fpgrowth->getHeaderTable()->descendingIndexTable();
	for (int i = 0; i < item_vector.size(); i++)
	{//Transaction order
		for (auto j = item_vector[i].begin(); j != item_vector[i].end(); j++)
		{
			for (auto k = item_vector[i].begin(); k != item_vector[i].end(); k++)
			{
				if (fpgrowth->item_frequency((*k)) < fpgrowth->item_frequency((*j)))
				{//item_array selection sort by frequency
					string temp = (*j);
					(*j) = (*k);
					(*k) = temp;
				}
			}
		}
		for (auto j = item_vector[i].begin(); j != item_vector[i].end(); j++)
		{
			for (auto k = item_vector[i].begin(); k != item_vector[i].end(); k++)
			{
				if (strcmp((*j).c_str(), (*k).c_str()) > 0 && fpgrowth->item_frequency((*k)) == fpgrowth->item_frequency((*j)))
				{//item_array selection sort by frequency if same frequency
					string temp = (*j);
					(*j) = (*k);
					(*k) = temp;
				}
			}
		}
	}
	for (int i = 0; i < item_vector.size(); i++)
	{//Make FPtree
		fpgrowth->createFPtree(root, fpgrowth->getHeaderTable(), item_vector[i], 1);//make FPtree//fpgrowth->item_frequency((*j))
	}
	temp.close();
	return true;//LOAD success
}

bool Manager::BTLOAD()
{
	fstream temp;
	temp.open("result.txt");//read file open
	if (!temp || bptree->getRoot())//file open fail || bptree is not empty
		return false;//BTLOAD fail
	string cmd;
	while (!temp.eof())
	{
		set<string> fp;
		getline(temp, cmd);//read line
		char* frequency = strtok((char*)cmd.c_str(), "\t");
		if (frequency == NULL)
			break;
		while (1)
		{
			char* name = strtok(NULL, "\t");
			if (name == NULL)
				break;
			fp.insert(name);
		}
		bptree->Insert(atoi(frequency), fp);
	}
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
	if (!fpgrowth->printTree())
		return false;//fptree empty
	return true;
}

bool Manager::PRINT_MIN(char* item, int min_frequency) 
{
	if (!bptree->printFrequency(item, min_frequency) || bptree->getRoot() == NULL)
		return false;
	return true;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) 
{

	return true;
}

bool Manager::PRINT_RANGE(char* item, int start, int end) 
{
	if (!bptree->printRange(item, start, end) || bptree->getRoot() == NULL)
		return false;
	return true;
}

void Manager::printErrorCode(int n) 
{//ERROR CODE PRINT
	flog << "======= ERROR " << n << " =======" << endl;
	flog << "==========================" << endl << endl;
	cout << "======= ERROR " << n << " =======" << endl;
	cout << "==========================" << endl << endl;
}

void Manager::printSuccessCode() 
{//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "==========================" << endl << endl;
	cout << "Success" << endl;
	cout << "==========================" << endl << endl;
}

