#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "FileUtil.h"
#include <string>
using namespace std;

//���Խṹ��
struct TestData
{
	int data1;
	char data2[20];
};
//���Խṹ��2
struct TestData2
{
	char data1;
	TestData data2;
};
//���Դ����Ͳ������
void CreatAndInsertTest()
{
	FileUtil manager;
	char * slotName = "d:/1.txt";
	TestData data;
	data.data1 = 4;
	strcpy_s(data.data2, "Hello world!");
	TestData2 data2;
	data2.data1 = 100;
	data2.data2.data1 = 123;
	strcpy_s(data2.data2.data2, "It's just a test!");
	manager.CreateEmptyFile(slotName, 4 * 1024);
	manager.InsertSlot(slotName, &data, sizeof(TestData));
	manager.InsertSlot(slotName, &data2, sizeof(TestData2));
}
//���Զ�����
void ReadTest()
{
	FileUtil manager;
	char * slotName = "d:/1.txt";
	TestData * data = (TestData*)manager.ReadData(slotName, 1);
	TestData2 * data2 = (TestData2 *)manager.ReadData(slotName, 2);
	printf("TestData from %s \n\tdata1: %d \n\tdata2: %s\n", slotName, data->data1, data->data2);
	printf("TestData2 from %s \n\tdata1: %d \n\tdata2: TestData(data1: %d, data2: %s)\n", slotName, data2->data1, data2->data2.data1, data2->data2.data2);
	system("pause");
}

void main()
{
	int dbSize;
	int pageSize;
	string str;
	cout << "���������ݿ�����";
	cin >> str;
	cout << endl;
	cout << "�������ݿ��ļ���С��";
	cin >> dbSize;
	cout << endl;
	cout << "��������С";
	cin >> pageSize;
	cout << endl;
}
//void __cdecl _tmain(int argc, TCHAR *argv[])
//
//{	
//	FileStorageManager manager;
//	PageHeaderData pageData;
//	/*
//	//����ָ����С���ļ�����ҳͷ�ṹ��д��ָ���ļ���ָ��λ�ã�Ȼ������д���ļ�β��
//	Data data;
//	pageData.pd_lower = sizeof(PageHeaderData);
//	pageData.pd_upper = 4 * 1024;
//	data.data1 = 20;
//	strcpy_s(data.data2, "Hello World!");
//	manager.CreateEmptyFile("d:/1.txt", 4 * 1024);
//	manager.WriteToFile("d:/1.txt", 0, &pageData, sizeof(pageData));
//	manager.WriteToFile("d:/1.txt", 4 * 1024 - sizeof(Data), &data, sizeof(data));*/
//	PageHeaderData pageData2;
//	Data data2;
//
//	pageData2 = *(PageHeaderData * )manager.ReadFromFile("d:/1.txt", 0, sizeof(PageHeaderData));
//	data2 = *(Data *)manager.ReadFromFile("d:/1.txt", 4 * 1024 - sizeof(Data), sizeof(Data));
//	printf("%d\n", pageData2.pd_lower);
//	printf("%d\n", data2.data1);
//	system("pause");
//}