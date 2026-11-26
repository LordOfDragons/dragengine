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

#ifndef _DEOGLSPBACCESSOR_H_
#define _DEOGLSPBACCESSOR_H_

#include "deoglSPBParameter.h"
#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglShaderParameterBlock;


/**
 * OpenGL shader parameter block data accessor.
 */
class deoglSPBAccessor{
private:
	const deoglShaderParameterBlock &pParamBlock;
	const deoglSPBParameter *pParameter;
	int pOffset;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader parameter block data accessor. */
	explicit deoglSPBAccessor(const deoglShaderParameterBlock &paramBlock);
	
	/** Create shader parameter block data accessor. */
	deoglSPBAccessor(const deoglShaderParameterBlock &paramBlock, int element);
	
	/** Create copy of shader parameter block data accessor. */
	deoglSPBAccessor(const deoglSPBAccessor &other);
	
	/** Clean up shader parameter block data accessor. */
	~deoglSPBAccessor();
	
	deoglSPBAccessor(const deoglSPBAccessor &&other) = delete;
	deoglSPBAccessor &operator=(const deoglSPBAccessor &other) = delete;
	deoglSPBAccessor &operator=(const deoglSPBAccessor &&other) = delete;
	
private:
	deoglSPBAccessor(const deoglSPBAccessor &accessor,
		const deoglSPBParameter &parameter, int arrayIndex);
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shader parameter block. */
	inline const deoglShaderParameterBlock &GetShaderParameterBlock() const{ return pParamBlock; }
	
	/** Parameter or nullptr. */
	inline const deoglSPBParameter *GetParameter() const{ return pParameter; }
	
	
	/** Set data for float type parameter. */
	void SetFloat(int parameter, float value);
	
	void SetVec2(int parameter, float value1, float value2);
	void SetVec2(int parameter, const decVector2 &vector);
	
	void SetVec3(int parameter, float value1, float value2, float value3);
	void SetVec3(int parameter, const decColor &color);
	void SetVec3(int parameter, const decVector &vector);
	void SetVec3(int parameter, const decDVector &vector);
	void SetVec3(int parameter, const oglVector3 &vector);
	
	void SetVec4(int parameter, float value1, float value2,
		float value3, float value4);
	void SetVec4(int parameter, const decColor &color);
	void SetVec4(int parameter, const decColor &color, float alpha);
	void SetVec4(int parameter, const decVector4 &vector);
	void SetVec4(int parameter, const decDVector4 &vector);
	void SetVec4(int parameter, const decVector &vector, float w);
	void SetVec4(int parameter, const decDVector &vector, double w);
	void SetVec4(int parameter, const decDVector4 &vector, double w);
	void SetVec4(int parameter, const oglVector3 &vector, float w);
	
	void SetMat3x2(int parameter, const float *values);
	void SetMat3x2(int parameter, const decMatrix &matrix);
	void SetMat3x2(int parameter, const decDMatrix &matrix);
	void SetMat3x2(int parameter, const decTexMatrix &matrix);
	void SetMat3x2(int parameter, const decTexMatrix2 &matrix);
	void SetMat3x3(int parameter, const float *values);
	void SetMat3x3(int parameter, const decMatrix &matrix);
	void SetMat3x3(int parameter, const decDMatrix &matrix);
	void SetMat3x3(int parameter, const decTexMatrix &matrix);
	void SetMat4x3(int parameter, const float *values);
	void SetMat4x3(int parameter, const decMatrix &matrix);
	void SetMat4x3(int parameter, const decDMatrix &matrix);
	void SetMat4x3(int parameter, const oglMatrix3x4 &matrix);
	void SetMat4x4(int parameter, const float *values);
	void SetMat4x4(int parameter, const decMatrix &matrix);
	void SetMat4x4(int parameter, const decDMatrix &matrix);
	
	/** Set data for int type parameter. */
	void SetInt(int parameter, int value);
	void SetIVec2(int parameter, int value1, int value2);
	void SetIVec2(int parameter, const decPoint &point);
	void SetIVec3(int parameter, int value1, int value2, int value3);
	void SetIVec3(int parameter, const decPoint3 &value);
	void SetIVec4(int parameter, int value1, int value2, int value3, int value4);
	
	/** Set data for int type parameter. */
	void SetUInt(int parameter, unsigned int value);
	void SetUVec2(int parameter, unsigned int value1, unsigned int value2);
	void SetUVec3(int parameter, unsigned int value1, unsigned int value2, unsigned int value3);
	void SetUVec4(int parameter, unsigned int value1, unsigned int value2,
		unsigned int value3, unsigned int value4);
	
	/** Set data for bool type parameter. */
	void SetBool(int parameter, bool value);
	void SetBVec2(int parameter, bool value1, bool value2);
	void SetBVec3(int parameter, bool value1, bool value2, bool value3);
	void SetBVec4(int parameter, bool value1, bool value2, bool value3, bool value4);
	
	/** Set data for array float type parameter. */
	void SetArrayFloat(int parameter, int index, float value);
	
	void SetArrayVec2(int parameter, int index, float value1, float value2);
	void SetArrayVec2(int parameter, int index, const decVector2 &vector);
	void SetArrayVec3(int parameter, int index,
		float value1, float value2, float value3);
	
