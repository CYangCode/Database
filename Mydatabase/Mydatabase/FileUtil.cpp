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
	//如果打开失败
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("创建文件失败! (错误  %d)/r/n", GetLastError());;
		return FALSE;
	}
	//创建文件映射，使文件变成指定的大小
	hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READWRITE,
		0,
		size,
		NULL
		);
	//如果创建失败
	if (hMapFile == NULL)
	{
		printf("创建文件映射失败 (错误 %d) /r/n", GetLastError());
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
		printf("不能打开文件 (错误 %d)\n", GetLastError());
		return NULL;
	}

	if (SetFilePointer(hFile, position, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		printf("不能设置文件指针位置 (错误 %d)\n", GetLastError());
		return NULL;
	}
	if (FALSE == ReadFile(hFile, ReadBuffer, length, &dwBytesRead, NULL))
	{
		printf("不能读文件 (错误 %d)\n", GetLastError());
		CloseHandle(hFile);
		return NULL;
	}

	if (dwBytesRead > 0)
	{
		ReadBuffer[dwBytesRead] = '\0';

	}
	else
	{
		printf("没有从文件%s中读到数据", fileName);
	}

	CloseHandle(hFile);

	return ReadBuffer;
}

void FileUtil::WriteToFile(const char * fileName, long position, void * dataBuffer, DWORD strLen)
{
	HANDLE hFile;
	DWORD dwBytesToWrite = strLen;
	DWORD dwBytesWritten = 0;
	hFile = CreateFile(fileName,                // 写入的文件名
		GENERIC_WRITE,          // 打开方式为写方式
		0,                      // 共享方式为不共享
		NULL,                   // 默认安全模式
		OPEN_ALWAYS,          // 文件创建方式，打开或者是创建
		FILE_ATTRIBUTE_NORMAL,  // 正常文件
		NULL);                  // no attr. template\
			//如果文件打开失败
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("不能打开文件 (错误 %d)\n", GetLastError());
		return;
	}

	if (SetFilePointer(hFile, position, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		printf("不能设置文件指针位置 (错误 %d)\n", GetLastError());
		return;
	}

	while (dwBytesWritten < dwBytesToWrite)
	{
		if (FALSE == WriteFile(hFile,           // 打开文件句柄
			dataBuffer,     // 开始进行写操作的位置
			dwBytesToWrite - dwBytesWritten, // 未写入的字节数
			&dwBytesWritten, // 已经写入的字节数
			NULL)            // no overlapped structure
			)
		{
			printf("不能写入文件 (错误 %d)\n", GetLastError());
			CloseHandle(hFile);
			return;
		}
	}
	CloseHandle(hFile);
}

int FileUtil::InsertSlot(const char * slotName, void * dataBuffer, int dataLen)
{
	PageHeader * ph = (PageHeader *)ReadFromFile(slotName, 0, sizeof(PageHeader));
	//如果块已经存满，则带错返回
	if (ph->pd_lower + sizeof(Record) > ph->pd_lower - dataLen) {
		return -1;
	}
	Record record;
	//将数据写到尾部
	record.offset = ph->pd_upper - dataLen;
	record.length = dataLen;
	//记录从前往后写
	WriteToFile(slotName, ph->pd_lower, &record, sizeof(Record));
	//数据从后往前写
	WriteToFile(slotName, record.offset, dataBuffer, dataLen);
	//更新PageHeader
	ph->pd_lower += sizeof(Record);
	ph->pd_upper -= dataLen;
	WriteToFile(slotName, 0, ph, sizeof(PageHeader));
	delete ph;
	return 1;
}

void * FileUtil::ReadData(const char * slotName, int pageNumber)
{
	//读第一个pageHeader
	Record * record = (Record *)ReadFromFile(slotName, sizeof(PageHeader)+(pageNumber - 1)* sizeof(Record), sizeof(Record));
	return ReadFromFile(slotName, record->offset, record->length);
}
