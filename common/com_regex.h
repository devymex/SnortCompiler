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
* 模式选项。净载中必须出现的字节串的模式，用与Perl语言兼容的正则表达式表式。
*
* @subsection subPcreSuf pcre选项的修饰项
*
* @li	i	大小写不敏感的修饰符
* @li	m	多行匹配, 默认情况下, PCRE认为目标字符串是一组单行字符组成的(然而实际上它可能会包含多行). 
*			元字符^仅匹配字符串的开始位置，而元字符$则仅匹配字符串的末尾, 或者新行之前(除非设置了D修饰符).
*			使用后, ^和$会匹配每一行的开始和结尾.如果目标字符串中没有/n字符或者模式中没有出现^和$, 则设置此修饰符是没有任何影响的.
* @li	s	用来增强字符类.(点号)的功能, 默认情况下, 点号匹配任意字符, 但不包括换行符. 使用此修饰符后, 可以匹配任意字符包括换行符.
* @li	x	如果设置了这个修饰符, 模式中的空白字符, 除非被转义或者在一个字符类中, 否则都被忽略. 
*			在一个未转义的#之后直到下一个换行符之间的内容都会被正则表达式引擎忽略以方便添加注释.
* @li	A	如果设置了此修饰符, 模式被强制成为”anchored”(锚点), 也就是说约束使其始终从字符串的最前面开始搜索. 
*			这个影响也可以通过构造适当的规则来实现.
* @li	E	如果设置了这个修饰符, $会匹配目标字符串的结尾之前, 而如果没有设置这个修饰符,如果目标字符串最后一个字符是换行符, 
*			$就会匹配该换行符之前而不是字符串结尾之前.	
* @li	G	这个修饰符逆转了贪婪的模式, 如果没有这个设置, 默认情况下的正则匹配时贪婪的, 量词后增加了?的模式是非贪婪的, 但是如果设置了这个修饰符, 则刚好相反. 
* @li	R	约束pcre选项相对于前一个匹配项成功匹配的串尾开始搜索.
* @li	U	约束pcre选项匹配解码后的URI缓冲区（类似于uricontent和http_uri).
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
* 例：一个网站如果要求你填写的QQ号必须为5位到12位数字时，可以使用：“^\d{5,12}$”.
*
* @li 字符转义@n
* 反斜线后跟一个没有特殊含义的字符会以该字符原意解释.@n
* 例："deerchao\.net"匹配"deerchao.net"，"C:\\Windows"匹配"C:\Windows"  
*
* @li 重复@n
* "*"		重复零次或多次@n
* "+"		重复一次或多次@n
* "?"		重复零次或一次@n
* "{n}"		重复n次@n
* "{n,}"	至少重复n次@n
* "{n,m}"	重复n到m次@n
* 例："Windows\d+"匹配"Windows"后面跟1个或更多数字.
*
* @li 字符类@n
* 使用"[]"用于匹配没有预定义元字符的字符集合@n
* 例："[aeiou]"匹配任何一个英文元音字母@n
*
* @li 分枝条件@n
* 正则表达式里的分枝条件指的是有几种规则，如果满足其中任意一种规则都应该当成匹配，具体方法是用|把不同的规则分隔开.@n
* 例："0\d{2}-\d{8}|0\d{3}-\d{7}"这个表达式能匹配两种以连字号分隔的电话号码：一种是三位区号，8位本地号(如010-12345678)，一种是4位区号，7位本地号(0376-2233445).
* 
* @li 反义@n
* 用于表示不属于某个能简单定义的字符类的字符.@n
* "\W"		匹配任意不是字母，数字，下划线，汉字的字符@n
* "\S"		匹配任意不是空白符的字符@n
* "\D"		匹配任意非数字的字符@n
* "[^x]"	匹配除了x以外的任意字符@n
* 例："<a[^>]+>"匹配用尖括号括起来的以a开头的字符串.
* 
* @subsection subUnsupport 不支持的语法列表@n
*
* @li 分组@n
* 用小括号来指定子表达式(也叫做分组).@n
* 例："(\d{1,3}\.){3}"匹配三位数字加上一个英文句号(这个整体也就是这个分组)重复3次.
* 
* @li 后向引用@n
*  默认情况下，每个分组会自动拥有一个组号，规则是：从左向右，以分组的左括号为标志，第一个出现的分组的组号为1，第二个为2，以此类推.
* 后向引用用于重复搜索前面某个分组匹配的文本.
* 例：\b(\w+)\b\s+\1\b可以用来匹配重复的单词，像go go, 或者kitty kitty. 这个表达式首先是一个单词，
* 也就是单词开始处和结束处之间的多于一个的字母或数字(\b(\w+)\b)，这个单词会被捕获到编号为1的分组中，
* 然后是1个或几个空白符(\s+)，最后是分组1中捕获的内容（也就是前面匹配的那个单词）(\1).
* 
* @li 零宽断言@n
* 断言用来声明一个应该为真的事实。正则表达式中只有当断言为真时才会继续进行匹配。零宽断言是用于查找在某些内容(但并不包括这些内容)之前或之后的东西，
* 也就是说它们像\b,^,$那样用于指定一个位置，这个位置应该满足一定的条件(即断言)。@n
* “(?=exp)”也叫零宽度正预测先行断言：它断言自身出现的位置的后面能匹配表达式exp。比如\b\w+(?=ing\b)，
* 匹配以ing结尾的单词的前面部分(除了ing以外的部分)，如查找I'm singing while you're dancing.时，它会匹配sing和danc。@n
* “(?<=exp)”也叫零宽度正回顾后发断言：它断言自身出现的位置的前面能匹配表达式exp。比如(?<=\bre)\w+\b会匹配以re开头的单词的后半部分
* (除了re以外的部分)，例如在查找reading a book时，它匹配ading。
* 
* @li 负向零宽断言@n
* 负向零宽断言只匹配一个位置，并不消费任何字符。@n
* 零宽度负预测先行断言“(?!exp)”：断言此位置的后面不能匹配表达式exp。例如：\d{3}(?!\d)匹配三位数字，而且这三位数字的后面不能是数字。@n
* 零宽度负回顾后发断言"(?<!exp)":断言此位置的前面不能匹配表达式exp。例如：(?<![a-z])\d{7}匹配前面不是小写字母的七位数字。
* 
* @li 注释@n
* 小括号的另一种用途是通过语法(?#comment)来包含注释。
* 
* @li 贪婪@n
* 当正则表达式中包含能接受重复的限定符时，匹配尽可能多的字符。
* 例：a.*b，它将会匹配最长的以a开始，以b结束的字符串。如果用它来搜索aabab的话，它会匹配整个字符串aabab。
* 
* @li 处理选项@n
* IgnoreCase(忽略大小写)：匹配时不区分大小写。@n
* Multiline(多行模式):更改^和$的含义，使它们分别在任意一行的行首和行尾匹配，而不仅仅在整个字符串的开头和结尾匹配。(在此模式下,$的精确含意是:匹配\n之前的位置以及字符串结束前的位置。)。@n
* Singleline(单行模式):更改.的含义，使它与每一个字符匹配（包括换行符\n）。@n
* IgnorePatternWhitespace(忽略空白):忽略表达式中的非转义空白并启用由#标记的注释。@n
* ExplicitCapture(显式捕获):仅捕获已被显式命名的组。
* 
* @li 平衡组/递归匹配@n
* 匹配可嵌套性的层次结构。@n
* 需要用到以下的语法构造：@n
* “(?'group')”：把捕获的内容命名为group,并压入堆栈(Stack)。@n
* "(?'-group')":从堆栈上弹出最后压入堆栈的名为group的捕获内容，如果堆栈本来为空，则本分组的匹配失败@n
* "(?(group)yes|no)":如果堆栈上存在以名为group的捕获内容的话，继续匹配yes部分的表达式，否则继续匹配no部分@n
* "(?!)":零宽负向先行断言，由于没有后缀表达式，试图匹配总是失败
*
* @subsection subLazy 懒惰处理
* @li	懒惰匹配@n
* 当正则表达式中包含能接受重复的限定符时，匹配尽可能少的字符。本项目的pcre选项处理采用懒惰匹配。@n
* 例：a.*?b匹配最短的，以a开始，以b结束的字符串。如果把它应用于aabab的话，它会匹配aab（第一到第三个字符）和ab（第四到第五个字符）。@n
* @li	懒惰限定符@n
* "*?"		重复任意次，但尽可能少重复。
* "+?"		重复1次或更多次，但尽可能少重复。
* "??"		重复0次或1次，但尽可能少重复
* "{n,m}?"	重复n到m次，但尽可能少重复。
* "{n,}?"	重复n次以上，但尽可能少重复。
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
