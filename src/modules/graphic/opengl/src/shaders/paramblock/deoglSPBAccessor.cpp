/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglSPBAccessor.h"
#include "deoglShaderParameterBlock.h"
#include "../deoglShaderCompiled.h"

#include <dragengine/common/exceptions.h>


// Helper Functions
/////////////////////

namespace{

static inline void fSet(GLfloat *d, float v){
	d[0] = (GLfloat)v;
}
static inline void fSet(GLfloat *d, float v1, float v2){
	d[0] = (GLfloat)v1;
	d[1] = (GLfloat)v2;
}
static inline void fSet(GLfloat *d, float v1, float v2, float v3){
	d[0] = (GLfloat)v1;
	d[1] = (GLfloat)v2;
	d[2] = (GLfloat)v3;
}
static inline void fSet(GLfloat *d, float v1, float v2, float v3, float v4){
	d[0] = (GLfloat)v1;
	d[1] = (GLfloat)v2;
	d[2] = (GLfloat)v3;
	d[3] = (GLfloat)v4;
}

// static inline void fSet( GLfloat *d, double v ){
// 	fSet( d, ( GLfloat )v );
// }
static inline void fSet(GLfloat *d, double v1, double v2){
	fSet(d, (GLfloat)v1, (GLfloat)v2);
}
static inline void fSet(GLfloat *d, double v1, double v2, double v3){
	fSet(d, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3);
}
static inline void fSet(GLfloat *d, double v1, double v2, double v3, double v4){
	fSet(d, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3, (GLfloat)v4);
}

static inline void fSet(GLint *d, int v){
	d[0] = (GLint)v;
}
static inline void fSet(GLint *d, int v1, int v2){
	d[0] = (GLint)v1;
	d[1] = (GLint)v2;
}
static inline void fSet(GLint *d, int v1, int v2, int v3){
	d[0] = (GLint)v1;
	d[1] = (GLint)v2;
	d[2] = (GLint)v3;
}
static inline void fSet(GLint *d, int v1, int v2, int v3, int v4){
	d[0] = (GLint)v1;
	d[1] = (GLint)v2;
	d[2] = (GLint)v3;
	d[3] = (GLint)v4;
}

static inline void fSet(GLuint *d, bool v){
	d[0] = (GLuint)v;
}
static inline void fSet(GLuint *d, bool v1, bool v2){
	d[0] = (GLuint)v1;
	d[1] = (GLuint)v2;
}
static inline void fSet(GLuint *d, bool v1, bool v2, bool v3){
	d[0] = (GLuint)v1;
	d[1] = (GLuint)v2;
	d[2] = (GLuint)v3;
}
static inline void fSet(GLuint *d, bool v1, bool v2, bool v3, bool v4){
	d[0] = (GLuint)v1;
	d[1] = (GLuint)v2;
	d[2] = (GLuint)v3;
	d[3] = (GLuint)v4;
}

static inline void fSet(GLuint *d, unsigned int v){
	d[0] = (GLuint)v;
}
static inline void fSet(GLuint *d, unsigned int v1, unsigned int v2){
	d[0] = (GLuint)v1;
	d[1] = (GLuint)v2;
}
static inline void fSet(GLuint *d, unsigned int v1, unsigned int v2, unsigned int v3){
	d[0] = (GLuint)v1;
	d[1] = (GLuint)v2;
	d[2] = (GLuint)v3;
}
static inline void fSet(GLuint *d, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4){
	d[0] = (GLuint)v1;
	d[1] = (GLuint)v2;
	d[2] = (GLuint)v3;
	d[3] = (GLuint)v4;
}

}


// Class deoglSPBAccessor
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBAccessor::deoglSPBAccessor(const deoglShaderParameterBlock &paramBlock) :
deoglSPBAccessor(paramBlock, paramBlock.GetElementLower()){
}

deoglSPBAccessor::deoglSPBAccessor(const deoglShaderParameterBlock &paramBlock, int element) :
pParamBlock(paramBlock),
pParameter(nullptr)
{
	DEASSERT_NOTNULL(pParamBlock.GetMappedBuffer())
	DEASSERT_TRUE(element >= pParamBlock.GetElementLower())
	DEASSERT_TRUE(element <= pParamBlock.GetElementUpper())
	
	pOffset = paramBlock.GetElementStride() * (element - pParamBlock.GetElementLower());
}

deoglSPBAccessor::deoglSPBAccessor(const deoglSPBAccessor &other) :
pParamBlock(other.pParamBlock),
pParameter(other.pParameter),
pOffset(other.pOffset){
}

