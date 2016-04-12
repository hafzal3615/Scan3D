//=============================================================================
//
// BSD 3-Clause License
//
// Copyright © 2013-2015, DAVID 3D Solutions GbR
// All rights reserved.
//
// Redistribution and use in source and binary forms, 
// with or without modification, are permitted provided that 
// the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// - Neither the name of DAVID 3D Solutions GbR nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//=============================================================================

#ifndef __FASTVECTOR_H__
#define __FASTVECTOR_H__

/*! \file FastVector.h
    \brief Basic vector/frame operations working on template<>-arrays.
*/

#include <math.h>

#ifndef PI                       
const double PI = 3.14159265358979323846; //!< define pi if its not already defined
#endif

//!set vector (3d)
template<class T1,class T2>
inline void Vset(T1 *Vr, T2 x, T2 y, T2 z)
{
	Vr[0] = (T1)x; Vr[1] = (T1)y; Vr[2] = (T1)z;
}

//! copy vector
template<class T1,class T2>
inline void VcV(T1 *Vr, const T2 *V)
{
	Vr[0] = (T1)V[0];  Vr[1] = (T1)V[1];  Vr[2] = (T1)V[2];
}

//! subtract vector
template<class T1, class T2, class T3>
inline void VmV(T1 *Vr, const T2 *V1, const T3 *V2)
{
	Vr[0] = (T1)(V1[0] - V2[0]);
	Vr[1] = (T1)(V1[1] - V2[1]);
	Vr[2] = (T1)(V1[2] - V2[2]);
}

//! substract vector inplace
template<class T1, class T2>
inline T1* VmV(T1 *Vr, const T2 *V1)
{
	Vr[0] -= (T1)V1[0];
	Vr[1] -= (T1)V1[1];
	Vr[2] -= (T1)V1[2];
	return Vr;
}

//! add vector
template<class T1, class T2, class T3>
inline void VpV(T1 *Vr, const T2 *V1, const T3 *V2)
{
	Vr[0] = (T1)(V1[0] + V2[0]);
	Vr[1] = (T1)(V1[1] + V2[1]);
	Vr[2] = (T1)(V1[2] + V2[2]);
}

//! add vector inplace
template<class T1, class T2>
inline T1* VpV(T1 *Vr, const T2 *V1)
{
	Vr[0] += (T1)V1[0];
	Vr[1] += (T1)V1[1];
	Vr[2] += (T1)V1[2];
	return Vr;
}

//! scale vector
template<class T1, class T2, class T3>
inline void VxS(T1 *Vr, const T2 *V, T3 s)
{
	Vr[0] = (T1)(V[0] * s);
	Vr[1] = (T1)(V[1] * s);
	Vr[2] = (T1)(V[2] * s);
}

//! scale vector inplace
template<class T1, class T2>
inline T1* VxS(T1 *Vr, T2 s)
{
	Vr[0] *= s;
	Vr[1] *= s;
	Vr[2] *= s;
	return Vr;
}

//! cross(vector)-product
template<class T1, class T2, class T3>
inline void VcrossV(T1 *Vr, const T2 *V1, const T3 *V2)
{
	Vr[0] = (T1)(V1[1]*V2[2] - V1[2]*V2[1]);
	Vr[1] = (T1)(V1[2]*V2[0] - V1[0]*V2[2]);
	Vr[2] = (T1)(V1[0]*V2[1] - V1[1]*V2[0]);
}

//! dot-product
template<class T1, class T2>
inline T1 VdotV(const T1 *V1, const T2 *V2)
{
	return (V1[0]*V2[0] + V1[1]*V2[1] + V1[2]*V2[2]);
}

//! length of a vector
template<class T1>
inline T1 Vlength(const T1 *V)
{
	return sqrt(VdotV(V,V));
}


//! distance between two vectors (=length of difference vector)
template<class T1,class T2>
inline T1 VdistV(const T1 *V1, const T2 *V2)
{
	return sqrt((V1[0]-V2[0])*(V1[0]-V2[0]) + 
				(V1[1]-V2[1])*(V1[1]-V2[1]) + 
				(V1[2]-V2[2])*(V1[2]-V2[2]) );
}

