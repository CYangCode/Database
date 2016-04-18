#pragma once
#include "MemoryContext.h"
/************************************************************************/
/* ���ļ���Ϊ��ʵ��MemoryContextMethod�еķ���                                                                     */
/************************************************************************/
//�����ڴ溯��
void * AllocSetContextData(MemoryContext context, int size);

//�ͷ��ڴ溯��
void AlllocSetFree(MemoryContext context, void * pointer);
//�ط����ڴ�
void * AllocSetRealloc(MemoryContext context, void * pointer, int size);
//��ʼ���ڴ�������
void AllocSetInit(MemoryContext contex);
//�����ڴ�������
void AllocSetReset(MemoryContext context);
//ɾ���ڴ�Ƭ�εĴ�С
void AllocSetDelete(MemoryContext context);
//����ڴ�Ƭ�εĴ�С
int AllocSetGetChunkSpace(MemoryContext context);
//����ڴ��������Ƿ�Ϊ��
bool AllocSetIsEmpty(MemoryContext context);
//��ӡ�ڴ�������״̬
void AllocSetStats(MemoryContext context, int level);
//��������ڴ�Ƭ��
void AllocSetCheck(MemoryContext context);