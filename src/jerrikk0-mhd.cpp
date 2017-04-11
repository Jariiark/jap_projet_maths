// vim: set sw=4 ts=4 sts=4 et tw=78 foldmarker,}}} foldlevel=0 foldmethod=marker spell:

#include "jerrikk0-mhd.h"

// Ref2PhysMap {{{
void Ref2PhysMap(real *xx, real *yy, real *x, real *y)
{
	*x = *xx * _LONGUEURX + _XMIN;
	*y = *yy * _LONGUEURY + _YMIN;
}
// }}}

// primitives {{{
void primitives(real * Y, real * W)
{
	Y[1] = W[1];
	Y[2] = W[2]/ W[1];
	Y[3] = W[3]/ W[1];
	Y[4] = W[4]/ W[1];
	Y[6] = W[6];
	Y[7] = W[7];
	Y[8] = W[8];
	Y[5] = (_GAM - 1) * (W[5] - W[1]/2 * ((W[2]/W[1] * W[2]/W[1]) + (W[3]/W[1] * W[3]/W[1]) + (W[4]/W[1] * W[4]/W[1]) - 1/2 * (W[6]*W[6] + W[7]*W[7] + W[8]*W[8])));
}
// }}}

// conservatives {{{
void conservatives(real * Y, real * W)
{
	W[1] = Y[1];
	W[2] = Y[2] * Y[1];
	W[3] = Y[3] * Y[1];
	W[4] = Y[4] * Y[1];
	W[6] = Y[6];
	W[7] = Y[7];
	W[8] = Y[8];
	//W[5] = (Y[5]+1/2*(Y[6]*Y[6]+Y[7]*Y[7]+Y[8]*Y[8]))/(_GAM-1)+Y[1]/2*(Y[2]*Y[2]+Y[3]*Y[3]+Y[4]*Y[4]);
	W[5] = Y[5]/(_GAM - 1) + Y[1]/2 * (Y[2]*Y[2] + Y[3]*Y[3] + Y[4]*Y[4]) + 1./2 * (Y[6]*Y[6] + Y[7]*Y[7] + Y[8]*Y[8]);
}
// }}}


// InitData {{{
/*
 1 * rô = conservatives(_GAM);
 2 u(x)
 3 P
 4	u(y)
 */
void InitData(real *Wh1)
{
	real x, y, xx, yy;
	real W[_M], Y[_M], consGam[_M];
	real gam[_M] = {_GAM, _GAM, _GAM, _GAM, _GAM, _GAM, _GAM, _GAM, _GAM};
	
// 	for (int i=0; i<_LONGUEURX; i++) {
// 		for(int j=0; j<_LONGUEURY; j++){
// 			xx = i*1.0/_NXTRANSBLOCK;
// 			yy = j*1.0/_NYTRANSBLOCK;
// 			Ref2PhysMap(&xx, &yy, &x, &y);
// 			Wexact(&x, &y, W);
// 			for(int iv=0;iv<_M;iv++){
// 				Wh1[iv*(int)_LONGUEURX*(int)_LONGUEURY+i+j]=W[iv];
// 			}
// 		}
// 	}
	
	for(int j = 0; j < _NY; j++) {
		for(int i = 0; i < _NX; i++) {
			real xx = 1. / _NX * (i + 0.5);
			real yy = 0;
			real x, y;
			
			Ref2PhysMap(&xx, &yy, &x, &y);
			Wexact(&x, &y, W);
			
			for (int k = 0; k < _M; ++k) {
				Wh1[i + 0 * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK] = W[i];
			}
		}
	}
}
// }}}

// TimeStepCPU {{{
void TimeStepCPU_1D(real *Wh1, real *dtt)
{
	
}

void TimeStepCPU_2D(real *, real *)
{
	
}
// }}}
