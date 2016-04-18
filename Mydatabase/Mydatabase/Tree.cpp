#include "Tree.h"
#include <queue>
#include <iostream>
using namespace std;

AllocSetContextData::AllocSetContextData() :blocks(nullptr)
{

}

void * AllocSetContextData::AllocSetAlloc(MemoryContext context, int size)
{
	header.addChild(context);
	AllocBlock curBlock;
	//如果没有分配内存块
	if (blocks == nullptr) {
		blocks = new AllocBlockData;
		curBlock = blocks;
	}
	else {//如果已经分配了内存块
		//找到blocks的尾部
		for (curBlock = blocks; curBlock->next; curBlock = curBlock->next);
		curBlock->next = new AllocBlockData;
	}
	//将
	curBlock->freeptr = (char *)curBlock + sizeof(AllocBlockData);
	curBlock->endptr = (char *)curBlock + size;
	//清空空间
	
	return curBlock;
}

void AllocSetContextData::AlllocSetFree(MemoryContext context, void * pointer)
{

}

void * AllocSetContextData::AllocSetRealloc(MemoryContext context, void * pointer, int size)
{
	return nullptr;
}

void AllocSetContextData::AllocSetInit(MemoryContext contex)
{

}

void AllocSetContextData::AllocSetReset(MemoryContext context)
{

}

void AllocSetContextData::AllocSetDelete(MemoryContext context)
{

}

int AllocSetContextData::AllocSetGetChunkSpace(MemoryContext context)
{
	return -1;
}

bool AllocSetContextData::AllocSetIsEmpty(MemoryContext context)
{
	return false;
}

void AllocSetContextData::AllocSetStats(MemoryContext context, int level)
{

}

void AllocSetContextData::AllocSetCheck(MemoryContext context)
{

}

void AllocSetContextData::PutDataToMemory(void * pData, int size)
{
	AllocBlock curBlock = new AllocBlockData;
	if (this->blocks == nullptr) {
		this->blocks = curBlock;
	}
	else {
		AllocBlock tailBlock = nullptr;
		for (tailBlock = blocks; tailBlock->next; tailBlock = tailBlock->next);
		tailBlock->next = curBlock;
	}
	curBlock->freeptr = (char *)pData;
}


MemoryContextData::MemoryContextData() :firstChild(nullptr), nextBrother(nullptr)
{

}

MemoryContextData::MemoryContextData(char *& name) : firstChild(nullptr), nextBrother(nullptr)
{
	this->name = name;
}

void MemoryContextData::addChild(MemoryContextData * child)
{
	//如果没有子节点
	if (firstChild == nullptr) {
		firstChild = child;
		child->parent = this;
		return;
	}
	MemoryContextData * tail;
	//找到尾结点
	for (tail = firstChild; tail->nextBrother; tail = tail->nextBrother);
	tail->nextBrother = child;
	child->parent = this;
}

MemoryContext MemoryContextData::searchNodeByName(MemoryContext root, char * name)
{
	//如果根节点没有数据
	if (root == nullptr) {
		return nullptr;
	}
	queue<MemoryContextData*> queue;
	queue.push(root);
	while (!queue.empty()) {
		MemoryContextData * result = queue.front();
		queue.pop();
		//如果字符串相等
		if (strcmp(result->name, name) == 0) {
			//清空队列
			while (!queue.empty()) {
				queue.pop();
			}
			return result;
		}
		//将temp节点所有的子节点进队列
		for (MemoryContextData * child = result->firstChild; child; child = child->nextBrother) {
			queue.push(child);
		}
	}
	return nullptr;
}