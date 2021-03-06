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
*
* - 解析规则：读入规则集文件，过滤规则头，提取选项等。详见@ref secSnortOption 。
*
* - 规则的规格化：根据规则选项建立匹配链结构，删除冗余链，提取匹配链的SIG。
* 详见@ref subRuleLogic 。
*
* - 编译pcre：将匹配链转为NFA，然后将NFA转为DFA。详见@ref pageRegex 和
* @ref pageNfa2dfa 。
*
* - DFA分组：根据DFA的SIG进行分组合并，使得合并后的DFA状态数不超过255。
* 详见@ref secGroup 。
*
* - 哈希映射：从能代表一组的SIG中选取一个SIG代表该分组，将该SIG通过哈希函数映射到哈希表中
* 的一个槽，将该SIG和其对应的DFA存入该槽，以降低冲突率。详见@ref secHash 。
*
* - 优化分组结果：详见@ref secOptimize 。
*
* @subsection subMatchProc 匹配流程
*
* - 读入哈希表的全部SIG与槽的对应关系。
* - 收到将数据包后，以每4个字节作为一个SIG的方式扫描数据包，并在哈希表中查找该SIG对应的DFA。
* - 将命中到的所有DFA载入内存，分别在数据包上运行，并将接受的终态所对应的DFA的编号记录下来。
* - 状态机运行结束后，收集接受的所有DFA的编号，确认是否覆盖某条规则的全部匹配链。
* 如果覆盖，则认为该规则匹配上了收到的数据包，并将规则的sid编号上报。
*
* @subsection subImplement 部署流程
* 详见@ref pageUsage 。
*/
