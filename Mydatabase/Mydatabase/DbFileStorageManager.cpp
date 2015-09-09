#include "DbFileStorageManager.h"
#include "FileUtil.h"


int DbFileManager::CreateDBFile(string dbName, int fileSize, int blockSize)
{
	this->dbName = dbName;
	FileUtil util;
	//������С�����ļ���С�����������򱨴�
	if (fileSize % blockSize != 0) {
		return -1;
	}
	util.CreateEmptyFile(dbName.c_str(), fileSize);
	
}

int DbFileManager::InsertSlot(void * dataBuffer, int dataLen)
{
	PageHeader * ph = (PageHeader *)util.ReadFromFile(dbName.c_str(), 0, sizeof(PageHeader));
	//������Ѿ��������������
	if (ph->pd_lower + sizeof(Record) > ph->pd_lower - dataLen) {
		return -1;
	}
	Record record;
	//������д��β��
	record.offset = ph->pd_upper - dataLen;
	record.length = dataLen;
	//��¼��ǰ����д
	util.WriteToFile(dbName.c_str(), ph->pd_lower, &record, sizeof(Record));
	//���ݴӺ���ǰд
	util.WriteToFile(dbName.c_str(), record.offset, dataBuffer, dataLen);
	//����PageHeader
	ph->pd_lower += sizeof(Record);
	ph->pd_upper -= dataLen;
	util.WriteToFile(dbName.c_str(), 0, ph, sizeof(PageHeader));
	delete ph;
	return 1;
}

void * DbFileManager::ReadData(int blockNumber, int pageNumber)
{
	//����һ��pageHeader
	Record * record = (Record *)util.ReadFromFile(dbName.c_str(), sizeof(PageHeader)+(pageNumber - 1)* sizeof(Record), sizeof(Record));
	return util.ReadFromFile(dbName.c_str(), record->offset, record->length);
}
