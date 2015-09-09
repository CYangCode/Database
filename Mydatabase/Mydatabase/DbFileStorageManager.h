#pragma once
#include <string>
using namespace std;

class DbFileManager
{
public:
	int CreateDBFile(string dbName, int fileSize, int blockSize);
	int InsertSlot(void * dataBuffer, int dataLen);
	//从槽名为slotName的文件中找到第pageNumber个数据的信息
	void * ReadData(int blockNumber, int pageNumber);
private:
	string dbName;
	FileUtil util;
};
