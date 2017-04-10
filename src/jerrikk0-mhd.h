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
	
	#ifdef _1D
		#define _LONGUEURX (10)							//Longueur du domaine suivant x
		#define _LONGUEURY (10)							//Longueur du domaine suivant y
		#define _XMIN (-5)
		#define _XMAX (5)
		#define _YMIN (-5)
		#define _YMAX (5)
	
		#define TimeStepCPU TimeStepCPU_1D
	#endif
	
	#ifdef _2D
		// Orzag Tang
		#define _LONGUEURX (6.2831853)					//Longueur du domaine suivant x
		#define _LONGUEURY (6.2831853)					//Longueur du domaine suivant y
		#define _XMIN (0)
		#define _XMAX (6.2831853)
		#define _YMIN (0)
		#define _YMAX (6.2831853)
	
		#define TimeStepCPU TimeStepCPU_2D
	#endif

	typedef float real;

	void conservatives(real *, real *);
	void Ref2PhysMap(real *, real *, real *, real *);
	void InitData(real *);
	void TimeStepCPU(real *, real *);
	void primitives(real *, real *);

#endif
