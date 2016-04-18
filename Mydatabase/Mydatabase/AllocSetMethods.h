#pragma once
#include "MemoryContext.h"
/************************************************************************/
/* 该文件是为了实现MemoryContextMethod中的方法                                                                     */
/************************************************************************/
//分配内存函数
void * AllocSetContextData(MemoryContext context, int size);

//释放内存函数
void AlllocSetFree(MemoryContext context, void * pointer);
//重分配内存
void * AllocSetRealloc(MemoryContext context, void * pointer, int size);
//初始化内存上下文
void AllocSetInit(MemoryContext contex);
//重置内存上下文
void AllocSetReset(MemoryContext context);
//删除内存片段的大小
void AllocSetDelete(MemoryContext context);
//检查内存片段的大小
int AllocSetGetChunkSpace(MemoryContext context);
//检查内存上下文是否为空
bool AllocSetIsEmpty(MemoryContext context);
//打印内存上下文状态
void AllocSetStats(MemoryContext context, int level);
//检查所有内存片段
void AllocSetCheck(MemoryContext context);