/*!
* @page pageRegex ������ʽ����
*
* @tableofcontents
*
* ��ҳ�������������������ʽ������˵�����������Դ������������ܴ���������
* �Լ���δ���ȡ�
*
* @section secPcreIntro PCRE����
* PCRE��Perl Compatibel Regulate Express��Ϊһ����Perl���Ե��﷨����ݵ�������ʽ��
* Snort�������ѡ���а���ʹ��PCRE��д��ƥ��ģʽ���ܹ���������ʽ�ķ�ʽ��ƥ�����ݰ���
*
* @subsection subPcreOpt pcreѡ��
*
* @subsection subPcreSuf pcreѡ���������
*
* @section secRegex ������ʽ�﷨
* ������ʽ������಻ͬ���﷨�ṹ����Щ���Ա���Ϊ״̬������Щ���ܡ����潫�ֱ���н��ܡ�
*
* @subsection subSupport ֧�ֵ��﷨�б�
*
* @subsection subUnsupport ��֧�ֵ��﷨�б�
*
* @subsection subLazy ���账��
*
* @section secCompile ����������ʽ
* ���½���Բ�ͬ���﷨�ṹ˵��������ʽ����ΪNFA�ķ�����
* 
* @subsection subMetaF Ԫ�ַ���^��
* ^	����Ŀ���ַ����Ŀ�ʼλ��(�ڶ���ģʽ��Ϊ����)
* �ڶ���ģʽ�¹���nfa�ķ�ʽ�������£�
* ��/^abc/m��
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 5 [peripheries=2];
* s -> 1 [label="��"];
* s -> 2 [label="��"];
* 1 -> 1 [label="Any"];
* 1 -> 2 [label="\n"];
* 2 -> 3 [label="a"];
* 3 -> 4 [label="b"];
* 4 -> 5 [label="c"];
* }
* @enddot
* 
* @subsection subMetaE Ԫ�ַ���$��
* $	����Ŀ���ַ�����ĩβλ��(�ڶ���ģʽ��Ϊ��β)
* �ڶ���ģʽ�¹���nfa�ķ�ʽ�������£�
* 1����/abc$/m��
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
* 3 -> 5 [label="��"];
* 4 -> 4 [label="Any"];
* 4 -> 5 [label="��"];
* }
* @enddot
* 
* 2����/ab(c|$)/m��
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
* 3 -> 4 [label="��"];
* 2 -> 4 [label="��"];
* }
* @enddot
* 
* @subsection subRepeat Ԫ�ַ���?��
* ?	0����1��
* ����nfa�ķ�ʽ�������£�
* ��/ab?c/��
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 3 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 2 [label="b"];
* 1 -> 2 [label="��"];
* 2 -> 3 [label="c"];
* }
* @enddot
*
* @subsection subRepeat Ԫ�ַ���*��
* *	0����������
* ����nfa�ķ�ʽ�������£�
* ��/ab*c/��
* 
* @dot
* digraph onedfa {
* graph [rankdir="LR"];
* node [shape=record];
* node [shape="circle"];
* 3 [peripheries=2];
* s -> 1 [label="a"];
* 1 -> 1 [label="b"];
* 1 -> 2 [label="��"];
* 2 -> 3 [label="c"];
* }
* @enddot
*
* @subsection subRepeat Ԫ�ַ���+��
* +	1����������
* ����nfa�ķ�ʽ�������£�
* ��/ab+c/��	==>	��/abb*c/��
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
* 2 -> 3 [label="��"];
* 3 -> 4 [label="c"];
* }
* @enddot
*
* @subsection subRepeat Ԫ�ַ���{}��
* 1��{m} ����m��
* ����nfa�ķ�ʽ�������£�
* ��/ab{2}c/��	==>	��/abbc/��
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
* 2��{m,n} ���ٳ���m��,�������n��
* ����nfa�ķ�ʽ�������£�
* ��/ab{2,4}c/��	==>	��/abbb?b?c/��
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
* 3 -> 4 [label="��"];
* 4 -> 5 [label="b"];
* 4 -> 5 [label="��"];
* 5 -> 6 [label="c"];
* }
* @enddot
*
* 3��{m,} ���ٳ���m��
* ����nfa�ķ�ʽ�������£�
* ��/ab{2,}c/��	==>	��/abbb*c/��
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
* 3 -> 4 [label="��"];
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
* 3 -> 4 [label="����"];
* }
* @enddot
*
*/
