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
/* Generated on Sat Jul  5 22:22:02 EDT 2003 */

#include "codelet-rdft.h"

/* Generated by: /homee/stevenj/cvs/fftw3.0.1/genfft/gen_r2r -compact -variables 4 -redft01 -n 8 -name e01_8 -include r2r.h */

/*
 * This function contains 26 FP additions, 15 FP multiplications,
 * (or, 20 additions, 9 multiplications, 6 fused multiply/add),
 * 27 stack variables, and 16 memory accesses
 */
/*
 * Generator Id's : 
 * $Id: algsimp.ml,v 1.7 2003/03/15 20:29:42 stevenj Exp $
 * $Id: fft.ml,v 1.2 2003/03/15 20:29:42 stevenj Exp $
 * $Id: gen_r2r.ml,v 1.3 2003/04/17 19:25:50 athena Exp $
 */

#include "r2r.h"

static void e01_8_0(const R *I, R *O, stride istride, stride ostride)
{
     DK(KP1_662939224, +1.662939224605090474157576755235811513477121624);
     DK(KP1_111140466, +1.111140466039204449485661627897065748749874382);
     DK(KP390180644, +0.390180644032256535696569736954044481855383236);
     DK(KP1_961570560, +1.961570560806460898252364472268478073947867462);
     DK(KP707106781, +0.707106781186547524400844362104849039284835938);
     DK(KP1_414213562, +1.414213562373095048801688724209698078569671875);
     DK(KP765366864, +0.765366864730179543456919968060797733522689125);
     DK(KP1_847759065, +1.847759065022573512256366378793576573644833252);
     {
	  E T7, Tl, T4, Tk, Td, To, Tg, Tn;
	  {
	       E T5, T6, T1, T3, T2;
	       T5 = I[WS(istride, 2)];
	       T6 = I[WS(istride, 6)];
	       T7 = FMA(KP1_847759065, T5, KP765366864 * T6);
	       Tl = FNMS(KP1_847759065, T6, KP765366864 * T5);
	       T1 = I[0];
	       T2 = I[WS(istride, 4)];
	       T3 = KP1_414213562 * T2;
	       T4 = T1 + T3;
	       Tk = T1 - T3;
	       {
		    E T9, Tf, Tc, Te, Ta, Tb;
		    T9 = I[WS(istride, 1)];
		    Tf = I[WS(istride, 7)];
		    Ta = I[WS(istride, 5)];
		    Tb = I[WS(istride, 3)];
		    Tc = KP707106781 * (Ta + Tb);
		    Te = KP707106781 * (Ta - Tb);
		    Td = T9 + Tc;
		    To = Te + Tf;
		    Tg = Te - Tf;
		    Tn = T9 - Tc;
	       }
	  }
	  {
	       E T8, Th, Tq, Tr;
	       T8 = T4 + T7;
	       Th = FNMS(KP390180644, Tg, KP1_961570560 * Td);
	       O[WS(ostride, 7)] = T8 - Th;
	       O[0] = T8 + Th;
	       Tq = Tk - Tl;
	       Tr = FMA(KP1_111140466, Tn, KP1_662939224 * To);
	       O[WS(ostride, 5)] = Tq - Tr;
	       O[WS(ostride, 2)] = Tq + Tr;
	  }
	  {
	       E Ti, Tj, Tm, Tp;
	       Ti = T4 - T7;
	       Tj = FMA(KP390180644, Td, KP1_961570560 * Tg);
	       O[WS(ostride, 4)] = Ti - Tj;
	       O[WS(ostride, 3)] = Ti + Tj;
	       Tm = Tk + Tl;
	       Tp = FNMS(KP1_111140466, To, KP1_662939224 * Tn);
	       O[WS(ostride, 6)] = Tm - Tp;
	       O[WS(ostride, 1)] = Tm + Tp;
	  }
     }
}

static void e01_8(const R *I, R *O, stride is, stride os, int v, int ivs, int ovs)
{
     int i;
     for (i = v; i > 0; --i) {
	  e01_8_0(I, O, is, os);
	  I += ivs;
	  O += ovs;
     }
}

static const kr2r_desc desc = { 8, "e01_8", {20, 9, 6, 0}, &GENUS, REDFT01, 0, 0, 0, 0 };

void X(codelet_e01_8) (planner *p) {
     X(kr2r_register) (p, e01_8, &desc);
}
