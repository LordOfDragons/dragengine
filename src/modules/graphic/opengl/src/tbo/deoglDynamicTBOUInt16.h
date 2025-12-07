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

#ifndef _DEOGLDYNAMICTBOUINT16_H_
#define _DEOGLDYNAMICTBOUINT16_H_

#include "deoglDynamicTBO.h"


/**
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOUInt16 : public deoglDynamicTBO{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBOUInt16(deoglRenderThread &renderThread, int componentCount);
	
protected:
	/** Clean up dynamic tbo. */
	virtual ~deoglDynamicTBOUInt16();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Data entries. */
	inline uint16_t *GetDataUInt(){return (uint16_t*)pData;}
	inline const uint16_t *GetDataUInt() const{return (uint16_t*)pData;}
	
	/**
	 * Add boolean value.
	 * 
	 * True adds 1 and False 0.
	 */
	void AddBool(bool value);
	
	/** Add integer value. */
	void AddInt(uint16_t value);
	
	/** Add two integer values. */
	void AddVec2(uint16_t value1, uint16_t value2);
	void AddVec2(const decPoint &value);
	
	/** Add three integer values. */
	void AddVec3(uint16_t value1, uint16_t value2, uint16_t value3);
	void AddVec3(const decPoint3 &value);
	
	/** Add four integer values. */
	void AddVec4(uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4);
	
	/** Set boolean value at offset in components. True adds 1 and False 0. */
	void SetBoolAt(int offset, bool value);
	
	/** Set integer value at offset in components. */
	void SetIntAt(int offset, uint16_t value);
	
	/** Set two integer values at offset in components. */
	void SetVec2At(int offset, uint16_t value1, uint16_t value2);
	void SetVec2At(int offset, const decPoint &value);
	
	/** Set three integer values at offset in components. */
	void SetVec3At(int offset, uint16_t value1, uint16_t value2, uint16_t value3);
	void SetVec3At(int offset, const decPoint3 &value);
	
	/** Set four integer values at offset in components. */
	void SetVec4At(int offset, uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4);
	
	/** Debug print. */
	virtual void DebugPrint();
	
	/** TBO format. */
	virtual GLenum GetTBOFormat();
	/*@}*/
};

#endif
