#include "pcre.h"

#ifndef MATCHHDR_DS
#define MATCHHDR __declspec(dllimport)
#else
#define MATCHHDR __declspec(dllexport)
#endif

#define OVECCOUNT 30 /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

MATCHHDR bool match(const char* src, int length, std::string Regex, int &Pos);
