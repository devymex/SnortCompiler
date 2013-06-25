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
* 模式选项。净载中必须出现的字节串的模式，用与Perl语言兼容的正则表达式表示。
*
* @subsection subPcreSuf pcre选项的修饰项
*
* @li	i	大小写不敏感的修饰符
* @li	m	多行匹配, 默认情况下，PCRE认为目标字符串是一组单行字符组成的(然而实际上它可能会包含多行)。 
*			元字符^仅匹配字符串的开始位置，而元字符$则仅匹配字符串的末尾，或者新行之前(除非设置了D修饰符)。
*			使用后，^和$会匹配每一行的开始和结尾。如果目标字符串中没有/n字符或者模式中没有出现^和$，则设置此修饰符是没有任何影响的。
* @li	s	用来增强字符类.(点号)的功能，默认情况下，点号匹配任意字符，但不包括换行符。使用此修饰符后，可以匹配任意字符包括换行符。
* @li	x	如果设置了这个修饰符，模式中的空白字符，除非被转义或者在一个字符类中，否则都被忽略。 
*			在一个未转义的#之后直到下一个换行符之间的内容都会被正则表达式引擎忽略以方便添加注释。
* @li	e	使用此修饰符后，preg_replace的$replacement参数除了可以使用正常的反向引用来取得匹配值，
			还可以在其中书写eval语法允许的字符串进行求值，并将返回结果用于最终的替换。
