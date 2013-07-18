#include <hwprj/dfa.h>
#include <utility>
#include <vector>
#include <map>
#include "dfaarray.h"

#ifndef DENCPRESSHDR_DS
#define DENCPRESSHDR __declspec(dllimport)
#else
#define DENCPRESSHDR __declspec(dllexport)
#endif


// ���ڴ洢ÿһ�еĲ���
struct ROWOBJ
{
	// ���ľ���
	double coreDis;

	// �ɴ����
	double reachDis;

	// ����������DFA
	byte dfaRowInd;
};

typedef std::vector<std::map<byte,STATEID>>  ROWDIF;

class DENCPRESSHDR DenCpressDfa
{
public:

	// ���캯��
	DenCpressDfa();

	// ���ƹ��캯��
	DenCpressDfa(const DenCpressDfa &other);

	// ��������
	~DenCpressDfa();

	// ���ز�������=��
	DenCpressDfa& operator = (const DenCpressDfa &other);


	// ���㵱�����������Ĵ�
	byte getClusterNum(byte row);

	// ���������ص�����������
	CDfaRow& getCurCluster(byte Cluster);

	// ��������
	std::map<byte,STATEID> getDif(byte row);

private:

	// ÿһ�ж�Ӧ�Ĵ�
	byte m_bClusterNum; 

	// ��¼��Ӧ��DFAID
	ulong m_nID;

	// ��¼������еĲ�ͬ������Ӧ��ͬ��ֵ
	ROWDIF  m_Dif;


};