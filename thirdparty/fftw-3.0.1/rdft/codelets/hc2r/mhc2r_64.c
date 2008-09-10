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
/* Generated on Sat Jul  5 22:11:18 EDT 2003 */

#include "codelet-rdft.h"

/* Generated by: /homee/stevenj/cvs/fftw3.0.1/genfft/gen_hc2r_noinline -compact -variables 4 -sign 1 -n 64 -name mhc2r_64 -include hc2r.h */

/*
 * This function contains 394 FP additions, 134 FP multiplications,
 * (or, 342 additions, 82 multiplications, 52 fused multiply/add),
 * 109 stack variables, and 128 memory accesses
 */
/*
 * Generator Id's : 
 * $Id: algsimp.ml,v 1.7 2003/03/15 20:29:42 stevenj Exp $
 * $Id: fft.ml,v 1.2 2003/03/15 20:29:42 stevenj Exp $
 * $Id: gen_hc2r_noinline.ml,v 1.1 2003/04/17 19:25:50 athena Exp $
 */

#include "hc2r.h"

static void mhc2r_64_0(const R *ri, const R *ii, R *O, stride ris, stride iis, stride os)
{
     DK(KP1_268786568, +1.268786568327290996430343226450986741351374190);
     DK(KP1_546020906, +1.546020906725473921621813219516939601942082586);
     DK(KP196034280, +0.196034280659121203988391127777283691722273346);
     DK(KP1_990369453, +1.990369453344393772489673906218959843150949737);
     DK(KP942793473, +0.942793473651995297112775251810508755314920638);
     DK(KP1_763842528, +1.763842528696710059425513727320776699016885241);
     DK(KP580569354, +0.580569354508924735272384751634790549382952557);
     DK(KP1_913880671, +1.913880671464417729871595773960539938965698411);
     DK(KP1_111140466, +1.111140466039204449485661627897065748749874382);
     DK(KP1_662939224, +1.662939224605090474157576755235811513477121624);
     DK(KP390180644, +0.390180644032256535696569736954044481855383236);
     DK(KP1_961570560, +1.961570560806460898252364472268478073947867462);
     DK(KP923879532, +0.923879532511286756128183189396788286822416626);
     DK(KP382683432, +0.382683432365089771728459984030398866761344562);
     DK(KP707106781, +0.707106781186547524400844362104849039284835938);
     DK(KP765366864, +0.765366864730179543456919968060797733522689125);
     DK(KP1_847759065, +1.847759065022573512256366378793576573644833252);
     DK(KP1_414213562, +1.414213562373095048801688724209698078569671875);
     DK(KP2_000000000, +2.000000000000000000000000000000000000000000000);
     {
	  E Ta, T2S, T18, T2u, T3F, T4V, T5l, T61, Th, T2T, T1h, T2v, T3M, T4W, T5o;
	  E T62, T3Q, T5q, T5u, T44, Tp, Tw, T2V, T2W, T2X, T2Y, T3X, T5t, T1r, T2x;
	  E T41, T5r, T1A, T2y, T4a, T5y, T5N, T4H, TN, T31, T4E, T5z, T39, T3q, T1L;
	  E T2B, T4h, T5M, T2h, T2F, T12, T36, T5D, T5J, T5G, T5K, T1U, T26, T23, T27;
	  E T4p, T4z, T4w, T4A, T34, T3r;
	  {
	       E T5, T3A, T3, T3y, T9, T3C, T17, T3D, T6, T14;
	       {
		    E T4, T3z, T1, T2;
		    T4 = ri[WS(ris, 16)];
		    T5 = KP2_000000000 * T4;
		    T3z = ii[WS(iis, 16)];
		    T3A = KP2_000000000 * T3z;
		    T1 = ri[0];
		    T2 = ri[WS(ris, 32)];
		    T3 = T1 + T2;
		    T3y = T1 - T2;
		    {
			 E T7, T8, T15, T16;
			 T7 = ri[WS(ris, 8)];
			 T8 = ri[WS(ris, 24)];
			 T9 = KP2_000000000 * (T7 + T8);
			 T3C = T7 - T8;
			 T15 = ii[WS(iis, 8)];
			 T16 = ii[WS(iis, 24)];
			 T17 = KP2_000000000 * (T15 - T16);
			 T3D = T15 + T16;
		    }
	       }
	       T6 = T3 + T5;
	       Ta = T6 + T9;
	       T2S = T6 - T9;
	       T14 = T3 - T5;
	       T18 = T14 - T17;
	       T2u = T14 + T17;
	       {
		    E T3B, T3E, T5j, T5k;
		    T3B = T3y - T3A;
		    T3E = KP1_414213562 * (T3C - T3D);
		    T3F = T3B + T3E;
		    T4V = T3B - T3E;
		    T5j = T3y + T3A;
		    T5k = KP1_414213562 * (T3C + T3D);
		    T5l = T5j - T5k;
		    T61 = T5j + T5k;
	       }
	  }
	  {
	       E Td, T3G, T1c, T3K, Tg, T3J, T1f, T3H, T19, T1g;
	       {
		    E Tb, Tc, T1a, T1b;
		    Tb = ri[WS(ris, 4)];
		    Tc = ri[WS(ris, 28)];
		    Td = Tb + Tc;
		    T3G = Tb - Tc;
		    T1a = ii[WS(iis, 4)];
		    T1b = ii[WS(iis, 28)];
		    T1c = T1a - T1b;
		    T3K = T1a + T1b;
	       }
	       {
		    E Te, Tf, T1d, T1e;
		    Te = ri[WS(ris, 20)];
		    Tf = ri[WS(ris, 12)];
		    Tg = Te + Tf;
		    T3J = Te - Tf;
		    T1d = ii[WS(iis, 20)];
		    T1e = ii[WS(iis, 12)];
		    T1f = T1d - T1e;
		    T3H = T1d + T1e;
	       }
	       Th = KP2_000000000 * (Td + Tg);
	       T2T = KP2_000000000 * (T1f + T1c);
	       T19 = Td - Tg;
	       T1g = T1c - T1f;
	       T1h = KP1_414213562 * (T19 - T1g);
	       T2v = KP1_414213562 * (T19 + T1g);
	       {
		    E T3I, T3L, T5m, T5n;
		    T3I = T3G - T3H;
		    T3L = T3J + T3K;
		    T3M = FNMS(KP765366864, T3L, KP1_847759065 * T3I);
		    T4W = FMA(KP765366864, T3I, KP1_847759065 * T3L);
		    T5m = T3G + T3H;
		    T5n = T3K - T3J;
		    T5o = FNMS(KP1_847759065, T5n, KP765366864 * T5m);
		    T62 = FMA(KP1_847759065, T5m, KP765366864 * T5n);
	       }
	  }
	  {
	       E Tl, T3O, T1v, T43, To, T42, T1y, T3P, Ts, T3R, T1p, T3S, Tv, T3U, T1m;
	       E T3V;
	       {
		    E Tj, Tk, T1t, T1u;
		    Tj = ri[WS(ris, 2)];
		    Tk = ri[WS(ris, 30)];
		    Tl = Tj + Tk;
		    T3O = Tj - Tk;
		    T1t = ii[WS(iis, 2)];
		    T1u = ii[WS(iis, 30)];
		    T1v = T1t - T1u;
		    T43 = T1t + T1u;
	       }
	       {
		    E Tm, Tn, T1w, T1x;
		    Tm = ri[WS(ris, 18)];
		    Tn = ri[WS(ris, 14)];
		    To = Tm + Tn;
		    T42 = Tm - Tn;
		    T1w = ii[WS(iis, 18)];
		    T1x = ii[WS(iis, 14)];
		    T1y = T1w - T1x;
		    T3P = T1w + T1x;
	       }
	       {
		    E Tq, Tr, T1n, T1o;
		    Tq = ri[WS(ris, 10)];
		    Tr = ri[WS(ris, 22)];
		    Ts = Tq + Tr;
		    T3R = Tq - Tr;
		    T1n = ii[WS(iis, 10)];
		    T1o = ii[WS(iis, 22)];
		    T1p = T1n - T1o;
		    T3S = T1n + T1o;
	       }
	       {
		    E Tt, Tu, T1k, T1l;
		    Tt = ri[WS(ris, 6)];
		    Tu = ri[WS(ris, 26)];
		    Tv = Tt + Tu;
		    T3U = Tt - Tu;
		    T1k = ii[WS(iis, 26)];
		    T1l = ii[WS(iis, 6)];
		    T1m = T1k - T1l;
		    T3V = T1l + T1k;
	       }
	       T3Q = T3O - T3P;
	       T5q = T3O + T3P;
	       T5u = T43 - T42;
	       T44 = T42 + T43;
	       Tp = Tl + To;
	       Tw = Ts + Tv;
	       T2V = Tp - Tw;
	       {
		    E T3T, T3W, T1j, T1q;
		    T2W = T1y + T1v;
		    T2X = T1p + T1m;
		    T2Y = T2W - T2X;
		    T3T = T3R - T3S;
		    T3W = T3U - T3V;
		    T3X = KP707106781 * (T3T + T3W);
		    T5t = KP707106781 * (T3T - T3W);
		    T1j = Tl - To;
		    T1q = T1m - T1p;
		    T1r = T1j + T1q;
		    T2x = T1j - T1q;
		    {
			 E T3Z, T40, T1s, T1z;
			 T3Z = T3R + T3S;
			 T40 = T3U + T3V;
			 T41 = KP707106781 * (T3Z - T40);
			 T5r = KP707106781 * (T3Z + T40);
			 T1s = Ts - Tv;
			 T1z = T1v - T1y;
			 T1A = T1s + T1z;
			 T2y = T1z - T1s;
		    }
	       }
	  }
	  {
	       E TB, T48, T2c, T4G, TE, T4F, T2f, T49, TI, T4b, T1J, T4c, TL, T4e, T1G;
	       E T4f;
	       {
		    E Tz, TA, T2a, T2b;
		    Tz = ri[WS(ris, 1)];
		    TA = ri[WS(ris, 31)];
		    TB = Tz + TA;
		    T48 = Tz - TA;
		    T2a = ii[WS(iis, 1)];
		    T2b = ii[WS(iis, 31)];
		    T2c = T2a - T2b;
		    T4G = T2a + T2b;
	       }
	       {
		    E TC, TD, T2d, T2e;
		    TC = ri[WS(ris, 17)];
		    TD = ri[WS(ris, 15)];
		    TE = TC + TD;
		    T4F = TC - TD;
		    T2d = ii[WS(iis, 17)];
		    T2e = ii[WS(iis, 15)];
		    T2f = T2d - T2e;
		    T49 = T2d + T2e;
	       }
	       {
		    E TG, TH, T1H, T1I;
		    TG = ri[WS(ris, 9)];
		    TH = ri[WS(ris, 23)];
		    TI = TG + TH;
		    T4b = TG - TH;
		    T1H = ii[WS(iis, 9)];
		    T1I = ii[WS(iis, 23)];
		    T1J = T1H - T1I;
		    T4c = T1H + T1I;
	       }
	       {
		    E TJ, TK, T1E, T1F;
		    TJ = ri[WS(ris, 7)];
		    TK = ri[WS(ris, 25)];
		    TL = TJ + TK;
		    T4e = TJ - TK;
		    T1E = ii[WS(iis, 25)];
		    T1F = ii[WS(iis, 7)];
		    T1G = T1E - T1F;
		    T4f = T1F + T1E;
	       }
	       {
		    E TF, TM, T1D, T1K;
		    T4a = T48 - T49;
		    T5y = T48 + T49;
		    T5N = T4G - T4F;
		    T4H = T4F + T4G;
		    TF = TB + TE;
		    TM = TI + TL;
		    TN = TF + TM;
		    T31 = TF - TM;
		    {
			 E T4C, T4D, T37, T38;
			 T4C = T4b + T4c;
			 T4D = T4e + T4f;
			 T4E = KP707106781 * (T4C - T4D);
			 T5z = KP707106781 * (T4C + T4D);
			 T37 = T2f + T2c;
			 T38 = T1J + T1G;
			 T39 = T37 - T38;
			 T3q = T38 + T37;
		    }
		    T1D = TB - TE;
		    T1K = T1G - T1J;
		    T1L = T1D + T1K;
		    T2B = T1D - T1K;
		    {
			 E T4d, T4g, T29, T2g;
			 T4d = T4b - T4c;
			 T4g = T4e - T4f;
			 T4h = KP707106781 * (T4d + T4g);
			 T5M = KP707106781 * (T4d - T4g);
			 T29 = TI - TL;
			 T2g = T2c - T2f;
			 T2h = T29 + T2g;
			 T2F = T2g - T29;
		    }
	       }
	  }
	  {
	       E TQ, T4j, T1P, T4n, TT, T4m, T1S, T4k, TX, T4q, T1Y, T4u, T10, T4t, T21;
	       E T4r;
	       {
		    E TO, TP, T1N, T1O;
		    TO = ri[WS(ris, 5)];
		    TP = ri[WS(ris, 27)];
		    TQ = TO + TP;
		    T4j = TO - TP;
		    T1N = ii[WS(iis, 5)];
		    T1O = ii[WS(iis, 27)];
		    T1P = T1N - T1O;
		    T4n = T1N + T1O;
	       }
	       {
		    E TR, TS, T1Q, T1R;
		    TR = ri[WS(ris, 21)];
		    TS = ri[WS(ris, 11)];
		    TT = TR + TS;
		    T4m = TR - TS;
		    T1Q = ii[WS(iis, 21)];
		    T1R = ii[WS(iis, 11)];
		    T1S = T1Q - T1R;
		    T4k = T1Q + T1R;
	       }
	       {
		    E TV, TW, T1W, T1X;
		    TV = ri[WS(ris, 3)];
		    TW = ri[WS(ris, 29)];
		    TX = TV + TW;
		    T4q = TV - TW;
		    T1W = ii[WS(iis, 29)];
		    T1X = ii[WS(iis, 3)];
		    T1Y = T1W - T1X;
		    T4u = T1X + T1W;
	       }
	       {
		    E TY, TZ, T1Z, T20;
		    TY = ri[WS(ris, 13)];
		    TZ = ri[WS(ris, 19)];
		    T10 = TY + TZ;
		    T4t = TY - TZ;
		    T1Z = ii[WS(iis, 13)];
		    T20 = ii[WS(iis, 19)];
		    T21 = T1Z - T20;
		    T4r = T1Z + T20;
	       }
	       {
		    E TU, T11, T5B, T5C;
		    TU = TQ + TT;
		    T11 = TX + T10;
		    T12 = TU + T11;
		    T36 = TU - T11;
		    T5B = T4j + T4k;
		    T5C = T4n - T4m;
		    T5D = FNMS(KP923879532, T5C, KP382683432 * T5B);
		    T5J = FMA(KP923879532, T5B, KP382683432 * T5C);
	       }
	       {
		    E T5E, T5F, T1M, T1T;
		    T5E = T4q + T4r;
		    T5F = T4t + T4u;
		    T5G = FNMS(KP923879532, T5F, KP382683432 * T5E);
		    T5K = FMA(KP923879532, T5E, KP382683432 * T5F);
		    T1M = TQ - TT;
		    T1T = T1P - T1S;
		    T1U = T1M - T1T;
		    T26 = T1M + T1T;
	       }
	       {
		    E T1V, T22, T4l, T4o;
		    T1V = TX - T10;
		    T22 = T1Y - T21;
		    T23 = T1V + T22;
		    T27 = T22 - T1V;
		    T4l = T4j - T4k;
		    T4o = T4m + T4n;
		    T4p = FNMS(KP382683432, T4o, KP923879532 * T4l);
		    T4z = FMA(KP382683432, T4l, KP923879532 * T4o);
	       }
	       {
		    E T4s, T4v, T32, T33;
		    T4s = T4q - T4r;
		    T4v = T4t - T4u;
		    T4w = FMA(KP923879532, T4s, KP382683432 * T4v);
		    T4A = FNMS(KP382683432, T4s, KP923879532 * T4v);
		    T32 = T21 + T1Y;
		    T33 = T1S + T1P;
		    T34 = T32 - T33;
		    T3r = T33 + T32;
	       }
	  }
	  {
	       E T13, T3x, Ty, T3w, Ti, Tx;
	       T13 = KP2_000000000 * (TN + T12);
	       T3x = KP2_000000000 * (T3r + T3q);
	       Ti = Ta + Th;
	       Tx = KP2_000000000 * (Tp + Tw);
	       Ty = Ti + Tx;
	       T3w = Ti - Tx;
	       O[WS(os, 32)] = Ty - T13;
	       O[WS(os, 48)] = T3w + T3x;
	       O[0] = Ty + T13;
	       O[WS(os, 16)] = T3w - T3x;
	  }
	  {
	       E T3g, T3k, T3j, T3l;
	       {
		    E T3e, T3f, T3h, T3i;
		    T3e = T2S + T2T;
		    T3f = KP1_414213562 * (T2V + T2Y);
		    T3g = T3e - T3f;
		    T3k = T3e + T3f;
		    T3h = T31 - T34;
		    T3i = T39 - T36;
		    T3j = FNMS(KP1_847759065, T3i, KP765366864 * T3h);
		    T3l = FMA(KP1_847759065, T3h, KP765366864 * T3i);
	       }
	       O[WS(os, 44)] = T3g - T3j;
	       O[WS(os, 60)] = T3k + T3l;
	       O[WS(os, 12)] = T3g + T3j;
	       O[WS(os, 28)] = T3k - T3l;
	  }
	  {
	       E T3o, T3u, T3t, T3v;
	       {
		    E T3m, T3n, T3p, T3s;
		    T3m = Ta - Th;
		    T3n = KP2_000000000 * (T2X + T2W);
		    T3o = T3m - T3n;
		    T3u = T3m + T3n;
		    T3p = TN - T12;
		    T3s = T3q - T3r;
		    T3t = KP1_414213562 * (T3p - T3s);
		    T3v = KP1_414213562 * (T3p + T3s);
	       }
	       O[WS(os, 40)] = T3o - T3t;
	       O[WS(os, 56)] = T3u + T3v;
	       O[WS(os, 8)] = T3o + T3t;
	       O[WS(os, 24)] = T3u - T3v;
	  }
	  {
	       E T30, T3c, T3b, T3d;
	       {
		    E T2U, T2Z, T35, T3a;
		    T2U = T2S - T2T;
		    T2Z = KP1_414213562 * (T2V - T2Y);
		    T30 = T2U + T2Z;
		    T3c = T2U - T2Z;
		    T35 = T31 + T34;
		    T3a = T36 + T39;
		    T3b = FNMS(KP765366864, T3a, KP1_847759065 * T35);
		    T3d = FMA(KP765366864, T35, KP1_847759065 * T3a);
	       }
	       O[WS(os, 36)] = T30 - T3b;
	       O[WS(os, 52)] = T3c + T3d;
	       O[WS(os, 4)] = T30 + T3b;
	       O[WS(os, 20)] = T3c - T3d;
	  }
	  {
	       E T25, T2p, T2i, T2q, T1C, T2k, T2o, T2s, T24, T28;
	       T24 = KP707106781 * (T1U + T23);
	       T25 = T1L + T24;
	       T2p = T1L - T24;
	       T28 = KP707106781 * (T26 + T27);
	       T2i = T28 + T2h;
	       T2q = T2h - T28;
	       {
		    E T1i, T1B, T2m, T2n;
		    T1i = T18 + T1h;
		    T1B = FNMS(KP765366864, T1A, KP1_847759065 * T1r);
		    T1C = T1i + T1B;
		    T2k = T1i - T1B;
		    T2m = T18 - T1h;
		    T2n = FMA(KP765366864, T1r, KP1_847759065 * T1A);
		    T2o = T2m - T2n;
		    T2s = T2m + T2n;
	       }
	       {
		    E T2j, T2t, T2l, T2r;
		    T2j = FNMS(KP390180644, T2i, KP1_961570560 * T25);
		    O[WS(os, 34)] = T1C - T2j;
		    O[WS(os, 2)] = T1C + T2j;
		    T2t = FMA(KP1_662939224, T2p, KP1_111140466 * T2q);
		    O[WS(os, 26)] = T2s - T2t;
		    O[WS(os, 58)] = T2s + T2t;
		    T2l = FMA(KP390180644, T25, KP1_961570560 * T2i);
		    O[WS(os, 18)] = T2k - T2l;
		    O[WS(os, 50)] = T2k + T2l;
		    T2r = FNMS(KP1_662939224, T2q, KP1_111140466 * T2p);
		    O[WS(os, 42)] = T2o - T2r;
		    O[WS(os, 10)] = T2o + T2r;
	       }
	  }
	  {
	       E T2D, T2N, T2G, T2O, T2A, T2I, T2M, T2Q, T2C, T2E;
	       T2C = KP707106781 * (T27 - T26);
	       T2D = T2B + T2C;
	       T2N = T2B - T2C;
	       T2E = KP707106781 * (T1U - T23);
	       T2G = T2E + T2F;
	       T2O = T2F - T2E;
	       {
		    E T2w, T2z, T2K, T2L;
		    T2w = T2u - T2v;
		    T2z = FNMS(KP1_847759065, T2y, KP765366864 * T2x);
		    T2A = T2w + T2z;
		    T2I = T2w - T2z;
		    T2K = T2u + T2v;
		    T2L = FMA(KP1_847759065, T2x, KP765366864 * T2y);
		    T2M = T2K - T2L;
		    T2Q = T2K + T2L;
	       }
	       {
		    E T2H, T2R, T2J, T2P;
		    T2H = FNMS(KP1_111140466, T2G, KP1_662939224 * T2D);
		    O[WS(os, 38)] = T2A - T2H;
		    O[WS(os, 6)] = T2A + T2H;
		    T2R = FMA(KP1_961570560, T2N, KP390180644 * T2O);
		    O[WS(os, 30)] = T2Q - T2R;
		    O[WS(os, 62)] = T2Q + T2R;
		    T2J = FMA(KP1_111140466, T2D, KP1_662939224 * T2G);
		    O[WS(os, 22)] = T2I - T2J;
		    O[WS(os, 54)] = T2I + T2J;
		    T2P = FNMS(KP1_961570560, T2O, KP390180644 * T2N);
		    O[WS(os, 46)] = T2M - T2P;
		    O[WS(os, 14)] = T2M + T2P;
	       }
	  }
	  {
	       E T5p, T5T, T5w, T5U, T5I, T5W, T5P, T5X, T5s, T5v;
	       T5p = T5l + T5o;
	       T5T = T5l - T5o;
	       T5s = T5q - T5r;
	       T5v = T5t + T5u;
	       T5w = FNMS(KP1_111140466, T5v, KP1_662939224 * T5s);
	       T5U = FMA(KP1_111140466, T5s, KP1_662939224 * T5v);
	       {
		    E T5A, T5H, T5L, T5O;
		    T5A = T5y - T5z;
		    T5H = T5D + T5G;
		    T5I = T5A + T5H;
		    T5W = T5A - T5H;
		    T5L = T5J - T5K;
		    T5O = T5M + T5N;
		    T5P = T5L + T5O;
		    T5X = T5O - T5L;
	       }
	       {
		    E T5x, T5Q, T5Z, T60;
		    T5x = T5p + T5w;
		    T5Q = FNMS(KP580569354, T5P, KP1_913880671 * T5I);
		    O[WS(os, 35)] = T5x - T5Q;
		    O[WS(os, 3)] = T5x + T5Q;
		    T5Z = T5T + T5U;
		    T60 = FMA(KP1_763842528, T5W, KP942793473 * T5X);
		    O[WS(os, 27)] = T5Z - T60;
		    O[WS(os, 59)] = T5Z + T60;
	       }
	       {
		    E T5R, T5S, T5V, T5Y;
		    T5R = T5p - T5w;
		    T5S = FMA(KP580569354, T5I, KP1_913880671 * T5P);
		    O[WS(os, 19)] = T5R - T5S;
		    O[WS(os, 51)] = T5R + T5S;
		    T5V = T5T - T5U;
		    T5Y = FNMS(KP1_763842528, T5X, KP942793473 * T5W);
		    O[WS(os, 43)] = T5V - T5Y;
		    O[WS(os, 11)] = T5V + T5Y;
	       }
	  }
	  {
	       E T3N, T4N, T46, T4O, T4y, T4Q, T4J, T4R, T3Y, T45;
	       T3N = T3F + T3M;
	       T4N = T3F - T3M;
	       T3Y = T3Q + T3X;
	       T45 = T41 + T44;
	       T46 = FNMS(KP390180644, T45, KP1_961570560 * T3Y);
	       T4O = FMA(KP390180644, T3Y, KP1_961570560 * T45);
	       {
		    E T4i, T4x, T4B, T4I;
		    T4i = T4a + T4h;
		    T4x = T4p + T4w;
		    T4y = T4i + T4x;
		    T4Q = T4i - T4x;
		    T4B = T4z + T4A;
		    T4I = T4E + T4H;
		    T4J = T4B + T4I;
		    T4R = T4I - T4B;
	       }
	       {
		    E T47, T4K, T4T, T4U;
		    T47 = T3N + T46;
		    T4K = FNMS(KP196034280, T4J, KP1_990369453 * T4y);
		    O[WS(os, 33)] = T47 - T4K;
		    O[WS(os, 1)] = T47 + T4K;
		    T4T = T4N + T4O;
		    T4U = FMA(KP1_546020906, T4Q, KP1_268786568 * T4R);
		    O[WS(os, 25)] = T4T - T4U;
		    O[WS(os, 57)] = T4T + T4U;
	       }
	       {
		    E T4L, T4M, T4P, T4S;
		    T4L = T3N - T46;
		    T4M = FMA(KP196034280, T4y, KP1_990369453 * T4J);
		    O[WS(os, 17)] = T4L - T4M;
		    O[WS(os, 49)] = T4L + T4M;
		    T4P = T4N - T4O;
		    T4S = FNMS(KP1_546020906, T4R, KP1_268786568 * T4Q);
		    O[WS(os, 41)] = T4P - T4S;
		    O[WS(os, 9)] = T4P + T4S;
	       }
	  }
	  {
	       E T63, T6h, T66, T6i, T6a, T6k, T6d, T6l, T64, T65;
	       T63 = T61 - T62;
	       T6h = T61 + T62;
	       T64 = T5q + T5r;
	       T65 = T5u - T5t;
	       T66 = FNMS(KP1_961570560, T65, KP390180644 * T64);
	       T6i = FMA(KP1_961570560, T64, KP390180644 * T65);
	       {
		    E T68, T69, T6b, T6c;
		    T68 = T5y + T5z;
		    T69 = T5J + T5K;
		    T6a = T68 - T69;
		    T6k = T68 + T69;
		    T6b = T5D - T5G;
		    T6c = T5N - T5M;
		    T6d = T6b + T6c;
		    T6l = T6c - T6b;
	       }
	       {
		    E T67, T6e, T6n, T6o;
		    T67 = T63 + T66;
		    T6e = FNMS(KP1_268786568, T6d, KP1_546020906 * T6a);
		    O[WS(os, 39)] = T67 - T6e;
		    O[WS(os, 7)] = T67 + T6e;
		    T6n = T6h + T6i;
		    T6o = FMA(KP1_990369453, T6k, KP196034280 * T6l);
		    O[WS(os, 31)] = T6n - T6o;
		    O[WS(os, 63)] = T6n + T6o;
	       }
	       {
		    E T6f, T6g, T6j, T6m;
		    T6f = T63 - T66;
		    T6g = FMA(KP1_268786568, T6a, KP1_546020906 * T6d);
		    O[WS(os, 23)] = T6f - T6g;
		    O[WS(os, 55)] = T6f + T6g;
		    T6j = T6h - T6i;
		    T6m = FNMS(KP1_990369453, T6l, KP196034280 * T6k);
		    O[WS(os, 47)] = T6j - T6m;
		    O[WS(os, 15)] = T6j + T6m;
	       }
	  }
	  {
	       E T4X, T5b, T50, T5c, T54, T5e, T57, T5f, T4Y, T4Z;
	       T4X = T4V - T4W;
	       T5b = T4V + T4W;
	       T4Y = T3Q - T3X;
	       T4Z = T44 - T41;
	       T50 = FNMS(KP1_662939224, T4Z, KP1_111140466 * T4Y);
	       T5c = FMA(KP1_662939224, T4Y, KP1_111140466 * T4Z);
	       {
		    E T52, T53, T55, T56;
		    T52 = T4a - T4h;
		    T53 = T4A - T4z;
		    T54 = T52 + T53;
		    T5e = T52 - T53;
		    T55 = T4p - T4w;
		    T56 = T4H - T4E;
		    T57 = T55 + T56;
		    T5f = T56 - T55;
	       }
	       {
		    E T51, T58, T5h, T5i;
		    T51 = T4X + T50;
		    T58 = FNMS(KP942793473, T57, KP1_763842528 * T54);
		    O[WS(os, 37)] = T51 - T58;
		    O[WS(os, 5)] = T51 + T58;
		    T5h = T5b + T5c;
		    T5i = FMA(KP1_913880671, T5e, KP580569354 * T5f);
		    O[WS(os, 29)] = T5h - T5i;
		    O[WS(os, 61)] = T5h + T5i;
	       }
	       {
		    E T59, T5a, T5d, T5g;
		    T59 = T4X - T50;
		    T5a = FMA(KP942793473, T54, KP1_763842528 * T57);
		    O[WS(os, 21)] = T59 - T5a;
		    O[WS(os, 53)] = T59 + T5a;
		    T5d = T5b - T5c;
		    T5g = FNMS(KP1_913880671, T5f, KP580569354 * T5e);
		    O[WS(os, 45)] = T5d - T5g;
		    O[WS(os, 13)] = T5d + T5g;
	       }
	  }
     }
}

static void mhc2r_64(const R *ri, const R *ii, R *O, stride ris, stride iis, stride os, int v, int ivs, int ovs)
{
     int i;
     for (i = v; i > 0; --i) {
	  mhc2r_64_0(ri, ii, O, ris, iis, os);
	  ri += ivs;
	  ii += ivs;
	  O += ovs;
     }
}

static const khc2r_desc desc = { 64, "mhc2r_64", {342, 82, 52, 0}, &GENUS, 0, 0, 0, 0, 0 };

void X(codelet_mhc2r_64) (planner *p) {
     X(khc2r_register) (p, mhc2r_64, &desc);
}
