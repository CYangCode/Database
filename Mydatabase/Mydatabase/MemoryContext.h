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
	//�ڴ�ڵ�����
	int type;
	//�ڴ洦����ָ��
	MemoryContextMethod * method;
	//���ڵ�ָ��
	MemoryContext parent;
	//��һ�����ӽ��ָ��
	MemoryContext firstchild;
	//��һ���ֵܽڵ�ָ��
	MemoryContext nextchild;
	//�ڵ������
	char *name;
};

struct MemoryContextMethod
{
	//�����ڴ溯��
	void * (*alloc)(MemoryContext context, int size);
	//�ͷ��ڴ溯��
	void(*free_p)(MemoryContext context, void * pointer);
	//�ط����ڴ�
	void * (*realloc)(MemoryContext context, void * pointer, int size);
	//��ʼ���ڴ�������
	void(*init)(MemoryContext contex);
	//�����ڴ�������
	void(*reset)(MemoryContext context);
	//ɾ���ڴ�Ƭ�εĴ�С
	void(*del)(MemoryContext context);
	//����ڴ�Ƭ�εĴ�С
	int (*get_chunk_space)(MemoryContext context);
	//����ڴ��������Ƿ�Ϊ��
	bool(*is_empty)(MemoryContext context);
	//��ӡ�ڴ�������״̬
	void(*stats)(MemoryContext context, int level);
	//��������ڴ�Ƭ��
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
	//���ڴ�Ƭ���ڵ�AllocSet������ڴ�ƬΪ���У������������������
	void * aset;
	int size;
	int requested_size;
};

class AllocSetContext
{
private:
	//��Ӧ�ڸ��ڴ������ĵ�ͷ����Ϣ
	MemoryContextData header;
	//���ڴ��������������ڴ�������
	AllocBlock blocks;
	//���ڴ��������п����ڴ�Ƭ������
	AllocChunk freelist[ALLOCSET_NUM_FREELISTS];
	//���Ϊ���ʾ���ϴ���������û�з�����ڴ�
	bool isReset;
	//��ʼ�ڴ��Ĵ�С
	int initBlockSize;
	//����ڴ��Ĵ�С
	int maxBlockSize;  
	//��һ��Ҫ������ڴ��Ĵ�С
	int nextBlockSize;
	//�����ڴ�Ƭ�ĳߴ���ֵ����ֵ�ڷ����ڴ�Ƭʱ���õ�
	int allocChunkLimit;
	//������keeper�е�
	AllocBlock keeper;
public:
	//�����ڴ溯��
	void * AllocSetAlloc(MemoryContext context, int size);
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
};