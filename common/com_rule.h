/*!
* @page pageRule ������
*
* @tableofcontents
*
* ��ҳ������˴ӽ���Snort����Ĵ��������߼���
* 
* @section secSnortOption Snort����ѡ��
* 
* ����Ϊ���Դ����Snort����ѡ���б�
* @li content��Snort����ģʽѡ�
* @li nocase��content��uricontentѡ�������ѡ�
* @li offset��content��uricontentѡ�������ѡ�
* @li depth��content��uricontentѡ�������ѡ�
* @li distance��content��uricontentѡ�������ѡ�
* @li within��content��uricontentѡ�������ѡ�
* @li uricontent��Snort����ģʽѡ�
* @li pcre��Snort����ģʽѡ�
*
* @subsection subContentOption contentѡ��
* 
* contentѡ�������ϵ��ı��Ͷ��������ݡ�����������ͨ���ùܵ�(|)��������������ʮ�����Ʊ�ʾ���ֽ���ķ�ʽָ����@n
* contentѡ����ж���������Щ����ѡ�����£�@n
* @bnocase��
* ���ν�������֮ǰ��contentѡ�ʹ����ƥ��ʱ���Դ�Сд��@n
* ʾ����alert tcp any any -> any 21 (msg:"FTP ROOT"; content:"USER"; nocase;)��
*		��ʾ���ݰ�ƥ��ʱ��"user"��"USER"��"uSER"�Ⱦ���ƥ���content���ݡ�@n
*
* @boffset��
* ���ν�������֮ǰ��contentѡ�����ָ�������ݰ����صĵڼ����ֽڿ�ʼ����ƥ�䡣@n
* ʾ����alert tcp any any -> any 80 (content: "cgi-bin/phf"; offset:4;)��
*		��ʾ�����ݰ����صĵ�5���ֽڿ�ʼ����"cgi-bin/phf"�ַ�����@n
*
* @bdepth��
* ���ν�������֮ǰ��contentѡ�����ָ�����������������ض�ģʽ����ȡ�@n
* ʾ����alert tcp any any -> any 80 (content: "root"; depth:10;)��
*		��ʾ�����ݰ����ص���ʼλ�õ�10���ֽ��в���"root"�ַ�����@n
*
* @bdistance��
* ���ν�������֮ǰ��contentѡ�����ָ���������һ��contentѡ��ƥ��ɹ��Ĵ�β�ټ�
* �����ֽڿ�ʼ����distance���ε�contentѡ��ָ����ƥ�����ݡ�@n
* ʾ����alert tcp any any -> any any (content:"ABC"; content: "DEF";distance:1;)��
*		��ʾ�����ݰ����Ѿ�ƥ�䵽"ABC"��λ�ÿ�ʼ������1���ֽڣ��ٿ�ʼ����"DEF"�ַ�����@n
*
* @bwithin��
* ���ν�������֮ǰ��contentѡ�����ָ���������һ��contentѡ��ƥ��ɹ��Ĵ�β��ʼ��
* �ض���������������within���ε�contentѡ��ָ����ƥ�����ݡ�@n
* ʾ����alert tcp any any -> any any (content:"ABC"; content: "EFG"; within:10;)��
*		��ʾ�����ݰ����Ѿ�ƥ�䵽"ABC"��λ�ÿ�ʼ��10���ֽ��ڲ���"EFG"�ַ�����@n
*
* ע��contentѡ������֮ǰ�����ˡ�!���ַ�����ʾ�������в�����ѡ������ʱ�������������ڰ�������content��
* ����������@n
*
* @subsection subUriOption uricontentѡ��
* 
* uricontentѡ������ƥ���׼�����URI�ֶΡ�uricontentһ����HTTP���Ԥ���������ʹ�á�@n
* ��������URI��"/cgi-bin/aaaaaaaaaaaaaaaaaaaaaaaaaa/..%252fp%68f?"
* ��׼��֮��"/cgi-bin/phf?"����uricontentѡ������Ϊ"/cgi-bin/phf?"��@n
* uricontent������content��ͬ�����������������Ĵ����ɲο�contentѡ���й����������˵����@n
*
* ע������ƥ��û�б�׼��֮ǰ�����ݣ�ʹ��contentѡ��ƥ�䡣@n
*
* @subsection subPcreOption pcreѡ��
* 
* pcreѡ������ʹ����PERL��������ݵ�������ʽ��@n
* ���������ʽ����ϸ���ݣ���μ�@ref secPcreIntro@n
*
* @section subRuleLogic �����߼��ṹ
*
* @subsection subChainStruct ѡ�����ʽ�ṹ
* 
* ģʽѡ��������ƥ��λ�ã�@n
* (1)�����ݰ������еĿ�ʼλ�ý���ƥ�䣬��Ϊ
* @b���ƥ�䣻@n
* (2)��ǰһ��ѡ��ɹ�ƥ��������ص�λ��Ϊ��㣬����ָ����ֵ��Χ����ת���ٿ�ʼƥ�䣬��Ϊ
* @b����ƥ�䡣@n
*
* ����Snort�����﷨�Ĺ涨��֪������������ѡ���content��uricontentģʽѡ���Լ���������/R��Լ����pcreΪ���ƥ�䣻
* ����distance��withinԼ����content��uricontent�Լ�������/R��Լ����pcre��������ƥ�䡣
* ����������Ҫ�����ݰ����ص���ʼλ�ÿ�ʼƥ���һ��ѡ��Լ���ѡ��֮��Ķ������ƥ��ѡ�˳��������һ�𣬳�Ϊ
* @bѡ������
* һ��ѡ��������ֻ��һ��ѡ�Ҳ���ܰ��������������ƥ���ϵ��ѡ�ѡ����֮������ӹ�ϵ����������ʽ��@n
*
* @subsection subChainCompress ѡ������ɾ��
* 
* ���ݶ�Snort����ķ������֣�һЩ����contentѡ���pcreѡ��Ĺ����У������content������pcre���ݲ����ظ��������@n
* ���ڸ�����Ĵ����ڱ�֤�����岻�ı��ǰ���£��޳��ظ����ݡ�@n
*
* @section subRuleSignature ����������ַ���
* �����ÿһ��Ҫѡ��һ������ȫ��Ψһ�������ַ�����Signature����������飬��˴�ѡ��������ȡSignature�Ƿ����ǰ�ᡣ
* @subsection subConSig ��content��uricontent����ȡSignature
*
* ��Snort�����content��uricontent�ֶ�����ȡ����������4Byte���ַ�����
* һ������Ϊn��ѡ������n-4��Signautre��@n
* ���磺ĳһ�����е�contentΪ��NetBus�������������3����ʼ��Signature����NetB������etBu���͡�tBus����@n
*
* @subsection subPcreSig ��pcre����ȡSignature
* 
*/