deoglSPBAccessor::~deoglSPBAccessor() = default;

deoglSPBAccessor::deoglSPBAccessor(const deoglSPBAccessor &accessor,
	const deoglSPBParameter &parameter, int arrayIndex) :
pParamBlock(accessor.pParamBlock),
pParameter(&parameter)
{
	DEASSERT_TRUE(parameter.GetValueType() == deoglSPBParameter::evtStruct)
	DEASSERT_TRUE(arrayIndex >= 0)
	DEASSERT_TRUE(arrayIndex < parameter.GetArrayCount())
	
	pOffset = accessor.pOffset + parameter.GetArrayStride() * arrayIndex + parameter.GetOffset();
}


// Management
///////////////

void deoglSPBAccessor::SetFloat(int p, float v){
	fSet(pDataFloat(pParamFloat(p, 1)), v);
}
 
void deoglSPBAccessor::SetVec2(int p, float v1, float v2){
	fSet(pDataFloat(pParamFloat(p, 2)), v1, v2);
}
void deoglSPBAccessor::SetVec2(int p, const decVector2 &v){
	fSet(pDataFloat(pParamFloat(p, 2)), v.x, v.y);
}

void deoglSPBAccessor::SetVec3(int p, float v1, float v2, float v3){
	fSet(pDataFloat(pParamFloat(p, 3)), v1, v2, v3);
}
void deoglSPBAccessor::SetVec3(int p, const decColor &c){
	fSet(pDataFloat(pParamFloat(p, 3)), c.r, c.g, c.b);
}
void deoglSPBAccessor::SetVec3(int p, const decVector &v){
	fSet(pDataFloat(pParamFloat(p, 3)), v.x, v.y, v.z);
}
void deoglSPBAccessor::SetVec3(int p, const decDVector &v){
	fSet(pDataFloat(pParamFloat(p, 3)), v.x, v.y, v.z);
}
void deoglSPBAccessor::SetVec3(int p, const oglVector3 &v){
	fSet(pDataFloat(pParamFloat(p, 3)), v.x, v.y, v.z);
}

void deoglSPBAccessor::SetVec4(int p, float v1, float v2, float v3, float v4){
	fSet(pDataFloat(pParamFloat(p, 4)), v1, v2, v3, v4);
}
void deoglSPBAccessor::SetVec4(int p, const decColor &c){
	fSet(pDataFloat(pParamFloat(p, 4)), c.r, c.g, c.b, c.a);
}
void deoglSPBAccessor::SetVec4(int p, const decColor &c, float a){
	fSet(pDataFloat(pParamFloat(p, 4)), c.r, c.g, c.b, a);
}
void deoglSPBAccessor::SetVec4(int p, const decVector4 &v){
	fSet(pDataFloat(pParamFloat(p, 4)), v.x, v.y, v.z, v.w);
}
void deoglSPBAccessor::SetVec4(int p, const decDVector4 &v){
	fSet(pDataFloat(pParamFloat(p, 4)), v.x, v.y, v.z, v.w);
}
void deoglSPBAccessor::SetVec4(int p, const decVector &v, float w){
	fSet(pDataFloat(pParamFloat(p, 4)), v.x, v.y, v.z, w);
}
void deoglSPBAccessor::SetVec4(int p, const decDVector &v, double w){
	fSet(pDataFloat(pParamFloat(p, 4)), v.x, v.y, v.z, w);
}
void deoglSPBAccessor::SetVec4(int p, const decDVector4 &v, double w){
	fSet(pDataFloat(pParamFloat(p, 4)), v.x, v.y, v.z, w);
}
void deoglSPBAccessor::SetVec4(int p, const oglVector3 &v, float w){
	fSet(pDataFloat(pParamFloat(p, 4)), v.x, v.y, v.z, w);
}


void deoglSPBAccessor::SetMat3x2(int p, const float *v){
	GLfloat *d[3];
	if(pParamBlock.GetRowMajor()){
		pDataMatrix2(pParamFloat(p, 3, 2), d);
		fSet(d[0], v[0], v[1], v[2]);
		fSet(d[1], v[3], v[4], v[5]);
		
	}else{
		pDataMatrix3(pParamFloat(p, 3, 2), d);
		fSet(d[0], v[0], v[3]);
		fSet(d[1], v[1], v[4]);
		fSet(d[2], v[2], v[5]);
	}
}

