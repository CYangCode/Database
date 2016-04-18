#pragma once

#define ALLOCSET_NUM_FREELISTS 11

//类和结构体的声明和新类型的定义
class MemoryContextData;
typedef MemoryContextData * MemoryContext;
struct AllocBlockData;
typedef AllocBlockData * AllocBlock;
struct AllocChunk;

struct AllocBlockData
{
	int aset;//该内存块所在的AllocSet
	AllocBlock next;//指向下一个内存块的指针
	char * freeptr;//指向该块空闲区域的首地址
	char * endptr;//该内存块的末地址
	AllocBlockData() :next(nullptr), freeptr(nullptr), endptr(nullptr){};
};

struct AllocChunk
{
	//该内存片所在的AllocSet，如果内存片为空闲，则用于链表空闲链表
	void * aset;
	//内存片的实际大小，由于内存片都是以2的幂为大小进行对齐，因此申请的大小可能比实际大小小
	int size;
	//内存片中被使用的空间大小，如果空闲内存片则置为0
	int requested_size;
};
class MemoryContextData
{
private:
	int type;//类型
	MemoryContext parent;//父节点
	MemoryContext firstChild;//子节点
	MemoryContext nextBrother;//第一个兄弟节点指针
	char * name;//节点名
public:
	MemoryContextData();
	MemoryContextData(char *& name);
	//添加一个子节点，子节点为child
	void addChild(MemoryContextData * child);
	//从根节点root进行查找姓名为name的节点，查找成功则返回查找到的节点，如果失败则返回空
	static MemoryContext searchNodeByName(MemoryContext root, char * name);
};

class AllocSetContextData
{
public:
	AllocSetContextData();
	//将长度为size的数据放入指定内存之中
	void PutDataToMemory(void * pData, int size);
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
};
