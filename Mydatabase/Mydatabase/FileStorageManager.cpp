#include "FileStorageManager.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>


FileStorageManager::FileStorageManager()
{
}

FileStorageManager::~FileStorageManager()
{
}

int FileStorageManager::CreateEmptyFile(TCHAR * pFileName, int size)
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

void * FileStorageManager::ReadFromFile(char * fileName, long position, int length)
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

void FileStorageManager::WriteToFile(char * fileName, long position, void * dataBuffer, DWORD strLen)
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