#define IMPL_SET_PARAM_DATA_MAT_3x2(p,e,m) \
	GLfloat *d[3]; \
	if(pParamBlock.GetRowMajor()){ \
		pDataMatrix2(pParamFloat(p, 3, 2), d); \
		fSet(d[0], m.a11, m.a12, m.a13); \
		fSet(d[1], m.a21, m.a22, m.a23); \
	}else{ \
		pDataMatrix3(pParamFloat(p, 3, 2), d); \
		fSet(d[0], m.a11, m.a21); \
		fSet(d[1], m.a12, m.a22); \
		fSet(d[2], m.a13, m.a23); \
	}

void deoglSPBAccessor::SetMat3x2(int p, const decMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_3x2(p, e, m)
}
void deoglSPBAccessor::SetMat3x2(int p, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_3x2(p, e, m)
}
void deoglSPBAccessor::SetMat3x2(int p, const decTexMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_3x2(p, e, m)
}
void deoglSPBAccessor::SetMat3x2(int p, const decTexMatrix2 &m){
	IMPL_SET_PARAM_DATA_MAT_3x2(p, e, m)
}


void deoglSPBAccessor::SetMat3x3(int p, const float *v){
	GLfloat *d[3];
	pDataMatrix3(pParamFloat(p, 3, 3), d);
	
	if(pParamBlock.GetRowMajor()){
		fSet(d[0], v[0], v[1], v[2]);
		fSet(d[1], v[3], v[4], v[5]);
		fSet(d[2], v[6], v[7], v[8]);
		
	}else{
		fSet(d[0], v[0], v[3], v[6]);
		fSet(d[1], v[1], v[4], v[7]);
		fSet(d[2], v[2], v[5], v[8]);
	}
}

#define IMPL_SET_PARAM_DATA_MAT_3x3(p,m) \
	GLfloat *d[3]; \
	pDataMatrix3(pParamFloat(p, 3, 3), d); \
	if(pParamBlock.GetRowMajor()){ \
		fSet(d[0], m.a11, m.a12, m.a13); \
		fSet(d[1], m.a21, m.a22, m.a23); \
		fSet(d[2], m.a31, m.a32, m.a33); \
	}else{ \
		fSet(d[0], m.a11, m.a21, m.a31); \
		fSet(d[1], m.a12, m.a22, m.a32); \
		fSet(d[2], m.a13, m.a23, m.a33); \
	}

void deoglSPBAccessor::SetMat3x3(int p, const decMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_3x3(p, m)
}
void deoglSPBAccessor::SetMat3x3(int p, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_3x3(p, m)
}
void deoglSPBAccessor::SetMat3x3(int p, const decTexMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_3x3(p, m)
}

void deoglSPBAccessor::SetMat4x3(int p, const float *v){
	GLfloat *d[4];
	if(pParamBlock.GetRowMajor()){
		pDataMatrix3(pParamFloat(p, 4, 3), d);
		fSet(d[0], v[0], v[1], v[2], v[3]);
		fSet(d[1], v[4], v[5], v[6], v[7]);
		fSet(d[2], v[8], v[9], v[10], v[11]);
		
	}else{
		pDataMatrix4(pParamFloat(p, 4, 3), d);
		fSet(d[0], v[0], v[4], v[8]);
		fSet(d[1], v[1], v[5], v[9]);
		fSet(d[2], v[2], v[6], v[10]);
		fSet(d[3], v[3], v[7], v[11]);
	}
}

#define IMPL_SET_PARAM_DATA_MAT_4x3(p,m) \
	GLfloat *d[4]; \
	if(pParamBlock.GetRowMajor()){ \
		pDataMatrix3(pParamFloat(p, 4, 3), d); \
		fSet(d[0], m.a11, m.a12, m.a13, m.a14); \
		fSet(d[1], m.a21, m.a22, m.a23, m.a24); \
		fSet(d[2], m.a31, m.a32, m.a33, m.a34); \
	}else{ \
		pDataMatrix4(pParamFloat(p, 4, 3), d); \
		fSet(d[0], m.a11, m.a21, m.a31); \
		fSet(d[1], m.a12, m.a22, m.a32); \
		fSet(d[2], m.a13, m.a23, m.a33); \
		fSet(d[3], m.a14, m.a24, m.a34); \
	}

void deoglSPBAccessor::SetMat4x3(int p, const decMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_4x3(p, m)
}
void deoglSPBAccessor::SetMat4x3(int p, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_4x3(p, m)
}
void deoglSPBAccessor::SetMat4x3(int p, const oglMatrix3x4 &m){
	IMPL_SET_PARAM_DATA_MAT_4x3(p, m)
}


