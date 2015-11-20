#include "stdafx.h"                                                            //预编译头文件 .h文件应放在其后
#include "iostream"
#include "Decompress.h"

//---------------------------------------------------------------------------------------------------------------------

Decompress::Decompress(char* inName)
{
	HTree = new HTNode[512];
	m_fileName = inName;
	m_in.open(m_fileName, ios::binary | ios::in);
	char lenTemp[4];
	m_in.read(lenTemp, 4);
	m_nameLen = (lenTemp[3] << 0) + (lenTemp[2] << 8) + (lenTemp[1] << 16) + (lenTemp[0] << 24);
	m_in.read(m_fileName, m_nameLen);                                          //获得原文件名
	m_out.open(m_fileName, ios::binary | ios::out);                            //打开文件输出流 注意这时m_fileName已经变了

	for (int i = 0; i < 256; i++)
	{
		char temp[4];
		m_in.read(temp, 4);                                                    //插入符<<是个大坑啊 会跳过空格的 要改用read
		m_frequence[i] = ((0xFF & temp[0]) << 24) | ((0xFF & temp[1]) << 16) | ((0xFF & temp[2]) << 8) | (0xFF & temp[3]);
	}                                                                          //http://stackoverflow.com/questions/2840190/java-convert-4-bytes-to-int

	for (int i = 0; i < 512; i++)
	{
		HTree[i].weight = 0;
		HTree[i].parent = 0;
		HTree[i].lchild = 0;
		HTree[i].rchild = 0;
	}

	CreateTree();
	char m_temp[1];
	while (!m_in.eof())
	{
		if (m_in.fail())
		{
			break;
		}
		m_in.read(m_temp, 1);
		ReadByte(m_temp[0]);
	}
	m_out.close();
	m_in.close();
}

Decompress::~Decompress()
{

}

//---------------------------------------------------------------------------------------------------------------------

void Decompress::CreateTree()
{
	//构建哈夫曼树
	for (int i = 1; i <= 256; i++)
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
}

//---------------------------------------------------------------------------------------------------------------------

void Decompress::ReadByte(char byte)
{
	char m_byte[1];
	int bit;
	for (int i = 0; i < 8; i++)
	{
		char byteTemp = byte;
		bit = (byteTemp >> (7 - i)) & 1;
		if (bit)
		{
			m_pos = HTree[m_pos].rchild;
		}
		else
		{
			m_pos = HTree[m_pos].lchild;
		}

		if (HTree[m_pos].lchild == 0 && HTree[m_pos].rchild == 0)
		{
			unsigned char temp = m_pos - 1;
			m_byte[0] = (char)temp;
			m_out.write(m_byte, 1);
			m_pos = 511;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

int Decompress::MinWeight(HTNode* HTree, int len)
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