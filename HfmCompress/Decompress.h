#include <fstream>
using namespace std;

class Decompress {
public:
	int m_nameLen;
	int m_frequence[256];
	string m_code[256];                                                        //������¼256��byte�����
	char* m_fileName;                                                          //���ԭ�ļ��� ���ĺ�׺��
	string outName;
	Decompress(char*);
	~Decompress();
	void CreateTree();
	void ReadByte(char);                                                       //���ֽڶ�ȡ�ļ� ������д�����ļ�
private:
	fstream m_in;
	fstream m_out;
	int m_pos = 511;                                                           //ָ�����������ǰ��λ�� �տ�ʼ��ʱ��ָ����Ǹ�

	struct HTNode
	{
		int weight;
		int parent;
		int lchild;
		int rchild;
	};
	HTNode* HTree;

	int MinWeight(HTNode*, int);
};