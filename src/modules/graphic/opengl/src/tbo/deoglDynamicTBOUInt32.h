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

#ifndef _DEOGLDYNAMICTBOUINT32_H_
#define _DEOGLDYNAMICTBOUINT32_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglBasics.h"

class deoglRenderThread;


/**
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOUInt32{
public:
	deoglRenderThread &pRenderThread;
	int pComponentCount;
	GLuint pVBO;
	GLuint pTBO;
	
	uint32_t *pDataUInt;
	int pDataSize;
	int pDataCount;
	
	int pMemoryGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBOUInt32( deoglRenderThread &renderThread, int componentCount );
	
	/** Clean up dynamic tbo. */
	~deoglDynamicTBOUInt32();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** TBO. */
	inline GLuint GetTBO() const{ return pTBO; }
	
	/** Data entries. */
	inline uint32_t *GetData(){ return pDataUInt; }
	inline const uint32_t *GetData() const{ return pDataUInt; }
	
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
	
	/** Add boolean value. True adds 1 and False 0. */
	void AddBool( bool value );
	
	/** Add integer value. */
	void AddInt( uint32_t value );
	
	/** Add two integer values. */
	void AddVec2( uint32_t value1, uint32_t value2 );
	void AddVec2( const decPoint &value );
	
	/** Add three integer values. */
	void AddVec3( uint32_t value1, uint32_t value2, uint32_t value3 );
	void AddVec3( const decPoint3 &value );
	
	/** Add four integer values. */
	void AddVec4( uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4 );
	
	/** Add content from another dynamic TBO. */
	void AddTBO( const deoglDynamicTBOUInt32 &tbo );
	
	/** Set boolean value at offset in components. True adds 1 and False 0. */
	void SetBoolAt( int offset, bool value );
	
	/** Set integer value at offset in components. */
	void SetIntAt( int offset, uint32_t value );
	
	/** Set two integer values at offset in components. */
	void SetVec2At( int offset, uint32_t value1, uint32_t value2 );
	void SetVec2At( int offset, const decPoint &value );
	
	/** Set three integer values at offset in components. */
	void SetVec3At( int offset, uint32_t value1, uint32_t value2, uint32_t value3 );
	void SetVec3At( int offset, const decPoint3 &value );
	
	/** Set four integer values at offset in components. */
	void SetVec4At( int offset, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4 );
	
	/** Update TBO with added data. */
	void Update();
	
	/** GPU memory consumption. */
	inline int GetMemoryConsumptionGPU() const{ return pMemoryGPU; }
	
	/** Debug print. */
	void DebugPrint();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnlarge( int count );
};

#endif
