#include <fstream>
using namespace std;

class Decompress {
public:
	int m_nameLen;
	int m_frequence[256];
	string m_code[256];                                                        //用来记录256个byte的码表
	char* m_fileName;                                                          //获得原文件名 更改后缀名
	string outName;
	Decompress(char*);
	~Decompress();
	void CreateTree();
	void ReadByte(char);                                                       //逐字节读取文件 将编码写入新文件
private:
	fstream m_in;
	fstream m_out;
	int m_pos = 511;                                                           //指向哈夫曼树当前的位置 刚开始的时候指向的是根

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