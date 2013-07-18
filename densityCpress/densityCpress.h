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


// 用于存储每一行的参数
struct ROWOBJ
{
	// 核心距离
	double coreDis;

	// 可达距离
	double reachDis;

	// 该行所属的DFA
	byte dfaRowInd;
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


	// 计算当行属于所属的簇
	byte getClusterNum(byte row);

	// 返回所属簇的特征行引用
	CDfaRow& getCurCluster(byte Cluster);

	// 返回区别
	std::map<byte,STATEID> getDif(byte row);

private:

	// 每一行对应的簇
	byte m_bClusterNum; 

	// 记录对应的DFAID
	ulong m_nID;

	// 记录与代表行的不同处及相应不同的值
	ROWDIF  m_Dif;


};