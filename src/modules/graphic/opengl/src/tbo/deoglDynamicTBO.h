/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLDYNAMICTBO_H_
#define _DEOGLDYNAMICTBO_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglBasics.h"

class deoglRenderThread;


/**
 * Store dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of subclass specific data type values.
 * Once done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBO{
protected:
	deoglRenderThread &pRenderThread;
	const int pComponentCount;
	const int pDataTypeSize;
	GLuint pVBO;
	GLuint pTBO;
	
	uint8_t *pData;
	int pDataSize;
	int pDataCount;
	
	int pMemoryGPU;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBO( deoglRenderThread &renderThread, int componentCount, int dataTypeSize );
	
public:
	/** Clean up dynamic tbo. */
	virtual ~deoglDynamicTBO();
	/*@}*/
	
	
	
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
	
	/** Get pixel count. */
	int GetPixelCount() const;
	
	/** Increase count of pixels. New entries have undefined content. */
	void IncreasePixelCount( int byAmount );
	
	/** Data point offset at start of pixel. */
	int GetPixelOffset( int pixel ) const;
	
	/** Clear TBO. */
	void Clear();
	
	/** Add content from another dynamic TBO. */
	void AddTBO( const deoglDynamicTBO &tbo );
	
	/** Update TBO with added data. */
	void Update();
	
	/** Update sub range of TBO with data measured in pixels. */
	void Update( int offset, int count );
	
	/** GPU memory consumption. */
	inline int GetMemoryConsumptionGPU() const{ return pMemoryGPU; }
	
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
