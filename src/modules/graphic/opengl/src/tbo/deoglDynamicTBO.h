/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
 * @brief Dynamic TBO.
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBO{
public:
	deoglRenderThread &pRenderThread;
	GLuint pVBO;
	GLuint pTBO;
	
	float *pDataFloat;
	int pDataSize;
	int pDataCount;
	
	int pMemoryGPU;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dynamic tbo. */
	deoglDynamicTBO( deoglRenderThread &renderThread );
	/** Cleans up the dynamic tbo. */
	~deoglDynamicTBO();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the TBO. */
	inline GLuint GetTBO() const{ return pTBO; }
	
	/** Retrieves the number of data entries. */
	inline int GetDataCount() const{ return pDataCount; }
	/** Clears the TBO. */
	void Clear();
	/** Adds a boolean value. True adds 1 and False 0. */
	void AddBool( bool value );
	/** Adds a float value. */
	void AddFloat( float value );
	/** Adds two float values. */
	void AddVec2( float value1, float value2 );
	void AddVec2( const decVector2 &value );
	/** Adds three float values. */
	void AddVec3( float value1, float value2, float value3 );
	void AddVec3( const decVector &value );
	/** Adds four float values. */
	void AddVec4( float value1, float value2, float value3, float value4 );
	void AddVec4( const decVector4 &value );
	/** Adds a 4x3 matrix in colon major order adding 12 float. */
	void AddMat4x3( const decMatrix &value );
	/** Adds a 3x3 matrix in colon major order adding 9 float. */
	void AddMat3x3( const decMatrix &value );
	/** Adds a 3x2 matrix in colon major order adding 6 float. */
	void AddMat3x2( const decMatrix &value );
	/** Updates the TBO with the added data. */
	void Update();
	
	/** Retrieves the GPU memory consumption. */
	inline int GetMemoryConsumptionGPU() const{ return pMemoryGPU; }
	/*@}*/
	
private:
	void pCleanUp();
	void pEnlarge( int count );
};

#endif