//! square distance between two vectors (=length of difference vector)
template<class T1,class T2>
inline T1 VsdistV(const T1 *V1, const T2 *V2)
{
	return      (V1[0]-V2[0])*(V1[0]-V2[0]) + 
				(V1[1]-V2[1])*(V1[1]-V2[1]) + 
				(V1[2]-V2[2])*(V1[2]-V2[2]);
}

//! cosine of angle between two vectors
template<class T1,class T2>
inline T1 VcosV(const T1 *V1, const T2 *V2)
{
	return VdotV(V1,V2) / sqrt(VdotV(V1,V1)*VdotV(V2,V2));
}

//! sinus of angle between two vectors
template<class T1,class T2>
inline T1 VsinV(const T1 *V1, const T2 *V2)
{
	T1 tmpV[3];
	VcrossV(tmpV,V1,V2);
	return Vlength(tmpV) / sqrt(VdotV(V1,V1)*VdotV(V2,V2));
}


//!angle between two vectors
template<class T1, class T2>
inline T1 VangleV(const T1 *V1, const T2 *V2)
{
	return acos(VcosV(V1,V2));
}

//! calculates (sine,cosine)-vector of the angle between two given vectors
template <class T1, class T2, class T3>
inline void VV2SC(T1 *SC,const T2* V1,const T3* V2)
{
	T1 d=sqrt(VdotV(V1,V1)*VdotV(V2,V2));
	T1 tmpV[3];
	VcrossV(tmpV,V1,V2);
	SC[0] = Vlength(tmpV)/d;		//sine
	SC[1] = VdotV(V1,V2)/d;			//cosine
}

//! Calculates (sin(a+b),cos(a+b)) out of (sin(a),cos(a)) and (sin(b),cos(b))
template <class T1,class T2, class T3>
inline void SCpSC(T1 *SC_sum,const T2* SC_a, const T3* SC_b)
{
	SC_sum[0] = SC_a[0]*SC_b[1]+SC_b[0]*SC_a[1];
	SC_sum[1] = SC_a[1]*SC_b[1]-SC_a[0]*SC_b[0];
}

//! Calculates (sin(a+b),cos(a+b)) out of (sin(a),cos(a)) and (sin(b),cos(b)) inplace
template <class T1,class T2>
inline T1* SCpSC(T1* SC_r, const T2* SC_b)
{
	T1 SC_r0 = SC_r[0];
	SC_r[0] = SC_r[0]*SC_b[1]+SC_b[0]*SC_r[1];
	SC_r[1] = SC_r[1]*SC_b[1]-SC_r0*SC_b[0];
	return SC_r;
}

//! Calculates (sin(a),cos(a))
template <class T1, class T2>
inline void Angle2SC(T1 *SC, T2 angle)
{
	SC[0] = sin(angle);
	SC[1] = cos(angle);
}

//! Calculates (sin(a-b),cos(a-b)) out of (sin(a),cos(a)) and (sin(b),cos(b))
template <class T1,class T2, class T3>
inline void SCmSC(T1 *SC_diff,const T2* SC_a, const T3* SC_b)
{
	SC_diff[0] = SC_a[0]*SC_b[1]-SC_b[0]*SC_a[1];
	SC_diff[1] = SC_a[1]*SC_b[1]+SC_a[0]*SC_b[0];
}

//! Calculates (sin(2*a),cos(2*a) out of (sin(a),cos(a))
template <class T1,class T2, class T3>
inline void SCdouble(T1 *SC_double,const T2* SC_a)
{
	SC_double[0] = 2*SC_a[0]*SC_a[1];
	SC_double[1] = 2*SC_a[1]*SC_a[1]-1;
}

//! normalize vector 
template <class T1, class T2>
inline void Vnormalize(T1 *Vr,const T2 *V)
{
  T1 d = (T1)1.0 / Vlength(V);
  VxS(Vr,V,d);
}

