#pragma once
#include <tchar.h>
#include <windows.h>

class FileStorageManager
{
public:
	FileStorageManager();
	~FileStorageManager();
	//����ָ����С���ļ����ļ���ΪfileName�� ��СΪsize
	int CreateEmptyFile(TCHAR * fileName, int size);
	//��ȡָ��λ�õ��ļ�����,�ļ�ΪpFile����ʼλ��Ϊposition������Ϊlength������ֵ���Ը��ݾ����������ǿ��ת��
	void * ReadFromFile(char * fileName, long position, int length);
	//������д��ָ��λ��,�ļ���ΪfileName����ʼλ��Ϊposition��д������ΪDataBuffer
	void WriteToFile(char * fileName, long position, void * dataBuffer, DWORD strLen);
};

