#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "FileUtil.h"
#include "DbFileManager.h"
#include "Tree.h"
#include <string>
#include <io.h>
using namespace std;

void putToMemory(void * freeptr, void * data, int size);

void testFileManager();
void main()
{
	/*char str[5] = "abcd";
	char * ptr = new char[50];
	putToMemory(ptr, str, 5);
	system("pause");*/
	testFileManager();
}

//测试结构体
struct TestData
{
	int data1;
	char data2[1024];
};
//测试结构体2
struct TestData2
{
	char data1;
	TestData data2;
};
//对磁盘管理进行的测试
void testFileManager()
{
	int dbSize;
	int pageSize;
	string str;
	cout << "请输入数据库名：";
	cin >> str;
	cout << endl;
	cout << "请输数据库文件大小(KB)：";
	cin >> dbSize;
	cout << endl;
	cout << "请输入块大小(KB)：";
	cin >> pageSize;
	cout << endl;
	DbFileManager manager;
	int aff = manager.CreateDBFile(str, dbSize, pageSize);
	TestData data;
	data.data1 = 15;
	strcpy_s(data.data2, "hello world");
	cout << "数据内容： data.data1: " << data.data1 << " data.data2" << data.data2 << endl;
	cout << "数据大小：" << sizeof(data) << endl;
	manager.InsertData(&data, sizeof(data));
	manager.InsertData(&data, sizeof(data));
	manager.InsertData(&data, sizeof(data));
	manager.InsertData(&data, sizeof(data));
	manager.InsertData(&data, sizeof(data));
	manager.InsertData(&data, sizeof(data));
	TestData * data2 = (TestData*)manager.ReadData(1, 2);
	cout << data2->data1 << endl;
	cout << data2->data2 << endl;
	system("pause");
	//FileUtil util;
	//string str("d:/");
	//str += "test.txt";
	//util.CreateEmptyFile(str.c_str(), 48 * 1024);
}
//对共享内存和私有内存进行申请，并对内存进行管理
void manageMemoryTest()
{
	DWORD length = 1 >> 20;
	char * sharedMemory = (char *)malloc(sizeof(char)* length);
	char * privateMemory = (char *)malloc(sizeof(char)* length);
	ZeroMemory(sharedMemory, length);
	DWORD dwBytesRead = 0;

	HANDLE hFile;
	HANDLE hFile2;
	hFile = CreateFile("d:/1.txt",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	hFile2 = CreateFile("d:/2.txt",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE || hFile2 == INVALID_HANDLE_VALUE)
	{
		printf("不能打开文件 (错误 %d)\n", GetLastError());
		return;
	}

	ReadFile(hFile, sharedMemory, length, &dwBytesRead, NULL);
	ReadFile(hFile2, privateMemory, length, &dwBytesRead, NULL);
	cout << sharedMemory << endl;
	cout << privateMemory << endl;
	system("pause");
}

void putToMemory(void * freeptr, void * data, int size) {
	char * ptr1 = (char *)freeptr;
	char * ptr2 = (char *)data;
	int i = 0;
	for (; i < size; ptr1++, ptr2++, i++) {
		*ptr1 = *ptr2;
	}
}