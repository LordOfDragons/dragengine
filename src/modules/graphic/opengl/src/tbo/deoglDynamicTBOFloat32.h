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

#include "deoglDynamicTBO.h"


/**
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of float or half-float values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOFloat32 : public deoglDynamicTBO{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBOFloat32( deoglRenderThread &renderThread, int componentCount );
	
	/** Clean up dynamic tbo. */
	virtual ~deoglDynamicTBOFloat32();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Data entries. */
	inline float *GetDataFloat(){ return ( float* )pData; }
	inline const float *GetDataFloat() const{ return ( float* )pData; }
	
	/**
	 * Add boolean value.
	 * 
	 * True adds 1 and False 0.
	 */
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
	
	/** Debug print. */
	virtual void DebugPrint();
	
	/** TBO format. */
	virtual GLenum GetTBOFormat();
	/*@}*/
};

#endif
