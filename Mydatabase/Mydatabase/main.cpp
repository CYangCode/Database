#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "FileUtil.h"
#include "DbFileManager.h"
#include <string>
using namespace std;

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

void main()
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
