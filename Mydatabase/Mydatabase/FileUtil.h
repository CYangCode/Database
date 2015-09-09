#pragma once
#include <tchar.h>
#include <windows.h>
//����Ҳҳͷ����Ϣ��ͨ��ҳͷ�ҵ��洢���ݵ�λ��
struct PageHeader
{
	int pd_lower; //�ļ��п������ݵĿ�ʼλ��
	int pd_upper; //�ļ��п������ݵĽ���λ��
};
//��¼�洢���ݵ�λ��
struct Record
{
	int offset; //ƫ����
	int length; //���ݳ���
};
class FileUtil
{
public:
	FileUtil();
	~FileUtil();
	//����ָ����С���ļ����ļ���ΪfileName�� ��СΪsize
	int CreateEmptyFile(const char * fileName, int size);
	//��ȡָ��λ�õ��ļ�����,�ļ�ΪpFile����ʼλ��Ϊposition������Ϊlength������ֵ���Ը��ݾ����������ǿ��ת��
	void * ReadFromFile(const char * fileName, long position, int length);
	//������д��ָ��λ��,�ļ���ΪfileName����ʼλ��Ϊposition��д������ΪDataBuffer
	void WriteToFile(const char * fileName, long position, void * dataBuffer, DWORD strLen);
	//������ΪslotName�Ĳ��в���dataBuffer
	int InsertSlot(const  char * slotName, void * dataBuffer, int dataLen);
	//�Ӳ���ΪslotName���ļ����ҵ���pageNumber�����ݵ���Ϣ
	void * ReadData(const  char * slotName, int pageNumber);
};

