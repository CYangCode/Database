#pragma once
#include <tchar.h>
#include <windows.h>
//存入也页头的信息，通过页头找到存储内容的位置
struct PageHeader
{
	int pd_lower; //文件中空闲内容的开始位置
	int pd_upper; //文件中空闲内容的结束位置
};
//记录存储数据的位置
struct Record
{
	int offset; //偏移量
	int length; //数据长度
};
class FileUtil
{
public:
	FileUtil();
	~FileUtil();
	//创建指定大小的文件，文件名为fileName， 大小为size
	int CreateEmptyFile(const char * fileName, int size);
	//读取指定位置的文件内容,文件为pFile，开始位置为position，长度为length，返回值可以根据具体情况进行强制转换
	void * ReadFromFile(const char * fileName, long position, int length);
	//将内容写入指定位置,文件名为fileName，开始位置为position，写入内容为DataBuffer
	void WriteToFile(const char * fileName, long position, void * dataBuffer, DWORD strLen);
	//向名字为slotName的槽中插入dataBuffer
	int InsertSlot(const  char * slotName, void * dataBuffer, int dataLen);
	//从槽名为slotName的文件中找到第pageNumber个数据的信息
	void * ReadData(const  char * slotName, int pageNumber);
};

