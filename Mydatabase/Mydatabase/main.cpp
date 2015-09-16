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
	cout << "请输数据库文件大小(KB)：";
	cin >> dbSize;
	cout << "请输入块大小(KB)：";
	cin >> pageSize;
	DbFileManager manager;
	int aff = manager.CreateDBFile(str, dbSize, pageSize);
	TestData data;
	data.data1 = 15;
	strcpy_s(data.data2, "hello world");
	cout << "插入数据大小为：" << sizeof(data) << endl;
	cout << "输入插入次数：" << endl;
	int times;
	cin >> times;
	for (int i = 0; i < times; ++i) {
		manager.InsertData(&data, sizeof(data));
	}
	int page;
	int record;
	cout << "请输入第几页：";
	cin >> page;
	cout << "请输入第几条记录：";
	cin >> record;
	TestData * data2 = (TestData*)manager.ReadData(page, record);
	cout << data2->data1 << endl;
	cout << data2->data2 << endl;
	system("pause");
	//FileUtil util;
	//string str("d:/");
	//str += "test.txt";
	//util.CreateEmptyFile(str.c_str(), 48 * 1024);
}
