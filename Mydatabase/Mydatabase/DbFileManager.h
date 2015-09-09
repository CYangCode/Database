#pragma once
#include <string>
#include "FileUtil.h"
using namespace std;

class DbFileManager
{
public:
	DbFileManager();
	//创建数据库名为dbName的数据库文件，文件尺寸为fileSize，其中blockSize为单位块的大小
	int CreateDBFile(string dbName, int fileSize, int blockSize);
	//将长度为dataLen的数据dataBuffer存储于curBlockNum所指向的块中，当块满时放入下一块位置，如果整个文件已经满时带错返回
	int InsertData(void * dataBuffer, int dataLen);
	//从第blockNumber的块中获得第slotNumber的槽的内容，blockNumber和slotNumber都是从0开始
	void * ReadData(int blockNumber, int slotNumber);
private:
	string dbName; //数据库名
	int blockSize; //块的大小
	FileUtil util;
	int curBlockNum; //当前正在操作的块的位置
	int blockAccount; //块的总数量
};
