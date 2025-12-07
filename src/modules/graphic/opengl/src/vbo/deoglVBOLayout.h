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

#ifndef _DEOGLVBOLAYOUT_H_
#define _DEOGLVBOLAYOUT_H_

class deoglVBOAttribute;
class deoglRenderThread;



/**
 * OpenGL VBO Layout.
 */
class deoglVBOLayout{
public:
	/** Index types. */
	enum eIndexTypes{
		/** No indices. */
		eitNone,
		
		/** Unsigned int indices. */
		eitUnsignedInt,
		
		/** Unsigned short indices. */
		eitUnsignedShort,
		
		/** Unsigned byte indices. */
		eitUnsignedByte
	};
	
	
	
private:
	int pSize;
	int pStride;
	deoglVBOAttribute *pAttributes;
	int pAttributeCount;
	eIndexTypes pIndexType;
	int pIndexSize;
	int pIndexGLType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vbo layout. */
	deoglVBOLayout();
	
	/** Create vbo layout. */
	deoglVBOLayout(const deoglVBOLayout &layout);
	
	/** Clean up vbo layout. */
	~deoglVBOLayout();
	/*@}*/
	

	
	/** \name Management */
	/*@{*/
	/** VBO size. */
	inline int GetSize() const{ return pSize; }
	
	/** Set VBO size. */
	void SetSize(int size);
	
	/** Stride. */
	inline int GetStride() const{ return pStride; }
	
	/** Set stride. */
	void SetStride(int stride);
	
	/** Index type. */
	inline eIndexTypes GetIndexType() const{ return pIndexType; }
	
	/** Set index type. */
	void SetIndexType(eIndexTypes indexType);
	
	/** Size in bytes of indices. */
	inline int GetIndexSize() const{ return pIndexSize; }
	
	/** Opengl type of the indices. */
	inline int GetIndexGLType() const{ return pIndexGLType; }
	/*@}*/
	
	
	
	/** \name Attributes */
	/*@{*/
	/** Number of attributes. */
	inline int GetAttributeCount() const{ return pAttributeCount; }
	
	/** Set number of attributes. */
	void SetAttributeCount(int count);
	
	/** Attribute by index. */
	deoglVBOAttribute &GetAttributeAt(int index) const;
	
	/**
	 * Set VAO attribute.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int target) const;
	
	/**
	 * Set VAO attribute.
	 * 
	 * The VBO is supposed to be already selected. The attribute is enabled prior to be set.
	 */
	void SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int target, int offset) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set layout. */
	deoglVBOLayout &operator=(const deoglVBOLayout &layout);
	
	/** Layout matches another one. */
	bool operator==(const deoglVBOLayout &layout) const;
	
	/** Layout does not match another one. */
	bool operator!=(const deoglVBOLayout &layout) const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Print layout to the console. */
	void PrintToConsole(deoglRenderThread &renderThread, const char *name);
	/*@}*/
};

#endif
