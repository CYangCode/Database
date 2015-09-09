#pragma once
#include <string>
using namespace std;

class DbFileManager
{
public:
	int CreateDBFile(string dbName, int fileSize, int blockSize);
	int InsertSlot(void * dataBuffer, int dataLen);
	//�Ӳ���ΪslotName���ļ����ҵ���pageNumber�����ݵ���Ϣ
	void * ReadData(int blockNumber, int pageNumber);
private:
	string dbName;
	FileUtil util;
};