void deoglSPBAccessor::SetMat4x4(int p, const float *v){
	GLfloat *d[4];
	pDataMatrix4(pParamFloat(p, 4, 4), d);
	
	if(pParamBlock.GetRowMajor()){
		fSet(d[0], v[0], v[1], v[2], v[3]);
		fSet(d[1], v[4], v[5], v[6], v[7]);
		fSet(d[2], v[8], v[9], v[10], v[11]);
		fSet(d[3], v[12], v[13], v[14], v[15]);
		
	}else{
		fSet(d[0], v[0], v[4], v[8], v[12]);
		fSet(d[1], v[1], v[5], v[9], v[13]);
		fSet(d[2], v[2], v[6], v[10], v[14]);
		fSet(d[3], v[3], v[7], v[11], v[15]);
	}
}

#define IMPL_SET_PARAM_DATA_MAT_4x4(p,m) \
	GLfloat *d[4]; \
	pDataMatrix4(pParamFloat(p, 4, 4), d); \
	if(pParamBlock.GetRowMajor()){ \
		fSet(d[0], m.a11, m.a12, m.a13, m.a14); \
		fSet(d[1], m.a21, m.a22, m.a23, m.a24); \
		fSet(d[2], m.a31, m.a32, m.a33, m.a34); \
		fSet(d[3], m.a41, m.a42, m.a43, m.a44); \
	}else{ \
		fSet(d[0], m.a11, m.a21, m.a31, m.a41); \
		fSet(d[1], m.a12, m.a22, m.a32, m.a42); \
		fSet(d[2], m.a13, m.a23, m.a33, m.a43); \
		fSet(d[3], m.a14, m.a24, m.a34, m.a44); \
	}

void deoglSPBAccessor::SetMat4x4(int p, const decMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_4x4(p, m)
}
void deoglSPBAccessor::SetMat4x4(int p, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_MAT_4x4(p, m)
}

void deoglSPBAccessor::SetInt(int p, int v){
	fSet(pDataInt(pParamInt(p, 1)), v);
}
void deoglSPBAccessor::SetIVec2(int p, int v1, int v2){
	fSet(pDataInt(pParamInt(p, 2)), v1, v2);
}
void deoglSPBAccessor::SetIVec3(int p, int v1, int v2, int v3){
	fSet(pDataInt(pParamInt(p, 3)), v1, v2, v3);
}
void deoglSPBAccessor::SetIVec3(int p, const decPoint3 &v){
	fSet(pDataInt(pParamInt(p, 3)), v.x, v.y, v.z);
}

void deoglSPBAccessor::SetIVec4(int p, int v1, int v2, int v3, int v4){
	fSet(pDataInt(pParamInt(p, 4)), v1, v2, v3, v4);
}

void deoglSPBAccessor::SetUInt(int p, unsigned int v){
	fSet(pDataUInt(pParamInt(p, 1)), v);
}
void deoglSPBAccessor::SetUVec2(int p, unsigned int v1, unsigned int v2){
	fSet(pDataUInt(pParamInt(p, 2)), v1, v2);
}
void deoglSPBAccessor::SetUVec3(int p, unsigned int v1, unsigned int v2, unsigned int v3){
	fSet(pDataUInt(pParamInt(p, 3)), v1, v2, v3);
}
void deoglSPBAccessor::SetUVec4(int p, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4){
	fSet(pDataUInt(pParamInt(p, 4)), v1, v2, v3, v4);
}

void deoglSPBAccessor::SetBool(int p, bool v){
	fSet(pDataUInt(pParamBool(p, 1)), v);
}
void deoglSPBAccessor::SetBVec2(int p, bool v1, bool v2){
	fSet(pDataUInt(pParamBool(p, 2)), v1, v2);
}
void deoglSPBAccessor::SetBVec3(int p, bool v1, bool v2, bool v3){
	fSet(pDataUInt(pParamBool(p, 3)), v1, v2, v3);
}
void deoglSPBAccessor::SetBVec4(int p, bool v1, bool v2, bool v3, bool v4){
	fSet(pDataUInt(pParamBool(p, 4)), v1, v2, v3, v4);
}

void deoglSPBAccessor::SetArrayFloat(int p, int a, float v){
	fSet(pDataFloat(pParamFloat(p, 1, 1, a), a), v);
}

