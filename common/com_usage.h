/*!
* @page pageUsage 使用说明书
*
* @tableofcontents
*
* 引页面介绍软件的使用条件与使用方法
*
* @section requirement 一、平台要求
* @li 操作系统：只能在Microsoft Windows XP及以上版本的操作系统中编译和运行。
* @li 开发环境：只能在Microsoft Visual Studio 2012开发环境中执行编译。
* @li 运行要求：运行要求具有至少2.0GHz以上的中央处理器，以及1GiB以上的可用内存空间。
* @li 存储要求：支持将结果数据写入硬盘，但需保证硬盘的剩余空间大于需要写入的数据大小。
*
* @section module 二、模块结构
*
* @dot
* digraph module {
* node [shape=record, fontname="Microsoft YaHei", fontsize=11];
* common [label="公用模块" URL="\ref groupCommon"];
* automaton [label="自动机模块" URL="\ref groupAutomaton"];
* compiler [label="编译器模块" URL="\ref groupCompiler"];
* grouphash [label="分组与哈希模块" URL="\ref groupGrouphash"];
* automaton -> common;
* compiler -> common;
* compiler -> automaton;
* grouphash -> common;
* grouphash -> automaton;
* grouphash -> compiler;
* }
* @enddot
*
* @section directory 三、目录结构
* <div class="generic_text_mapping_table">
* <table>
* <tr>
* <th align="left">.\\include</th>
* <td>软件包提供的接口头文件。</td>
* </tr>
* <tr>
* <th align="left">.\\bin</th>
* <td>软件包编译生成的二进制文件，包括.dll和.lib文件。</td>
* </tr>
* <tr>
* <th align="left">.\\common</th>
* <td>@ref groupCommon 源代码。</td>
* </tr>
* <tr>
* <th align="left">.\\automaton</th>
* <td>@ref groupAutomaton 源代码。</td>
* </tr>
* <tr>
* <th align="left">.\\compiler</th>
* <td>@ref groupCompiler 源代码。</td>
* </tr>
* <tr>
* <th align="left">.\\grouphash</th>
* <td>@ref groupGrouphash 源代码。</td>
* </tr>
* </table>
* </div>
*
* @section compile 四、编译
* 在Visual Studio 2012中打开解决方案文件（位于.\\SnortCompiler.sln）直接编译即可。
*
* @section call 五、调用
* - 将软件提供的.\\include目录作为解决方案的包含目录。
* - 将软件提供的.\\lib目录作为解决方案的库目录。
* - 在调用程序编译完成之后，将软件提供的所有*.dll文件复制到调用程序的工作路径中。
*/
