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

#ifndef _DEOGLDYNAMICTBOFLOAT16_H_
#define _DEOGLDYNAMICTBOFLOAT16_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglBasics.h"
#include "../utils/deoglConvertFloatHalf.h"

class deoglRenderThread;


/**
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOFloat16{
private:
	deoglRenderThread &pRenderThread;
	int pComponentCount;
	GLuint pVBO;
	GLuint pTBO;
	
	HALF_FLOAT *pDataFloat;
	int pDataSize;
	int pDataCount;
	
	int pMemoryGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBOFloat16( deoglRenderThread &renderThread, int componentCount );
	
	/** Clean up dynamic tbo. */
	~deoglDynamicTBOFloat16();
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
	inline HALF_FLOAT *GetData(){ return pDataFloat; }
	inline const HALF_FLOAT *GetData() const{ return pDataFloat; }
	
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
	
	/** Add float value. */
	void AddFloat( float value );
	
	/** Add two float values. */
	void AddVec2( float value1, float value2 );
	void AddVec2( const decVector2 &value );
	
	/** Add three float values. */
	void AddVec3( float value1, float value2, float value3 );
	void AddVec3( const decVector &value );
	
	/** Add four float values. */
	void AddVec4( float value1, float value2, float value3, float value4 );
	void AddVec4( const decVector &value, float value4 );
	void AddVec4( const decVector4 &value );
	
	/** Add 4x3 matrix in colon major order adding 12 float. */
	void AddMat4x3( const decMatrix &value );
	
	/** Add 4x3 matrix in row major order adding 12 float. */
	void AddMat3x4( const decMatrix &value );
	
	/** Add 3x3 matrix in colon major order adding 9 float. */
	void AddMat3x3( const decMatrix &value );
	
	/** Add 3x2 matrix in colon major order adding 6 float. */
	void AddMat3x2( const decMatrix &value );
	
	/** Add content from another dynamic TBO. */
	void AddTBO( const deoglDynamicTBOFloat16 &tbo );
	
	/** Set boolean value at offset in components. True adds 1 and False 0. */
	void SetBoolAt( int offset, bool value );
	
	/** Set float value at offset in components. */
	void SetFloatAt( int offset, float value );
	
	/** Set two float values at offset in components. */
	void SetVec2At( int offset, float value1, float value2 );
	void SetVec2At( int offset, const decVector2 &value );
	
	/** Set three float values at offset in components. */
	void SetVec3At( int offset, float value1, float value2, float value3 );
	void SetVec3At( int offset, const decVector &value );
	
	/** Set four float values at offset in components. */
	void SetVec4At( int offset, float value1, float value2, float value3, float value4 );
	void SetVec4At( int offset, const decVector &value, float value4 );
	void SetVec4At( int offset, const decVector4 &value );
	
	/** Set 4x3 matrix in colon major order adding 12 float at offset in components. */
	void SetMat4x3At( int offset, const decMatrix &value );
	
	/** Set 4x3 matrix in row major order adding 12 float at offset in components. */
	void SetMat3x4At( int offset, const decMatrix &value );
	
	/** Set 3x3 matrix in colon major order adding 9 float at offset in components. */
	void SetMat3x3At( int offset, const decMatrix &value );
	
	/** Set 3x2 matrix in colon major order adding 6 float at offset in components. */
	void SetMat3x2At( int offset, const decMatrix &value );
	
	/** Update TBO with added data. */
	void Update();
	
	/** Update sub range of TBO with data measured in pixels. */
	void Update( int offset, int count );
	
	/** GPU memory consumption. */
	inline int GetMemoryConsumptionGPU() const{ return pMemoryGPU; }
	
	/** Debug print. */
	void DebugPrint();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnlarge( int count );
	void pEnsureVBO();
	void pEnsureTBO();
	void pEnsurePadding();
};

#endif
