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

#ifndef _DEOGLDYNAMICTBOUINT16_H_
#define _DEOGLDYNAMICTBOUINT16_H_

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
class deoglDynamicTBOUInt16{
public:
	deoglRenderThread &pRenderThread;
	int pComponentCount;
	GLuint pVBO;
	GLuint pTBO;
	
	uint16_t *pDataUInt;
	int pDataSize;
	int pDataCount;
	
	int pMemoryGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dynamic tbo. */
	deoglDynamicTBOUInt16( deoglRenderThread &renderThread, int componentCount );
	
	/** \brief Clean up dynamic tbo. */
	~deoglDynamicTBOUInt16();
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
	
	/** \brief Add integer value. */
	void AddInt( uint16_t value );
	
	/** \brief Add two integer values. */
	void AddVec2( uint16_t value1, uint16_t value2 );
	void AddVec2( const decPoint &value );
	
	/** \brief Add three integer values. */
	void AddVec3( uint16_t value1, uint16_t value2, uint16_t value3 );
	void AddVec3( const decPoint3 &value );
	
	/** \brief Add four integer values. */
	void AddVec4( uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4 );
	
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
