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

#ifndef _DEOGLSPBLOCKVBO_H_
#define _DEOGLSPBLOCKVBO_H_

#include "deoglShaderParameterBlock.h"



/**
 * Shader parameter block based on VBO. Stores the data for a shader parameter block.
 * This is used to set the value of a GLSL uniform block in shader programs. Each block
 * is defined as a byte array with members in the uniform block mapped areas inde the
 * data block.
 */
class deoglSPBlockUBO : public deoglShaderParameterBlock{
public:
	typedef deTObjectReference<deoglSPBlockUBO> Ref;
	
	
private:
	GLuint pUBO;
	int pBindingPoint;
	bool pCompact;
	bool pAllocateBuffer;
	
	char *pWriteBuffer;
	int pWriteBufferCapacity;
	bool pWriteBufferUsed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader parameter block. */
	deoglSPBlockUBO(deoglRenderThread &renderThread);
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockUBO(const deoglSPBlockUBO &paramBlock);
	
protected:
	/** Clean up shader parameter block. */
	~deoglSPBlockUBO() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Uniform buffer object or 0 if not created yet. */
	inline GLuint GetUBO() const{ return pUBO; }
	
	/** Binding point. */
	inline int GetBindingPoint() const{ return pBindingPoint; }
	
	/** Set binding point. */
	void SetBindingPoint(int bindingPoint);
	
	/** Compact elements. If true mapping individual elements is prohibited. */
	inline bool GetCompact() const{ return pCompact; }
	
	/** Set if elements are compact. If true mapping individual elements is prohibited. */
	void SetCompact(bool compact);
	
	/** Activate buffer. */
	void Activate() const override;
	
	/** Activate buffer overriding binding point. */
	void Activate(int bindingPoint) const override;
	
	/** Deactivate buffer. */
	void Deactivate() const override;
	
	/** Deactivate buffer overriding binding point. */
	void Deactivate(int bindingPoint) const override;
	
	/** Map buffer discarding content. */
	void MapBuffer() override;
	
	/**
	 * Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	void MapBuffer(int element) override;
	
	/**
	 * Map buffer for specific elements discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	void MapBuffer(int element, int count) override;
	
	/** Unmap buffer uploading data to GPU. */
	void UnmapBuffer() override;
	
	/** Direct access to write buffer. Use with care. Can be nullptr. */
	inline char *GetWriteBuffer() const{ return pWriteBuffer; }
	
	/** Get platform alignment requirements. */
	int GetAlignmentRequirements() const override;
	
	/** Create copy of shader parameter block. */
	deoglShaderParameterBlock *Copy() const override;
	
	/** Debug print configuration. */
	void DebugPrintConfig(const char *name);
	/*@}*/
	
	
	
protected:
	void pUpdateBufferSize() override;
	
	
	
private:
	void pGrowWriteBuffer(int size);
};

#endif