	void SetArrayVec3(int parameter, int index, const decColor &color);
	void SetArrayVec3(int parameter, int index, const decVector &vector);
	void SetArrayVec3(int parameter, int index, const decDVector &vector);
	void SetArrayVec3(int parameter, int index, const oglVector3 &vector);
	
	void SetArrayVec4(int parameter, int index,
		float value1, float value2, float value3, float value4);
	void SetArrayVec4(int parameter, int index, const decColor &color);
	void SetArrayVec4(int parameter, int index, const decColor &color, float alpha);
	void SetArrayVec4(int parameter, int index, const decVector4 &vector);
	void SetArrayVec4(int parameter, int index, const decDVector4 &vector);
	void SetArrayVec4(int parameter, int index, const decVector &vector, float w);
	void SetArrayVec4(int parameter, int index, const decDVector &vector, double w);
	void SetArrayVec4(int parameter, int index, const oglVector3 &vector, float w);
	
	void SetArrayMat3x2(int parameter, int index, const float *values);
	void SetArrayMat3x2(int parameter, int index, const decMatrix &matrix);
	void SetArrayMat3x2(int parameter, int index, const decDMatrix &matrix);
	void SetArrayMat3x2(int parameter, int index, const decTexMatrix &matrix);
	void SetArrayMat3x2(int parameter, int index, const decTexMatrix2 &matrix);
	
	void SetArrayMat3x3(int parameter, int index, const float *values);
	void SetArrayMat3x3(int parameter, int index, const decMatrix &matrix);
	void SetArrayMat3x3(int parameter, int index, const decDMatrix &matrix);
	void SetArrayMat3x3(int parameter, int index, const decTexMatrix &matrix);
	
	void SetArrayMat4x3(int parameter, int index, const float *values);
	void SetArrayMat4x3(int parameter, int index, const decMatrix &matrix);
	void SetArrayMat4x3(int parameter, int index, const decDMatrix &matrix);
	void SetArrayMat4x3(int parameter, int index, const oglMatrix3x4 &matrix);
	
	void SetArrayMat4x4(int parameter, int index, const float *values);
	void SetArrayMat4x4(int parameter, int index, const decMatrix &matrix);
	void SetArrayMat4x4(int parameter, int index, const decDMatrix &matrix);
	
	void SetArrayFloat(int parameter, const float *values, int count);
	
	/** Set data for an array int type parameter. */
	void SetArrayInt(int parameter, int index, int value);
	void SetArrayIVec2(int parameter, int index, int value1, int value2);
	void SetArrayIVec3(int parameter, int index, int value1, int value2, int value3);
	void SetArrayIVec4(int parameter, int index, int value1, int value2, int value3, int value4);
	void SetArrayInt(int parameter, const int *values, int count);
	
	/** Set data for an array int type parameter. */
	void SetArrayUInt(int parameter, int index, unsigned int value);
	void SetArrayUVec2(int parameter, int index, unsigned int value1, unsigned int value2);
	void SetArrayUVec3(int parameter, int index, unsigned int value1,
		unsigned int value2, unsigned int value3);
	void SetArrayUVec4(int parameter, int index, unsigned int value1, unsigned int value2,
		unsigned int value3, unsigned int value4);
	void SetArrayUInt(int parameter, const unsigned int *values, int count);
	
	/** Set data for a array bool type parameter. */
	void SetArrayBool(int parameter, int index, bool value);
	void SetArrayBVec2(int parameter, int index, bool value1, bool value2);
	void SetArrayBVec3(int parameter, int index, bool value1, bool value2, bool value3);
	void SetArrayBVec4(int parameter, int index, bool value1, bool value2, bool value3, bool value4);
	void SetArrayBool(int parameter, const bool *values, int count);
	
	/** Struct at index. */
	deoglSPBAccessor GetStructAt(int parameter) const;
	deoglSPBAccessor GetStructAt(int parameter, int index) const;
	/*@}*/
	
	
private:
	const deoglSPBParameter &pParamFloat(int index, int componentCount,
		int vectorCount = 1, int arrayIndex = 0) const;
	const deoglSPBParameter &pParamInt(int index, int componentCount,
		int vectorCount = 1, int arrayIndex = 0) const;
	const deoglSPBParameter &pParamBool(int index, int componentCount,
		int vectorCount = 1, int arrayIndex = 0) const;
	const deoglSPBParameter &pParam(int index, deoglSPBParameter::eValueTypes valueType,
		int componentCount, int vectorCount, int arrayIndex) const;
	
	GLfloat *pDataFloat(const deoglSPBParameter &parameter, int arrayIndex = 0);
	GLint *pDataInt(const deoglSPBParameter &parameter, int arrayIndex = 0);
	GLuint *pDataUInt(const deoglSPBParameter &parameter, int arrayIndex = 0);
	char *pData(const deoglSPBParameter &parameter, int arrayIndex) const;
	
	void pDataMatrix2(const deoglSPBParameter &parameter, GLfloat **data, int arrayIndex = 0) const;
	void pDataMatrix3(const deoglSPBParameter &parameter, GLfloat **data, int arrayIndex = 0) const;
	void pDataMatrix4(const deoglSPBParameter &parameter, GLfloat **data, int arrayIndex = 0) const;
};

#endif
