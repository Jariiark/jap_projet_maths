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
	Y[5] = (_GAM - 1) * (W[5] - W[1]/2 * ((W[2]/W[1] * W[2]/W[1]) + (W[3]/W[1] * W[3]/W[1]) + (W[4]/W[1] * W[4]/W[1]) - 1/2 * (W[6]*W[6] + W[7]*W[7] + W[8]*W[8])));
}

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

void Ref2PhysMap(real *xx, real *yy, real *x, real *y)
{
	*x = *xx * _LONGUEURX + _XMIN;
	*y = *yy * _LONGUEURY + _YMIN;
}

void TimeStepCPU_1D(real *, real *)
{

}

void TimeStepCPU_2D(real *, real *)
{

}