//! normalize vector inplace
template <class T1>
inline T1* Vnormalize(T1 *V)
{
  T1 d = (T1)1.0 / Vlength(V);
  return VxS(V,d);
}

//! normalize vector inplace but only if length is != 0
template <class T1>
inline T1* VnormalizeIf(T1 *V)
{
  T1 length = (T1)Vlength(V);
  if (length>0) VxS(V,T1(1)/length);
  return V;
}

//! determinant of matrix (in columns or rows)
template<class T1>
inline T1 detVVV(const T1 *V1, const T1 *V2, const T1 *V3)
{
	return  (V1[0]*V2[1]*V3[2]) + (V1[2]*V2[0]*V3[1]) + (V1[1]*V2[2]*V3[0]) - 
			(V1[2]*V2[1]*V3[0]) - (V1[0]*V2[2]*V3[1]) - (V1[1]*V2[0]*V3[2]);

}

//! determinant of matrix (in single entries)
template<class T1>
inline T1 det3x3(const T1 a00, const T1 a01, const T1 a02,
				 const T1 a10, const T1 a11, const T1 a12,
				 const T1 a20, const T1 a21, const T1 a22)
{
	return  (a00*a11*a22) + (a02*a10*a21) + (a01*a12*a20) - 
			(a02*a11*a20) - (a00*a12*a21) - (a01*a10*a22);
}

//! frame-vector product
template <class Tr, class T1, class T2, class T3>
inline void FxV( Tr *Vr, const T1 *F, const T2 *V, const T3 V3)
{
	Vr[0]	= (Tr)( F[0]*V[0]	+	F[4]*V[1]	+	F[8]*V[2]	+	F[12]*V3);
	Vr[1]	= (Tr)( F[1]*V[0]	+	F[5]*V[1]	+	F[9]*V[2]	+	F[13]*V3);
	Vr[2]	= (Tr)( F[2]*V[0]	+	F[6]*V[1]	+	F[10]*V[2]	+	F[14]*V3);
}

//! frame-vector product inplace
template <class Tr, class T1, class T3>
inline Tr* FxV( const T1 *F, Tr *V, const T3 V3)
{
	Tr Vr[3];
	Vr[0]	= (Tr)( F[0]*V[0]	+	F[4]*V[1]	+	F[8]*V[2]	+	F[12]*V3);
	Vr[1]	= (Tr)( F[1]*V[0]	+	F[5]*V[1]	+	F[9]*V[2]	+	F[13]*V3);
	Vr[2]	= (Tr)( F[2]*V[0]	+	F[6]*V[1]	+	F[10]*V[2]	+	F[14]*V3);
	VcV( V, Vr);
	return V;
}

//! set vector4
template<class Tr,class T1>
inline void V4set(Tr *Vr, T1 x, T1 y, T1 z, T1 h)
{
	Vr[0] = (Tr)x; 
	Vr[1] = (Tr)y; 
	Vr[2] = (Tr)z;
	Vr[3] = (Tr)h;
}

//! copy vector4
template <class Tr, class T1>
inline void V4cV4( Tr *Vr, const T1 *V)
{
	Vr[0] = (Tr) V[0];
	Vr[1] = (Tr) V[1];
	Vr[2] = (Tr) V[2];
	Vr[3] = (Tr) V[3];
}

//! copy frame
template<class Tr,class T1>
inline void FcF( Tr *Fr, const T1 *F)
{
	V4cV4( &Fr[0], &F[0] );
	V4cV4( &Fr[4], &F[4] );
	V4cV4( &Fr[8], &F[8] );
	V4cV4( &Fr[12],&F[12]);
}

