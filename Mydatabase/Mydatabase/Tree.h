#pragma once

#define ALLOCSET_NUM_FREELISTS 11

//��ͽṹ��������������͵Ķ���
class MemoryContextData;
typedef MemoryContextData * MemoryContext;
struct AllocBlockData;
typedef AllocBlockData * AllocBlock;
struct AllocChunk;

struct AllocBlockData
{
	int aset;//���ڴ�����ڵ�AllocSet
	AllocBlock next;//ָ����һ���ڴ���ָ��
	char * freeptr;//ָ��ÿ����������׵�ַ
	char * endptr;//���ڴ���ĩ��ַ
	AllocBlockData() :next(nullptr), freeptr(nullptr), endptr(nullptr){};
};

struct AllocChunk
{
	//���ڴ�Ƭ���ڵ�AllocSet������ڴ�ƬΪ���У������������������
	void * aset;
	//�ڴ�Ƭ��ʵ�ʴ�С�������ڴ�Ƭ������2����Ϊ��С���ж��룬�������Ĵ�С���ܱ�ʵ�ʴ�СС
	int size;
	//�ڴ�Ƭ�б�ʹ�õĿռ��С����������ڴ�Ƭ����Ϊ0
	int requested_size;
};
class MemoryContextData
{
private:
	int type;//����
	MemoryContext parent;//���ڵ�
	MemoryContext firstChild;//�ӽڵ�
	MemoryContext nextBrother;//��һ���ֵܽڵ�ָ��
	char * name;//�ڵ���
public:
	MemoryContextData();
	MemoryContextData(char *& name);
	//���һ���ӽڵ㣬�ӽڵ�Ϊchild
	void addChild(MemoryContextData * child);
	//�Ӹ��ڵ�root���в�������Ϊname�Ľڵ㣬���ҳɹ��򷵻ز��ҵ��Ľڵ㣬���ʧ���򷵻ؿ�
	static MemoryContext searchNodeByName(MemoryContext root, char * name);
};

class AllocSetContextData
{
public:
	AllocSetContextData();
	//������Ϊsize�����ݷ���ָ���ڴ�֮��
	void PutDataToMemory(void * pData, int size);
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
};
