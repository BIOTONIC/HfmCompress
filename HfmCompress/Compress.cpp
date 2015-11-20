#include "stdafx.h"                                                            //Ԥ����ͷ�ļ� .h�ļ�Ӧ�������
#include "iostream"
#include "Compress.h"
#include <algorithm>

//---------------------------------------------------------------------------------------------------------------------

Compress::Compress(char* inName)                                               //���캯�� ���ֽ� ��¼Ƶ��
{
	HTree = new HTNode[512];
	m_fileName = inName;
	string temp = inName;
	m_nameLen = temp.length();                                                 //��ת��string ��¼�ļ�������
	for (int i = 0; i < 512; i++)
	{
		HTree[i].weight = 0;
		HTree[i].parent = 0;
		HTree[i].lchild = 0;
		HTree[i].rchild = 0;
	}

	char m_temp[1];
	m_in.open(inName, ios::binary | ios::in);                                  //binary:�����Ʒ�ʽ���ļ� in:�ļ������뷽ʽ��
	while (!m_in.eof())
	{
		m_in.read(m_temp, 1);
		if (m_in.fail())                                                       //������β��ʱ�� ��fail
		{
			break;
		}
		m_frequence[(unsigned char)m_temp[0]]++;                               //��¼Ƶ��
	}
	m_in.close();
}

Compress::~Compress()
{

}

//---------------------------------------------------------------------------------------------------------------------

void Compress::CreateTree()
{
	//������������

	int len = sizeof(m_frequence) / sizeof(m_frequence[0]);                    //len=256 �������鳤�ȵķ���
	//QuickSort(m_frequence, 0, len-1);                                        //�������� ������ΪҪ����� ��ʵ���������˳��
	//sort(m_frequence, m_frequence+len);                                      //ʹ��algorithm�Դ���sort����Ҳ����ʵ������
	for (int i = 1; i <= len; i++)
	{
		HTree[i].weight = m_frequence[i - 1];
	}
	for (int i = 257; i < 512; i++)
	{
		int min1 = MinWeight(HTree, i);                                        //���±�1��i��ѡȡweight��С���������
		int min2 = MinWeight(HTree, i);
		HTree[min1].parent = i;
		HTree[min2].parent = i;
		HTree[i].lchild = min1;
		HTree[i].rchild = min2;
		HTree[i].weight = HTree[min1].weight + HTree[min2].weight;
	}

	//-----------------------------------------------------------------------------------------------------------------
																			   //���ݹ��������������

	int cur = 511;                                                             //���������ĸ����±�
	int code_len = 0;                                                          //���볤��
	char* code = new char[code_len];                                           //�����ݴ���code��
	for (int i = 0; i < 512; i++)
	{                                                                          //�Ȱ����е�weight����Ϊ0 ����������û�з��ʹ�
		HTree[i].weight = 0;                                                   //weight==1��ʾ���ӷ��ʹ��� �Һ���û��
	}                                                                          //weight==2��ʾ�Һ���Ҳ���ʹ��� 

	while (cur != 0)
	{
		if (HTree[cur].weight == 0)                                            //û�з��ʹ�
		{
			HTree[cur].weight = 1;                                             //�ȷ������� �����
			if (HTree[cur].lchild != 0)                                        //���Ӳ���Ҷ�ӽ�� ���������һ��'0'
			{
				code[code_len++] = '0';
				cur = HTree[cur].lchild;                                       //cur�Ƶ��������
			}
			else
			{
				code[code_len] = '\0';                                         //������Ҷ�ӽ�� ��'\0'��β
				m_code[cur - 1] = code;                                        //�����code�浽m_code�����Ӧ��λ����
			}
		}
		else if (HTree[cur].weight == 1)                                       //�����Һ��� ����ͬ��
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
		if (m_fileName[i] == '.')                                              //�������� ��¼��׺���ĵ��λ��
			pos = i;
	}
	outName.replace(pos, length - pos, ".lvj");                                //ԭ��׺���滻 LOVEJOY	

	//-----------------------------------------------------------------------------------------------------------------

	m_in.open(m_fileName, ios::binary | ios::in);                              //������� ��ԭ�ļ����±��� д�����ļ���
	m_out.open(outName, ios::binary | ios::out);

	char m_intTemp[4];                                                         //��ԭ�ļ����ļ������� �ļ��� Ƶ��д��ѹ���ļ���
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
		if (m_in.fail())                                                       //������β��ʱ�� ��fail
		{
			break;
		}
		m_in.read(m_temp, 1);
		WriteByte(m_temp[0]);                                                  //����һ��char �͵���WriteByte()���� ���µı���д�����ļ���
	}
	m_in.close();
	m_out.close();
	int temp[256];
}

//---------------------------------------------------------------------------------------------------------------------

int Compress::MinWeight(HTNode* HTree, int len)                                //��parentΪ0�Ľ�����ҵ�weight��С��
{
	int min, min_weight;                                                       //min��Сweight���±� min_weight��Сweight��ֵ
	int i = 1;                                                                 //HTree��1��ʼ��¼
	while (HTree[i].parent != 0)                                               //ֻ����parent��0�Ľ��
	{
		i++;
	}
	min_weight = HTree[i].weight;                                              //������ǰ���Ϊ��Сֵ
	min = i;

	for (; i < len; i++)
	{
		if (HTree[i].weight < min_weight&&HTree[i].parent == 0)
		{
			min_weight = HTree[i].weight;
			min = i;
		}
	}
	HTree[min].parent = -1;                                                    //����Сֵ��parent�ĳɲ���0 �´�����������Сֵ��ʱ�������
	return min;
}

//---------------------------------------------------------------------------------------------------------------------

void Compress::WriteByte(unsigned char byte)                                   //��һ��8�ֽڵ�byte ��һ����bit����int��0 1���뵽WriteBit��
{
	int len = m_code[byte].length();
	//const unsigned char* code = reinterpret_cast<const unsigned char*>(m_code[byte].c_str());
	const char* code = m_code[byte].c_str();
	for (int i = 0; i < len; i++)
	{
		if (m_pos == 8)
		{
			int sum = 0;                                                       //m_box���� �ͰѰ�λ��Ϊһ��char д�뵽m_out��ȥ
			for (int j = 0; j < 8; j++)
			{
				sum += m_box[j] * pow(2, 7 - j);
			}
			char new_born[1];
			new_born[0] = sum;
			m_out.write(new_born, 1);                                          //�ò�������ܻ������ո� ����write()�� 
			m_pos = 0;                                                         //���m_b0x m_pos��Ϊ0
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

void Compress::QuickSort(int a[], int p, int r)                                //��������
{                                                                              //http://www.cnblogs.com/pugang/archive/2012/06/27/2565093.html
	if (p < r)
	{
		int q = Partition(a, p, r);
		QuickSort(a, p, q - 1);                                                //�ݹ�
		QuickSort(a, q + 1, r);
	}
}

int Compress::Partition(int a[], int p, int r)                                 //���� ��ʼλ�� ��ֹλ�� ʵ�־͵�����
{
	int x = a[r];                                                              //����ֹλ��Ϊ�Ƚϻ�׼
	int i = p, j = p - 1;                                                      //j�������ƶ��±�	
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
	Swap(a[j + 1], a[r]);                                                      //�ѻ�׼�ŵ��м�
	return j + 1;
}

void Compress::Swap(int &a, int &b)                                            //����a b��ֵ
{
	int temp = b;
	b = a;
	a = temp;
}