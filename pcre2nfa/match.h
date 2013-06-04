#include "pcre.h"

#ifndef MATCH_H_
#define MATCHSC __declspec(dllimport)
#else
#define MATCHSC __declspec(dllexport)
#endif

#define OVECCOUNT 30 /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

MATCHSC bool match(char* src, int length, std::string Regex, int &Pos);
