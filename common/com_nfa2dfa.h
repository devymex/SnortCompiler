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
* 元素<i,j>表示第i个状态状经过字符j可以跳转到的状态。@n
*
* 在软件中，NFA跳转表的每一行数据由@ref CNfaRow 类封装，并由@ref CNfa 类封装
* 一组@CNfaRow，并提供了相关操作。

* @section secDfaData DFA的表示
* 
* @subsection subDfaFin DFA的终态
* @subsection subDfaFlags DFA的标记
*
* @section secDeterm NFA的确定化
* 
* @section secMinimize DFA的最小化
* 
*/
