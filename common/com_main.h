/*!
* @mainpage 软件介绍
*
* @section copy_sec 版本声明
* @b SnortCompiler软件、源代码以及相关文档的版权归本项目组所有，在未经许可的情况下，
* @b 任何人不得以任何形式复制、更改、编译和分发本软件及源代码的任何一个部分。

* @section intro_sec 软件介绍
* 本软件以动态链接库的形式提供了Snort规则集的解析、编译、分组和哈希的相关数据结构和算法。
*
* @section documentation 文档
* 目前已基本完成接口类、函数、宏、全局变量、类型定义的文档编写。实现部分的文档正在编写中，
* 实现部分所使用的算法将以另外的文档形式提供。@n
* @li @b 查看软件类库的参考文档请点击上方导航栏的“模块”链接。
* @li @b 查看软件算法的相关文档请点击上方导航栏的“相关页面”链接。
*
* @section secProcess 工作流程
*
* 本软件可以将Snort规则编译为正则表达式，以便在硬件上实现对网络数据包的快速预匹配。
*
* @subsection subCompProc 编译流程
* - 解析规则：读入文件，过滤规则头，提取选项等。见@ref pageRule
* - 处理规则：分链，删链，提取SIG。见@ref pageRule
* - 编译规则：将链转为NFA，然后将NFA转为DFA。见@ref pageNfa2dfa
* - 分组：根据DFA的SIG进行分组合并，使得合并后的DFA状态数不超过255。见@ref pageGrouphash
* - 哈希：从能代表一组的SIG中选取一个SIG代表该分组，将该SIG通过哈希函数映射到哈希表中的一个槽，
*        将该SIG和其对应的DFA存入该槽，使得冲突率尽可能低，不超过3‰。见@ref pageGrouphash
*
* @subsection subMatchProc 匹配流程
*
* 首先将数据包扫描一遍，每四个字节为一个SIG，并对该SIG进行哈希并映射到哈希表@ref HASHRES中，匹配哈希到的DFA，
* 匹配结果为多个链的DfaId。若某条规则的所有链均被匹配到，则该规则被该数据包匹配。
*
* @subsection subImplement 部署流程
* 见@ref pageUsage
*/
