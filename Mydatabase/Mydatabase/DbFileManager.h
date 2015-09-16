#pragma once
#include <string>
#include "FileUtil.h"
using namespace std;

class DbFileManager
{
public:
	DbFileManager();
	//�������ݿ���ΪdbName�����ݿ��ļ����ļ��ߴ�ΪfileSize������blockSizeΪ��λ��Ĵ�С
	int CreateDBFile(string dbName, int fileSize, int blockSize);
	//������ΪdataLen������dataBuffer�洢��curBlockNum��ָ��Ŀ��У�������ʱ������һ��λ�ã���������ļ��Ѿ���ʱ������
	int InsertData(void * dataBuffer, int dataLen);
	//�ӵ�blockNumber�Ŀ��л�õ�slotNumber�Ĳ۵����ݣ�blockNumber��slotNumber���Ǵ�0��ʼ
	void * ReadData(int blockNumber, int slotNumber);
private:
	string dbName; //���ݿ���
	int blockSize; //��Ĵ�С
	FileUtil util;
	int curBlockNum; //��ǰ���ڲ����Ŀ��λ��
	int blockAccount; //���������
};
