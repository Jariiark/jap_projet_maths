#ifndef JERRIKK0_MHD_H
#define JERRIKK0_MHD_H

#include <math.h>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define Min(a,b) (((a) < (b)) ? (a) : (b))
#define Max(a,b) (((a) > (b)) ? (a) : (b))
#define Abs(a) ((a) > (0) ? (a) : (-a))

//#define _GAM (2)
#define _GAM (1.666666666666)
#define _PI (3.14159265359)
#define _CH (5)

typedef float real;

void conservatives(real *, real *);

void Ref2PhysMap(real *, real *, real *, real *);

void InitData(real *);

void TimeStepCPU(real *, real *);

#endif
