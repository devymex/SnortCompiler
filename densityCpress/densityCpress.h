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
// 用于存储每一行的参数
struct ROWOBJ
{
	// 核心距离
	double coreDis;

	// 可达距离
	double reachDis;

	// 该行所属的DFA
	ushort dfaRowInd;
};


struct SeedsSort
{
public:
	ROWOBJ *parrobj;
	SeedsSort (ROWOBJ *p ) : parrobj(p)
	{}
	
	// 递增排序
	bool operator () (ushort arg1, ushort arg2)
	{
		return parrobj[arg1].reachDis < parrobj[arg2].reachDis;
	}
};



typedef std::vector<std::map<byte,STATEID>>  ROWDIF;

class DENCPRESSHDR DenCpressDfa
{
public:

	// 构造函数
	DenCpressDfa();

	// 复制构造函数
	DenCpressDfa(const DenCpressDfa &other);

	// 析构函数
	~DenCpressDfa();

	// 重载操作符“=”
	DenCpressDfa& operator = (const DenCpressDfa &other);

	// 计算当行所属的簇
	byte getClusterNum(byte row);

	// 返回所属簇的特征行引用
	CDfaRow& getCurCluster(byte Cluster);

	// 设置m_nDfaID
	void SetDfaID(ulong ID);
	
	// 设置staId2CluId
	void SetStaID2CluID();

	//添加一个特征行
	void AddCluRow(CClusterRow& cluRow);
	
	// 设置m_Dif
	void SetDif(CClusterRow& coreRow, CDfaRow& dfaRow);

	// 返回区别
	std::map<byte,STATEID>& getDif(byte row);

private:
	// 记录对应的DFAID
	ulong m_nDfaID;

	// 每一行对应的簇
	std::map<STATEID, ushort> staId2CluId; 

	//存放所有簇的特征值
	CCluster m_Cluster;

	// 记录与代表行的不同处及相应不同的值
	ROWDIF m_Dif;

};