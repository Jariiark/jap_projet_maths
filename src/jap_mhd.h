#ifndef JAP_MHD_H
#define JAP_MHD_H

// include {{{
#include <math.h>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;
// }}}

typedef float real;

void conservatives(real *, real *);

void Ref2PhysMap(real *, real *, real *, real *);

void InitData(real *);

void TimeStepCPU(real *, real *);

#endif
