#include <fstream>
using namespace std;

class Compress {
public:
	int m_nameLen;
	int m_frequence[256] = { 0 };                                              //用来记录256个byte的频数 
	string m_code[256];                                                        //用来记录256个byte的哈夫曼编码
	char* m_fileName;                                                          //获得原文件名 更改后缀名
	string outName;
	Compress(char*);
	~Compress();
	void CreateTree();                                                         //创建一个哈夫曼树
	void CompressCode();                                                       //逐字节读取文件 将编码写入新文件
private:
	fstream m_in;
	fstream m_out;
	int m_pos = 0;                                                             //记录数组中记录了多少个bit了
	int m_box[8] = { 0 };                                                      //每八个字节构成一个新的char
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

	void QuickSort(int[], int, int);                                           //快速排序的三个函数
	int Partition(int[], int, int);
	void Swap(int &, int &);
};