#include "FileUtil.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>


FileUtil::FileUtil()
{
}

FileUtil::~FileUtil()
{
}

int FileUtil::CreateEmptyFile(const char * pFileName, int size)
{
	HANDLE hFile;
	HANDLE hMapFile;
	hFile = CreateFile(
		pFileName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	//�����ʧ��
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�����ļ�ʧ��! (����  %d)/r/n", GetLastError());;
		return FALSE;
	}
	//�����ļ�ӳ�䣬ʹ�ļ����ָ���Ĵ�С
	hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READWRITE,
		0,
		size,
		NULL
		);
	//�������ʧ��
	if (hMapFile == NULL)
	{
		printf("�����ļ�ӳ��ʧ�� (���� %d) /r/n", GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	return TRUE;
}

void * FileUtil::ReadFromFile(const char * fileName, long position, int length)
{
	HANDLE hFile;
	DWORD dwBytesRead = 0;
	char * ReadBuffer = new char[length + 1];

	hFile = CreateFile(fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("���ܴ��ļ� (���� %d)\n", GetLastError());
		return NULL;
	}

	if (SetFilePointer(hFile, position, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		printf("���������ļ�ָ��λ�� (���� %d)\n", GetLastError());
		return NULL;
	}
	if (FALSE == ReadFile(hFile, ReadBuffer, length, &dwBytesRead, NULL))
	{
		printf("���ܶ��ļ� (���� %d)\n", GetLastError());
		CloseHandle(hFile);
		return NULL;
	}

	if (dwBytesRead > 0)
	{
		ReadBuffer[dwBytesRead] = '\0';

	}
	else
	{
		printf("û�д��ļ�%s�ж�������", fileName);
	}

	CloseHandle(hFile);

	return ReadBuffer;
}

void FileUtil::WriteToFile(const char * fileName, long position, void * dataBuffer, DWORD strLen)
{
	HANDLE hFile;
	DWORD dwBytesToWrite = strLen;
	DWORD dwBytesWritten = 0;
	hFile = CreateFile(fileName,                // д����ļ���
		GENERIC_WRITE,          // �򿪷�ʽΪд��ʽ
		0,                      // ����ʽΪ������
		NULL,                   // Ĭ�ϰ�ȫģʽ
		OPEN_ALWAYS,          // �ļ�������ʽ���򿪻����Ǵ���
		FILE_ATTRIBUTE_NORMAL,  // �����ļ�
		NULL);                  // no attr. template\
			//����ļ���ʧ��
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("���ܴ��ļ� (���� %d)\n", GetLastError());
		return;
	}

	if (SetFilePointer(hFile, position, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		printf("���������ļ�ָ��λ�� (���� %d)\n", GetLastError());
		return;
	}

	while (dwBytesWritten < dwBytesToWrite)
	{
		if (FALSE == WriteFile(hFile,           // ���ļ����
			dataBuffer,     // ��ʼ����д������λ��
			dwBytesToWrite - dwBytesWritten, // δд����ֽ���
			&dwBytesWritten, // �Ѿ�д����ֽ���
			NULL)            // no overlapped structure
			)
		{
			printf("����д���ļ� (���� %d)\n", GetLastError());
			CloseHandle(hFile);
			return;
		}
	}
	CloseHandle(hFile);
}

int FileUtil::InsertSlot(const char * slotName, void * dataBuffer, int dataLen)
{
	PageHeader * ph = (PageHeader *)ReadFromFile(slotName, 0, sizeof(PageHeader));
	//������Ѿ��������������
	if (ph->pd_lower + sizeof(Record) > ph->pd_lower - dataLen) {
		return -1;
	}
	Record record;
	//������д��β��
	record.offset = ph->pd_upper - dataLen;
	record.length = dataLen;
	//��¼��ǰ����д
	WriteToFile(slotName, ph->pd_lower, &record, sizeof(Record));
	//���ݴӺ���ǰд
	WriteToFile(slotName, record.offset, dataBuffer, dataLen);
	//����PageHeader
	ph->pd_lower += sizeof(Record);
	ph->pd_upper -= dataLen;
	WriteToFile(slotName, 0, ph, sizeof(PageHeader));
	delete ph;
	return 1;
}

void * FileUtil::ReadData(const char * slotName, int pageNumber)
{
	//����һ��pageHeader
	Record * record = (Record *)ReadFromFile(slotName, sizeof(PageHeader)+(pageNumber - 1)* sizeof(Record), sizeof(Record));
	return ReadFromFile(slotName, record->offset, record->length);
}
