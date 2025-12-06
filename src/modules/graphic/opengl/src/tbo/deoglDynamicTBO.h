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

#ifndef _DEOGLDYNAMICTBO_H_
#define _DEOGLDYNAMICTBO_H_

#include <stdint.h>

#include "../deoglBasics.h"
#include "../memory/consumption/deoglMemoryConsumptionGPUUse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRenderThread;


/**
 * Store dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of subclass specific data type values.
 * Once done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBO : public deObject{
protected:
	deoglRenderThread &pRenderThread;
	const int pComponentCount;
	const int pDataTypeSize;
	GLuint pVBO;
	GLuint pTBO;
	
	uint8_t *pData;
	int pDataSize;
	int pDataCount;
	
	deoglMemoryConsumptionGPUUse pMemUse;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBO( deoglRenderThread &renderThread, int componentCount, int dataTypeSize );
	
	/** Clean up dynamic tbo. */
	virtual ~deoglDynamicTBO();
	/*@}*/
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglDynamicTBO> Ref;


	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Data type size. */
	inline int GetDataTypeSize() const{ return pDataTypeSize; }
	
	/** TBO. */
	inline GLuint GetTBO() const{ return pTBO; }
	
	/** Data. */
	inline uint8_t *GetData(){ return pData; }
	inline const uint8_t *GetData() const{ return pData; }
	
	/** Count of data entries. */
	inline int GetDataCount() const{ return pDataCount; }
	
	/** Increase count of entries. New entries have undefined content. */
	void IncreaseDataCount( int byAmount );
	
	/** Set count of entries. New entries have undefined content. */
	void SetDataCount( int count );
	
	/** Get pixel count. */
	int GetPixelCount() const;
	
	/** Increase count of pixels. New entries have undefined content. */
	void IncreasePixelCount( int byAmount );
	
	/** Set count of pixels. New entries have undefined content. */
	void SetPixelCount( int count );
	
	/** Data point offset at start of pixel. */
	int GetPixelOffset( int pixel ) const;
	
	/** Clear TBO. */
	void Clear();
	
	/** Add content from another dynamic TBO. */
	void AddTBO( const deoglDynamicTBO &tbo );
	
	/** Set content from another dynamic TBO at offset measured in entries. */
	void SetTBO( int offset, const deoglDynamicTBO &tbo );
	
	/** Update TBO with added data. */
	void Update();
	
	/** Update sub range of TBO with data measured in pixels. */
	void Update( int offset, int count );
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionGPUUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** Debug print. */
	virtual void DebugPrint() = 0;
	
	/** TBO format. */
	virtual GLenum GetTBOFormat() = 0;
	/*@}*/
	
	
	
protected:
	void pEnlarge( int count );
	void pEnsureVBO();
	void pEnsureTBO();
	void pEnsurePadding();
	
	
	
private:
	void pCleanUp();
};

#endif
