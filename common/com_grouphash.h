/*!
* @page pageGrouphash 分组与哈希
*
* @tableofcontents
*
* 此页面介绍了分组与哈希的数据结构和相关算法。
*
* @section secGroup 分组
*
* @subsection subGroupFlow 分组流程图
*
* 注：下文所述的“SIG”是指DFA所具有的特征字符串，即Signature。
* @dot
* digraph module {
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
* 经过步一步之后，各分组都已合并为一个DFA，且不存在尚为进入分组的孤立DFA。将各分组的DFA尝试合并。若两个分组具有共同的特征字符串集且它们合并后的状态数未超限，则将它们合并为新的分组，新组的特征字符串集为原两个分组的特征字符串集的交集。
*
* @section secHash 压缩
*
*/
