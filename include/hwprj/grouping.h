#pragma once

#include <hwprj\compres.h>
#include <hwprj\groupres.h>

#ifndef GROUPINGHDR_DS
#define GROUPINGHDR __declspec(dllimport)
#else
#define GROUPINGHDR __declspec(dllexport)
#endif

GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);
