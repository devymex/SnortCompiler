#include <hwprj/dfa.h>
#include <utility>
#include <vector>
#include <map>
#include <hwprj/dfaarray.h>
#ifndef DENCPRESSHDR_DS
#define DENCPRESSHDR __declspec(dllimport)
#else
#define DENCPRESSHDR __declspec(dllexport)
#endif

typedef CDfa CCluster;
typedef CDfaRow CClusterRow;
// ���ڴ洢ÿһ�еĲ���
struct ROWOBJ
{
	// ���ľ���
	double coreDis;

	// �ɴ����
	double reachDis;

	// ����������DFA
	ushort dfaRowInd;
};


struct SeedsSort
{
public:
	ROWOBJ *parrobj;
	SeedsSort (ROWOBJ *p ) : parrobj(p)
	{}
	
	// ��������
	bool operator () (ushort arg1, ushort arg2)
	{
		return parrobj[arg1].reachDis < parrobj[arg2].reachDis;
	}
};



typedef std::vector<std::map<byte,STATEID>>  ROWDIF;

class DENCPRESSHDR DenCpressDfa
{
public:

	// ���캯��
	DenCpressDfa(ushort rowSize);

	// ���ƹ��캯��
	DenCpressDfa(const DenCpressDfa &other);

	// ��������
	~DenCpressDfa();

	// ���ز�������=��
	DenCpressDfa& operator = (const DenCpressDfa &other);

	// ���㵱�������Ĵ�
	byte getClusterNum(byte row);

	// ���������ص�����������
	CDfaRow& getCurCluster(byte Cluster);
	CDfa& GetCoreMatrix();

	// ����m_nDfaID
	void SetDfaID(ulong ID);
	
	// ����staId2CluId
	void SetStaID2CluID(STATEID staID, ushort CluID);

	//���һ��������
	void AddCluRow(CClusterRow& cluRow);
	
	// ����m_Dif
	void SetDif(CClusterRow& coreRow, CDfaRow& dfaRow, ushort rowNum);

	// ��������
	std::map<byte,STATEID>& getDif(byte row);

	ushort Size();

	void Dump(char* file);
	////���ö�Ӧ�Ĵ�С
	//void SetStaSize(ushort size);

	//void InitRowDIf();

private:
	// ��¼��Ӧ��DFAID
	ulong m_nDfaID;

	//
	ushort m_usRowSize;

	// ÿһ�ж�Ӧ�Ĵ�
	std::map<STATEID, ushort> staId2CluId; 

	//������дص�����ֵ
	CCluster m_Cluster;

	// ��¼������еĲ�ͬ������Ӧ��ͬ��ֵ
	ROWDIF m_Dif;

};