/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "decMath.h"
#include "../exceptions.h"



// Class decDMatrix
/////////////////////

// Constructors
/////////////////

decDMatrix::decDMatrix() :
a11(1.0), a12(0.0), a13(0.0), a14(0.0),
a21(0.0), a22(1.0), a23(0.0), a24(0.0),
a31(0.0), a32(0.0), a33(1.0), a34(0.0),
a41(0.0), a42(0.0), a43(0.0), a44(1.0){
}

decDMatrix::decDMatrix(const decDMatrix &m) :
a11(m.a11), a12(m.a12), a13(m.a13), a14(m.a14),
a21(m.a21), a22(m.a22), a23(m.a23), a24(m.a24),
a31(m.a31), a32(m.a32), a33(m.a33), a34(m.a34),
a41(m.a41), a42(m.a42), a43(m.a43), a44(m.a44){
}

decDMatrix::decDMatrix(const decMatrix &m) :
a11((double)m.a11), a12((double)m.a12), a13((double)m.a13), a14((double)m.a14),
a21((double)m.a21), a22((double)m.a22), a23((double)m.a23), a24((double)m.a24),
a31((double)m.a31), a32((double)m.a32), a33((double)m.a33), a34((double)m.a34),
a41((double)m.a41), a42((double)m.a42), a43((double)m.a43), a44((double)m.a44){
}



// Creation
/////////////

decDMatrix decDMatrix::CreateIdentity(){
	return decDMatrix();
}

decDMatrix decDMatrix::CreateTranslation(double x, double y, double z){
	decDMatrix m;
	m.SetTranslation(x, y, z);
	return m;
}

decDMatrix decDMatrix::CreateTranslation(const decDVector &t){
	decDMatrix m;
	m.SetTranslation(t);
	return m;
}

decDMatrix decDMatrix::CreateScale(double sx, double sy, double sz){
	decDMatrix m;
	m.SetScale(sx, sy, sz);
	return m;
}

decDMatrix decDMatrix::CreateScale(const decDVector &s){
	decDMatrix m;
	m.SetScale(s);
	return m;
}

decDMatrix decDMatrix::CreateRotationX(double arc){
	decDMatrix m;
	m.SetRotationX(arc);
	return m;
}

decDMatrix decDMatrix::CreateRotationY(double arc){
	decDMatrix m;
	m.SetRotationY(arc);
	return m;
}

decDMatrix decDMatrix::CreateRotationZ(double arc){
	decDMatrix m;
	m.SetRotationZ(arc);
	return m;
}

decDMatrix decDMatrix::CreateRotation(double rx, double ry, double rz){
	decDMatrix m;
	m.SetRotation(rx, ry, rz);
	return m;
}

decDMatrix decDMatrix::CreateRotation(const decDVector &r){
	decDMatrix m;
	m.SetRotation(r);
	return m;
}

decDMatrix decDMatrix::CreateRotationAxis(const decDVector &axis, double angle){
	decDMatrix m;
	m.SetRotationAxis(axis, angle);
	return m;
}

decDMatrix decDMatrix::CreateCamera(const decDVector &pos, const decDVector &view, const decDVector &up){
	decDMatrix m;
	m.SetCamera(pos, view, up);
	return m;
}

decDMatrix decDMatrix::CreateWorld(const decDVector &pos, const decDVector &view, const decDVector &up){
	decDMatrix m;
	m.SetWorld(pos, view, up);
	return m;
}

decDMatrix decDMatrix::CreateWorld(const decDVector &pos, const decQuaternion &orientation){
	decDMatrix m;
	m.SetWorld(pos, orientation);
	return m;
}

decDMatrix decDMatrix::CreateWorld(const decDVector &pos, const decQuaternion &orientation, const decDVector &scale){
	decDMatrix m;
	m.SetWorld(pos, orientation, scale);
	return m;
}

decDMatrix decDMatrix::CreateProjection(int width, int height, double fov, double fovRatio, double znear, double zfar){
	decDMatrix m;
	m.SetProjection(width, height, fov, fovRatio, znear, zfar);
	return m;
}

decDMatrix decDMatrix::CreateProjectionInfinite(int width, int height, double fov, double fovRatio, double znear){
	decDMatrix m;
	m.SetProjectionInfinite(width, height, fov, fovRatio, znear);
	return m;
}

decDMatrix decDMatrix::CreateOrtho(int left, int right, int top, int bottom, int znear, int zfar){
	decDMatrix m;
	m.SetOrtho(left, right, top, bottom, znear, zfar);
	return m;
}

decDMatrix decDMatrix::CreateClipSpace(){
	decDMatrix m;
	m.SetClipSpace();
	return m;
}

decDMatrix decDMatrix::CreateBiasMatrix(){
	decDMatrix m;
	m.SetBiasMatrix();
	return m;
}

decDMatrix decDMatrix::CreateRT(const decDVector &rot, const decDVector &pos){
	decDMatrix m;
	m.SetRT(rot, pos);
	return m;
}

decDMatrix decDMatrix::CreateST(const decDVector &scale, const decDVector &translate){
	decDMatrix m;
	m.SetST(scale, translate);
	return m;
}

decDMatrix decDMatrix::CreateSRT(const decDVector &scale, const decDVector &rot, const decDVector &pos){
	decDMatrix m;
	m.SetSRT(scale, rot, pos);
	return m;
}

decDMatrix decDMatrix::CreateSVUT(const decDVector &scale, const decDVector &view, const decDVector &up, const decDVector &pos){
	decDMatrix m;
	m.SetSVUT(scale, view, up, pos);
	return m;
}

decDMatrix decDMatrix::CreateVU(const decDVector &view, const decDVector &up){
	decDMatrix m;
	m.SetVU(view, up);
	return m;
}

decDMatrix decDMatrix::CreateFromQuaternion(const decQuaternion &q){
	decDMatrix m;
	m.SetFromQuaternion(q);
	return m;
}

decDMatrix decDMatrix::CreateFromQuaternion(const decQuaternion &q, const decDVector &pivot){
	decDMatrix m;
	m.SetFromQuaternion(q, pivot);
	return m;
}



// management
///////////////

