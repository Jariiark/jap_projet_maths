#include "jerrikk0-mhd.h"

void primitives(real * Y, real * W)
{
	Y[1] = W[1];
	Y[2] = W[2]/ W[1];
	Y[3] = W[3]/ W[1];
	Y[4] = W[4]/ W[1];
	Y[6] = W[6];
	Y[7] = W[7];
	Y[8] = W[8];
	Y[5] = (_GAM-1)*(W[5]-W[1]/2*((W[2]/W[1]*W[2]/W[1])+(W[3]/W[1]*W[3]/W[1])+(W[4]/W[1]*W[4]/W[1])-1/2*(W[6]*W[6]+W[7]*W[7]+W[8]*W[8])));
}

void conservatives(real * Y, real * W)
{
	W[1] = Y[1];
	W[2] = Y[2]*W[1];
	W[3] = Y[3]*W[1];
	W[4] = Y[4]*W[1];
	W[6] = Y[6];
	W[7] = Y[7];
	W[8] = Y[8];
	W[5] = (Y[5]+1/2*(Y[6]*Y[6]+Y[7]*Y[7]+Y[8]*Y[8]))/(_GAM-1)+Y[1]/2*(Y[2]*Y[2]+Y[3]*Y[3]+Y[4]*Y[4]);
}

void Ref2PhysMap(real *xx, real *yy, real *x, real *y)
{
	*x = *xx * (_XMIN + _XMAX) / 2. + _XMIN;
	*y = *yy * (_YMIN + _YMAX) / 2. + _YMIN;
}


/*
1 rô = conservatives(_GAM);
2 u(x)
3 P
4	u(y)
*/
void InitData(real * Wh1)
{
	int iv;
	real x;
	real y;
	real xx;
	real yy;
	int i, j;

	for(iv=0;iv<_M;i++){
		Wh1[_X*_Y]=W[iv];
		for (i=iv*_X; i<iv_Y; i++) {
			for(j=iv*_Y; j<iv_Y; j++){
				xx = i*1.0/_NXTRANSBLOCK
				yy = j*1.0/_NYTRANSBLOCK
				Ref2PhysMap(xx, yy, x, y);
				Wh1[i+j]=Wexact(x,y);
			}
		}
	}
}

void TimeStepCPU_1D(real *, real *)
{
	
}

void TimeStepCPU_2D(real *, real *)
{
	
}
