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
	Y[2] = (_GAM - 1) * (W[2] - W[0]/2 * (Y[1]*Y[1] + Y[3]*Y[3] + Y[4]*Y[4])) - 0.5 * (W[5]*W[5] + W[6]*W[6] + W[7]*W[7]);
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
	W[2] = Y[2]/(_GAM - 1) + Y[0]/2 * (Y[1]*Y[1] + Y[3]*Y[3] + Y[4]*Y[4]) + 0.5 * (Y[5]*Y[5] + Y[6]*Y[6] + Y[7]*Y[7]);
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

void flux(real *WL, real *WR, real *vn, real *fluxLR)
{
	real fluxL[_M];
	real fluxR[_M];
	
	flux(WL, vn, fluxL);
	flux(WR, vn, fluxR);
	
	for (int i = 0; i < _M; ++i) {
		fluxLR[i] = 0.5 * (fluxL[i] + fluxR[i]) - _CH * 0.5 * (WR[i] - WL[i]);
	}
}

// TimeStepCPU {{{
void TimeStepCPU_1D(real *Wn1, real *dtt)
{
	real WL[_M];
	real WM[_M];
	real WR[_M];
	real fluxLM[_M];
	real fluxMR[_M];
	real vnX[3] = {1, 0, 0};
	real dx = (real)_LONGUEURX / _NBWORKSX;
	real Wn2[_NX * _NY * _M];
	
	*dtt = _CFL * dx / _CH;
	//*dtt = .000001;
	//cout << "dx= " << dx << endl << "dt= " << *dtt << endl;
	
	for (int j = 0; j < _NY; ++j) {
		for (int i = 0; i < _NX; ++i) {
			for (int k = 0; k < _M; ++k) {
				WM[k] = Wn1[i + k * _NX * _NY + j * _NX];
				
				if (i == 0) {
					real xx = 1. / _NX * (i + 0.5);
					real yy = 1. / _NY * (j + 0.5);
					real x, y;
					Ref2PhysMap(&xx, &yy, &x, &y);
					
					Wexact(&x, &y, WL);
					//WL[k] = 0;
					WR[k] = Wn1[i + 1 + k * _NX * _NY + j * _NX];
				}
				else if (i == _NX - 1) {
					real xx = 1. / _NX * (i + 0.5);
					real yy = 1. / _NY * (j + 0.5);
					real x, y;
					Ref2PhysMap(&xx, &yy, &x, &y);
					
					WL[k] = Wn1[i - 1 + k * _NX * _NY + j * _NX];
					Wexact(&x, &y, WR);
					//WR[k] = 0;
				}
				else {
					WL[k] = Wn1[i - 1 + k * _NX * _NY + j * _NX];
					WR[k] = Wn1[i + 1 + k * _NX * _NY + j * _NX];
				}				
				
				flux(WL, WM, vnX, fluxLM);
				flux(WM, WR, vnX, fluxMR);
					
				int n = i + k * _NX * _NY + j * _NX;
				Wn2[n] = Wn1[n] - *dtt/dx * (fluxMR[n] - fluxLM[n]);
			}
		}
	}
	
	for (int i = 0; i < _NX * _NY * _M; ++i)
		Wn1[i] = Wn2[i];
}

void TimeStepCPU_2D(real *Wn1, real *dtt)
{
	real WL[_M];
	real WM[_M];
	real WU[_M];
	real WD[_M];
	real WR[_M];
	real fluxLM[_M];
	real fluxMR[_M];
	real fluxUM[_M];
	real fluxMD[_M];
	real vnX[3] = {1, 0, 0};
	real vnY[3] = {0, 1, 0};
	real dx = (real)_LONGUEURX / _NBWORKSX;
	real dy = (real)_LONGUEURY / _NBWORKSY;
	real Wn2[_NX * _NY * _M];
	
	*dtt = _CFL * dx / _CH;
	//*dtt = .000001;
	//cout << "dx= " << dx << endl << "dt= " << *dtt << endl;
	
	for (int j = 0; j < _NY; ++j) {
		for (int i = 0; i < _NX; ++i) {
			for (int k = 0; k < _M; ++k) {
				WM[k] = Wn1[i + k * _NX * _NY + j * _NX];
				
				if (i == 0) {
					real xx = 1. / _NX * (i + 0.5);
					real yy = 1. / _NY * (j + 0.5);
					real x, y;
					Ref2PhysMap(&xx, &yy, &x, &y);
					
					Wexact(&x, &y, WL);
					//WL[k] = 0;
					WR[k] = Wn1[i + 1 + k * _NX * _NY + j * _NX];
				}
				else if (i == _NX - 1) {
					real xx = 1. / _NX * (i + 0.5);
					real yy = 1. / _NY * (j + 0.5);
					real x, y;
					Ref2PhysMap(&xx, &yy, &x, &y);
					
					WL[k] = Wn1[i - 1 + k * _NX * _NY + j * _NX];
					Wexact(&x, &y, WR);
					//WR[k] = 0;
				}
				else {
					WL[k] = Wn1[i - 1 + k * _NX * _NY + j * _NX];
					WR[k] = Wn1[i + 1 + k * _NX * _NY + j * _NX];
				}
				
				if (j == 0) {
					real xx = 1. / _NX * (i + 0.5);
					real yy = 1. / _NY * (j + 0.5);
					real x, y;
					Ref2PhysMap(&xx, &yy, &x, &y);
					
					Wexact(&x, &y, WU);
					WD[k] = Wn1[i + k * _NX * _NY + (j + 1) * _NX];
				}
				else if (j == _NY - 1) {
					real xx = 1. / _NX * (i + 0.5);
					real yy = 1. / _NY * (j + 0.5);
					real x, y;
					Ref2PhysMap(&xx, &yy, &x, &y);
					
					WU[k] = Wn1[i + k * _NX * _NY + (j - 1) * _NX];
					Wexact(&x, &y, WD);
				}
				else {
					WU[k] = Wn1[i + k * _NX * _NY + (j - 1) * _NX];
					WD[k] = Wn1[i + k * _NX * _NY + (j + 1) * _NX];
				}				
				
				flux(WL, WM, vnX, fluxLM);
				flux(WM, WR, vnX, fluxMR);
				flux(WU, WM, vnX, fluxUM);
				flux(WM, WD, vnX, fluxMD);
				
				int n = i + k * _NX * _NY + j * _NX;
				Wn2[n] = Wn1[n] - *dtt/dx * (fluxMR[n] - fluxLM[n]) - *dtt/dy * (fluxUM[n] - fluxMD[n]);
			}
		}
	}
	
	for (int i = 0; i < _NX * _NY * _M; ++i)
		Wn1[i] = Wn2[i];
}
// }}}
