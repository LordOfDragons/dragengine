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

#ifndef _IGDECODECPROPERTYSTRING_H_
#define _IGDECODECPROPERTYSTRING_H_

#include <dragengine/common/math/decMath.h>

class decString;
class decStringList;
class decShapeList;
class decShapeBox;


/**
 * \brief Property String Codec.
 * 
 * Provides support for encoding/decoding various data types from/to property strings.
 */
class DE_DLL_EXPORT igdeCodecPropertyString{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decoder. */
	igdeCodecPropertyString();
	
	/** \brief Clean up decoder. */
	virtual ~igdeCodecPropertyString();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	
	
	/** \name Decoding */
	/*@{*/
	/**
	 * \brief Decode property string into a fixed sized list of floating point values.
	 * 
	 * Returns the number of parsed values.
	 */
	int DecodeFixedFloatList( const char *string, float *values, int valueCount ) const;
	int DecodeFixedDoubleList( const char *string, double *values, int valueCount ) const;
	
	/**
	 * \brief Decode property string into a fixed sized list of integer point values.
	 * 
	 * Returns the number of parsed values.
	 */
	int DecodeFixedIntList( const char *string, int *values, int valueCount ) const;
	
	/** \brief Decode property string into a 3-component vector. */
	void DecodeVector( const char *string, decVector &vector ) const;
	void DecodeDVector( const char *string, decDVector &vector ) const;
	
	/** \brief Decode property string into up to 3-component vector. */
	void DecodeVector( const char *string, decVector &vector, const decVector &defaultVector ) const;
	void DecodeDVector( const char *string, decDVector &vector, const decDVector &defaultVector ) const;
	
	/** \brief Decode property string into a 2-component vector. */
	void DecodeVector2( const char *string, decVector2 &vector ) const;
	
	/** \brief Decode property string into up to 2-component vector. */
	void DecodeVector2( const char *string, decVector2 &vector, const decVector2 &defaultVector ) const ;
	
	/** \brief Decode property string into a 4-component vector. */
	void DecodeVector4( const char *string, decVector4 &vector ) const;
	void DecodeDVector4( const char *string, decDVector4 &vector ) const;
	
	/** \brief Decode property string into up to 4-component vector. */
	void DecodeVector4( const char *string, decVector4 &vector, const decVector4 &defaultVector ) const;
	void DecodeDVector4( const char *string, decDVector4 &vector, const decDVector4 &defaultVector ) const;
	
	/** \brief Decode property string into a 3-component color. */
	void DecodeColor3( const char *string, decColor &color ) const;
	
	/** \brief Decode property string into a 4-component color. */
	void DecodeColor4( const char *string, decColor &color ) const;
	
	/** \brief Decode property string into up to 4-component color filling undefined with default. */
	void DecodeColor( const char *string, decColor &color, const decColor &defaultColor ) const;
	
	/** \brief Decode property string into a shape list. */
	void DecodeShapeList( const char *string, decShapeList &list ) const;
	/*@}*/
	
	
	
	/** \name Encoding */
	/*@{*/
	/** \brief Encode a string list into a property string. */
	void EncodeStringList( const decStringList &list, decString &string ) const;
	
	/** \brief Encode a 3-component vector into a property string. */
	void EncodeVector( const decVector &vector, decString &string ) const;
	void EncodeDVector( const decDVector &vector, decString &string ) const;
	
	/** \brief Encode a 2-component vector into a property string. */
	void EncodeVector2( const decVector2 &vector, decString &string ) const;
	
	/** \brief Encode a 4-component vector into a property string. */
	void EncodeVector4( const decVector4 &vector, decString &string ) const;
	void EncodeDVector4( const decDVector4 &vector, decString &string ) const;
	
	/** \brief Encode a 3-component color into a property string. */
	void EncodeColor3( const decColor &color, decString &string ) const;
	
	/** \brief Encode a 4-component color into a property string. */
	void EncodeColor4( const decColor &color, decString &string ) const;
	
	/** \brief Encode shape list into a property string. */
	void EncodeShapeList( const decShapeList &list, decString &string ) const;
	/*@}*/
};

#endif