//! frame-vector4 product
template <class Tr, class T1, class T2>
inline void FxV4( Tr *Vr, const T1 *F, const T2 *V)
{
	Vr[0]	= (Tr)( F[0]*V[0]	+	F[4]*V[1]	+	F[8]*V[2]	+	F[12]*V[3]);
	Vr[1]	= (Tr)( F[1]*V[0]	+	F[5]*V[1]	+	F[9]*V[2]	+	F[13]*V[3]);
	Vr[2]	= (Tr)( F[2]*V[0]	+	F[6]*V[1]	+	F[10]*V[2]	+	F[14]*V[3]);
	Vr[3]	= (Tr)( F[3]*V[0]	+	F[7]*V[1]	+	F[11]*V[2]	+	F[15]*V[3]);
}

//! frame-frame product
template< class Tr, class T1, class T2>
inline void FxF( Tr *Fr, const T1 *F1, const T2 *F2)
{
	FxV4( &Fr[0], F1, &F2[0] );
	FxV4( &Fr[4], F1, &F2[4] );
	FxV4( &Fr[8], F1, &F2[8] );
	FxV4( &Fr[12],F1, &F2[12]);
}

//! frame inversion (only for orthonormed frames!!)
template <class Tr>
inline void Finverse( Tr *Fr)
{
	//neue position berechnen
	Tr old_pos[4];
	V4cV4( old_pos, &Fr[12]);
	
	Fr[12] = - VdotV( old_pos, &Fr[0]);
	Fr[13] = - VdotV( old_pos, &Fr[4]);
	Fr[14] = - VdotV( old_pos, &Fr[8]);
    
	//neue rotation ist einfach transponierte alte
	Tr old_ny	= Fr[1];
	Tr old_nz	= Fr[2];
	Tr old_oz	= Fr[6];
	//nx bleibt
    Fr[1]	= Fr[4];
	Fr[2]	= Fr[8];
	Fr[4]	= old_ny;
	//oy bleibt
	Fr[6]	= Fr[9];
	Fr[8]	= old_nz;
	Fr[9]	= old_oz;
	//az bleibt
}

//! load identity frame
template <class Tr>
inline void Fidentity( Tr *Fr)
{
	V4set( &Fr[0], 1.0, 0.0, 0.0, 0.0);
	V4set( &Fr[4], 0.0, 1.0, 0.0, 0.0);
	V4set( &Fr[8], 0.0, 0.0, 1.0, 0.0);
	V4set( &Fr[12],0.0, 0.0, 0.0, 1.0);
}


//! load x-rotation frame
template <class Tr, class T1>
inline void Frotx( Tr *Fr, const T1 angle)
{
	double Sa = sin(angle);
	double Ca = cos(angle);
	V4set( &Fr[0], 1.0, 0.0, 0.0, 0.0);
	V4set( &Fr[4], 0.0, Ca , Sa , 0.0);
	V4set( &Fr[8], 0.0, -Sa, Ca , 0.0);
	V4set( &Fr[12],0.0, 0.0, 0.0, 1.0);
}


//! load y-rotation frame
template <class Tr, class T1>
inline void Froty( Tr *Fr, const T1 angle)
{
	double Sa = sin(angle);
	double Ca = cos(angle);
	V4set( &Fr[0], Ca , 0.0, -Sa, 0.0);
	V4set( &Fr[4], 0.0, 1.0, 0.0, 0.0);
	V4set( &Fr[8], Sa , 0.0, Ca , 0.0);
	V4set( &Fr[12],0.0, 0.0, 0.0, 1.0);
}


//! load z-rotation frame
template <class Tr, class T1>
inline void Frotz( Tr *Fr, const T1 angle)
{
	double Sa = sin(angle);
	double Ca = cos(angle);
	V4set( &Fr[0], Ca , Sa , 0.0, 0.0);
	V4set( &Fr[4], -Sa, Ca , 0.0, 0.0);
	V4set( &Fr[8], 0.0, 0.0, 1.0, 0.0);
	V4set( &Fr[12],0.0, 0.0, 0.0, 1.0);
}


