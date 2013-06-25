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
* ^	代表目标字符串的开始位置(在多行模式下为行首)
* 在多行模式下构建nfa的方式举例如下：
* “/^abc/m”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 5 [peripheries=2];
* s -> 1 [label="ε"];
* s -> 2 [label="ε"];
* 1 -> 1 [label="Any"];
* 1 -> 2 [label="\n"];
* 2 -> 3 [label="a"];
* 3 -> 4 [label="b"];
* 4 -> 5 [label="c"];
* }
* @enddot
* 
* @subsection subMetaE 元字符“$”
* $	代表目标字符串的末尾位置(在多行模式下为行尾)
* 在多行模式下构建nfa的方式举例如下：
* 1、“/abc$/m”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 5 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="c"];
* 3 -> 4 [label="\n"];
* 3 -> 5 [label="ε"];
* 4 -> 4 [label="Any"];
* 4 -> 5 [label="ε"];
* }
* @enddot
* 
* 2、“/ab(c|$)/m”
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
* 2 -> 3 [label="\n"];
* 3 -> 3 [label="Any"];
* 3 -> 4 [label="ε"];
* 2 -> 4 [label="ε"];
* }
* @enddot
* 
* @subsection subRepeat 元字符“?”
* ?	0个或1个
* 构建nfa的方式举例如下：
* “/ab?c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 3 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 1 -> 2 [label="ε"];
* 2 -> 3 [label="c"];
* }
* @enddot
*
* @subsection subRepeat 元字符“*”
* *	0个或任意多个
* 构建nfa的方式举例如下：
* “/ab*c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 3 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 1 [label="b"];
* 1 -> 2 [label="ε"];
* 2 -> 3 [label="c"];
* }
* @enddot
*
* @subsection subRepeat 元字符“+”
* +	1个或任意多个
* 构建nfa的方式举例如下：
* “/ab+c/”	==>	“/abb*c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 4 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 2 [label="b"];
* 2 -> 3 [label="ε"];
* 3 -> 4 [label="c"];
* }
* @enddot
*
* @subsection subRepeat 元字符“{}”
* 1、{m} 出现m次
* 构建nfa的方式举例如下：
* “/ab{2}c/”	==>	“/abbc/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 4 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="b"];
* 3 -> 4 [label="c"];
* }
* @enddot
*
* 2、{m,n} 至少出现m次,至多出现n次
* 构建nfa的方式举例如下：
* “/ab{2,4}c/”	==>	“/abbb?b?c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 6 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="b"];
* 3 -> 4 [label="b"];
* 3 -> 4 [label="ε"];
* 4 -> 5 [label="b"];
* 4 -> 5 [label="ε"];
* 5 -> 6 [label="c"];
* }
* @enddot
*
* 3、{m,} 至少出现m次
* 构建nfa的方式举例如下：
* “/ab{2,}c/”	==>	“/abbb*c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 5 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="b"];
* 3 -> 3 [label="b"];
* 3 -> 4 [label="ε"];
* 4 -> 5 [label="c"];
* }
* @enddot
*
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
