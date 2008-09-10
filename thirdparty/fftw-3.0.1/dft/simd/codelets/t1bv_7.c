/*
 * Copyright (c) 2003 Matteo Frigo
 * Copyright (c) 2003 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Sat Jul  5 21:43:43 EDT 2003 */

#include "codelet-dft.h"

/* Generated by: /homee/stevenj/cvs/fftw3.0.1/genfft/gen_twiddle_c -simd -compact -variables 4 -n 7 -name t1bv_7 -include t1b.h -sign 1 */

/*
 * This function contains 36 FP additions, 30 FP multiplications,
 * (or, 24 additions, 18 multiplications, 12 fused multiply/add),
 * 21 stack variables, and 14 memory accesses
 */
/*
 * Generator Id's : 
 * $Id: algsimp.ml,v 1.7 2003/03/15 20:29:42 stevenj Exp $
 * $Id: fft.ml,v 1.2 2003/03/15 20:29:42 stevenj Exp $
 * $Id: gen_twiddle_c.ml,v 1.7 2003/04/16 19:51:27 athena Exp $
 */

#include "t1b.h"

static const R *t1bv_7(R *ri, R *ii, const R *W, stride ios, int m, int dist)
{
     DVK(KP222520933, +0.222520933956314404288902564496794759466355569);
     DVK(KP900968867, +0.900968867902419126236102319507445051165919162);
     DVK(KP623489801, +0.623489801858733530525004884004239810632274731);
     DVK(KP433883739, +0.433883739117558120475768332848358754609990728);
     DVK(KP781831482, +0.781831482468029808708444526674057750232334519);
     DVK(KP974927912, +0.974927912181823607018131682993931217232785801);
     int i;
     R *x;
     x = ii;
     BEGIN_SIMD();
     for (i = m; i > 0; i = i - VL, x = x + (VL * dist), W = W + (TWVL * 12)) {
	  V Th, Tf, Ti, T5, Tk, Ta, Tj, To, Tp;
	  Th = LD(&(x[0]), dist, &(x[0]));
	  {
	       V Tc, Te, Tb, Td;
	       Tb = LD(&(x[WS(ios, 2)]), dist, &(x[0]));
	       Tc = BYTW(&(W[TWVL * 2]), Tb);
	       Td = LD(&(x[WS(ios, 5)]), dist, &(x[WS(ios, 1)]));
	       Te = BYTW(&(W[TWVL * 8]), Td);
	       Tf = VSUB(Tc, Te);
	       Ti = VADD(Tc, Te);
	  }
	  {
	       V T2, T4, T1, T3;
	       T1 = LD(&(x[WS(ios, 1)]), dist, &(x[WS(ios, 1)]));
	       T2 = BYTW(&(W[0]), T1);
	       T3 = LD(&(x[WS(ios, 6)]), dist, &(x[0]));
	       T4 = BYTW(&(W[TWVL * 10]), T3);
	       T5 = VSUB(T2, T4);
	       Tk = VADD(T2, T4);
	  }
	  {
	       V T7, T9, T6, T8;
	       T6 = LD(&(x[WS(ios, 3)]), dist, &(x[WS(ios, 1)]));
	       T7 = BYTW(&(W[TWVL * 4]), T6);
	       T8 = LD(&(x[WS(ios, 4)]), dist, &(x[0]));
	       T9 = BYTW(&(W[TWVL * 6]), T8);
	       Ta = VSUB(T7, T9);
	       Tj = VADD(T7, T9);
	  }
	  ST(&(x[0]), VADD(Th, VADD(Tk, VADD(Ti, Tj))), dist, &(x[0]));
	  To = VBYI(VFNMS(LDK(KP781831482), Ta, VFNMS(LDK(KP433883739), Tf, VMUL(LDK(KP974927912), T5))));
	  Tp = VFMA(LDK(KP623489801), Tj, VFNMS(LDK(KP900968867), Ti, VFNMS(LDK(KP222520933), Tk, Th)));
	  ST(&(x[WS(ios, 2)]), VADD(To, Tp), dist, &(x[0]));
	  ST(&(x[WS(ios, 5)]), VSUB(Tp, To), dist, &(x[WS(ios, 1)]));
	  {
	       V Tg, Tl, Tm, Tn;
	       Tg = VBYI(VFMA(LDK(KP433883739), T5, VFNMS(LDK(KP781831482), Tf, VMUL(LDK(KP974927912), Ta))));
	       Tl = VFMA(LDK(KP623489801), Ti, VFNMS(LDK(KP222520933), Tj, VFNMS(LDK(KP900968867), Tk, Th)));
	       ST(&(x[WS(ios, 3)]), VADD(Tg, Tl), dist, &(x[WS(ios, 1)]));
	       ST(&(x[WS(ios, 4)]), VSUB(Tl, Tg), dist, &(x[0]));
	       Tm = VBYI(VFMA(LDK(KP781831482), T5, VFMA(LDK(KP974927912), Tf, VMUL(LDK(KP433883739), Ta))));
	       Tn = VFMA(LDK(KP623489801), Tk, VFNMS(LDK(KP900968867), Tj, VFNMS(LDK(KP222520933), Ti, Th)));
	       ST(&(x[WS(ios, 1)]), VADD(Tm, Tn), dist, &(x[WS(ios, 1)]));
	       ST(&(x[WS(ios, 6)]), VSUB(Tn, Tm), dist, &(x[0]));
	  }
     }
     END_SIMD();
     return W;
}

static const tw_instr twinstr[] = {
     VTW(1),
     VTW(2),
     VTW(3),
     VTW(4),
     VTW(5),
     VTW(6),
     {TW_NEXT, VL, 0}
};

static const ct_desc desc = { 7, "t1bv_7", twinstr, {24, 18, 12, 0}, &GENUS, 0, 0, 0 };

void X(codelet_t1bv_7) (planner *p) {
     X(kdft_dit_register) (p, t1bv_7, &desc);
}
