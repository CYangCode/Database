#include "DbFileStorageManager.h"
#include "FileUtil.h"


int DbFileManager::CreateDBFile(string dbName, int fileSize, int blockSize)
{
	this->dbName = dbName;
	FileUtil util;
	//如果块大小不是文件大小的整数倍，则报错
	if (fileSize % blockSize != 0) {
		return -1;
	}
	util.CreateEmptyFile(dbName.c_str(), fileSize);
	
}

int DbFileManager::InsertSlot(void * dataBuffer, int dataLen)
{
	PageHeader * ph = (PageHeader *)util.ReadFromFile(dbName.c_str(), 0, sizeof(PageHeader));
	//如果块已经存满，则带错返回
	if (ph->pd_lower + sizeof(Record) > ph->pd_lower - dataLen) {
		return -1;
	}
	Record record;
	//将数据写到尾部
	record.offset = ph->pd_upper - dataLen;
	record.length = dataLen;
	//记录从前往后写
	util.WriteToFile(dbName.c_str(), ph->pd_lower, &record, sizeof(Record));
	//数据从后往前写
	util.WriteToFile(dbName.c_str(), record.offset, dataBuffer, dataLen);
	//更新PageHeader
	ph->pd_lower += sizeof(Record);
	ph->pd_upper -= dataLen;
	util.WriteToFile(dbName.c_str(), 0, ph, sizeof(PageHeader));
	delete ph;
	return 1;
}

void * DbFileManager::ReadData(int blockNumber, int pageNumber)
{
	//读第一个pageHeader
	Record * record = (Record *)util.ReadFromFile(dbName.c_str(), sizeof(PageHeader)+(pageNumber - 1)* sizeof(Record), sizeof(Record));
	return util.ReadFromFile(dbName.c_str(), record->offset, record->length);
}
