#include <hwprj/dfa.h>
#include "densityCpress.h"

void SetDistance(CDfa dfa, double *disMatrix);
//��ø���������ھӣ�����Ǹö����Ƿ�Ϊ���Ķ���
void GetNeighbors(ushort dfasize, double *disMatrix, double eps, ushort minPts,
				double *coreDis, std::vector<std::vector<ushort>> &neighbors);
double Distance (ushort row1, ushort row2, double *disMatrix);

template <typename T>
double Max(T d1, T d2);

void ExtractCoreSta(CDfa &dfa, std::vector<ushort> &orderObj, CClusterRow &coreRow);

void Update(std::vector<ushort> &neighbors, ROWOBJ &curobj, byte *pProcessed, 
			double *disMatrix, ROWOBJ *allObjs, std::vector<ushort> &orderSeeds);

ushort GetNextObj(std::vector<ushort> &orderSeeds, ROWOBJ *arrObj);

void ExpandClusterOrder(ROWOBJ &obj, double eps, ushort minPts,byte *pProcessed, double *disMatrix, double *coreDis, 
						ROWOBJ *allObjs, std::vector<std::vector<ushort>> &neighbors, std::vector<ushort> &orderObj);

int OPTICS(CDfa &dfa, double *disMatrix, double eps, ushort minPts, DenCpressDfa &clusters);
