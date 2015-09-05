#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "FileStorageManager.h"

const int BUFFER_SIZE = 81;
//页头结构体
struct PageHeaderData
{
	UINT16 pd_tli;
	UINT16 pd_flag;
	int pd_lower;
	int pd_upper;
	int pd_special;
	UINT16 pd_pagesize_version;
	int pd_prune_xid;
};

struct StructA
{
	int a;
	int b;
};
void __cdecl _tmain(int argc, TCHAR *argv[])

{	
	FileStorageManager manager;
	//manager.CreateEmptyFile("d:/2.txt", 2 * 1024);
	char * str = "Hello World";
	//manager.WriteToFile("d:/2.txt", 13, str, strlen(str));
	StructA a;
	/*a.a = 123;
	a.b = 43;*/
//	manager.WriteToFile("d:/2.txt", 0, &a, sizeof(a));
	a = *(StructA *)manager.ReadFromFile("d:/2.txt", 0, sizeof(StructA));
	printf("%d", a.a);
	system("pause");
}