//! load vector-rotation frame
template <class Tr, class T1, class T2>
inline void Frotv( Tr *Fr, const T1 *V, const T2 angle)
{
	// see Fu, Gonzalez, Lee, ROBOTICS, p. 21
    double vn[3];
	VcV( vn, V);
	Vnormalize(vn);

	double Sa = sin(angle);
	double Ca = cos(angle);
	double Va = 1.0 - Ca;

	V4set( &Fr[0], vn[0]*vn[0]*Va + Ca,			vn[0]*vn[1]*Va + vn[2]*Sa,	vn[0]*vn[2]*Va - vn[1]*Sa,	0.0 );
	V4set( &Fr[4], vn[1]*vn[0]*Va - vn[2]*Sa,	vn[1]*vn[1]*Va + Ca,		vn[1]*vn[2]*Va + vn[0]*Sa,	0.0 );
	V4set( &Fr[8], vn[2]*vn[0]*Va + vn[1]*Sa,	vn[2]*vn[1]*Va - vn[0]*Sa,	vn[2]*vn[2]*Va + Ca,		0.0 );
	V4set( &Fr[12], 0.0, 0.0, 0.0, 1.0);
}

//!  set 6d-vector
template<class T1,class T2>
inline void V6set(T1 *Vr, T2 x, T2 y, T2 z,T2 nx,T2 ny,T2 nz)
{
	Vr[0] = (T1)x; Vr[1] = (T1)y; Vr[2] = (T1)z;
	Vr[3] = (T1)nx; Vr[4] = (T1)ny; Vr[5] = (T1)nz;
}

//! copy 6d-vector
template<class T1,class T2>
inline void V6cV6(T1 *Vr, const T2 *V)
{
	Vr[0] = (T1)V[0];  Vr[1] = (T1)V[1];  Vr[2] = (T1)V[2];
	Vr[3] = (T1)V[3];  Vr[4] = (T1)V[4];  Vr[5] = (T1)V[5];
}

//! subtract 6d-vector
template<class T1, class T2, class T3>
inline void V6mV6(T1 *Vr, const T2 *V1, const T3 *V2)
{
	Vr[0] = (T1)(V1[0] - V2[0]);
	Vr[1] = (T1)(V1[1] - V2[1]);
	Vr[2] = (T1)(V1[2] - V2[2]);
	Vr[3] = (T1)(V1[3] - V2[3]);
	Vr[4] = (T1)(V1[4] - V2[4]);
	Vr[5] = (T1)(V1[5] - V2[5]);
}

//! substract 6d-vector inplace
template<class T1, class T2>
inline T1* V6mV6(T1 *Vr, const T2 *V1)
{
	Vr[0] -= (T1)V1[0];
	Vr[1] -= (T1)V1[1];
	Vr[2] -= (T1)V1[2];
	Vr[3] -= (T1)V1[3];
	Vr[4] -= (T1)V1[4];
	Vr[5] -= (T1)V1[5];
	return Vr;
}

//! add 6d-vector
template<class T1, class T2, class T3>
inline void V6pV6(T1 *Vr, const T2 *V1, const T3 *V2)
{
	Vr[0] = (T1)(V1[0] + V2[0]);
	Vr[1] = (T1)(V1[1] + V2[1]);
	Vr[2] = (T1)(V1[2] + V2[2]);
	Vr[3] = (T1)(V1[3] + V2[3]);
	Vr[4] = (T1)(V1[4] + V2[4]);
	Vr[5] = (T1)(V1[5] + V2[5]);
}

//! add 6d-vector inplace
template<class T1, class T2>
inline T1* V6pV6(T1 *Vr, const T2 *V1)
{
	Vr[0] += (T1)V1[0];
	Vr[1] += (T1)V1[1];
	Vr[2] += (T1)V1[2];
	Vr[3] += (T1)V1[3];
	Vr[4] += (T1)V1[4];
	Vr[5] += (T1)V1[5];
	return Vr;
}

//! scale 6d-vector
template<class T1, class T2, class T3>
inline void V6xS(T1 *Vr, const T2 *V, T3 s)
{
	Vr[0] = (T1)(V[0] * s);
	Vr[1] = (T1)(V[1] * s);
	Vr[2] = (T1)(V[2] * s);
	Vr[3] = (T1)(V[3] * s);
	Vr[4] = (T1)(V[4] * s);
	Vr[5] = (T1)(V[5] * s);
}

