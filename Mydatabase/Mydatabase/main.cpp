#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "FileUtil.h"
#include "DbFileManager.h"
#include <string>
using namespace std;

//���Խṹ��
struct TestData
{
	int data1;
	char data2[1024];
};
//���Խṹ��2
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
	cout << "���������ݿ�����";
	cin >> str;
	cout << "�������ݿ��ļ���С(KB)��";
	cin >> dbSize;
	cout << "��������С(KB)��";
	cin >> pageSize;
	DbFileManager manager;
	int aff = manager.CreateDBFile(str, dbSize, pageSize);
	TestData data;
	data.data1 = 15;
	strcpy_s(data.data2, "hello world");
	cout << "�������ݴ�СΪ��" << sizeof(data) << endl;
	cout << "������������" << endl;
	int times;
	cin >> times;
	for (int i = 0; i < times; ++i) {
		manager.InsertData(&data, sizeof(data));
	}
	int page;
	int record;
	cout << "������ڼ�ҳ��";
	cin >> page;
	cout << "������ڼ�����¼��";
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
