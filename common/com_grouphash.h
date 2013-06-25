/*!
* @page pageGrouphash 分组与哈希
*
* @tableofcontents
*
* 此页面介绍了分组与哈希的算法步骤以及相关数据结构。
*
* @section secGroup 分组
*
*下文所述的“SIG”是指DFA所具有的特征字符串，即Signature。
*
* @subsection subGroupFlow 分组流程图
*
* @dot
* digraph groupProc {
*	node [shape=record, fontname="Microsoft YaHei", fontsize=11];
*	1 [label="合并具有唯一SIG的DFA"];
*	2 [label="将具有多个SIG的DFA合并到已有分组"];
*	3 [label="对未分组的DFA进行聚组"];
*	4 [label="对分组进行再合并"];
*	1 -> 2;
*	2 -> 3;
*	3 -> 4;
* }
* @enddot
*
* @subsection subGroupStep1 第一步、合并具有唯一SIG的DFA
*
* 对只能提出一个特征字符串的有限自动机进行分组，具有相同特征字符串的DFA分为一组，
* 并尝试合并一个分组内的DFA。合并时，由一个DFA开始，将候选的DFA逐个合并入该组。@n
* 
* 如果在合并入某个DFA后，分组的状态数超限，则构造一个新的分组，从该DFA开始继续尝试合并。
* 通过上述步骤，各已有分组内的所有DFA都已合并为一个DFA。
*
* @subsection subGroupStep2 第二步、将具有多个SIG的DFA合并到已有分组
*
* 经过上一步之后，规则集所对应的全部DFA即分为已分组和未分组两类，各已有DFA分组记为
* @f$g_i(i=1,2,\dots)@f$，对于每一个DFA分组@f$g_i@f$：若某个未分组的DFA的SIG集中，
* 存在一个与@f$g_i@f$相同的SIG，且该DFA与@f$g_i@f$合并后的DFA状态数没有超限，
* 则将该DFA合并入@f$g_i@f$中。
*
* @subsection subGroupStep3 第三步、对未分组的DFA进行聚组
*
* 经过上一步之后，剩下未能够合并入已有的分组均具有多个SIG，具都无法在总状态数不超限的前提下
* 合并入已有分组。代表一个分组的SIG集，为分组内各DFA特征字符串集的交集。为避免因求交集
* 操作使得代表分组的SIG集过少，在剩下这些尚未合并入分组的DFA中，先将具有完全相同SIG集的
* 分为一组，则这些新增分组的SIG集与其内各DFA的SIG集均相同。然后，将分组内的DFA进行逐个合并，
* 若合并过程中分组的DFA总状态数超限，则构造一个新的分组，并从超限的DFA开始继续合并。
*
* @subsection subGroupStep4 第四步、对分组进行再合并
* 经过上一步之后，各分组都已合并为一个DFA，且不存在尚未进入分组的孤立DFA。接下来，
* 尝试将具有共同SIG的分组进行合并，若它们合并后的状态数未超限则将它们合并为新的分组。
* 新组的SIG集为原两个分组的SIG集的交集。
*
* @section secHash 哈希
*
* @subsection subHashFlow 哈希流程图
*
* @dot
* digraph hashProc {
*	node [shape=record, fontname="Microsoft YaHei", fontsize=11];
*	1 [label="将具有唯一SIG的分组映射入哈希表"];
*	2 [label="将具有多个SIG的分组映射入哈希表"];
*	3 [label="调整哈希表中代表各分组的SIG以减少冲突"];
*	4 [label="对哈希表中的分组进行再合并"];
*	1 -> 2;
*	2 -> 3;
*	3 -> 4;
* }
* @enddot
*
* @subsection subHashStep1 第一步、将具有唯一SIG的分组映射入哈希表
* 
* 每个SIG均计算为一个整数值，与特征字符串一一对应，用以下公式计算
* 采用的哈希函数为：@n
* @f$hash(SIG)=(c_1×2^24+c_2×2^16+c_3×2^8+c_4×2^0)mod L@f$
* 上式中@f$c_1, c_2, c_3, c_4@f$为SIG的4个字节ASCII码，L为哈希表的长度，
* 为哈希表中所存储分组个数的4倍。
* 
* @subsection subHashStep2 第二步、将具有多个SIG的分组映射入哈希表
* 在具有多个SIG的分组中选取与当前哈希表产生冲突最小的SIG，代表该分组，并通过第一步中的
* 哈希函数映射到哈希表中。
*
* @subsection subHashStep3 第三步、调整哈希表中代表各分组的SIG以减少冲突
* 对哈希表中存在冲突的分组做如下处理：
* - (1) 若代表分组A的SIG @f$s_1@f$ 与其它分组的SIG产生了冲突，则尝试选用分组A的
* 另一个SIG @f$s_2@f$ 来代表分组。
* - (2) 将@f$s_2@f$映射到槽@f$S=hash(s_2)@f$中，若@f$s_2@f$亦产生冲突，
* 则以（1）的方式做递归调整。
* 
* @subsection subHashStep4 第四步、对哈希表中的分组进行再合并
* - (1) 若哈希表中的一个槽内具有多个分组，且代表这些分组的SIG相同，则尝试合并这些分组。
* 合并后不改变代表分组的SIG。
* - (2) 若两个分组A和B位于哈希表中不同的槽内，若他们具有共同的SIG，并且这些共同的SIG中
* 的某个SIG s可以映射到哈希表中的一个空槽内或A、B所在的槽内，则尝试将A和B进行合并。
* 合并后的分组由s代表。
*
*/
