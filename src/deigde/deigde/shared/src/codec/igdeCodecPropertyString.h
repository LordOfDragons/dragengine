/* 
 * Drag[en]gine IGDE
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
class igdeCodecPropertyString{
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
