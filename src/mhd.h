// vim: set sw=4 ts=4 sts=4 et tw=78 foldmarker={{{,}}} foldlevel=0 foldmethod=marker spell:

#ifndef MHD_H
	#define MHD_H
	
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
	
	// define {{{
	
	// Dans ce cas sequentiel, pour modifier le nombre de mailles il ne faut
	// modifier que _NBWORKSX et _NBWORKSY
	
	#define _M (9)                                      //Number of conservative variable
	#define _TMAX (1)
	#define _GAP (0)                                    // taille du recouvrement entre les work groupe
	#define _TRANSBLOCK 1                               // size of the cached transposed block
	
	// X
	#define _NBWORKSX (1<<7)                            // number of work-items in a work-group
	#define _NBLOCKSX (1)                               // number of work-groups
	#define _NX (_NBLOCKSX*(_NBWORKSX-2*_GAP)+2*_GAP)   // number of volume finite
	#define _NXTRANSBLOCK ( (_NX%_TRANSBLOCK==0)? _NX : _NX+_TRANSBLOCK-_NX%_TRANSBLOCK )
	// Y
	#define _NBWORKSY (1<<7)
	#define _NBLOCKSY (1)
	#define _NY (_NBLOCKSY*(_NBWORKSY-2*_GAP)+2*_GAP)
	#define _NYTRANSBLOCK ( (_NY%_TRANSBLOCK==0)? _NY : _NY+_TRANSBLOCK-_NY%_TRANSBLOCK )
	
	#define _CFL (0.5)
	#define _SPLIT (1)                                  // affiche 1 maille sur _SPLIT dans le fichier .msh
	
	#define Min(a,b) (((a) < (b)) ? (a) : (b))
	#define Max(a,b) (((a) > (b)) ? (a) : (b))
	#define Abs(a) ((a) > (0) ? (a) : (-a))
	
	#ifdef _1D
		#define _LONGUEURX (10)                         //Longueur du domaine suivant x
		#define _LONGUEURY (10)                         //Longueur du domaine suivant y
		#define _XMIN (-5)
		#define _XMAX (5)
		#define _YMIN (-5)
		#define _YMAX (5)
		
		#define TimeStepCPU TimeStepCPU_1D 
	#endif
	
	#ifdef _2D
		// Orzag Tang
		#define _LONGUEURX (6.2831853)                  //Longueur du domaine suivant x
		#define _LONGUEURY (6.2831853)                  //Longueur du domaine suivant y
		#define _XMIN (0)
		#define _XMAX (6.2831853)
		#define _YMIN (0)
		#define _YMAX (6.2831853)
		
		#define TimeStepCPU TimeStepCPU_2D 
	#endif
	
	
	
	//#define _GAM (2)
	#define _GAM (1.666666666666)
	#define _PI (3.14159265359)
	#define _CH (5)
	
	typedef float real;
	
	// }}}
	
	void Wexact(real *, real *, real *);
	void flux(real *, real *, real *);
	
#endif
