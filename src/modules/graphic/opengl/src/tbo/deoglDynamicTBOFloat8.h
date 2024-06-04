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

#ifndef _DEOGLDYNAMICTBOFLOAT8_H_
#define _DEOGLDYNAMICTBOFLOAT8_H_

#include "deoglDynamicTBO.h"
#include "../utils/deoglConvertFloatHalf.h"


/**
 * Stores a dynamic texture buffer object destined to be filled and rendered often.
 * The data is first assembled into an array of unsigned char values. Once
 * done the written data is uploaded to the VBO. The VBO grows with the needed
 * size but never shrinks. This way the usage is as fast as possible.
 */
class deoglDynamicTBOFloat8 : public deoglDynamicTBO{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic tbo. */
	deoglDynamicTBOFloat8( deoglRenderThread &renderThread, int componentCount );
	
protected:
	/** Clean up dynamic tbo. */
	virtual ~deoglDynamicTBOFloat8();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Data entries. */
	inline uint8_t *GetDataFloat(){ return ( uint8_t* )pData; }
	inline const uint8_t *GetDataFloat() const{ return ( const uint8_t* )pData; }
	
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
	void AddVec3( const decColor &value );
	
	/** Add four float values. */
	void AddVec4( float value1, float value2, float value3, float value4 );
	void AddVec4( const decVector &value, float value4 );
	void AddVec4( const decVector4 &value );
	void AddVec4( const decColor &value );
	
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
	void SetVec3At( int offset, const decColor &value );
	
	/** Set four float values at offset in components. */
	void SetVec4At( int offset, float value1, float value2, float value3, float value4 );
	void SetVec4At( int offset, const decVector &value, float value4 );
	void SetVec4At( int offset, const decVector4 &value );
	void SetVec4At( int offset, const decColor &value );
	
	/** Debug print. */
	virtual void DebugPrint();
	
	/** TBO format. */
	virtual GLenum GetTBOFormat();
	/*@}*/
};

#endif