void decDMatrix::SetIdentity(){
	a11 = 1.0; a12 = 0.0; a13 = 0.0; a14 = 0.0;
	a21 = 0.0; a22 = 1.0; a23 = 0.0; a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = 1.0; a34 = 0.0;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetTranslation(double x, double y, double z){
	a11 = 1.0; a12 = 0.0; a13 = 0.0; a14 = x;
	a21 = 0.0; a22 = 1.0; a23 = 0.0; a24 = y;
	a31 = 0.0; a32 = 0.0; a33 = 1.0; a34 = z;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetTranslation(const decDVector &t){
	a11 = 1.0; a12 = 0.0; a13 = 0.0; a14 = t.x;
	a21 = 0.0; a22 = 1.0; a23 = 0.0; a24 = t.y;
	a31 = 0.0; a32 = 0.0; a33 = 1.0; a34 = t.z;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetScale(double sx, double sy, double sz){
	a11 = sx;  a12 = 0.0; a13 = 0.0; a14 = 0.0;
	a21 = 0.0; a22 = sy;  a23 = 0.0; a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = sz;  a34 = 0.0;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetScale(const decDVector &s){
	a11 = s.x; a12 = 0.0; a13 = 0.0; a14 = 0.0;
	a21 = 0.0; a22 = s.y; a23 = 0.0; a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = s.z; a34 = 0.0;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetRotationX(double arc){
	const double s = sin(arc);
	const double c = cos(arc);
	
	a11 = 1.0; a12 = 0.0; a13 = 0.0; a14 = 0.0;
	a21 = 0.0; a22 = c;   a23 = s;   a24 = 0.0;
	a31 = 0.0; a32 = -s;  a33 = c;   a34 = 0.0;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetRotationY(double arc){
	const double s = sin(arc);
	const double c = cos(arc);
	
	a11 = c;   a12 = 0.0; a13 = -s;  a14 = 0.0;
	a21 = 0.0; a22 = 1.0; a23 = 0.0; a24 = 0.0;
	a31 = s;   a32 = 0.0; a33 = c;   a34 = 0.0;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetRotationZ(double arc){
	const double s = sin(arc);
	const double c = cos(arc);
	
	a11 = c;   a12 = s;   a13 = 0.0; a14 = 0.0;
	a21 = -s;  a22 = c;   a23 = 0.0; a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = 1.0; a34 = 0.0;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetRotation(double rx, double ry, double rz){
	const double a = sin(rx);
	const double b = cos(rx);
	const double c = sin(ry);
	const double d = cos(ry);
	const double e = sin(rz);
	const double f = cos(rz);
	const double g = d * f;
	const double h = c * f;
	const double i = c * e;
	const double j = d * e;
	
	a11 = g - a * i; a12 = a * h + j;  a13 = -c * b; a14 = 0.0;
	a21 = -b * e;    a22 = b * f;      a23 = a;      a24 = 0.0;
	a31 = h + a * j; a32 = -a * g + i; a33 = b * d;  a34 = 0.0;
	a41 = 0.0;       a42 = 0.0;        a43 = 0.0;    a44 = 1.0;
}

void decDMatrix::SetRotation(const decDVector &r){
	const double a = sin(r.x);
	const double b = cos(r.x);
	const double c = sin(r.y);
	const double d = cos(r.y);
	const double e = sin(r.z);
	const double f = cos(r.z);
	const double g = d * f;
	const double h = c * f;
	const double i = c * e;
	const double j = d * e;
	
	a11 = g - a * i; a12 = a * h + j;  a13 = -c * b; a14 = 0.0;
	a21 = -b * e;    a22 = b * f;      a23 = a;      a24 = 0.0;
	a31 = h + a * j; a32 = -a * g + i; a33 = b * d;  a34 = 0.0;
	a41 = 0.0;       a42 = 0.0;        a43 = 0.0;    a44 = 1.0;
}

void decDMatrix::SetRotationAxis(const decDVector &axis, double angle){
	const decDVector v = axis.Normalized();
	const double s = sin(angle);
	const double c = cos(angle);
	const double c1 = 1.0 - c;
	
	a11 = v.x * v.x * c1 + c;
	a12 = v.x * v.y * c1 + v.z * s;
	a13 = v.x * v.z * c1 - v.y * s;
	a14 = 0.0;
	
	a21 = v.x * v.y * c1 - v.z * s;
	a22 = v.y * v.y * c1 + c;
	a23 = v.y * v.z * c1 + v.x * s;
	a24 = 0.0;
	
	a31 = v.x * v.z * c1 + v.y * s;
	a32 = v.y * v.z * c1 - v.x * s;
	a33 = v.z * v.z * c1 + c;
	a34 = 0.0;
	
	a41 = 0.0;
	a42 = 0.0;
	a43 = 0.0;
	a44 = 1.0;
}

void decDMatrix::SetCamera(const decDVector &pos, const decDVector &view, const decDVector &up){
	const decDVector dir = view.Normalized();
	const double dot = up * dir;
	const decDVector nup = (up - dir * dot).Normalized();
	const decDVector right = nup % dir;
	
	a11 = right.x; a12 = right.y; a13 = right.z; a14 = -(pos * right);
	a21 = nup.x;   a22 = nup.y;   a23 = nup.z;   a24 = -(pos * nup);
	a31 = dir.x;   a32 = dir.y;   a33 = dir.z;   a34 = -(pos * dir);
	a41 = 0.0;     a42 = 0.0;     a43 = 0.0;     a44 = 1.0;
}

void decDMatrix::SetWorld(const decDVector &pos, const decDVector &view, const decDVector &up){
	const decDVector dir = view.Normalized();
	const decDVector nup = up.Normalized();
	const decDVector right = (nup % dir).Normalized();
	const decDVector nup2 = dir % right;
	
	a11 = right.x; a12 = nup2.x; a13 = dir.x; a14 = pos.x;
	a21 = right.y; a22 = nup2.y; a23 = dir.y; a24 = pos.y;
	a31 = right.z; a32 = nup2.z; a33 = dir.z; a34 = pos.z;
	a41 = 0.0f;    a42 = 0.0f;   a43 = 0.0;   a44 = 1.0;
}

void decDMatrix::SetWorld(const decDVector &pos, const decQuaternion &orientation){
	const double sqw = orientation.w * orientation.w;
	const double sqx = orientation.x * orientation.x;
	const double sqy = orientation.y * orientation.y;
	const double sqz = orientation.z * orientation.z;
	
	const double inv = 1.0 / (sqx + sqy + sqz + sqw);
	a11 = (sqx - sqy - sqz + sqw) * inv;
	a22 = (-sqx + sqy - sqz + sqw) * inv;
	a33 = (-sqx - sqy + sqz + sqw) * inv;
	
	const double t1 = orientation.x * orientation.y;
	const double t2 = orientation.z * orientation.w;
	a21 = 2.0 * (t1 + t2) * inv;
	a12 = 2.0 * (t1 - t2) * inv;
	
	const double t3 = orientation.x * orientation.z;
	const double t4 = orientation.y * orientation.w;
	a31 = 2.0 * (t3 - t4) * inv;
	a13 = 2.0 * (t3 + t4) * inv;
	
	const double t5 = orientation.y * orientation.z;
	const double t6 = orientation.x * orientation.w;
	a32 = 2.0 * (t5 + t6) * inv;
	a23 = 2.0 * (t5 - t6) * inv;
	
	a14 = pos.x;
	a24 = pos.y;
	a34 = pos.z;
	a41 = 0.0;
	a42 = 0.0;
	a43 = 0.0;
	a44 = 1.0;
}

void decDMatrix::SetWorld(const decDVector &pos, const decQuaternion &orientation, const decDVector &scale){
	const double sqw = orientation.w * orientation.w;
	const double sqx = orientation.x * orientation.x;
	const double sqy = orientation.y * orientation.y;
	const double sqz = orientation.z * orientation.z;
	
	const double inv = 1.0 / (sqx + sqy + sqz + sqw);
	const double invX = inv * scale.x;
	const double invY = inv * scale.y;
	const double invZ = inv * scale.z;
	a11 = (sqx - sqy - sqz + sqw) * invX;
	a22 = (-sqx + sqy - sqz + sqw) * invY;
	a33 = (-sqx - sqy + sqz + sqw) * invZ;
	
	const double t1 = orientation.x * orientation.y;
	const double t2 = orientation.z * orientation.w;
	const double invX2 = invX * 2.0;
	const double invY2 = invY * 2.0;
	a21 = (t1 + t2) * invX2;
	a12 = (t1 - t2) * invY2;
	
	const double t3 = orientation.x * orientation.z;
	const double t4 = orientation.y * orientation.w;
	const double invZ2 = invZ * 2.0;
	a31 = (t3 - t4) * invX2;
	a13 = (t3 + t4) * invZ2;
	
	const double t5 = orientation.y * orientation.z;
	const double t6 = orientation.x * orientation.w;
	a32 = (t5 + t6) * invY2;
	a23 = (t5 - t6) * invZ2;
	
	a14 = pos.x;
	a24 = pos.y;
	a34 = pos.z;
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
}

void decDMatrix::SetProjection(int width, int height, double fov, double fovRatio, double znear, double zfar){
	if(width < 1 || height < 1 || fov <= 0.0 || fov >= PI || fovRatio == 0.0 || znear >= zfar){
		DETHROW(deeInvalidParam);
	}
	
	const double a = (double)width / (double)height;
	const double fx = 1.0 / tan(fov * 0.5);
	const double fy = a / tan(fov * fovRatio * 0.5);
	const double q = zfar / (zfar - znear);
	
	a11 = fx;  a12 = 0.0; a13 = 0.0; a14 = 0.0;
	a21 = 0.0; a22 = fy;  a23 = 0.0; a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = q;   a34 = -q * znear;
	a41 = 0.0; a42 = 0.0; a43 = 1.0; a44 = 0.0;
}

void decDMatrix::SetProjectionInfinite(int width, int height, double fov, double fovRatio, double znear){
	if(width < 1 || height < 1 || fov <= 0.0 || fov >= PI || fovRatio == 0.0){
		DETHROW(deeInvalidParam);
	}
	
	const double a = (double)width / (double)height;
	const double fx = 1.0 / tan(fov * 0.5);
	const double fy = a / tan(fov * fovRatio * 0.5);
	const double e = 0.001;
	
	a11 = fx;  a12 = 0.0; a13 = 0.0;     a14 = 0.0;
	a21 = 0.0; a22 = fy;  a23 = 0.0;     a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = 1.0 - e; a34 = znear * (e - 2.0);
	a41 = 0.0; a42 = 0.0; a43 = 1.0;     a44 = 0.0;
}

void decDMatrix::SetOrtho(int left, int right, int top, int bottom, int znear, int zfar){
	const double a = (double)(right - left);
	const double b = (double)(bottom - top);
	const double c = (double)(zfar - znear);
	
	a11 = 2.0 / a; a12 = 0.0;     a13 = 0.0;     a14 = -(double)(right + left) / a;
	a21 = 0.0;     a22 = 2.0 / b; a23 = 0.0;     a24 = -(double)(bottom + top) / b;
	a31 = 0.0;     a32 = 0.0;     a33 = 2.0 / c; a34 = -(double)(zfar + znear) / c;
	a41 = 0.0;     a42 = 0.0;     a43 = 0.0;     a44 = 1.0;
}

void decDMatrix::SetClipSpace(){
	a11 = 0.5; a12 = 0.0; a13 = 0.0; a14 = 0.0;
	a21 = 0.0; a22 = 0.5; a23 = 0.0; a24 = 0.0;
	a31 = 0.0; a32 = 0.0; a33 = 1.0; a34 = 0.0;
	a41 = 0.5; a42 = 0.5; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetBiasMatrix(){
	a11 = 0.5; a12 = 0.0; a13 = 0.0; a14 = 0.5;
	a21 = 0.0; a22 = 0.5; a23 = 0.0; a24 = 0.5;
	a31 = 0.0; a32 = 0.0; a33 = 0.5; a34 = 0.5;
	a41 = 0.0; a42 = 0.0; a43 = 0.0; a44 = 1.0;
}

void decDMatrix::SetRT(const decDVector &rot, const decDVector &pos){
	const double a = sin(rot.x);
	const double b = cos(rot.x);
	const double c = sin(rot.y);
	const double d = cos(rot.y);
	const double e = sin(rot.z);
	const double f = cos(rot.z);
	const double g = d * f;
	const double h = c * f;
	const double i = c * e;
	const double j = d * e;
	
	a11 = g - a * i; a12 = a * h + j;  a13 = -c * b; a14 = pos.x;
	a21 = -b * e;    a22 = b * f;      a23 = a;      a24 = pos.y;
	a31 = h + a * j; a32 = -a * g + i; a33 = b * d;  a34 = pos.z;
	a41 = 0.0;       a42 = 0.0;        a43 = 0.0;    a44 = 1.0;
}

void decDMatrix::SetST(const decDVector &scale, const decDVector &translate){
	a11 = scale.x; a12 = 0.0;     a13 = 0.0;     a14 = translate.x;
	a21 = 0.0;     a22 = scale.y; a23 = 0.0;     a24 = translate.y;
	a31 = 0.0;     a32 = 0.0;     a33 = scale.z; a34 = translate.z;
	a41 = 0.0;     a42 = 0.0;     a43 = 0.0;     a44 = 1.0;
}

void decDMatrix::SetSRT(const decDVector &scale, const decDVector &rot, const decDVector &pos){
	const double a = sin(rot.x);
	const double b = cos(rot.x);
	const double c = sin(rot.y);
	const double d = cos(rot.y);
	const double e = sin(rot.z);
	const double f = cos(rot.z);
	const double g = d * f;
	const double h = c * f;
	const double i = c * e;
	const double j = d * e;
	
	a11 = scale.x * (g - a * i); a12 = scale.y * (a * h + j);  a13 = scale.z * (-c * b); a14 = pos.x;
	a21 = scale.x * (-b * e);    a22 = scale.y * (b * f);      a23 = scale.z * a;          a24 = pos.y;
	a31 = scale.x * (h + a * j); a32 = scale.y * (-a * g + i); a33 = scale.z * (b * d);  a34 = pos.z;
	a41 = 0.0;                     a42 = 0.0;                      a43 = 0.0;                  a44 = 1.0;
}

void decDMatrix::SetSVUT(const decDVector &scale, const decDVector &view, const decDVector &up, const decDVector &pos){
	const decDVector dir = view.Normalized();
	const decDVector nup = up.Normalized();
	const decDVector right = nup % dir;
	const decDVector nup2 = dir % right;
	
	a11 = scale.x * right.x; a12 = scale.y * nup2.x; a13 = scale.z * dir.x; a14 = pos.x;
	a21 = scale.x * right.y; a22 = scale.y * nup2.y; a23 = scale.z * dir.y; a24 = pos.y;
	a31 = scale.x * right.z; a32 = scale.y * nup2.z; a33 = scale.z * dir.z; a34 = pos.z;
	a41 = 0.0;               a42 = 0.0;              a43 = 0.0;             a44 = 1.0;
}

void decDMatrix::SetVU(const decDVector &view, const decDVector &up){
	const decDVector dir = view.Normalized();
	const decDVector nup = up.Normalized();
	const decDVector right = nup % dir;
	const decDVector nup2 = dir % right;
	
	a11 = right.x; a12 = nup2.x; a13 = dir.x; a14 = 0.0;
	a21 = right.y; a22 = nup2.y; a23 = dir.y; a24 = 0.0;
	a31 = right.z; a32 = nup2.z; a33 = dir.z; a34 = 0.0;
	a41 = 0.0;     a42 = 0.0;    a43 = 0.0;   a44 = 1.0;
}

void decDMatrix::SetFromQuaternion(const decQuaternion &q){
	const double qx = (double)q.x;
	const double qy = (double)q.y;
	const double qz = (double)q.z;
	const double qw = (double)q.w;
	const double qlen = sqrt(qx * qx + qy * qy + qz * qz + qw * qw);
	if(qlen == 0.0){
		DETHROW(deeDivisionByZero);
	}
	const double inv = 1.0 / qlen;
	
	const double qnx = qx * inv;
	const double qny = qy * inv;
	const double qnz = qz * inv;
	const double qnw = qw * inv;
	
	const double sqnx = qnx * qnx;
	const double sqny = qny * qny;
	const double sqnz = qnz * qnz;
	const double sqnw = qnw * qnw;
	
	a11 =  sqnx - sqny - sqnz + sqnw;
	a22 = -sqnx + sqny - sqnz + sqnw;
	a33 = -sqnx - sqny + sqnz + sqnw;
	
	const double t1 = qnx * qny;
	const double t2 = qnz * qnw;
	a21 = 2.0 * (t1 + t2);
	a12 = 2.0 * (t1 - t2);
	
	const double t3 = qnx * qnz;
	const double t4 = qny * qnw;
	a31 = 2.0 * (t3 - t4);
	a13 = 2.0 * (t3 + t4);
	
	const double t5 = qny * qnz;
	const double t6 = qnx * qnw;
	a32 = 2.0 * (t5 + t6);
	a23 = 2.0 * (t5 - t6);
	
	a14 = 0.0;
	a24 = 0.0;
	a34 = 0.0;
	a41 = 0.0;
	a42 = 0.0;
	a43 = 0.0;
	a44 = 1.0;
}

void decDMatrix::SetFromQuaternion(const decQuaternion &q, const decDVector &pivot){
	SetFromQuaternion(q);
	
	a14 = pivot.x - pivot.x * a11 - pivot.y * a12 - pivot.z * a13;
	a24 = pivot.y - pivot.x * a21 - pivot.y * a22 - pivot.z * a23;
	a34 = pivot.z - pivot.x * a31 - pivot.y * a32 - pivot.z * a33;
}

void decDMatrix::Set(const decMatrix &matrix){
	a11 = matrix.a11;
	a12 = matrix.a12;
	a13 = matrix.a13;
	a14 = matrix.a14;
	
	a21 = matrix.a21;
	a22 = matrix.a22;
	a23 = matrix.a23;
	a24 = matrix.a24;
	
	a31 = matrix.a31;
	a32 = matrix.a32;
	a33 = matrix.a33;
	a34 = matrix.a34;
	
	a41 = matrix.a41;
	a42 = matrix.a42;
	a43 = matrix.a43;
	a44 = matrix.a44;
}

double decDMatrix::Determinant() const{
	return
		a14 * a23 * a32 * a41 - a13 * a24 * a32 * a41 - a14 * a22 * a33 * a41 + a12 * a24 * a33 * a41 +
		a13 * a22 * a34 * a41 - a12 * a23 * a34 * a41 - a14 * a23 * a31 * a42 + a13 * a24 * a31 * a42 +
		a14 * a21 * a33 * a42 - a11 * a24 * a33 * a42 - a13 * a21 * a34 * a42 + a11 * a23 * a34 * a42 +
		a14 * a22 * a31 * a43 - a12 * a24 * a31 * a43 - a14 * a21 * a32 * a43 + a11 * a24 * a32 * a43 +
		a12 * a21 * a34 * a43 - a11 * a22 * a34 * a43 - a13 * a22 * a31 * a44 + a12 * a23 * a31 * a44 +
		a13 * a21 * a32 * a44 - a11 * a23 * a32 * a44 - a12 * a21 * a33 * a44 + a11 * a22 * a33 * a44;
}

decDMatrix decDMatrix::Invert() const{
	decDMatrix m;
	
	m.a11 = a23 * a34 * a42 - a24 * a33 * a42 + a24 * a32 * a43 - a22 * a34 * a43 - a23 * a32 * a44 + a22 * a33 * a44;
	m.a12 = a14 * a33 * a42 - a13 * a34 * a42 - a14 * a32 * a43 + a12 * a34 * a43 + a13 * a32 * a44 - a12 * a33 * a44;
	m.a13 = a13 * a24 * a42 - a14 * a23 * a42 + a14 * a22 * a43 - a12 * a24 * a43 - a13 * a22 * a44 + a12 * a23 * a44;
	m.a14 = a14 * a23 * a32 - a13 * a24 * a32 - a14 * a22 * a33 + a12 * a24 * a33 + a13 * a22 * a34 - a12 * a23 * a34;
	m.a21 = a24 * a33 * a41 - a23 * a34 * a41 - a24 * a31 * a43 + a21 * a34 * a43 + a23 * a31 * a44 - a21 * a33 * a44;
	m.a22 = a13 * a34 * a41 - a14 * a33 * a41 + a14 * a31 * a43 - a11 * a34 * a43 - a13 * a31 * a44 + a11 * a33 * a44;
	m.a23 = a14 * a23 * a41 - a13 * a24 * a41 - a14 * a21 * a43 + a11 * a24 * a43 + a13 * a21 * a44 - a11 * a23 * a44;
	m.a24 = a13 * a24 * a31 - a14 * a23 * a31 + a14 * a21 * a33 - a11 * a24 * a33 - a13 * a21 * a34 + a11 * a23 * a34;
	m.a31 = a22 * a34 * a41 - a24 * a32 * a41 + a24 * a31 * a42 - a21 * a34 * a42 - a22 * a31 * a44 + a21 * a32 * a44;
	m.a32 = a14 * a32 * a41 - a12 * a34 * a41 - a14 * a31 * a42 + a11 * a34 * a42 + a12 * a31 * a44 - a11 * a32 * a44;
	m.a33 = a12 * a24 * a41 - a14 * a22 * a41 + a14 * a21 * a42 - a11 * a24 * a42 - a12 * a21 * a44 + a11 * a22 * a44;
	m.a34 = a14 * a22 * a31 - a12 * a24 * a31 - a14 * a21 * a32 + a11 * a24 * a32 + a12 * a21 * a34 - a11 * a22 * a34;
	m.a41 = a23 * a32 * a41 - a22 * a33 * a41 - a23 * a31 * a42 + a21 * a33 * a42 + a22 * a31 * a43 - a21 * a32 * a43;
	m.a42 = a12 * a33 * a41 - a13 * a32 * a41 + a13 * a31 * a42 - a11 * a33 * a42 - a12 * a31 * a43 + a11 * a32 * a43;
	m.a43 = a13 * a22 * a41 - a12 * a23 * a41 - a13 * a21 * a42 + a11 * a23 * a42 + a12 * a21 * a43 - a11 * a22 * a43;
	m.a44 = a12 * a23 * a31 - a13 * a22 * a31 + a13 * a21 * a32 - a11 * a23 * a32 - a12 * a21 * a33 + a11 * a22 * a33;
	
	return m / Determinant();
}

decDMatrix decDMatrix::Transpose() const{
	decDMatrix m;
	
	m.a11 = a11;
	m.a12 = a21;
	m.a13 = a31;
	m.a14 = a41;
	m.a21 = a12;
	m.a22 = a22;
	m.a23 = a32;
	m.a24 = a42;
	m.a31 = a13;
	m.a32 = a23;
	m.a33 = a33;
	m.a34 = a43;
	m.a41 = a14;
	m.a42 = a24;
	m.a43 = a34;
	m.a44 = a44;
	
	return m;
}

decDVector decDMatrix::GetEulerAngles() const{
	// the typical test uses angles.x=asinf(a23) and then tests for <PI/2 and >-PI/2 . unfortunately
	// the asinf implementation is quite picky about values outside the [-1..1] range. it is though
	// possible that a23 contains values which are slightly larger than 1 or slightly smaller than -1
	// due to rounding errors. this results in asinf returning NAN which breaks a lot of things. to
	// counter this problem the test is replaced with a24<1 and a24>-1 . this is the same as the asinf
	// based test but does not suffer from the problems due to rounding errors
	
	/*
	double halfPi = PI / 2.0;
	decDVector angles;
	angles.x = asin(a23);
	if(angles.x < halfPi){
		if(angles.x > -halfPi){
			angles.z = atan2(-a21, a22);
			angles.y = atan2(-a13, a33);
		}else{
			// undefined
			angles.z = -atan2(a31, a11);
			angles.y = 0.0;
		}
	}else{
		// undefined
		angles.z = atan2(a31, a11);
		angles.y = 0.0;
	}
	return angles;
	*/
	
	if(a23 < 1.0){
		if(a23 > -1.0){
			return decDVector(asin(a23), atan2(-a13, a33), atan2(-a21, a22));
			
		}else{ // undefined
			return decDVector(-PI * 0.5, 0.0, -atan2(a31, a11));
		}
		
	}else{ // undefined
		return decDVector(PI * 0.5, 0.0, atan2(a31, a11));
	}
}

decQuaternion decDMatrix::ToQuaternion() const{
	const double trace = a11 + a22 + a33;
	
	if(trace > 0.0){
		const double s = 0.5 / sqrt(trace + 1.0);
		return decQuaternion((float)((a32 - a23) * s), (float)((a13 - a31) * s),
			(float)((a21 - a12) * s), (float)(0.25 / s));
		
	}else if(a11 > a22 && a11 > a33){
		const double s = 2.0 * sqrt(1.0 + a11 - a22 - a33);
		return decQuaternion((float)(0.25 * s), (float)((a12 + a21) / s),
			(float)((a13 + a31) / s), (float)((a32 - a23) / s));
		
	}else if(a22 > a33){
		const double s = 2.0 * sqrt(1.0 + a22 - a11 - a33);
		return decQuaternion((float)((a12 + a21) / s), (float)(0.25 * s), 
			(float)((a23 + a32) / s), (float)((a13 - a31) / s));
		
	}else{
		const double s = 2.0 * sqrt(1.0 + a33 - a11 - a22);
		return decQuaternion((float)((a13 + a31) / s), (float)((a23 + a32) / s),
			(float)(0.25 * s), (float)((a21 - a12) / s));
	}
	
	/*
	const double trace = a11 + a22 + a33 + 1.0;
	
	if(trace > 0.0001) { // do not choose this too small or the numerical errors explode the result
		//printf( "version 1\n" );
		const double s = 0.5 / sqrt(trace);
		return decQuaternion((a32 - a23) * s, (a13 - a31) * s, (a21 - a12) * s, 0.25 / s);
		
	}else if(a11 > a22 && a11 > a33){
		//printf( "version 2\n" );
		const double s = 2.0 * sqrt(1.0 + a11 - a22 - a33);
		return decQuaternion(0.25 * s, (a12 + a21) / s, (a13 + a31) / s, (a23 - a32) / s);
		
	}else if(a22 > a33){
		//printf( "version 3\n" );
		const double s = 2.0 * sqrt(1.0 + a22 - a11 - a33);
		return decQuaternion((a12 + a21) / s, 0.25 * s, (a23 + a32) / s, (a13 - a31) / s);
		
	}else{
		//printf( "version 4\n" );
		const double s = 2.0 * sqrt(1.0 + a33 - a11 - a22);
		return decQuaternion((a13 + a31) / s, (a23 + a32) / s, 0.25 * s, (a12 - a21) / s);
	}
	*/
}

decDVector decDMatrix::GetScale() const{
	// the scaling is the length of the coordinate axes. with only translation and rotation
	// the length of the coordinate axes would be 1. thus their length is the scaling
	return decDVector(
		sqrt(a11 * a11 + a21 * a21 + a31 * a31),
		sqrt(a12 * a12 + a22 * a22 + a32 * a32),
		sqrt(a13 * a13 + a23 * a23 + a33 * a33));
}

decDVector decDMatrix::GetPosition() const{
	return decDVector(a14, a24, a34);
}

decDVector decDMatrix::TransformView() const{
	return decDVector(a13, a23, a33);
}

decDVector decDMatrix::TransformRight() const{
	return decDVector(a11, a21, a31);
}

decDVector decDMatrix::TransformUp() const{
	return decDVector(a12, a22, a32);
}

decMatrix decDMatrix::ToMatrix() const{
	decMatrix m;
	
	m.a11 = (float)a11;
	m.a12 = (float)a12;
	m.a13 = (float)a13;
	m.a14 = (float)a14;
	
	m.a21 = (float)a21;
	m.a22 = (float)a22;
	m.a23 = (float)a23;
	m.a24 = (float)a24;
	
	m.a31 = (float)a31;
	m.a32 = (float)a32;
	m.a33 = (float)a33;
	m.a34 = (float)a34;
	
	m.a41 = (float)a41;
	m.a42 = (float)a42;
	m.a43 = (float)a43;
	m.a44 = (float)a44;
	
	return m;
}

decDVector decDMatrix::TransformNormal(const decDVector &v) const{
	return decDVector(
		a11 * v.x + a12 * v.y + a13 * v.z,
		a21 * v.x + a22 * v.y + a23 * v.z,
		a31 * v.x + a32 * v.y + a33 * v.z);
}

decDVector decDMatrix::Transform(double x, double y, double z) const{
	return decDVector(
		a11 * x + a12 * y + a13 * z + a14,
		a21 * x + a22 * y + a23 * z + a24,
		a31 * x + a32 * y + a33 * z + a34);
}

decDVector4 decDMatrix::Transform(double x, double y, double z, double w) const{
	return decDVector4(
		a11 * x + a12 * y + a13 * z + a14 * w,
		a21 * x + a22 * y + a23 * z + a24 * w,
		a31 * x + a32 * y + a33 * z + a34 * w,
		a41 * x + a42 * y + a43 * z + a44 * w);
}

void decDMatrix::Transform(decDVector &result, double x, double y, double z) const{
	result.x = a11 * x + a12 * y + a13 * z + a14;
	result.y = a21 * x + a22 * y + a23 * z + a24;
	result.z = a31 * x + a32 * y + a33 * z + a34;
}

void decDMatrix::Transform(decDVector4 &result, double x, double y, double z, double w) const{
	result.x = a11 * x + a12 * y + a13 * z + a14 * w;
	result.y = a21 * x + a22 * y + a23 * z + a24 * w;
	result.z = a31 * x + a32 * y + a33 * z + a34 * w;
	result.w = a41 * x + a42 * y + a43 * z + a44 * w;
}

decDMatrix decDMatrix::GetRotationMatrix() const{
	decDMatrix m;
	
	m.a11 = a11;
	m.a12 = a12;
	m.a13 = a13;
	m.a21 = a21;
	m.a22 = a22;
	m.a23 = a23;
	m.a31 = a31;
	m.a32 = a32;
	m.a33 = a33;
	
	return m;
}

void decDMatrix::Normalize(){
	decVector view(TransformView());
	if(view.IsZero()){
		view.Set(0.0, 0.0, 1.0);
	}
	
	decVector up(TransformUp());
	if(up.IsZero()){
		up.Set(0.0, 1.0, 0.0);
	}
	
	SetWorld(GetPosition(), view, up);
}

decDMatrix decDMatrix::Normalized() const{
	decVector view(TransformView());
	if(view.IsZero()){
		view.Set(0.0, 0.0, 1.0);
	}
	
	decVector up(TransformUp());
	if(up.IsZero()){
		up.Set(0.0, 1.0, 0.0);
	}
	
	return CreateWorld(GetPosition(), view, up);
}

bool decDMatrix::IsEqualTo(const decDMatrix &matrix, double threshold) const{
	return fabs(a11 - matrix.a11) < threshold
		&& fabs(a12 - matrix.a12) < threshold
		&& fabs(a13 - matrix.a13) < threshold
		&& fabs(a14 - matrix.a14) < threshold
		&& fabs(a21 - matrix.a21) < threshold
		&& fabs(a22 - matrix.a22) < threshold
		&& fabs(a23 - matrix.a23) < threshold
		&& fabs(a24 - matrix.a24) < threshold
		&& fabs(a31 - matrix.a31) < threshold
		&& fabs(a32 - matrix.a32) < threshold
		&& fabs(a33 - matrix.a33) < threshold
		&& fabs(a34 - matrix.a34) < threshold
		&& fabs(a41 - matrix.a41) < threshold
		&& fabs(a42 - matrix.a42) < threshold
		&& fabs(a43 - matrix.a43) < threshold
		&& fabs(a44 - matrix.a44) < threshold;
}

decDMatrix decDMatrix::QuickMultiply(const decDMatrix &m) const{
	decDMatrix n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13;
	n.a12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13;
	n.a13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13;
	n.a14 = a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + m.a14;
	n.a21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23;
	n.a22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23;
	n.a23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23;
	n.a24 = a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + m.a24;
	n.a31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33;
	n.a32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33;
	n.a33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33;
	n.a34 = a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + m.a34;
	n.a41 = 0.0;
	n.a42 = 0.0;
	n.a43 = 0.0;
	n.a44 = 1.0;
	
	return n;
}

decDMatrix decDMatrix::QuickMultiplyRotation(const decDMatrix &m) const{
	decDMatrix n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13;
	n.a12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13;
	n.a13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13;
	n.a14 = 0.0;
	n.a21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23;
	n.a22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23;
	n.a23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23;
	n.a24 = 0.0;
	n.a31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33;
	n.a32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33;
	n.a33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33;
	n.a34 = 0.0;
	n.a41 = 0.0;
	n.a42 = 0.0;
	n.a43 = 0.0;
	n.a44 = 1.0;
	
	return n;
}

decDMatrix decDMatrix::QuickMultiply(double k) const{
	decDMatrix n;
	
	n.a11 = a11 * k;
	n.a12 = a12 * k;
	n.a13 = a13 * k;
	n.a14 = a14 * k;
	n.a21 = a21 * k;
	n.a22 = a22 * k;
	n.a23 = a23 * k;
	n.a24 = a24 * k;
	n.a31 = a31 * k;
	n.a32 = a32 * k;
	n.a33 = a33 * k;
	n.a34 = a34 * k;
	n.a41 = 0.0;
	n.a42 = 0.0;
	n.a43 = 0.0;
	n.a44 = 1.0;
	
	return n;
}

double decDMatrix::QuickDeterminant() const{
	return a12 * a23 * a31 + a13 * a21 * a32 + a11 * a22 * a33
		- a13 * a22 * a31 - a11 * a23 * a32 - a12 * a21 * a33;
}

decDMatrix decDMatrix::QuickInvert() const{
	const double t01 = a11 * a22;
	const double t02 = a11 * a23;
	const double t03 = a11 * a32;
	const double t04 = a11 * a33;
	
	const double t05 = a12 * a21;
	const double t06 = a12 * a23;
	const double t07 = a12 * a31;
	const double t08 = a12 * a33;
	
	const double t09 = a13 * a21;
	const double t10 = a13 * a22;
	const double t11 = a13 * a31;
	const double t12 = a13 * a32;
	
	const double t13 = a21 * a32;
	const double t14 = a21 * a33;
	
	const double t15 = a22 * a31;
	const double t16 = a22 * a33;
	
	const double t17 = a23 * a32;
	const double t18 = a23 * a31;
	
	const double det = t06 * a31 + t09 * a32 + t01 * a33 - t10 * a31 - t02 * a32 - t05 * a33;
	if(det == 0.0){
		DETHROW(deeDivisionByZero);
	}
	const double factor = 1.0 / det;
	
	decDMatrix m;
	
	m.a11 = (t16 - t17) * factor;
	m.a12 = (t12 - t08) * factor;
	m.a13 = (t06 - t10) * factor;
	m.a14 = (t17 * a14 - t12 * a24 - t16 * a14 + t08 * a24 + t10 * a34 - t06 * a34) * factor;
	m.a21 = (t18 - t14) * factor;
	m.a22 = (t04 - t11) * factor;
	m.a23 = (t09 - t02) * factor;
	m.a24 = (t11 * a24 - t18 * a14 + t14 * a14 - t04 * a24 - t09 * a34 + t02 * a34) * factor;
	m.a31 = (t13 - t15) * factor;
	m.a32 = (t07 - t03) * factor;
	m.a33 = (t01 - t05) * factor;
	m.a34 = (t15 * a14 - t07 * a24 - t13 * a14 + t03 * a24 + t05 * a34 - t01 * a34) * factor;
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 0.0;
	m.a44 = 1.0;
	
	return m;
}

decDMatrix &decDMatrix::QuickAddTo(const decDMatrix &m){
	a11 += m.a11; a12 += m.a12; a13 += m.a13; a14 += m.a14;
	a21 += m.a21; a22 += m.a22; a23 += m.a23; a24 += m.a24;
	a31 += m.a31; a32 += m.a32; a33 += m.a33; a34 += m.a34;
	return *this;
}



// Operators
//////////////

decDMatrix &decDMatrix::operator=(const decDMatrix &m){
	a11 = m.a11; a12 = m.a12; a13 = m.a13; a14 = m.a14;
	a21 = m.a21; a22 = m.a22; a23 = m.a23; a24 = m.a24;
	a31 = m.a31; a32 = m.a32; a33 = m.a33; a34 = m.a34;
	a41 = m.a41; a42 = m.a42; a43 = m.a43; a44 = m.a44;
	return *this;
}

decDMatrix &decDMatrix::operator+=(const decDMatrix &m){
	a11 += m.a11; a12 += m.a12; a13 += m.a13; a14 += m.a14;
	a21 += m.a21; a22 += m.a22; a23 += m.a23; a24 += m.a24;
	a31 += m.a31; a32 += m.a32; a33 += m.a33; a34 += m.a34;
	a41 += m.a41; a42 += m.a42; a43 += m.a43; a44 += m.a44;
	return *this;
}

decDMatrix &decDMatrix::operator-=(const decDMatrix &m){
	a11 -= m.a11; a12 -= m.a12; a13 -= m.a13; a14 -= m.a14;
	a21 -= m.a21; a22 -= m.a22; a23 -= m.a23; a24 -= m.a24;
	a31 -= m.a31; a32 -= m.a32; a33 -= m.a33; a34 -= m.a34;
	a41 -= m.a41; a42 -= m.a42; a43 -= m.a43; a44 -= m.a44;
	return *this;
}

decDMatrix &decDMatrix::operator*=(double k){
	a11 *= k; a12 *= k; a13 *= k; a14 *= k;
	a21 *= k; a22 *= k; a23 *= k; a24 *= k;
	a31 *= k; a32 *= k; a33 *= k; a34 *= k;
	a41 *= k; a42 *= k; a43 *= k; a44 *= k;
	return *this;
}

decDMatrix &decDMatrix::operator/=(double k){
	if(k == 0.0f){
		DETHROW(deeDivisionByZero);
	}
	
	a11 /= k; a12 /= k; a13 /= k; a14 /= k;
	a21 /= k; a22 /= k; a23 /= k; a24 /= k;
	a31 /= k; a32 /= k; a33 /= k; a34 /= k;
	a41 /= k; a42 /= k; a43 /= k; a44 /= k;
	
	return *this;
}

decDMatrix &decDMatrix::operator*=(const decDMatrix &m){
	const double t11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13 + a41 * m.a14;
	const double t12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13 + a42 * m.a14;
	const double t13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13 + a43 * m.a14;
	const double t14 = a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + a44 * m.a14;
	const double t21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23 + a41 * m.a24;
	const double t22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23 + a42 * m.a24;
	const double t23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23 + a43 * m.a24;
	const double t24 = a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + a44 * m.a24;
	const double t31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33 + a41 * m.a34;
	const double t32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33 + a42 * m.a34;
	const double t33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33 + a43 * m.a34;
	const double t34 = a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + a44 * m.a34;
	const double t41 = a11 * m.a41 + a21 * m.a42 + a31 * m.a43 + a41 * m.a44;
	const double t42 = a12 * m.a41 + a22 * m.a42 + a32 * m.a43 + a42 * m.a44;
	const double t43 = a13 * m.a41 + a23 * m.a42 + a33 * m.a43 + a43 * m.a44;
	const double t44 = a14 * m.a41 + a24 * m.a42 + a34 * m.a43 + a44 * m.a44;
	
	a11 = t11; a12 = t12; a13 = t13; a14 = t14;
	a21 = t21; a22 = t22; a23 = t23; a24 = t24;
	a31 = t31; a32 = t32; a33 = t33; a34 = t34;
	a41 = t41; a42 = t42; a43 = t43; a44 = t44;
	
	return *this;
}

decDMatrix decDMatrix::operator+(const decDMatrix &m) const{
	decDMatrix n;
	
	n.a11 = a11 + m.a11; n.a12 = a12 + m.a12; n.a13 = a13 + m.a13; n.a14 = a14 + m.a14;
	n.a21 = a21 + m.a21; n.a22 = a22 + m.a22; n.a23 = a23 + m.a23; n.a24 = a24 + m.a24;
	n.a31 = a31 + m.a31; n.a32 = a32 + m.a32; n.a33 = a33 + m.a33; n.a34 = a34 + m.a34;
	n.a41 = a41 + m.a41; n.a42 = a42 + m.a42; n.a43 = a43 + m.a43; n.a44 = a44 + m.a44;
	
	return n;
}

decDMatrix decDMatrix::operator-(const decDMatrix &m) const{
	decDMatrix n;
	
	n.a11 = a11 - m.a11; n.a12 = a12 - m.a12; n.a13 = a13 - m.a13; n.a14 = a14 - m.a14;
	n.a21 = a21 - m.a21; n.a22 = a22 - m.a22; n.a23 = a23 - m.a23; n.a24 = a24 - m.a24;
	n.a31 = a31 - m.a31; n.a32 = a32 - m.a32; n.a33 = a33 - m.a33; n.a34 = a34 - m.a34;
	n.a41 = a41 - m.a41; n.a42 = a42 - m.a42; n.a43 = a43 - m.a43; n.a44 = a44 - m.a44;
	
	return n;
}

decDMatrix decDMatrix::operator*(double k) const{
	decDMatrix n;
	
	n.a11 = a11 * k; n.a12 = a12 * k; n.a13 = a13 * k; n.a14 = a14 * k;
	n.a21 = a21 * k; n.a22 = a22 * k; n.a23 = a23 * k; n.a24 = a24 * k;
	n.a31 = a31 * k; n.a32 = a32 * k; n.a33 = a33 * k; n.a34 = a34 * k;
	n.a41 = a41 * k; n.a42 = a42 * k; n.a43 = a43 * k; n.a44 = a44 * k;
	
	return n;
}

decDMatrix decDMatrix::operator/(double k) const{
	if(k == 0.0){
		DETHROW(deeDivisionByZero);
	}
	
	decDMatrix n;
	
	n.a11 = a11 / k; n.a12 = a12 / k; n.a13 = a13 / k; n.a14 = a14 / k;
	n.a21 = a21 / k; n.a22 = a22 / k; n.a23 = a23 / k; n.a24 = a24 / k;
	n.a31 = a31 / k; n.a32 = a32 / k; n.a33 = a33 / k; n.a34 = a34 / k;
	n.a41 = a41 / k; n.a42 = a42 / k; n.a43 = a43 / k; n.a44 = a44 / k;
	
	return n;
}

decDMatrix decDMatrix::operator*(const decDMatrix &m) const{
	decDMatrix n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13 + a41 * m.a14;
	n.a12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13 + a42 * m.a14;
	n.a13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13 + a43 * m.a14;
	n.a14 = a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + a44 * m.a14;
	n.a21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23 + a41 * m.a24;
	n.a22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23 + a42 * m.a24;
	n.a23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23 + a43 * m.a24;
	n.a24 = a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + a44 * m.a24;
	n.a31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33 + a41 * m.a34;
	n.a32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33 + a42 * m.a34;
	n.a33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33 + a43 * m.a34;
	n.a34 = a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + a44 * m.a34;
	n.a41 = a11 * m.a41 + a21 * m.a42 + a31 * m.a43 + a41 * m.a44;
	n.a42 = a12 * m.a41 + a22 * m.a42 + a32 * m.a43 + a42 * m.a44;
	n.a43 = a13 * m.a41 + a23 * m.a42 + a33 * m.a43 + a43 * m.a44;
	n.a44 = a14 * m.a41 + a24 * m.a42 + a34 * m.a43 + a44 * m.a44;
	
	return n;
}

decDVector decDMatrix::operator*(const decDVector &v) const{
	return decDVector(
		a11 * v.x + a12 * v.y + a13 * v.z + a14,
		a21 * v.x + a22 * v.y + a23 * v.z + a24,
		a31 * v.x + a32 * v.y + a33 * v.z + a34);
}
decDVector4 decDMatrix::operator*(const decDVector4 &v) const{
	return decDVector4(
		a11 * v.x + a12 * v.y + a13 * v.z + a14 * v.w,
		a21 * v.x + a22 * v.y + a23 * v.z + a24 * v.w,
		a31 * v.x + a32 * v.y + a33 * v.z + a34 * v.w,
		a41 * v.x + a42 * v.y + a43 * v.z + a44 * v.w);
}