//! scale 6d-vector inplace
template<class T1, class T2>
inline T1* V6xS(T1 *Vr, T2 s)
{
	Vr[0] *= s;
	Vr[1] *= s;
	Vr[2] *= s;
	Vr[3] *= s;
	Vr[4] *= s;
	Vr[5] *= s;
	return Vr;
}

//! 6d-vector dot-product
template<class T1, class T2>
inline T1 V6dotV6(const T1 *V1, const T2 *V2)
{
	return (V1[0]*V2[0] + V1[1]*V2[1] + V1[2]*V2[2] + V1[3]*V2[3] + V1[4]*V2[4] + V1[5]*V2[5]);
}

//! length of a 6d-vector
template<class T1>
inline T1 V6length(const T1 *V)
{
	return sqrt(V6dotV6(V,V));
}


//! distance between two 6d-vectors (=length of difference vector)
template<class T1,class T2>
inline T1 V6distV6(const T1 *V1, const T2 *V2)
{
	return sqrt((V1[0]-V2[0])*(V1[0]-V2[0]) + 
				(V1[1]-V2[1])*(V1[1]-V2[1]) + 
				(V1[2]-V2[2])*(V1[2]-V2[2]) +
				(V1[3]-V2[3])*(V1[3]-V2[3]) + 
				(V1[4]-V2[4])*(V1[4]-V2[4]) + 
				(V1[5]-V2[5])*(V1[5]-V2[5]));
}

//! square distance between two 6d-vectors (=length of difference vector)
template<class T1,class T2>
inline T1 V6sdistV6(const T1 *V1, const T2 *V2)
{
	return      (V1[0]-V2[0])*(V1[0]-V2[0]) + 
				(V1[1]-V2[1])*(V1[1]-V2[1]) + 
				(V1[2]-V2[2])*(V1[2]-V2[2]) +
				(V1[3]-V2[3])*(V1[3]-V2[3]) + 
				(V1[4]-V2[4])*(V1[4]-V2[4]) + 
				(V1[5]-V2[5])*(V1[5]-V2[5]);
}

//!angle between two 6d-vectors
template<class T1, class T2>
inline T1 V6angleV6(const T1 *V1, const T2 *V2)
{
	return acos( V6dotV6(V1,V2) / sqrt(V6dotV6(V1,V1)*V6dotV6(V2,V2)) );
}


//! normalize 6d-vector 
template <class T1, class T2>
inline void V6normalize(T1 *Vr,const T2 *V)
{
  T1 d = (T1)1.0 / V6length(V);
  V6xS(Vr,V,d);
}

//! normalize 6d-vector inplace
template <class T1>
inline T1* V6normalize(T1 *V)
{
  T1 d = (T1)1.0 / V6length(V);
  return V6xS(V,d);
}

//! 6x6Matrix-6d-vector product
template <class Tr,class T1, class T2>
inline void MxV6(Tr *Vr,const T1 *M,const T2 *V)
{
	Vr[0]	= (Tr)( M[0]*V[0]	+	M[6]*V[1]	+	M[12]*V[2]	+	M[18]*V[3]   +	M[24]*V[4]	+	M[30]*V[5]);
	Vr[1]	= (Tr)( M[1]*V[0]	+	M[7]*V[1]	+	M[13]*V[2]	+	M[19]*V[3]   +	M[25]*V[4]	+	M[31]*V[5]);
	Vr[2]	= (Tr)( M[2]*V[0]	+	M[8]*V[1]	+	M[14]*V[2]	+	M[20]*V[3]   +	M[26]*V[4]	+	M[32]*V[5]);
	Vr[3]	= (Tr)( M[3]*V[0]	+	M[9]*V[1]	+	M[15]*V[2]	+	M[21]*V[3]   +	M[27]*V[4]	+	M[33]*V[5]);
	Vr[4]	= (Tr)( M[4]*V[0]	+	M[10]*V[1]	+	M[16]*V[2]	+	M[22]*V[3]   +	M[28]*V[4]	+	M[34]*V[5]);
	Vr[5]	= (Tr)( M[5]*V[0]	+	M[11]*V[1]	+	M[17]*V[2]	+	M[23]*V[3]   +	M[29]*V[4]	+	M[35]*V[5]);
}

