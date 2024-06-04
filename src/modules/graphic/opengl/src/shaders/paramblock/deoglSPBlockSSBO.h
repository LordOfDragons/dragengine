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

#ifndef _DEOGLSPBLOCKSSBO_H_
#define _DEOGLSPBLOCKSSBO_H_

#include <stdint.h>

#include "deoglShaderParameterBlock.h"
#include "../../utils/deoglFence.h"


/**
 * OpenGL shader parameter block based on Shader Storage Buffer Object.
 */
class deoglSPBlockSSBO : public deoglShaderParameterBlock{
public:
	typedef deTObjectReference<deoglSPBlockSSBO> Ref;
	
	/** Type. */
	enum eType{
		/**
		 * CPU writes content once then uses it for GPU reading only.
		 */
		etStatic,
		
		/**
		 * CPU writes content often (typically every frame update) while using
		 * it for the GPU reading the content.
		 */
		etStream,
		
		/**
		 * CPU reads content written by GPU. CPU does not read content.
		 * GPU potentially reads content in other render steps.
		 */
		etRead,
		
		/**
		 * GPU writes content and reads it in later render steps. CPU never writes
		 * nor reads the content. Hence this is a pure GPU only buffer.
		 */
		etGpu
	};
	
	
	
private:
	const eType pType;
	const bool pUseDSA;
	GLuint pSSBO;
	GLuint pSSBOLocal;
	int pBindingPoint;
	int pBindingPointUBO;
	int pBindingPointAtomic;
	bool pCompact;
	bool pAllocateBuffer;
	
	char *pWriteBuffer;
	int pWriteBufferCapacity;
	
	char *pPersistentMapped;
	deoglFence::Ref pFenceTransfer;
	
	int pMemoryGPUSSBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader storage buffer object. */
	deoglSPBlockSSBO( deoglRenderThread &renderThread, eType type );
	
	/** Create copy of shader storage buffer object. */
	deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock );
	deoglSPBlockSSBO( const deoglSPBlockSSBO &paramBlock, eType eype );
	
protected:
	/** Clean up shader storage buffer object. */
	virtual ~deoglSPBlockSSBO();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Shader storage buffer object or 0 if not created yet. */
	inline GLuint GetSSBO() const{ return pSSBO; }
	
	/** Binding point. */
	inline int GetBindingPoint() const{ return pBindingPoint; }
	
	/** Set binding point. */
	void SetBindingPoint( int bindingPoint );
	
	/** UBO Binding point. */
	inline int GetBindingPointUBO() const{ return pBindingPointUBO; }
	
	/** Set UBO binding point. */
	void SetBindingPointUBO( int bindingPoint );
	
	/** Atmomic counter binding point. */
	inline int GetBindingPointAtomic() const{ return pBindingPointAtomic; }
	
	/** Set atomic counter binding point. */
	void SetBindingPointAtomic( int bindingPoint );
	
	/** Compact elements. If true mapping individual elements is prohibited. */
	inline bool GetCompact() const{ return pCompact; }
	
	/** Set if elements are compact. If true mapping individual elements is prohibited. */
	void SetCompact( bool compact );
	
	/** Activate buffer. */
	virtual void Activate() const;
	
	/** Activate buffer overriding binding point. */
	virtual void Activate( int bindingPoint ) const;
	
	/** Deactivate buffer. */
	virtual void Deactivate() const;
	
	/** Deactivate buffer overriding binding point. */
	virtual void Deactivate( int bindingPoint ) const;
	
	/** Activate buffer as UBO. */
	void ActivateUBO() const;
	
	/** Deactivate buffer as UBO. */
	void DeactivateUBO() const;
	
	/** Activate buffer as atomic buffer. */
	void ActivateAtomic() const;
	
	/** Activate buffer as atomic buffer overriding binding point. */
	void ActivateAtomic( int bindingPoint ) const;
	
	/** Deactivate buffer as atomic buffer. */
	void DeactivateAtomic() const;
	
	/** Deactivate buffer as atomic buffer overriding binding point. */
	void DeactivateAtomic( int bindingPoint ) const;
	
	/** Activate buffer as dispatch indirect. */
	void ActivateDispatchIndirect() const;
	
	/** Deactivate buffer as dispatch indirect. */
	void DeactivateDispatchIndirect() const;
	
	/** Map buffer discarding content. */
	virtual void MapBuffer();
	
	/**
	 * Map buffer for specific element discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element );
	
	/**
	 * Map buffer for specific elements discarding content.
	 * 
	 * Data outside the element range is retained. Any attempt to call SetParameter* with
	 * an element index other than the one used for mapping throws an exception.
	 */
	virtual void MapBuffer( int element, int count );
	
	/** Unmap buffer uploading data to GPU. */
	virtual void UnmapBuffer();
	
	/** Ensure buffer exists. */
	virtual void EnsureBuffer();
	
	/** Clear buffer. */
	void ClearDataUInt( uint32_t value );
	void ClearDataUInt( int offset, int count, uint32_t value );
	void ClearDataUInt( uint32_t r, uint32_t g, uint32_t b, uint32_t a );
	void ClearDataUInt( int offset, int count, uint32_t r, uint32_t g, uint32_t b, uint32_t a );
	
	void ClearDataFloat( float value );
	void ClearDataFloat( int offset, int count, float value );
	void ClearDataFloat( float r, float g, float b, float a );
	void ClearDataFloat( int offset, int count, float r, float g, float b, float a );
	
	/** Copy buffer data from SSBO into this SSBO. */
	void CopyData( const deoglSPBlockSSBO &ssbo, int offset, int count, int ssboOffset );
	
	/** GPU finished writing data to SSBO. */
	void GPUFinishedWriting();
	
	/** Asynchronously read data from GPU to CPU. */
	void GPUReadToCPU();
	void GPUReadToCPU( int elementCount );
	
	/** Map buffer for reading. */
	void MapBufferRead();
	void MapBufferRead( int element );
	void MapBufferRead( int element, int count );
	
	/** Unmap buffer after reading. */
	void UnmapBufferRead();
	
	/** Get platform alignment requirements. */
	virtual int GetAlignmentRequirements() const;
	
	/** Create copy of shader parameter block. */
	virtual deoglShaderParameterBlock *Copy() const;
	
	/** Map the parameter block definition to a shader uniform block using std430 layout. */
	void MapToStd430();
	
	
	
	/** GPU memory consumption for the VBO. */
	inline int GetMemoryConsumptionGPUVBO() const{ return pMemoryGPUSSBO; }
	
	/** Debug print configuration. */
	void DebugPrintConfig( const char *name );
	/*@}*/
	
	
	
protected:
	virtual void pUpdateBufferSize();
	
	
	
private:
	void pGrowWriteBuffer( int size );
	void pEnsureSSBO();
};

#endif
