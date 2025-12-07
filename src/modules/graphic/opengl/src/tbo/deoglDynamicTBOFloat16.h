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

#ifndef _DEOGLDYNAMICTBOFLOAT16_H_
#define _DEOGLDYNAMICTBOFLOAT16_H_

#include "deoglDynamicTBO.h"
#include "../utils/deoglConvertFloatHalf.h"


/**
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOFloat16 : public deoglDynamicTBO{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBOFloat16(deoglRenderThread &renderThread, int componentCount);
	
protected:
	/** Clean up dynamic tbo. */
	virtual ~deoglDynamicTBOFloat16();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Data entries. */
	inline HALF_FLOAT *GetDataFloat(){ return (HALF_FLOAT*)pData; }
	inline const HALF_FLOAT *GetDataFloat() const{ return (const HALF_FLOAT*)pData; }
	
	/** Add boolean value. True adds 1 and False 0. */
	void AddBool(bool value);
	
	/** Add float value. */
	void AddFloat(float value);
	
	/** Add two float values. */
	void AddVec2(float value1, float value2);
	void AddVec2(const decVector2 &value);
	
	/** Add three float values. */
	void AddVec3(float value1, float value2, float value3);
	void AddVec3(const decVector &value);
	
	/** Add four float values. */
	void AddVec4(float value1, float value2, float value3, float value4);
	void AddVec4(const decVector &value, float value4);
	void AddVec4(const decVector4 &value);
	
	/** Add 4x3 matrix in colon major order adding 12 float. */
	void AddMat4x3(const decMatrix &value);
	
	/** Add 4x3 matrix in row major order adding 12 float. */
	void AddMat3x4(const decMatrix &value);
	
	/** Add 3x3 matrix in colon major order adding 9 float. */
	void AddMat3x3(const decMatrix &value);
	
	/** Add 3x2 matrix in colon major order adding 6 float. */
	void AddMat3x2(const decMatrix &value);
	
	/** Set boolean value at offset in components. True adds 1 and False 0. */
	void SetBoolAt(int offset, bool value);
	
	/** Set float value at offset in components. */
	void SetFloatAt(int offset, float value);
	
	/** Set two float values at offset in components. */
	void SetVec2At(int offset, float value1, float value2);
	void SetVec2At(int offset, const decVector2 &value);
	
	/** Set three float values at offset in components. */
	void SetVec3At(int offset, float value1, float value2, float value3);
	void SetVec3At(int offset, const decVector &value);
	
	/** Set four float values at offset in components. */
	void SetVec4At(int offset, float value1, float value2, float value3, float value4);
	void SetVec4At(int offset, const decVector &value, float value4);
	void SetVec4At(int offset, const decVector4 &value);
	
	/** Set 4x3 matrix in colon major order adding 12 float at offset in components. */
	void SetMat4x3At(int offset, const decMatrix &value);
	
	/** Set 4x3 matrix in row major order adding 12 float at offset in components. */
	void SetMat3x4At(int offset, const decMatrix &value);
	
	/** Set 3x3 matrix in colon major order adding 9 float at offset in components. */
	void SetMat3x3At(int offset, const decMatrix &value);
	
	/** Set 3x2 matrix in colon major order adding 6 float at offset in components. */
	void SetMat3x2At(int offset, const decMatrix &value);
	
	/** Debug print. */
	virtual void DebugPrint();
	
	/** TBO format. */
	virtual GLenum GetTBOFormat();
	/*@}*/
};

#endif
