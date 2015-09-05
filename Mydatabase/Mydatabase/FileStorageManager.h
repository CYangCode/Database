#pragma once
#include <tchar.h>
#include <windows.h>

class FileStorageManager
{
public:
	FileStorageManager();
	~FileStorageManager();
	//创建指定大小的文件，文件名为fileName， 大小为size
	int CreateEmptyFile(TCHAR * fileName, int size);
	//读取指定位置的文件内容,文件为pFile，开始位置为position，长度为length，返回值可以根据具体情况进行强制转换
	void * ReadFromFile(char * fileName, long position, int length);
	//将内容写入指定位置,文件名为fileName，开始位置为position，写入内容为DataBuffer
	void WriteToFile(char * fileName, long position, void * dataBuffer, DWORD strLen);
};