void deoglSPBAccessor::SetArrayVec2(int p, int a, float v1, float v2){
	fSet(pDataFloat(pParamFloat(p, 2, 1, a), a), v1, v2);
}
void deoglSPBAccessor::SetArrayVec2(int p, int a, const decVector2& v) {
	fSet(pDataFloat(pParamFloat(p, 2, 1, a), a), v.x, v.y);
}

void deoglSPBAccessor::SetArrayVec3(int p, int a, float v1, float v2, float v3){
	fSet(pDataFloat(pParamFloat(p, 3, 1, a), a), v1, v2, v3);
}
void deoglSPBAccessor::SetArrayVec3(int p, int a, const decColor &c){
	fSet(pDataFloat(pParamFloat(p, 3, 1, a), a), c.r, c.g, c.b);
}
void deoglSPBAccessor::SetArrayVec3(int p, int a, const decVector &v){
	fSet(pDataFloat(pParamFloat(p, 3, 1, a), a), v.x, v.y, v.z);
}
void deoglSPBAccessor::SetArrayVec3(int p, int a, const decDVector &v){
	fSet(pDataFloat(pParamFloat(p, 3, 1, a), a), v.x, v.y, v.z);
}
void deoglSPBAccessor::SetArrayVec3(int p, int a, const oglVector3 &v){
	fSet(pDataFloat(pParamFloat(p, 3, 1, a), a), v.x, v.y, v.z);
}

void deoglSPBAccessor::SetArrayVec4(int p, int a, float v1, float v2, float v3, float v4){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), v1, v2, v3, v4);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const decColor &c){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), c.r, c.g, c.b, c.a);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const decColor &c, float al){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), c.r, c.g, c.b, al);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const decVector4 &v){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), v.x, v.y, v.z, v.w);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const decDVector4 &v){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), v.x, v.y, v.z, v.w);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const decVector &v, float w){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), v.x, v.y, v.z, w);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const decDVector &v, double w){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), v.x, v.y, v.z, w);
}
void deoglSPBAccessor::SetArrayVec4(int p, int a, const oglVector3 &v, float w){
	fSet(pDataFloat(pParamFloat(p, 4, 1, a), a), v.x, v.y, v.z, w);
}

