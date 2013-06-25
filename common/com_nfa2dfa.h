/*!
* @page pageNfa2dfa NFA到DFA
*
* @tableofcontents
*
* 此页面介绍了从NFA转换为DFA所使用的数据结构和相关算法。
* 
* @section secNfaData NFA的表示
* 
* NFA采用表式的结构存储。一个NFA对应一个跳转表，每一个状态对应表中的一行，
* 每一个跳转字符对应表中的一列。表中的每一个元素为一个状态编号集合，
* 元素@f$(i,j)@f$表示第@f$i@f$个状态经过字符j可以跳转到的状态。@n
*
* 在软件中，NFA跳转表的每一行数据由@ref CNfaRow 类封装，并由@ref CNfa 类封装
* 一组@CNfaRow，并提供了相关操作。

* @section secDfaData DFA的表示
* 
* DFA也是采用表式的结构存储。一个DFA对应一个跳转表，每一个状态对应表中的一行，
* 每一个跳转字符对应表中的一列。表中的每一个元素为一个状态，
* 元素@f$(i,j)@f$表示第@f$i@f$个状态经过字符@f$j@f$可以跳转到的状态。 @n
*
* 在软件中，DFA的跳转表的每一行数据由@ref CDfaRow 类封装，
* 并由@ref CDfa 类封装一组@ref CDfaRow，并提供了相关操作。

* @subsection subDfaFin DFA的终态
*
* @ref CDfa 中终态与其对应的DfaId的对应关系由类@ref CFinalStates来表示。
* 当前的DFA可能是由多个DFA合并而来，此时需记录@ref CDfa 的每一终态能够识别的合并之前的Dfaid。
* 在匹配过程中，若数据包匹配到了某一终态，则该数据包能匹配到该终态对应的所以的DFA。
* 
* @subsection subDfaFlags DFA的标记
*
* @ref CDfa 中的每一个状态都有一个标记，如果是终态，则标记为FINAL，否则标记为NORMAL。
* 另外，@ref CDfa 中还有一个标记用来指定起始状态。
*
* @section secDeterm NFA的确定化
* 
* 从NFA到DFA的转换采用经典的子集构造算法。@n
* 子集构造算法的基本思想为让构造得到的DFA的没一个个状态对应于NFA的一个状态集合。
* DFA在读入字符a1,a2,...an之后到达的状态对应于相应NFA从初始状态出发，
* 沿着以a1,a2,...an为标号的路径能到达的状态集合。
*
* @section secMinimize DFA的最小化
* 
*/
