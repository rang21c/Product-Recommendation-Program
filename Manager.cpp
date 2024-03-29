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
			char* check = strtok(NULL, " ");//over parameter
			flog << "========== " << tmp << " ==========" << endl;
			cout << "========== " << tmp << " ==========" << endl;
			if (check != NULL)
				printErrorCode(100);
			else if (LOAD())
				printSuccessCode();//LOAD success
			else
				printErrorCode(100);//LOAD fail
		}
		else if (strcmp(tmp, "BTLOAD") == 0)
		{
			char* check = strtok(NULL, " ");//over parameter
			flog << "========== " << tmp << " ==========" << endl;
			cout << "========== " << tmp << " ==========" << endl;
			if (check != NULL)
				printErrorCode(200);
			else if (BTLOAD())
				printSuccessCode();//BTLOAD success
			else
				printErrorCode(200);//BTLOAD fail
		}
		else if (strcmp(tmp, "PRINT_ITEMLIST") == 0)
		{
			char* check = strtok(NULL, " ");//over parameter
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (check != NULL)
				printErrorCode(300);
			else if (PRINT_ITEMLIST())
				continue;//PRINT_ITEMLIST success
			else
				printErrorCode(300);//PRINT_ITEMLIST fail
		}
		else if (strcmp(tmp, "PRINT_FPTREE") == 0)
		{
			char* check = strtok(NULL, " ");//over parameter
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (check != NULL)
				printErrorCode(400);
			else if (PRINT_FPTREE())
				continue;//PRINT_FPTREE success
			else
				printErrorCode(400);//PRINT_FPTREE fail
		}
		else if (strcmp(tmp, "PRINT_MIN") == 0)
		{
			char* item = strtok(NULL, " ");//word cutting
			char* min_frequency = strtok(NULL, " ");//word cutting
			char* check = strtok(NULL, " ");//over parameter
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (item == NULL || min_frequency == NULL || check != NULL || bptree->getRoot() == NULL)
				printErrorCode(500);
			else if (PRINT_MIN(item, atoi(min_frequency)))
				continue;//PRINT_MIN success
			else
				printErrorCode(500);//PRINT_MIN fail
		}
		else if (strcmp(tmp, "PRINT_CONFIDENCE") == 0)
		{
			char* item = strtok(NULL, " ");//word cutting
			char* confidence = strtok(NULL, " ");//word cutting
			char* check = strtok(NULL, " ");//over parameter
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (item == NULL || confidence == NULL || check != NULL || bptree->getRoot() == NULL)
				printErrorCode(600);
			else if (PRINT_CONFIDENCE(item, atof(confidence)))
				continue;//PRINT_CONFIDENCE success
			else
				printErrorCode(600);//PRINT_CONFIDENCE fail
		}
		else if (strcmp(tmp, "PRINT_RANGE") == 0)
		{
			char* item = strtok(NULL, " ");//word cutting
			char* min_frequency = strtok(NULL, " ");//word cutting
			char* max_frequency = strtok(NULL, " ");//word cutting
			char* check = strtok(NULL, " ");//over parameter
			flog << "====== " << tmp << " ======" << endl;
			cout << "====== " << tmp << " ======" << endl;
			if (item == NULL || min_frequency == NULL || max_frequency == NULL || check != NULL || bptree->getRoot() == NULL)
				printErrorCode(700);
			else if (PRINT_RANGE(item, atoi(min_frequency), atoi(max_frequency)))
				continue;//PRINT_RANGE success
			else
				printErrorCode(700);//PRINT_RANGE fail
		}
		else if (strcmp(tmp, "SAVE") == 0)
		{
			char* check = strtok(NULL, " ");//over parameter
			flog << "========== " << tmp << " ==========" << endl;
			cout << "========== " << tmp << " ==========" << endl;
			if (check != NULL)
				printErrorCode(800);//SAVE fail
			//else if (SAVE())
				//printSuccessCode();//SAVE success
			else
				printErrorCode(800);//SAVE fail
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
	if (!temp || !fpgrowth->getHeaderTable()->getdataTable().empty())//file open fail || already inserted
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
				{//item_array selection sort by name if same frequency
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
			fp.insert(name);//make set
		}
		bptree->Insert(atoi(frequency), fp);//insert bptree
	}
	temp.close();
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
		return false;//Frequent Pattern not exist || bptree is empty
	return true;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) 
{
	if (!bptree->printConfidence(item, rate, fpgrowth->item_frequency(item)) || bptree->getRoot() == NULL)
		return false;//Frequent Pattern not exist || bptree is empty
	return true;
}

bool Manager::PRINT_RANGE(char* item, int start, int end) 
{
	if (!bptree->printRange(item, start, end) || bptree->getRoot() == NULL)
		return false;//Frequent Pattern not exist || bptree is empty
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

