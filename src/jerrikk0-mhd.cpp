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
void primitives(real *W, real *Y)
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
void conservatives(real *Y, real *W)
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

void printW(real *W, int n) {
	for (int k = 0; k < n; ++k) {
		cout << W[k] << " ";
		
		if(isnan(W[k]))
			exit(1);
	}
	
	cout << endl;
}

void printWM(real *Wn1, real *WM, int i, int j) {
	for (int k = 0; k < _M; ++k) {
		cout << WM[k] - Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK] << "    ";
		
		if (WM[k] - Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK] != 0)
			exit(1);
	}
	
	cout << endl;
}

void printWn12(real *Wn1, real *Wn2, int i) {
	cout << Wn1[i] - Wn2[i] << " ";
	
	if (Wn1[i] - Wn2[i] != 0)
		exit(1);
}

void vector(real *Wn1, int i, int j, real *W)
{
	for (int k = 0; k < _M; ++k)
		W[k] = Wn1[i + k * _NXTRANSBLOCK * _NYTRANSBLOCK + j * _NXTRANSBLOCK];
}

void flux(real *WL, real *WR, real *vn, real *fluxLR)
{
	real fluxL[_M];
	real fluxR[_M];
	
	flux(WL, vn, fluxL);
	flux(WR, vn, fluxR);
	
	for (int k = 0; k < _M; ++k) {
		fluxLR[k] = 0.5 * (fluxL[k] + fluxR[k]) - _CH * 0.5 * (WR[k] - WL[k]);
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
	real dx = _LONGUEURX * 1. / _NBWORKSX;
	int NX = _NXTRANSBLOCK;
	int NY = _NYTRANSBLOCK;
	real Wn2[NX * NY * _M];
	
	*dtt = _CFL * dx / _CH;
	
	for (int j = 0; j < NY; ++j) {
		for (int i = 0; i < NX; ++i) {
			
			vector(Wn1, i, j, WM);
			
			if (i == 0) {
				real x = _XMIN, y = 0;
				Wexact(&x, &y, WL);
			} else {
				vector(Wn1, i - 1, j, WL);
			}
			
			if (i == NX - 1) {
				real x = _XMAX, y = 0;
				Wexact(&x, &y, WR);
			}
			else {
				vector(Wn1, i + 1, j, WR);
			}
			
			flux(WL, WM, vnX, fluxLM);
			flux(WM, WR, vnX, fluxMR);
			
			for (int k = 0; k < _M; ++k) {
				int n = i + k * NX * NY + j * NX;
				Wn2[n] = WM[k] - (*dtt)/dx * (fluxMR[k] - fluxLM[k]);
			}
		}
	}
	
	for (int i = 0; i < NX * NY * _M; ++i) {
		Wn1[i] = Wn2[i];
	}
}

void TimeStepCPU_2D(real *Wn1, real *dtt)
{
	real WM[_M];
	real WL[_M];
	real WR[_M];
	real WU[_M];
	real WD[_M];
	real fluxLM[_M];
	real fluxMR[_M];
	real fluxDM[_M];
	real fluxMU[_M];
	real vnX[3] = {1, 0, 0};
	real vnY[3] = {0, 1, 0};
	real dx = _LONGUEURX * 1. / _NBWORKSX;
	real dy = _LONGUEURY * 1. / _NBWORKSY;
	int NX = _NXTRANSBLOCK;
	int NY = _NYTRANSBLOCK;
	real Wn2[NX * NY * _M];
	
	*dtt = _CFL * dx / _CH;
	
	for (int j = 0; j < NY; ++j) {
		for (int i = 0; i < NX; ++i) {
			
			vector(Wn1, i, j, WM);
			
			if (i == 0) {
				real xx = (i + 0.5) / NX;
				real yy = (j + 0.5) / NY;
				real x, y;
				Ref2PhysMap(&xx, &yy, &x, &y);
				Wexact(&x, &y, WL);
			} else {
				vector(Wn1, i - 1, j, WL);
			}
			
			if (i == NX - 1) {
				real xx = (i + 0.5) / NX;
				real yy = (j + 0.5) / NY;
				real x, y;
				Ref2PhysMap(&xx, &yy, &x, &y);
				Wexact(&x, &y, WR);
			}
			else {
				vector(Wn1, i + 1, j, WR);
			}
			
			if (j == 0) {
				real xx = (i + 0.5) / NX;
				real yy = (j + 0.5) / NY;
				real x, y;
				Ref2PhysMap(&xx, &yy, &x, &y);
				Wexact(&x, &y, WD);
			} else {
				vector(Wn1, i, j - 1, WD);
			}
			
			if (j == NY - 1) {
				real xx = (i + 0.5) / NX;
				real yy = (j + 0.5) / NY;
				real x, y;
				Ref2PhysMap(&xx, &yy, &x, &y);
				Wexact(&x, &y, WU);
			}
			else {
				vector(Wn1, i, j + 1, WU);
			}
			
			flux(WL, WM, vnX, fluxLM);
			flux(WM, WR, vnX, fluxMR);
			flux(WD, WM, vnY, fluxDM);
			flux(WM, WU, vnY, fluxMU);
			
			for (int k = 0; k < _M; ++k) {
				int n = i + k * NX * NY + j * NX;
				Wn2[n] = WM[k] - (*dtt)/dx * (fluxMR[k] - fluxLM[k]) - (*dtt)/dy * (fluxMU[k] - fluxDM[k]);
			}
		}
	}
	
	for (int i = 0; i < NX * NY * _M; ++i) {
		Wn1[i] = Wn2[i];
	}
}
// }}}