void deoglSPBAccessor::SetArrayMat3x2(int p, int a, const float *v){
	GLfloat *d[3];
	if(pParamBlock.GetRowMajor()){
		pDataMatrix2(pParamFloat(p, 3, 2, a), d, a);
		fSet(d[0], v[0], v[1], v[2]);
		fSet(d[1], v[3], v[4], v[5]);
		
	}else{
		pDataMatrix3(pParamFloat(p, 3, 2, a), d, a);
		fSet(d[0], v[0], v[3]);
		fSet(d[1], v[1], v[4]);
		fSet(d[2], v[2], v[5]);
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2(p,a,m) \
	GLfloat *d[3]; \
	if(pParamBlock.GetRowMajor()){ \
		pDataMatrix2(pParamFloat(p, 3, 2, a), d, a); \
		fSet(d[0], m.a11, m.a12, m.a13); \
		fSet(d[1], m.a21, m.a22, m.a23); \
	}else{ \
		pDataMatrix3(pParamFloat(p, 3, 2, a), d, a); \
		fSet(d[0], m.a11, m.a21); \
		fSet(d[1], m.a12, m.a22); \
		fSet(d[2], m.a13, m.a23); \
	}

void deoglSPBAccessor::SetArrayMat3x2(int p, int a, const decMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2(p, a, m)
}
void deoglSPBAccessor::SetArrayMat3x2(int p, int a, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2(p, a, m)
}
void deoglSPBAccessor::SetArrayMat3x2(int p, int a, const decTexMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2(p, a, m)
}
void deoglSPBAccessor::SetArrayMat3x2(int p, int a, const decTexMatrix2 &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x2(p, a, m)
}

void deoglSPBAccessor::SetArrayMat3x3(int p, int a, const float *v){
	GLfloat *d[3];
	pDataMatrix3(pParamFloat(p, 3, 3, a), d, a);
	
	if(pParamBlock.GetRowMajor()){
		fSet(d[0], v[0], v[1], v[2]);
		fSet(d[1], v[3], v[4], v[5]);
		fSet(d[2], v[6], v[7], v[8]);
		
	}else{
		fSet(d[0], v[0], v[3], v[6]);
		fSet(d[1], v[1], v[4], v[7]);
		fSet(d[2], v[2], v[5], v[8]);
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3(p,a,m) \
	GLfloat *d[3]; \
	pDataMatrix3(pParamFloat(p, 3, 3, a), d, a); \
	if(pParamBlock.GetRowMajor()){ \
		fSet(d[0], m.a11, m.a12, m.a13); \
		fSet(d[1], m.a21, m.a22, m.a23); \
		fSet(d[2], m.a31, m.a32, m.a33); \
	}else{ \
		fSet(d[0], m.a11, m.a21, m.a31); \
		fSet(d[1], m.a12, m.a22, m.a32); \
		fSet(d[2], m.a13, m.a23, m.a33); \
	}

void deoglSPBAccessor::SetArrayMat3x3(int p, int a, const decMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3(p, a, m)
}
void deoglSPBAccessor::SetArrayMat3x3(int p, int a, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3(p, a, m)
}
void deoglSPBAccessor::SetArrayMat3x3(int p, int a, const decTexMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_3x3(p, a, m)
}

void deoglSPBAccessor::SetArrayMat4x3(int p, int a, const float *v){
	GLfloat *d[4];
	if(pParamBlock.GetRowMajor()){
		pDataMatrix3(pParamFloat(p, 4, 3, a), d, a);
		fSet(d[0], v[0], v[1], v[2], v[3]);
		fSet(d[1], v[4], v[5], v[6], v[7]);
		fSet(d[2], v[8], v[9], v[10], v[11]);
		
	}else{
		pDataMatrix4(pParamFloat(p, 4, 3, a), d, a);
		fSet(d[0], v[0], v[4], v[8]);
		fSet(d[1], v[1], v[5], v[9]);
		fSet(d[2], v[2], v[6], v[10]);
		fSet(d[3], v[3], v[7], v[11]);
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3(p,a,m) \
	GLfloat *d[4]; \
	if(pParamBlock.GetRowMajor()){ \
		pDataMatrix3(pParamFloat(p, 4, 3, a), d, a); \
		fSet(d[0], m.a11, m.a12, m.a13, m.a14); \
		fSet(d[1], m.a21, m.a22, m.a23, m.a24); \
		fSet(d[2], m.a31, m.a32, m.a33, m.a34); \
	}else{ \
		pDataMatrix4(pParamFloat(p, 4, 3, a), d, a); \
		fSet(d[0], m.a11, m.a21, m.a31); \
		fSet(d[1], m.a12, m.a22, m.a32); \
		fSet(d[2], m.a13, m.a23, m.a33); \
		fSet(d[3], m.a14, m.a24, m.a34); \
	}

void deoglSPBAccessor::SetArrayMat4x3(int p, int a, const decMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3(p, a, m)
}
void deoglSPBAccessor::SetArrayMat4x3(int p, int a, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3(p, a, m)
}
void deoglSPBAccessor::SetArrayMat4x3(int p, int a, const oglMatrix3x4 &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x3(p, a, m)
}

void deoglSPBAccessor::SetArrayMat4x4(int p, int a, const float *v){
	GLfloat *d[4];
	pDataMatrix4(pParamFloat(p, 4, 4, a), d, a);
	
	if(pParamBlock.GetRowMajor()){
		fSet(d[0], v[0], v[1], v[2], v[3]);
		fSet(d[1], v[4], v[5], v[6], v[7]);
		fSet(d[2], v[8], v[9], v[10], v[11]);
		fSet(d[3], v[12], v[13], v[14], v[15]);
		
	}else{
		fSet(d[0], v[0], v[4], v[8], v[12]);
		fSet(d[1], v[1], v[5], v[9], v[13]);
		fSet(d[2], v[2], v[6], v[10], v[14]);
		fSet(d[3], v[3], v[7], v[11], v[15]);
	}
}

#define IMPL_SET_PARAM_DATA_ARRAY_MAT_4x4(p,a,m) \
	GLfloat *d[4]; \
	pDataMatrix4(pParamFloat(p, 4, 4, a), d, a); \
	if(pParamBlock.GetRowMajor()){ \
		fSet(d[0], m.a11, m.a12, m.a13, m.a14); \
		fSet(d[1], m.a21, m.a22, m.a23, m.a24); \
		fSet(d[2], m.a31, m.a32, m.a33, m.a34); \
		fSet(d[3], m.a41, m.a42, m.a43, m.a44); \
	}else{ \
		fSet(d[0], m.a11, m.a21, m.a31, m.a41); \
		fSet(d[1], m.a12, m.a22, m.a32, m.a42); \
		fSet(d[2], m.a13, m.a23, m.a33, m.a43); \
		fSet(d[3], m.a14, m.a24, m.a34, m.a44); \
	}

void deoglSPBAccessor::SetArrayMat4x4(int p, int a, const decMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x4(p, a, m)
}
void deoglSPBAccessor::SetArrayMat4x4(int p, int a, const decDMatrix &m){
	IMPL_SET_PARAM_DATA_ARRAY_MAT_4x4(p, a, m)
}

void deoglSPBAccessor::SetArrayFloat(int p, const float *values, int count){
	if(count == 0){
		return;
	}
	if(!values || count < 0){
		DETHROW(deeInvalidParam);
	}
	const deoglSPBParameter &parameter = pParamFloat(p, 1, 1, count - 1);
	char * const mapped = pData(parameter, 0);
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for(i=0; i<count; i++){
		fSet(reinterpret_cast<GLfloat*>(mapped + stride * i), values[i]);
	}
}

void deoglSPBAccessor::SetArrayInt(int p, int a, int v){
	fSet(pDataInt(pParamInt(p, 1, 1, a), a), v);
}
void deoglSPBAccessor::SetArrayIVec2(int p, int a, int v1, int v2){
	fSet(pDataInt(pParamInt(p, 2, 1, a), a), v1, v2);
}
void deoglSPBAccessor::SetArrayIVec3(int p, int a, int v1, int v2, int v3){
	fSet(pDataInt(pParamInt(p, 3, 1, a), a), v1, v2, v3);
}
void deoglSPBAccessor::SetArrayIVec4(int p, int a, int v1, int v2, int v3, int v4){
	fSet(pDataInt(pParamInt(p, 4, 1, a), a), v1, v2, v3, v4);
}
	
void deoglSPBAccessor::SetArrayInt(int p, const int *values, int count){
	if(count == 0){
		return;
	}
	if(!values || count < 0){
		DETHROW(deeInvalidParam);
	}
	const deoglSPBParameter &parameter = pParamInt(p, 1, 1, count - 1);
	char * const mapped = pData(parameter, 0);
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for(i=0; i<count; i++){
		fSet(reinterpret_cast<GLint*>(mapped + stride * i), values[i]);
	}
}

void deoglSPBAccessor::SetArrayUInt(int p, int a, unsigned int v){
	fSet(pDataUInt(pParamInt(p, 1, 1, a), a), v);
}
void deoglSPBAccessor::SetArrayUVec2(int p, int a,
unsigned int v1, unsigned int v2){
	fSet(pDataUInt(pParamInt(p, 2, 1, a), a), v1, v2);
}
void deoglSPBAccessor::SetArrayUVec3(int p, int a,
unsigned int v1, unsigned int v2, unsigned int v3){
	fSet(pDataUInt(pParamInt(p, 3, 1, a), a), v1, v2, v3);
}
void deoglSPBAccessor::SetArrayUVec4(int p, int a,
unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4){
	fSet(pDataUInt(pParamInt(p, 4, 1, a), a), v1, v2, v3, v4);
}

void deoglSPBAccessor::SetArrayUInt(int p, const unsigned int *values, int count){
	if(count == 0){
		return;
	}
	if(!values || count < 0){
		DETHROW(deeInvalidParam);
	}
	const deoglSPBParameter &parameter = pParamInt(p, 1, 1, count - 1);
	char * const mapped = pData(parameter, 0);
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for(i=0; i<count; i++){
		fSet(reinterpret_cast<GLuint*>(mapped + stride * i), values[i]);
	}
}

void deoglSPBAccessor::SetArrayBool(int p, int a, bool v){
	fSet(pDataUInt(pParamBool(p, 1, 1, a), a), v);
}
void deoglSPBAccessor::SetArrayBVec2(int p, int a, bool v1, bool v2){
	fSet(pDataUInt(pParamBool(p, 2, 1, a), a), v1, v2);
}
void deoglSPBAccessor::SetArrayBVec3(int p, int a, bool v1, bool v2, bool v3){
	fSet(pDataUInt(pParamBool(p, 3, 1, a), a), v1, v2, v3);
}
void deoglSPBAccessor::SetArrayBVec4(int p, int a, bool v1, bool v2, bool v3, bool v4){
	fSet(pDataUInt(pParamBool(p, 4, 1, a), a), v1, v2, v3, v4);
}

void deoglSPBAccessor::SetArrayBool(int p, const bool *values, int count){
	if(count == 0){
		return;
	}
	if(!values || count < 0){
		DETHROW(deeInvalidParam);
	}
	const deoglSPBParameter &parameter = pParamBool(p, 1, 1, count - 1);
	char * const mapped = pData(parameter, 0);
	
	const int stride = parameter.GetArrayStride();
	int i;
	
	for(i=0; i<count; i++){
		fSet(reinterpret_cast<GLuint*>(mapped + stride * i), values[i]);
	}
}


deoglSPBAccessor deoglSPBAccessor::GetStructAt(int index) const{
	return deoglSPBAccessor(*this, pParam(index, deoglSPBParameter::evtStruct, 1, 1, 0), 0);
}

deoglSPBAccessor deoglSPBAccessor::GetStructAt(int index, int arrayIndex) const{
	return deoglSPBAccessor(*this, pParam(index, deoglSPBParameter::evtStruct, 1, 1, arrayIndex), arrayIndex);
}


// Private Functions
//////////////////////

const deoglSPBParameter &deoglSPBAccessor::pParamFloat(int index, int componentCount,
int vectorCount, int arrayIndex) const{
	return pParam(index, deoglSPBParameter::evtFloat, componentCount, vectorCount, arrayIndex);
}

const deoglSPBParameter &deoglSPBAccessor::pParamInt(int index, int componentCount,
int vectorCount, int arrayIndex) const{
	return pParam(index, deoglSPBParameter::evtInt, componentCount, vectorCount, arrayIndex);
}

const deoglSPBParameter &deoglSPBAccessor::pParamBool(int index, int componentCount,
int vectorCount, int arrayIndex) const{
	return pParam(index, deoglSPBParameter::evtBool, componentCount, vectorCount, arrayIndex);
}

const deoglSPBParameter &deoglSPBAccessor::pParam(int index, deoglSPBParameter::eValueTypes valueType,
int componentCount, int vectorCount, int arrayIndex) const{
	const deoglSPBParameter &p = pParameter ?
		pParameter->GetMembers()[index] : pParamBlock.GetParameterAt(index);
	
	DEASSERT_TRUE(p.GetValueType() == valueType)
	DEASSERT_TRUE(p.GetComponentCount() == componentCount)
	DEASSERT_TRUE(p.GetVectorCount() == vectorCount)
	DEASSERT_TRUE(arrayIndex >= 0)
	DEASSERT_TRUE(arrayIndex < p.GetArrayCount())
	
	return p;
}


GLfloat *deoglSPBAccessor::pDataFloat(const deoglSPBParameter &parameter, int arrayIndex){
	return reinterpret_cast<GLfloat*>(pData(parameter, arrayIndex));
}

GLint *deoglSPBAccessor::pDataInt(const deoglSPBParameter &parameter, int arrayIndex){
	return reinterpret_cast<GLint*>(pData(parameter, arrayIndex));
}

GLuint *deoglSPBAccessor::pDataUInt(const deoglSPBParameter &parameter, int arrayIndex){
	return reinterpret_cast<GLuint*>(pData(parameter, arrayIndex));
}

char *deoglSPBAccessor::pData(const deoglSPBParameter &parameter, int arrayIndex) const{
	auto buffer = pParamBlock.GetMappedBuffer();
	DEASSERT_NOTNULL(buffer)
	return buffer + pOffset + parameter.GetArrayStride() * arrayIndex + parameter.GetOffset();
}


void deoglSPBAccessor::pDataMatrix2(const deoglSPBParameter &parameter,
GLfloat **data, int arrayIndex) const{
	auto buffer = pData(parameter, arrayIndex);
	data[0] = reinterpret_cast<GLfloat*>(buffer);
	data[1] = reinterpret_cast<GLfloat*>(buffer + parameter.GetStride());
}

void deoglSPBAccessor::pDataMatrix3(const deoglSPBParameter &parameter,
GLfloat **data, int arrayIndex) const{
	auto buffer = pData(parameter, arrayIndex);
	data[0] = reinterpret_cast<GLfloat*>(buffer);
	data[1] = reinterpret_cast<GLfloat*>(buffer + parameter.GetStride());
	data[2] = reinterpret_cast<GLfloat*>(buffer + parameter.GetStride() * 2);
}

void deoglSPBAccessor::pDataMatrix4(const deoglSPBParameter &parameter,
GLfloat **data, int arrayIndex) const{
	auto buffer = pData(parameter, arrayIndex);
	data[0] = reinterpret_cast<GLfloat*>(buffer);
	data[1] = reinterpret_cast<GLfloat*>(buffer + parameter.GetStride());
	data[2] = reinterpret_cast<GLfloat*>(buffer + parameter.GetStride() * 2);
	data[3] = reinterpret_cast<GLfloat*>(buffer + parameter.GetStride() * 3);
}
