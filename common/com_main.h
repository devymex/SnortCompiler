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
* - 解析规则：读入文件，过滤规则头，提取选项等
* - 处理规则：分链，删链，提取SIG
* - 编译规则：
* - 分组：
* - 哈希：
*
* @subsection subMatchProc 匹配流程
*
* @subsection subImplement 部署流程
*/
