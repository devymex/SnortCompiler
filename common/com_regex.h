/*!
* @page pageRegex 正则表达式处理
*
* @tableofcontents
*
* 此页面对软件所处理的正则表达式进行了说明，包括可以处理的情况，不能处理的情况，
* 以及如何处理等。
*
* @section secPcreIntro PCRE介绍
* PCRE（Perl Compatibel Regulate Express）为一种与Perl语言的语法相兼容的正则表达式。
* Snort规则定义的选项中包括使用PCRE编写的匹配模式，能够以正则表达式的方式来匹配数据包。
*
* @subsection subPcreOpt pcre选项
*
* @subsection subPcreSuf pcre选项的修饰项
*
* @section secRegex 正则表达式语法
* 正则表达式包括许多不同的语法结构，有些可以编译为状态机，有些则不能。下面将分别进行介绍。
*
* @subsection subSupport 支持的语法列表
*
* @subsection subUnsupport 不支持的语法列表
*
* @subsection subLazy 懒惰处理
*
* @section secCompile 编译正则表达式
* 以下将针对不同的语法结构说明正则表达式编译为NFA的方法。
* 
* @subsection subMetaF 元字符“^”
* 
* @subsection subMetaE 元字符“$”
*
* @subsection subRepeat 元字符“+”
*
* @subsection subRepeat 元字符“*”
*
* @subsection subRepeat 元字符“{}”
*
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 4 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="c"];
* 3 -> 4 [label="任意"];
* }
* @enddot
*
*/
