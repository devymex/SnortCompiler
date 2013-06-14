#include <hwprj\common.h>
#include <hwprj\nfa.h>
#include <hwprj\signatures.h>

void GetSignature(BYTEARY &code, std::vector<BYTEARY> &strs);

ulong PcreToNFA(const char *pPcre, CNfa &nfa, CSignatures &sigs);
