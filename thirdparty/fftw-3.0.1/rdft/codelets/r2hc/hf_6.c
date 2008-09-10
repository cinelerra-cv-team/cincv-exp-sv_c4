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
/* Generated on Sat Jul  5 21:56:54 EDT 2003 */

#include "codelet-rdft.h"

/* Generated by: /homee/stevenj/cvs/fftw3.0.1/genfft/gen_hc2hc -compact -variables 4 -n 6 -dit -name hf_6 -include hf.h */

/*
 * This function contains 46 FP additions, 28 FP multiplications,
 * (or, 32 additions, 14 multiplications, 14 fused multiply/add),
 * 23 stack variables, and 24 memory accesses
 */
/*
 * Generator Id's : 
 * $Id: algsimp.ml,v 1.7 2003/03/15 20:29:42 stevenj Exp $
 * $Id: fft.ml,v 1.2 2003/03/15 20:29:42 stevenj Exp $
 * $Id: gen_hc2hc.ml,v 1.9 2003/04/17 19:25:50 athena Exp $
 */

#include "hf.h"

static const R *hf_6(R *rio, R *iio, const R *W, stride ios, int m, int dist)
{
     DK(KP500000000, +0.500000000000000000000000000000000000000000000);
     DK(KP866025403, +0.866025403784438646763723170752936183471402627);
     int i;
     for (i = m - 2; i > 0; i = i - 2, rio = rio + dist, iio = iio - dist, W = W + 10) {
	  E T7, TS, Tv, TO, Ti, TI, Tw, TC, Tt, TJ, Tx, TF;
	  {
	       E T1, TN, T6, TM;
	       T1 = rio[0];
	       TN = iio[-WS(ios, 5)];
	       {
		    E T3, T5, T2, T4;
		    T3 = rio[WS(ios, 3)];
		    T5 = iio[-WS(ios, 2)];
		    T2 = W[4];
		    T4 = W[5];
		    T6 = FMA(T2, T3, T4 * T5);
		    TM = FNMS(T4, T3, T2 * T5);
	       }
	       T7 = T1 - T6;
	       TS = TN - TM;
	       Tv = T1 + T6;
	       TO = TM + TN;
	  }
	  {
	       E Tc, TA, Th, TB;
	       {
		    E T9, Tb, T8, Ta;
		    T9 = rio[WS(ios, 2)];
		    Tb = iio[-WS(ios, 3)];
		    T8 = W[2];
		    Ta = W[3];
		    Tc = FMA(T8, T9, Ta * Tb);
		    TA = FNMS(Ta, T9, T8 * Tb);
	       }
	       {
		    E Te, Tg, Td, Tf;
		    Te = rio[WS(ios, 5)];
		    Tg = iio[0];
		    Td = W[8];
		    Tf = W[9];
		    Th = FMA(Td, Te, Tf * Tg);
		    TB = FNMS(Tf, Te, Td * Tg);
	       }
	       Ti = Tc - Th;
	       TI = TA - TB;
	       Tw = Tc + Th;
	       TC = TA + TB;
	  }
	  {
	       E Tn, TD, Ts, TE;
	       {
		    E Tk, Tm, Tj, Tl;
		    Tk = rio[WS(ios, 4)];
		    Tm = iio[-WS(ios, 1)];
		    Tj = W[6];
		    Tl = W[7];
		    Tn = FMA(Tj, Tk, Tl * Tm);
		    TD = FNMS(Tl, Tk, Tj * Tm);
	       }
	       {
		    E Tp, Tr, To, Tq;
		    Tp = rio[WS(ios, 1)];
		    Tr = iio[-WS(ios, 4)];
		    To = W[0];
		    Tq = W[1];
		    Ts = FMA(To, Tp, Tq * Tr);
		    TE = FNMS(Tq, Tp, To * Tr);
	       }
	       Tt = Tn - Ts;
	       TJ = TE - TD;
	       Tx = Tn + Ts;
	       TF = TD + TE;
	  }
	  {
	       E TK, Tu, TH, TT, TR, TU;
	       TK = KP866025403 * (TI + TJ);
	       Tu = Ti + Tt;
	       TH = FNMS(KP500000000, Tu, T7);
	       iio[-WS(ios, 3)] = T7 + Tu;
	       rio[WS(ios, 1)] = TH + TK;
	       iio[-WS(ios, 5)] = TH - TK;
	       TT = KP866025403 * (Tt - Ti);
	       TR = TJ - TI;
	       TU = FMA(KP500000000, TR, TS);
	       rio[WS(ios, 3)] = TR - TS;
	       iio[-WS(ios, 1)] = TT + TU;
	       rio[WS(ios, 5)] = TT - TU;
	  }
	  {
	       E TG, Ty, Tz, TP, TL, TQ;
	       TG = KP866025403 * (TC - TF);
	       Ty = Tw + Tx;
	       Tz = FNMS(KP500000000, Ty, Tv);
	       rio[0] = Tv + Ty;
	       iio[-WS(ios, 4)] = Tz + TG;
	       rio[WS(ios, 2)] = Tz - TG;
	       TP = KP866025403 * (Tw - Tx);
	       TL = TC + TF;
	       TQ = FNMS(KP500000000, TL, TO);
	       iio[0] = TL + TO;
	       iio[-WS(ios, 2)] = TP + TQ;
	       rio[WS(ios, 4)] = TP - TQ;
	  }
     }
     return W;
}

static const tw_instr twinstr[] = {
     {TW_FULL, 0, 6},
     {TW_NEXT, 1, 0}
};

static const hc2hc_desc desc = { 6, "hf_6", twinstr, {32, 14, 14, 0}, &GENUS, 0, 0, 0 };

void X(codelet_hf_6) (planner *p) {
     X(khc2hc_dit_register) (p, hf_6, &desc);
}
