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

#ifndef _DEOGLVBOATTRIBUTE_H_
#define _DEOGLVBOATTRIBUTE_H_

class deoglRenderThread;



/**
 * OpenGL VBO Attribute.
 */
class deoglVBOAttribute{
public:
	/** Data types. */
	enum eDataTypes{
		/** Float input floating point. */
		edtFloat,
		
		/** Float input normalized signed char. */
		edtByte,
		
		/** Float input normalized unsigned char. */
		edtUByte,
		
		/** Float input normalized signed short. */
		edtShort,
		
		/** Float input normalized unsigned short. */
		edtUShort,
		
		/** Float input normalized signed int. */
		edtInt,
		
		/** Float input normalized unsigned int. */
		edtUInt,
		
		/** Integer input signed char without normalization. */
		edtIByte,
		
		/** Integer input unsigned char without normalization. */
		edtIUByte,
		
		/** Integer input signed short without normalization. */
		edtIShort,
		
		/** Integer input unsigned short without normalization. */
		edtIUShort,
		
		/** Integer input signed int without normalization. */
		edtIInt,
		
		/** Integer input unsigned int without normalization. */
		edtIUInt
	};
	
	
	
private:
	int pComponentCount;
	eDataTypes pDataType;
	int pOffset;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vbo attribute. */
	deoglVBOAttribute();
	
	/** Clean up vbo attribute. */
	~deoglVBOAttribute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Set count of components. */
	void SetComponentCount(int componentCount);
	
	/** Data type. */
	inline eDataTypes GetDataType() const{ return pDataType; }
	
	/** Set data type. */
	void SetDataType(eDataTypes dataType);
	
	/** Offset. */
	inline int GetOffset() const{ return pOffset; }
	
	/** Set offset. */
	void SetOffset(int offset);
	
	/**
	 * Set VAO attribute from the stored properties.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int stride) const;
	
	/**
	 * Set VAO attribute from the stored properties.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int stride, int offset) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set attribute. */
	deoglVBOAttribute &operator=(const deoglVBOAttribute &attribute);
	
	/** Attribute matches another one. */
	bool operator==(const deoglVBOAttribute &attribute) const;
	
	/** Attribute does not match another one. */
	bool operator!=(const deoglVBOAttribute &attribute) const;
	/*@}*/
};

#endif
