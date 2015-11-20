#include "stdafx.h"                                                            //预编译头文件 .h文件应放在其后
#include "iostream"
#include "Compress.h"
#include <algorithm>

//---------------------------------------------------------------------------------------------------------------------

Compress::Compress(char* inName)                                               //构造函数 读字节 记录频度
{
	HTree = new HTNode[512];
	m_fileName = inName;
	string temp = inName;
	m_nameLen = temp.length();                                                 //先转成string 记录文件名长度
	for (int i = 0; i < 512; i++)
	{
		HTree[i].weight = 0;
		HTree[i].parent = 0;
		HTree[i].lchild = 0;
		HTree[i].rchild = 0;
	}

	char m_temp[1];
	m_in.open(inName, ios::binary | ios::in);                                  //binary:二进制方式打开文件 in:文件以输入方式打开
	while (!m_in.eof())
	{
		m_in.read(m_temp, 1);
		if (m_in.fail())                                                       //读到结尾的时候 会fail
		{
			break;
		}
		m_frequence[(unsigned char)m_temp[0]]++;                               //记录频度
	}
	m_in.close();
}

Compress::~Compress()
{

}

//---------------------------------------------------------------------------------------------------------------------

void Compress::CreateTree()
{
	//构建哈夫曼树

	int len = sizeof(m_frequence) / sizeof(m_frequence[0]);                    //len=256 计算数组长度的方法
	//QuickSort(m_frequence, 0, len-1);                                        //快速排序 本来以为要排序的 其实这样会打乱顺序
	//sort(m_frequence, m_frequence+len);                                      //使用algorithm自带的sort函数也可以实现排序
	for (int i = 1; i <= len; i++)
	{
		HTree[i].weight = m_frequence[i - 1];
	}
	for (int i = 257; i < 512; i++)
	{
		int min1 = MinWeight(HTree, i);                                        //从下标1到i中选取weight最小的两个结点
		int min2 = MinWeight(HTree, i);
		HTree[min1].parent = i;
		HTree[min2].parent = i;
		HTree[i].lchild = min1;
		HTree[i].rchild = min2;
		HTree[i].weight = HTree[min1].weight + HTree[min2].weight;
	}

	//-----------------------------------------------------------------------------------------------------------------
																			   //根据哈夫曼树创建码表

	int cur = 511;                                                             //哈夫曼树的根的下标
	int code_len = 0;                                                          //编码长度
	char* code = new char[code_len];                                           //编码暂存在code中
	for (int i = 0; i < 512; i++)
	{                                                                          //先把所有的weight都改为0 代表这个结点没有访问过
		HTree[i].weight = 0;                                                   //weight==1表示左孩子访问过了 右孩子没有
	}                                                                          //weight==2表示右孩子也访问过了 

	while (cur != 0)
	{
		if (HTree[cur].weight == 0)                                            //没有访问过
		{
			HTree[cur].weight = 1;                                             //先访问左孩子 并标记
			if (HTree[cur].lchild != 0)                                        //左孩子不是叶子结点 编码就增加一个'0'
			{
				code[code_len++] = '0';
				cur = HTree[cur].lchild;                                       //cur移到这个左孩子
			}
			else
			{
				code[code_len] = '\0';                                         //左孩子是叶子结点 以'\0'结尾
				m_code[cur - 1] = code;                                        //将这个code存到m_code数组对应的位置中
			}
		}
		else if (HTree[cur].weight == 1)                                       //访问右孩子 方法同上
		{
			HTree[cur].weight = 2;
			if (HTree[cur].rchild != 0)
			{
				code[code_len++] = '1';
				cur = HTree[cur].rchild;
			}
			else
			{
				code[code_len] = '\0';
				m_code[cur - 1] = code;
			}
		}
		else
		{
			cur = HTree[cur].parent;
			code_len--;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

void Compress::CompressCode()
{
	int pos = 0;
	outName = m_fileName;
	int length = outName.length();
	for (int i = length - 1; i >= 0; i--)
	{
		if (m_fileName[i] == '.')                                              //倒着排序 记录后缀名的点的位置
			pos = i;
	}
	outName.replace(pos, length - pos, ".lvj");                                //原后缀名替换 LOVEJOY	

	//-----------------------------------------------------------------------------------------------------------------

	m_in.open(m_fileName, ios::binary | ios::in);                              //根据码表 将原文件重新编码 写到新文件中
	m_out.open(outName, ios::binary | ios::out);

	char m_intTemp[4];                                                         //把原文件的文件名长度 文件名 频度写到压缩文件中
	m_intTemp[0] = (char)((m_nameLen >> 24) & 0xff);
	m_intTemp[1] = (char)((m_nameLen >> 16) & 0xff);
	m_intTemp[2] = (char)((m_nameLen >> 8) & 0xff);
	m_intTemp[3] = (char)((m_nameLen)& 0xff);
	m_out.write(m_intTemp, 4);

	m_out.write(m_fileName, m_nameLen);

	for (int i = 0; i < 256; i++)
	{
		char m_intTemp[4];
		int curInt = m_frequence[i];
		m_intTemp[0] = (char)((curInt >> 24) & 0xff);
		curInt = m_frequence[i];
		m_intTemp[1] = (char)((curInt >> 16) & 0xff);
		curInt = m_frequence[i];
		m_intTemp[2] = (char)((curInt >> 8) & 0xff);
		curInt = m_frequence[i];
		m_intTemp[3] = (char)((curInt)& 0xff);
		m_out.write(m_intTemp, 4);
	}

	char m_temp[1];
	while (!m_in.eof())
	{
		if (m_in.fail())                                                       //读到结尾的时候 会fail
		{
			break;
		}
		m_in.read(m_temp, 1);
		WriteByte(m_temp[0]);                                                  //读到一个char 就调用WriteByte()函数 将新的编码写到新文件中
	}
	m_in.close();
	m_out.close();
	int temp[256];
}

//---------------------------------------------------------------------------------------------------------------------

int Compress::MinWeight(HTNode* HTree, int len)                                //从parent为0的结点中找到weight最小的
{
	int min, min_weight;                                                       //min最小weight的下标 min_weight最小weight的值
	int i = 1;                                                                 //HTree从1开始记录
	while (HTree[i].parent != 0)                                               //只考虑parent是0的结点
	{
		i++;
	}
	min_weight = HTree[i].weight;                                              //先让最前面的为最小值
	min = i;

	for (; i < len; i++)
	{
		if (HTree[i].weight < min_weight&&HTree[i].parent == 0)
		{
			min_weight = HTree[i].weight;
			min = i;
		}
	}
	HTree[min].parent = -1;                                                    //把最小值的parent改成不是0 下次连续查找最小值的时候就跳过
	return min;
}

//---------------------------------------------------------------------------------------------------------------------

void Compress::WriteByte(unsigned char byte)                                   //读一个8字节的byte 把一个个bit按照int型0 1传入到WriteBit中
{
	int len = m_code[byte].length();
	//const unsigned char* code = reinterpret_cast<const unsigned char*>(m_code[byte].c_str());
	const char* code = m_code[byte].c_str();
	for (int i = 0; i < len; i++)
	{
		if (m_pos == 8)
		{
			int sum = 0;                                                       //m_box满了 就把八位变为一个char 写入到m_out中去
			for (int j = 0; j < 8; j++)
			{
				sum += m_box[j] * pow(2, 7 - j);
			}
			char new_born[1];
			new_born[0] = sum;
			m_out.write(new_born, 1);                                          //用插入符可能会跳过空格 改用write()吧 
			m_pos = 0;                                                         //清空m_b0x m_pos改为0
			for (int j = 0; j < 8; j++)
			{
				m_box[j] = 0;
			}
		}
		m_box[m_pos] = code[i] - 48;
		m_pos++;
	}
}

//---------------------------------------------------------------------------------------------------------------------

void Compress::QuickSort(int a[], int p, int r)                                //快速排序
{                                                                              //http://www.cnblogs.com/pugang/archive/2012/06/27/2565093.html
	if (p < r)
	{
		int q = Partition(a, p, r);
		QuickSort(a, p, q - 1);                                                //递归
		QuickSort(a, q + 1, r);
	}
}

int Compress::Partition(int a[], int p, int r)                                 //数组 起始位置 终止位置 实现就地排序
{
	int x = a[r];                                                              //以终止位置为比较基准
	int i = p, j = p - 1;                                                      //j是慢速移动下标	
	for (; i < r; i++)
	{
		if (a[i] <= x)
		{
			j++;
			if (i != j)
			{
				Swap(a[i], a[j]);
			}
		}
	}
	Swap(a[j + 1], a[r]);                                                      //把基准放到中间
	return j + 1;
}

void Compress::Swap(int &a, int &b)                                            //交换a b的值
{
	int temp = b;
	b = a;
	a = temp;
}