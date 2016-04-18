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
	//���û�з����ڴ��
	if (blocks == nullptr) {
		blocks = new AllocBlockData;
		curBlock = blocks;
	}
	else {//����Ѿ��������ڴ��
		//�ҵ�blocks��β��
		for (curBlock = blocks; curBlock->next; curBlock = curBlock->next);
		curBlock->next = new AllocBlockData;
	}
	//��
	curBlock->freeptr = (char *)curBlock + sizeof(AllocBlockData);
	curBlock->endptr = (char *)curBlock + size;
	//��տռ�
	
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
	//���û���ӽڵ�
	if (firstChild == nullptr) {
		firstChild = child;
		child->parent = this;
		return;
	}
	MemoryContextData * tail;
	//�ҵ�β���
	for (tail = firstChild; tail->nextBrother; tail = tail->nextBrother);
	tail->nextBrother = child;
	child->parent = this;
}

MemoryContext MemoryContextData::searchNodeByName(MemoryContext root, char * name)
{
	//������ڵ�û������
	if (root == nullptr) {
		return nullptr;
	}
	queue<MemoryContextData*> queue;
	queue.push(root);
	while (!queue.empty()) {
		MemoryContextData * result = queue.front();
		queue.pop();
		//����ַ������
		if (strcmp(result->name, name) == 0) {
			//��ն���
			while (!queue.empty()) {
				queue.pop();
			}
			return result;
		}
		//��temp�ڵ����е��ӽڵ������
		for (MemoryContextData * child = result->firstChild; child; child = child->nextBrother) {
			queue.push(child);
		}
	}
	return nullptr;
}