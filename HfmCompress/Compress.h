#include <fstream>
using namespace std;

class Compress {
public:
	int m_nameLen;
	int m_frequence[256] = { 0 };                                              //������¼256��byte��Ƶ�� 
	string m_code[256];                                                        //������¼256��byte�Ĺ���������
	char* m_fileName;                                                          //���ԭ�ļ��� ���ĺ�׺��
	string outName;
	Compress(char*);
	~Compress();
	void CreateTree();                                                         //����һ����������
	void CompressCode();                                                       //���ֽڶ�ȡ�ļ� ������д�����ļ�
private:
	fstream m_in;
	fstream m_out;
	int m_pos = 0;                                                             //��¼�����м�¼�˶��ٸ�bit��
	int m_box[8] = { 0 };                                                      //ÿ�˸��ֽڹ���һ���µ�char
	struct HTNode
	{
		int weight;
		int parent;
		int lchild;
		int rchild;
	};
	HTNode* HTree;

	int MinWeight(HTNode*, int);

	void WriteByte(unsigned char);

	void QuickSort(int[], int, int);                                           //�����������������
	int Partition(int[], int, int);
	void Swap(int &, int &);
};