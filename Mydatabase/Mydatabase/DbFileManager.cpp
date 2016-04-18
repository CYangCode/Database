#include "DbFileManager.h"
#include "FileUtil.h"


//d盘是数据库默认存储路径
DbFileManager::DbFileManager() :dbName("d:/")
{
}

int DbFileManager::CreateDBFile(string dbName, int fileSize, int blockSize)
{
	//将数据库存储于默认路径下
	this->dbName += dbName;
	this->dbName += ".txt";
	this->blockSize = blockSize;
	//如果块大小不是文件大小的整数倍，则报错
	if (fileSize % blockSize != 0) {
		return -1;
	}
	//可以存储的页的数量
	blockAccount = fileSize / blockSize;
	util.CreateEmptyFile(this->dbName.c_str(), fileSize);
	PageHeader ph;
	ph.pd_lower = sizeof(PageHeader);
	ph.pd_upper = blockSize;
	//将页头信息依次存储于页头中
	for (int pos = 0; pos < fileSize; pos += blockSize) {
		util.WriteToFile(this->dbName.c_str(), pos, &ph, sizeof(PageHeader));
	}
	return 1;
}

int DbFileManager::InsertData(void * dataBuffer, int dataLen)
{
	int curBlockNum = 0;
	//获得当前块的页头信息
	PageHeader * currentPageData = (PageHeader *)util.ReadFromFile(this->dbName.c_str(), curBlockNum * blockSize, sizeof(PageHeader));
	//如果块已经存满，则存储于下一个块中
	int lower = currentPageData->pd_lower + sizeof(Record);
	int upper = currentPageData->pd_upper - dataLen;
	while (lower > upper) {
		curBlockNum++;
		if (curBlockNum > blockAccount) {
			return -1;
		}
		currentPageData = (PageHeader *)util.ReadFromFile(this->dbName.c_str(), curBlockNum * blockSize, sizeof(PageHeader));
		lower = currentPageData->pd_lower + sizeof(Record);
		upper = currentPageData->pd_upper - dataLen;
	}
	Record record;
	//将数据写到尾部
	record.offset = currentPageData->pd_upper - dataLen;
	record.length = dataLen;
	//记录从前往后写
	util.WriteToFile(this->dbName.c_str(), curBlockNum * blockSize + currentPageData->pd_lower, &record, sizeof(Record));
	//数据从后往前写
	util.WriteToFile(this->dbName.c_str(), curBlockNum * blockSize + record.offset, dataBuffer, dataLen);
	//更新PageHeader
	currentPageData->pd_lower += sizeof(Record);
	currentPageData->pd_upper -= dataLen;
	util.WriteToFile(this->dbName.c_str(), curBlockNum * blockSize, currentPageData, sizeof(PageHeader));
	delete currentPageData;
	return 1;
}

void * DbFileManager::ReadData(int blockNumber, int slotNumber)
{
	int position = blockNumber * blockSize + sizeof(PageHeader)+ slotNumber * sizeof(Record);
	Record * record = (Record *)util.ReadFromFile(dbName.c_str(), position, sizeof(Record));
	return util.ReadFromFile(dbName.c_str(), record->offset, record->length);
}