//! copy vector
template<class T1,class T2>
inline void V0cV0(T1 *Vr, const T2 *V, const int dim)
{
	for(int i=0; i < dim; ++i) Vr[i] = (T1)V[i];
}

//! subtract vector
template<class T1, class T2, class T3>
inline void V0mV0(T1 *Vr, const T2 *V1, const T3 *V2, const int dim)
{
	for(int i=0; i < dim; ++i) Vr[i] = (T1) (V1[i] - V2[i]);
}

//! substract vector inplace
template<class T1, class T2>
inline T1* V0mV0(T1 *Vr, const T2 *V1, const int dim)
{
	for(int i=0; i <  dim; ++i) Vr[i] -= (T1) V1[i];
	return Vr;
}

//! add vector
template<class T1, class T2, class T3>
inline void V0pV0(T1 *Vr, const T2 *V1, const T3 *V2, const int dim)
{
	for(int i=0; i < dim; ++i) Vr[i] = (T1) (V1[i] + V2[i]);
}

//! add vector inplace
template<class T1, class T2>
inline T1* V0pV0(T1 *Vr, const T2 *V1, const int dim)
{
	for(int i=0; i < dim; ++i) Vr[i] += (T1) V1[i];
	return Vr;
}

//! scale vector
template<class T1, class T2, class T3>
inline void V0xS(T1 *Vr, const T2 *V, T3 s, const int dim)
{
	for(int i=0; i < dim; ++i) Vr[i] = (T1) (V[i]*s);
}

//! scale vector inplace
template<class T1, class T2>
inline T1* V0xS(T1 *Vr, T2 s, const int dim)
{
	for(int i=0; i < dim; ++i) Vr[i] *= s;
	return Vr;
}

//! vector dot-product
template<class T1, class T2>
inline T1 V0dotV0(const T1 *V1, const T2 *V2, const int dim)
{
	T1 sum = 0;
	for(int i=0; i < dim; ++i)
	{
		sum += V1[i] * V2[i];
	}
	return sum;
}

//! length of a vector
template<class T1>
inline T1 V0length(const T1 *V, const int dim)
{
	return sqrt(V0dotV0(V,V,dim));
}


//! distance between two vectors (=length of difference vector)
template<class T1,class T2>
inline T1 V0distV0(const T1 *V1, const T2 *V2, const int dim)
{
	T1 sum = 0;
	for(int i=0; i < dim; ++i)
	{
		const T1 dif = V1[i] - V2[i];
		sum += dif*dif;
	}
	return sqrt(sum);
}

//! square distance between two vectors (=length of difference vector)
template<class T1,class T2>
inline T1 V0sdistV0(const T1 *V1, const T2 *V2, const int dim)
{
	T1 sum = 0;
	for(int i=0; i < dim; ++i)
	{
		const T1 dif = V1[i] - V2[i];
		sum += dif*dif;
	}
	return sum;
}

//!angle between two vectors
template<class T1, class T2>
inline T1 V0angleV0(const T1 *V1, const T2 *V2, const int dim)
{
	return acos(V0dotV0(V1,V2,dim) / sqrt(V0dotV0(V1,V1,dim)*V0dotV0(V2,V2,dim)));
}


//! normalize vector 
template <class T1, class T2>
inline void V0normalize(T1 *Vr,const T2 *V, const int dim)
{
	T1 d = (T1) 1.0 / V0length(V);
	V0xS(Vr,V,d,dim);
}

//! normalize vector inplace
template <class T1>
inline T1* V0normalize(T1 *V, const int dim)
{
  T1 d = (T1)1.0 / V0length(V,dim);
  return V0xS(V,d,dim);
}

#endif