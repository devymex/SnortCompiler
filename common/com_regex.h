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
* 
* @subsection subMetaE Ԫ�ַ���$��
*
* @subsection subRepeat Ԫ�ַ���+��
*
* @subsection subRepeat Ԫ�ַ���*��
*
* @subsection subRepeat Ԫ�ַ���{}��
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
