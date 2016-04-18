#include "DbFileManager.h"
#include "FileUtil.h"


//d�������ݿ�Ĭ�ϴ洢·��
DbFileManager::DbFileManager() :dbName("d:/")
{
}

int DbFileManager::CreateDBFile(string dbName, int fileSize, int blockSize)
{
	//�����ݿ�洢��Ĭ��·����
	this->dbName += dbName;
	this->dbName += ".txt";
	this->blockSize = blockSize;
	//������С�����ļ���С�����������򱨴�
	if (fileSize % blockSize != 0) {
		return -1;
	}
	//���Դ洢��ҳ������
	blockAccount = fileSize / blockSize;
	util.CreateEmptyFile(this->dbName.c_str(), fileSize);
	PageHeader ph;
	ph.pd_lower = sizeof(PageHeader);
	ph.pd_upper = blockSize;
	//��ҳͷ��Ϣ���δ洢��ҳͷ��
	for (int pos = 0; pos < fileSize; pos += blockSize) {
		util.WriteToFile(this->dbName.c_str(), pos, &ph, sizeof(PageHeader));
	}
	return 1;
}

int DbFileManager::InsertData(void * dataBuffer, int dataLen)
{
	int curBlockNum = 0;
	//��õ�ǰ���ҳͷ��Ϣ
	PageHeader * currentPageData = (PageHeader *)util.ReadFromFile(this->dbName.c_str(), curBlockNum * blockSize, sizeof(PageHeader));
	//������Ѿ���������洢����һ������
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
	//������д��β��
	record.offset = currentPageData->pd_upper - dataLen;
	record.length = dataLen;
	//��¼��ǰ����д
	util.WriteToFile(this->dbName.c_str(), curBlockNum * blockSize + currentPageData->pd_lower, &record, sizeof(Record));
	//���ݴӺ���ǰд
	util.WriteToFile(this->dbName.c_str(), curBlockNum * blockSize + record.offset, dataBuffer, dataLen);
	//����PageHeader
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
