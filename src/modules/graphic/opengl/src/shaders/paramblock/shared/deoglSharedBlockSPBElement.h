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

#ifndef _DEOGLSHAREDBLOCKSPBELEMENT_H_
#define _DEOGLSHAREDBLOCKSPBELEMENT_H_

#include <dragengine/deObject.h>

class deoglSharedBlockSPB;
class deoglShaderParameterBlock;



/**
 * OpenGL shared shader parameter block element.
 */
class deoglSharedBlockSPBElement : public deObject{
public:
	/**
	 * Stores deoglSharedBlockSPBElement returning it to deoglSharedBlockSPB if out of scope.
	 * This is a convenience class to avoid having to remember to call Return().
	 */
	class Lease{
	private:
		deoglSharedBlockSPBElement *pElement;
		
		Lease(const Lease &ref); // disallow compiler auto-use
		
	public:
		Lease();
		~Lease();
		
		operator deoglSharedBlockSPBElement*() const;
		operator deoglSharedBlockSPBElement&() const;
		deoglSharedBlockSPBElement* operator->() const;
		
		Lease &operator=(deoglSharedBlockSPBElement *element);
		bool operator!() const;
		operator bool() const;
	};
	
	/** For use by deoglSharedBlockSPB only! */
	typedef deTObjectReference<deoglSharedBlockSPBElement> Ref;
	
	
private:
	deoglSharedBlockSPB &pSPB;
	int pIndex;
	int pCount;
	bool pEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared shader parameter block element. */
	deoglSharedBlockSPBElement(deoglSharedBlockSPB &spb, int index, int count);
	
protected:
	/** Clean up shared shader parameter block element. */
	virtual ~deoglSharedBlockSPBElement();
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shader parameter block this element belongs to. */
	inline deoglSharedBlockSPB &GetSPB() const{ return pSPB; }
	
	/** Element index in the shader parameter block. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Count of elements. */
	inline int GetCount() const{ return pCount; }
	
	/** Set count of elements. */
	void SetCount(int count);
	
	/** Block is empty. */
	inline bool GetEmpty() const{ return pEmpty; }
	
	/** Set if block is empty. */
	void SetEmpty(bool empty);
	
	/**
	 * Map buffer for element.
	 * 
	 * Call UnmapBuffer() on the shader parameter block once finished. You can call
	 * all methods without explicit element index to write into the mapped element
	 * while the buffer is mapped.
	 */
	deoglShaderParameterBlock &MapBuffer() const;
	
	/** Return to shared SPB. */
	void Return();
	/*@}*/
};

#endif
