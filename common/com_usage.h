/*!
* @page pageUsage ʹ��˵����
*
* @tableofcontents
*
* ��ҳ����������ʹ��������ʹ�÷���
*
* @section requirement һ��ƽ̨Ҫ��
* @li ����ϵͳ��ֻ����Microsoft Windows XP�����ϰ汾�Ĳ���ϵͳ�б�������С�
* @li ����������ֻ����Microsoft Visual Studio 2012����������ִ�б��롣
* @li ����Ҫ������Ҫ���������2.0GHz���ϵ����봦�������Լ�1GiB���ϵĿ����ڴ�ռ䡣
* @li �洢Ҫ��֧�ֽ��������д��Ӳ�̣����豣֤Ӳ�̵�ʣ��ռ������Ҫд������ݴ�С��
*
* @section module ����ģ��ṹ
*
* @dot
* digraph module {
* node [shape=record, fontname="Microsoft YaHei", fontsize=11];
* common [label="����ģ��" URL="\ref groupCommon"];
* automaton [label="�Զ���ģ��" URL="\ref groupAutomaton"];
* compiler [label="������ģ��" URL="\ref groupCompiler"];
* grouphash [label="�������ϣģ��" URL="\ref groupGrouphash"];
* automaton -> common;
* compiler -> common;
* compiler -> automaton;
* grouphash -> common;
* grouphash -> automaton;
* grouphash -> compiler;
* }
* @enddot
*
* @section directory ����Ŀ¼�ṹ
* <div class="generic_text_mapping_table">
* <table>
* <tr>
* <th align="left">.\\include</th>
* <td>������ṩ�Ľӿ�ͷ�ļ���</td>
* </tr>
* <tr>
* <th align="left">.\\bin</th>
* <td>������������ɵĶ������ļ�������.dll��.lib�ļ���</td>
* </tr>
* <tr>
* <th align="left">.\\common</th>
* <td>@ref groupCommon Դ���롣</td>
* </tr>
* <tr>
* <th align="left">.\\automaton</th>
* <td>@ref groupAutomaton Դ���롣</td>
* </tr>
* <tr>
* <th align="left">.\\compiler</th>
* <td>@ref groupCompiler Դ���롣</td>
* </tr>
* <tr>
* <th align="left">.\\grouphash</th>
* <td>@ref groupGrouphash Դ���롣</td>
* </tr>
* </table>
* </div>
*
* @section compile �ġ�����
* ��Visual Studio 2012�д򿪽�������ļ���λ��.\\SnortCompiler.sln��ֱ�ӱ��뼴�ɡ�
*
* @section call �塢����
* - ������ṩ��.\\includeĿ¼��Ϊ��������İ���Ŀ¼��
* - ������ṩ��.\\libĿ¼��Ϊ��������Ŀ�Ŀ¼��
* - �ڵ��ó���������֮�󣬽�����ṩ������*.dll�ļ����Ƶ����ó���Ĺ���·���С�
*/