* @li	A	如果设置了此修饰符，模式被强制成为”anchored”(锚点)，也就是说约束使其始终从字符串的最前面开始搜索。 
*			这个影响也可以通过构造适当的规则来实现。
* @li	D	如果设置了这个修饰符， $会匹配目标字符串的结尾之前，而如果没有设置这个修饰符，如果目标字符串最后一个字符是换行符，
*			$就会匹配该换行符之前而不是字符串结尾之前。如果设置了m修饰符，则这个修饰符会被忽略。
* @li	S	当一个模式需要多次使用的时候，我们为了获得更高的匹配效率，值得花费一些时间对其进行分析。如果设置了这个修饰符， 
*			将会进行这个额外的分析。当前，这种对一个模式的分析仅仅适用于非锚点的匹配(即没有一个单一固定的开始字符)。
* @li	U	这个修饰符逆转了贪婪的模式，如果没有这个设置，默认情况下的正则匹配时贪婪的，量词后增加了?的模式是非贪婪的，但是如果设置了这个修饰符，则刚好相反。
* @li	X	这个修饰符启用了一个PCRE中与perl不兼容的额外功能。任意反斜线后面跟一个没有特殊含义的字符会导致一个错误，以此来保留这些组合以备后期扩展。
*			默认情况下，和perl一样，反斜线后跟一个没有特殊含义的字符会以该字符原意解释。
* @li	J	允许子组有重复的名字。
* @li	u	模式字符串被认为是UTF-8的。 
*
* @section secRegex 正则表达式语法
* 正则表达式包括许多不同的语法结构，有些可以编译为状态机，有些则不能。下面将分别进行介绍。
*
* @subsection subSupport 支持的语法列表
*
* @li 元字符@n
* “.”：匹配除换行符以外的任意字符@n
* “\\w”：匹配字母或数字或下划线或汉字@n
* “\\s”：匹配任意的空白符@n
* “\\d”：匹配数字@n
* “\\b”：匹配单词的开始或结束@n
* “^”：匹配字符串的开始@n
* “$”：匹配字符串的结束@n
* 例：一个网站如果要求你填写的QQ号必须为5位到12位数字时，可以使用：“^\d{5,12}$”。
*
* @li 字符转义@n
* 反斜线后跟一个没有特殊含义的字符会以该字符原意解释.@n
* 例："deerchao\.net"匹配"deerchao.net"，"C:\\Windows"匹配"C:\Windows"。
*
* @li 重复@n
* "*":重复零次或多次@n
* "+":重复一次或多次@n
* "?":重复零次或一次@n
* "{n}":重复n次@n
* "{n,}":至少重复n次@n
* "{n,m}":重复n到m次@n
* 例："Windows\d+"匹配"Windows"后面跟1个或更多数字。
*
* @li 字符类@n
* 使用"[]"用于匹配没有预定义元字符的字符集合@n
* 例：[aeiou]匹配任何一个英文元音字母@n
*
* @li 分枝条件@n
* 正则表达式里的分枝条件指的是有几种规则，如果满足其中任意一种规则都应该当成匹配，具体方法是用|把不同的规则分隔开。@n
* 例："0\d{2}-\d{8}|0\d{3}-\d{7}"这个表达式能匹配两种以连字号分隔的电话号码：一种是三位区号，
*8位本地号(如010-12345678)，一种是4位区号，7位本地号(0376-2233445)。
* 
* @li 反义@n
* 用于表示不属于某个能简单定义的字符类的字符。@n
* "\\W":匹配任意不是字母，数字，下划线，汉字的字符@n
* "\\S":匹配任意不是空白符的字符@n
* "\\D":匹配任意非数字的字符@n
* "[^x]":匹配除了x以外的任意字符@n
* 例："<a[^>]+>"匹配用尖括号括起来的以a开头的字符串。
* 
* @subsection subUnsupport 不支持的语法列表@n
*			\\B	匹配不是单词开头或结束的位置@n
*
* @li 分组
* @li 后向引用
* @li 零宽断言
* @li 负向零宽断言
* @li 注释
* @li 贪婪与懒惰
* @li 处理选项
* @li 平衡组/递归匹配
*
* @subsection subLazy 懒惰处理
*
* @section secCompile 编译正则表达式
* 以下将针对不同的语法结构说明正则表达式编译为NFA的方法。
* 
* @subsection subMetaF 元字符“^”
* <b>含义：</b>代表目标字符串的开始位置(在多行模式下为行首)
* 在多行模式下构建nfa的方式举例如下：
* “/^abc/m”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 5 [peripheries=2, width=0.3];
* s -> 1 [label="ε"];
* s -> 2 [label="ε"];
* 1 -> 1 [label="Any"];
* 1 -> 2 [label="\\n"];
* 2 -> 3 [label="a"];
* 3 -> 4 [label="b"];
* 4 -> 5 [label="c"];
* }
* @enddot
* 
* @subsection subMetaE 元字符“$”
* <b>含义：</b>代表目标字符串的末尾位置(在多行模式下为行尾)
* 在多行模式下构建nfa的方式举例如下：@n
* - “/abc$/m”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 5 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="c"];
* 3 -> 4 [label="\\n"];
* 3 -> 5 [label="ε"];
* 4 -> 4 [label="Any"];
* 4 -> 5 [label="ε"];
* }
* @enddot
* 
* - “/ab(c|$)/m”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 4 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="c"];
* 2 -> 3 [label="\\n"];
* 3 -> 3 [label="Any"];
* 3 -> 4 [label="ε"];
* 2 -> 4 [label="ε"];
* }
* @enddot
* 
* @subsection subMetaQuest 元字符“?”
* <b>含义：</b>0个或1个
* 构建nfa的方式举例如下：
* “/ab?c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 3 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 1 -> 2 [label="ε"];
* 2 -> 3 [label="c"];
* }
* @enddot
*
* @subsection subMetaStar 重复“*”
* <b>含义：</b>0个或任意多个
* 构建nfa的方式举例如下：
* “/ab*c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 3 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 1 [label="b"];
* 1 -> 2 [label="ε"];
* 2 -> 3 [label="c"];
* }
* @enddot
*
* @subsection subMetaPlus 重复“+”
* <b>含义：</b>1个或任意多个
* 构建nfa的方式举例如下：
* “/ab+c/”	==>	“/abb*c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 4 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 2 [label="b"];
* 2 -> 3 [label="ε"];
* 3 -> 4 [label="c"];
* }
* @enddot
*
* @subsection subMetaBrace 重复“{}”
* - {m} <b>含义：</b>出现m次
* 构建nfa的方式举例如下：
* “/ab{2}c/”	==>	“/abbc/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 4 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="b"];
* 3 -> 4 [label="c"];
* }
* @enddot
*
* - {m,n} <b>含义：</b>至少出现m次,至多出现n次
* 构建nfa的方式举例如下：
* “/ab{2,4}c/”	==>	“/abbb?b?c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 6 [peripheries=2, width=0.3];
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
* - {m,} <b>含义：</b>至少出现m次
* 构建nfa的方式举例如下：
* “/ab{2,}c/”	==>	“/abbb*c/”
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape="circle", width=0.4, fontsize=14, fixedsize=true];
* 5 [peripheries=2, width=0.3];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 2 -> 3 [label="b"];
* 3 -> 3 [label="b"];
* 3 -> 4 [label="ε"];
* 4 -> 5 [label="c"];
* }
* @enddot
*
*/
