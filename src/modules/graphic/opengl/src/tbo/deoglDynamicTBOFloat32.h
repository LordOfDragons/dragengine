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

#ifndef _DEOGLDYNAMICTBOFLOAT32_H_
#define _DEOGLDYNAMICTBOFLOAT32_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglBasics.h"

class deoglRenderThread;


/**
 * \brief Dynamic TBO.
 * 
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOFloat32{
public:
	deoglRenderThread &pRenderThread;
	int pComponentCount;
	GLuint pVBO;
	GLuint pTBO;
	
	float *pDataFloat;
	int pDataSize;
	int pDataCount;
	
	int pMemoryGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dynamic tbo. */
	deoglDynamicTBOFloat32( deoglRenderThread &renderThread, int componentCount );
	
	/** \brief Clean up dynamic tbo. */
	~deoglDynamicTBOFloat32();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief TBO. */
	inline GLuint GetTBO() const{ return pTBO; }
	
	/** \brief Count of data entries. */
	inline int GetDataCount() const{ return pDataCount; }
	
	/** \brief Get pixel count. */
	int GetPixelCount() const;
	
	/** \brief Clear TBO. */
	void Clear();
	
	/**
	 * \brief Add boolean value.
	 * 
	 * True adds 1 and False 0.
	 */
	void AddBool( bool value );
	
	/** \brief Add float value. */
	void AddFloat( float value );
	
	/** \brief Add two float values. */
	void AddVec2( float value1, float value2 );
	void AddVec2( const decVector2 &value );
	
	/** \brief Add three float values. */
	void AddVec3( float value1, float value2, float value3 );
	void AddVec3( const decVector &value );
	
	/** \brief Add four float values. */
	void AddVec4( float value1, float value2, float value3, float value4 );
	void AddVec4( const decVector &value, float value4 );
	void AddVec4( const decVector4 &value );
	
	/** \brief Add 4x3 matrix in colon major order adding 12 float. */
	void AddMat4x3( const decMatrix &value );
	
	/** \brief Add 4x3 matrix in row major order adding 12 float. */
	void AddMat3x4( const decMatrix &value );
	
	/** \brief Add 3x3 matrix in colon major order adding 9 float. */
	void AddMat3x3( const decMatrix &value );
	
	/** \brief Add 3x2 matrix in colon major order adding 6 float. */
	void AddMat3x2( const decMatrix &value );
	
	/** \brief Update TBO with added data. */
	void Update();
	
	/** \brief GPU memory consumption. */
	inline int GetMemoryConsumptionGPU() const{ return pMemoryGPU; }
	
	/** \brief Debug print. */
	void DebugPrint();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnlarge( int count );
};

#endif
