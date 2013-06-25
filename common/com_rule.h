/*!
* @page pageRule 规则处理
*
* @tableofcontents
*
* 此页面介绍了从解析Snort规则的处理方法和逻辑。
* 
* @section secSnortOption Snort规则选项
* 
* Snort规则语法的语义非常丰富，能够以很多种方式精确的描述一个数据包的匹配方式，
* 并对匹配后的处理做出规定。由于状态机理论本身的局限性，以状态机的方式来编译数据包
* 无法支持全部的Snort规则选项，以下列出所有可以处理的Snort规则选项列表。
* @li <b>content</b>：Snort规则模式选项。
* @li <b>nocase</b>：content及uricontent选项的修饰选项。
* @li <b>offset</b>：content及uricontent选项的修饰选项。
* @li <b>depth</b>：content及uricontent选项的修饰选项。
* @li <b>distance</b>：content及uricontent选项的修饰选项。
* @li <b>within</b>：content及uricontent选项的修饰选项。
* @li <b>uricontent</b>：Snort规则模式选项。
* @li <b>pcre</b>：Snort规则模式选项。
*
* @subsection subContentOption content选项
* 
* content选项包含混合的文本和二进制数据。二进制数据通常用管道“|”符号括起来并以十六进制
*表示的字节码的方式指定。@n
*
* content选项带有多个修饰项。这些修饰选项如下：@n
* <b>nocase</b>：修饰紧接自身之前的content选项，使其在匹配时忽略大小写。@n
* <b>示例</b>：alert tcp any any -> any 21 (msg:"FTP ROOT"; content:"USER";
* nocase;)，表示数据包匹配时，"user"或"USER"或"uSER"等均能匹配该content内容。@n
*
* <b>offset</b>：修饰紧接自身之前的content选项，允许指定从数据包净载的第几个字节
* 开始进行匹配。@n
* <b>示例</b>：alert tcp any any -> any 80 (content: "cgi-bin/phf"; offset:4;)，
*表示从数据包净载的第5个字节开始查找"cgi-bin/phf"字符串。@n
*
* <b>depth</b>：
* 修饰紧接自身之前的content选项，允许指定在数据区中搜索特定模式的深度。@n
* <b>示例</b>：alert tcp any any -> any 80 (content: "root"; depth:10;)，
*表示从数据包净载的起始位置的10个字节中查找"root"字符串。@n
*
* <b>distance</b>：
* 修饰紧接自身之前的content选项，允许指定相对于上一个content选项匹配成功的串尾再加
*几个字节开始搜索distance修饰的content选项指定的匹配内容。@n
* <b>示例</b>：alert tcp any any -> any any (content:"ABC"; content: "DEF";
*distance:1;)，表示从数据包中已经匹配到"ABC"的位置开始，跳过1个字节，
* 再开始查找"DEF"字符串。@n
*
* <b>within</b>：
* 修饰紧接自身之前的content选项，允许指定相对于上一个content选项匹配成功的串尾开始，
*特定的数据区内搜索within修饰的content选项指定的匹配内容。@n
* <b>示例</b>：alert tcp any any -> any any (content:"ABC"; content: "EFG";
*within:10;)，表示从数据包中已经匹配到"ABC"的位置开始的10个字节内查找"EFG"字符串。@n
*
* 注：content选项内容之前放置了“!”字符，表示在数据中不包含选项内容时引发报警。
*对于包含此类content的规则不作处理。@n
*
* @subsection subUriOption uricontent选项
* 
* uricontent选项用于匹配标准化后的URI字段。uricontent一般与HTTP检测预处理器结合使用。@n
* 举例：若URI："/cgi-bin/aaaaaaaaaaaaaaaaaaaaaaaaaa/..%252fp%68f?"
* 标准化之后："/cgi-bin/phf?"，则uricontent选项内容为"/cgi-bin/phf?"。@n
* uricontent包含与content相同的修饰项，对于修饰项的处理，
*可参考content选项中关于修饰项的说明。@n
*
* 注：对于匹配没有标准化之前的内容，使用content选项匹配。@n
*
* @subsection subPcreOption pcre选项
* 
* pcre选项允许使用与PERL语言相兼容的正则表达式。@n
* 相关正则表达式的详细内容，请参见@ref secPcreIntro @n
*
* @section subRuleLogic 规则数据结构
*
* @subsection subChainStruct 选项的链式结构
* 
* 模式选项有两种匹配位置：@n
* (1)从数据包净载中的开始位置进行匹配，称为@b 起点匹配；@n
* (2)以前一个选项成功匹配结束后净载的位置为起点，经过指定数值范围的跳转后再开始匹配，
*称为@b 连续匹配。@n
*
* 根据Snort规则语法的规定可知：不带有修饰选项的content和uricontent模式选项以及不包括“/R”
*约束的pcre为起点匹配；含有distance或within约束的content、uricontent以及包括“/R”约束
*的pcre则是连续匹配。将规则中需要从数据包净载的起始位置开始匹配的一个选项，以及该选项之后的
*多个连续匹配选项按顺序连接在一起，称为<b>选项链</b>。一条选项链可能只有一个选项，
*也可能包含多个具有连续匹配关系的选项。选项链之间的连接关系可由正则表达式表达。@n
*
* @subsection subChainClass 规则的类与对象
*
* @li CRuleOption： Snort规则选项的基类，用来表示一个选项及其相关修饰项的数据。
*相关结构定义请参见@ref CRuleOption　@n
*
* @li CContentOption：从CRuleOption类继承，管理一个content选项的数据及其修饰选项的值，并
*将转换为CPcreOption。相关结构定义请参见@ref CContentOption　@n
*
* @li CPcreOption：从CRuleOption类继承，管理一个pcre选项的数据，包括正则表达式语句及修饰选项。
*相关结构定义请参见@ref CPcreOption　@n
*
* @section secRuleProc 规则的处理
*
* @subsection subChainCompress 选项链的删除
* 
* 根据对Snort规则的分析发现：一些包含content选项和pcre选项的规则中，会出现content内容
* 与pcre内容部分重复的情况。@n
* 对于该情况的处理，在保证规则含义不改变的前提下，剔除重复内容。@n
*
* @subsection subRuleSig 规则的特征字符串
*
* 分组后每一组要选出一个或多个全局唯一的特征字符串（Signature）来代表该组，因此从选项链
* 中提取Signature是分组的前提。
* - 从content和uricontent中提取Signature的方法：@n
*
* 从Snort规则的content和uricontent字段中提取出的连续的4Byte的字符串，
* 一个长度为n的选项，可提出n-4个Signautre。@n
* 例如：某一规则中的content为“NetBus”，则最多可提出3个初始的Signature：“NetB”、
* “etBu”和“tBus”。@n
*
* - subPcreSig 从pcre中提取Signature的方法：@n
* 
*/
