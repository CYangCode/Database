#pragma once

#define ALLOCSET_NUM_FREELISTS 11

struct MemoryContextMethod;
struct MemoryContextData;
typedef MemoryContextData * MemoryContext;
struct AllocBlockData;
typedef AllocBlockData * AllocBlock;
struct AllocChunk;

struct MemoryContextData
{
	//内存节点类型
	int type;
	//内存处理函数指针
	MemoryContextMethod * method;
	//父节点指针
	MemoryContext parent;
	//第一个孩子结点指针
	MemoryContext firstchild;
	//第一个兄弟节点指针
	MemoryContext nextchild;
	//节点的名称
	char *name;
};

struct MemoryContextMethod
{
	//分配内存函数
	void * (*alloc)(MemoryContext context, int size);
	//释放内存函数
	void(*free_p)(MemoryContext context, void * pointer);
	//重分配内存
	void * (*realloc)(MemoryContext context, void * pointer, int size);
	//初始化内存上下文
	void(*init)(MemoryContext contex);
	//重置内存上下文
	void(*reset)(MemoryContext context);
	//删除内存片段的大小
	void(*del)(MemoryContext context);
	//检查内存片段的大小
	int (*get_chunk_space)(MemoryContext context);
	//检查内存上下文是否为空
	bool(*is_empty)(MemoryContext context);
	//打印内存上下文状态
	void(*stats)(MemoryContext context, int level);
	//检查所有内存片段
	void(*check)(MemoryContext context);
};

struct AllocBlockData
{
	int aset;
	AllocBlock next;
	char * freeptr;
	char * endptr;
};

struct AllocChunk
{
	//该内存片所在的AllocSet，如果内存片为空闲，则用于链表空闲链表
	void * aset;
	int size;
	int requested_size;
};

class AllocSetContext
{
private:
	//对应于该内存上下文的头部信息
	MemoryContextData header;
	//该内存上下文中所有内存块的链表
	AllocBlock blocks;
	//该内存上下文中空闲内存片的数组
	AllocChunk freelist[ALLOCSET_NUM_FREELISTS];
	//如果为真表示从上次重置依赖没有分配过内存
	bool isReset;
	//初始内存块的大小
	int initBlockSize;
	//最大内存块的大小
	int maxBlockSize;  
	//下一个要分配的内存块的大小
	int nextBlockSize;
	//分配内存片的尺寸阈值，该值在分配内存片时会用到
	int allocChunkLimit;
	//保存在keeper中的
	AllocBlock keeper;
public:
	//分配内存函数
	void * AllocSetAlloc(MemoryContext context, int size);
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
};