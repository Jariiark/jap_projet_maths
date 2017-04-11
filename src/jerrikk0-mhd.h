#ifndef JERRIKK0_MHD_H
	#define JERRIKK0_MHD_H
	
	#include "mhd.h"
	
	void InitData(real *);
	void conservatives(real *, real *);
	void Ref2PhysMap(real *, real *, real *, real *);
	void TimeStepCPU(real *, real *);
	void primitives(real *, real *);

#endif
