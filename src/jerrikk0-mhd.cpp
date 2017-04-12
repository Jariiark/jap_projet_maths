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
	Y[0] = W[0];
	Y[1] = W[1]/ W[0];
	Y[3] = W[3]/ W[0];
	Y[4] = W[4]/ W[0];
	Y[2] = (_GAM - 1) * (W[2] - W[0]/2 * ((Y[1]*Y[1]) + (Y[3]*Y[3] + Y[4]*Y[4]) - 1./2 * (W[5]*W[5] + W[6]*W[6] + W[7]*W[7])));
	Y[5] = W[5];
	Y[6] = W[6];
	Y[7] = W[7];
	Y[8] = W[8];
}
// }}}

// conservatives {{{
void conservatives(real * Y, real * W)
{
	W[0] = Y[0];
	W[1] = Y[1] * Y[0];
	W[2] = Y[2]/(_GAM - 1) + Y[0]/2 * (Y[1]*Y[1] + Y[3]*Y[3] + Y[4]*Y[4]) + 1./2 * (Y[5]*Y[5] + Y[6]*Y[6] + Y[7]*Y[7]);
	W[3] = Y[3] * Y[0];
	W[4] = Y[4] * Y[0];
	W[5] = Y[5];
	W[6] = Y[6];
	W[7] = Y[7];
	W[8] = Y[8];
}
// }}}

// InitData {{{
/*
 1 * rô = conservatives(_GAM);
 2 u(x)
 3 P
 4	u(y)
 */
void InitData(real *Wn1)
{
	real W[_M];
	
	for(int j = 0; j < _NY; j++) {
		for(int i = 0; i < _NX; i++) {
			real xx = 1. / _NX * (i + 0.5);
			real yy = 1. / _NY * (j + 0.5);
			real x, y;
			
			Ref2PhysMap(&xx, &yy, &x, &y);
			Wexact(&x, &y, W);
			
			for (int k = 0; k < _M; ++k)
				Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK] = W[k];
		}
	}
	
// 	for(int j = 0; j < _NYTRANSBLOCK; j++) {
// 		for(int i = 0; i < _NXTRANSBLOCK; i++) {
// 			real xx = ((real)i) / _NXTRANSBLOCK;
// 			real yy = ((real)i) / _NXTRANSBLOCK;
// 			real x, y;
// 			
// 			Ref2PhysMap(&xx, &yy, &x, &y);
// 			Wexact(&x, &y, W);
// 			
// 			for (int k = 0; k < _M; ++k)
// 				Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK] = W[k];
// 		}
// 	}
}
// }}}

// TimeStepCPU {{{
void TimeStepCPU_1D(real *Wn1, real *dtt)
{
	real WL[_M];
	real WR[_M];
	real WM[_M];
	real fluxWL[_M];
	real fluxWR[_M];
	real vnX[3] = {1, 0, 0};
	real dx = (_XMAX - _XMIN) / _NX;
	real xMin = _XMIN;
	real xMax = _XMAX;
	real yZero = 0;
	
	for (int j = 0; j < _NYTRANSBLOCK; ++j) {
		for (int i = 0; i < _NXTRANSBLOCK; ++i) {
			for (int k = 0; k < _M; ++k) {
				
				if (i <= 0) {
					Wexact(&xMin, &yZero, WL);
					WR[k] = Wn1[i + 1 + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK];
				}
				else if (i >= _NXTRANSBLOCK) {
					WL[k] = Wn1[i - 1 + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK];
					Wexact(&xMax, &yZero, WR);
				}
				else {
					WL[k] = Wn1[i - 1 + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK];
					WL[k] = Wn1[i + 1 + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK];
				}
			
				flux(WL, vnX, fluxWL);
				flux(WR, vnX, fluxWR);
				
				for (int k = 0; k < _M; ++k) {
					int n = i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK;
					Wn1[n] = Wn1[n] - (1./2 * (real)(fluxWR[k] + fluxWL[k]) - _CH/2 * (WR[k] - WL[k]));
				}
			}
		}
	}
}

void TimeStepCPU_2D(real *Wn1, real *dtt)
{
	real W[_M], fX[_M], fY[_M];
	real vnX[3] = {1, 0, 0};
	real vnY[3] = {0, 1, 0};
	
	for(int j = 0; j < _NY; j++) {
		for(int i = 0; i < _NX; i++) {
			for (int k = 0; k < _M; ++k) {
				W[k] = Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK];
				flux(W, vnX, fX);
				flux(fX, vnY, fY);
				Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK] = fY[k];
			}
		}
	}
}
// }}}
