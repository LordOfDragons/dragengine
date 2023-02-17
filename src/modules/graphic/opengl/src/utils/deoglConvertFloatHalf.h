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

#ifndef _DEOGLCONVERTFLOATHALF_H_
#define _DEOGLCONVERTFLOATHALF_H_


/**
 * Convert Float to Half and vice verse.
 */
typedef unsigned short HALF_FLOAT;

/**
 * Converts a 32-bit float value to a 16-bit float value suitable for OpenGL 16-bit textures.
 * The conversion is a safe conversion properly converting NaN and Inf values.
 */
HALF_FLOAT convertFloatToHalf( float f );

/**
 * Converts a 16-bit float value to a 32-bit float value as used in OpenGL 16-bit textures.
 * The conversion is a safe conversion properly converting NaN and Inf values.
 */
float convertHalfToFloat( HALF_FLOAT h );

/**
 * Converts a 32-bit float value to a 16-bit float value suitable for OpenGL 16-bit textures.
 * The conversion is a quick conversion not converting NaN and Inf values. The input value
 * has to be inside the range of a non-NaN/non-Inf 16-bit float value or the result is incorrect.
 */
inline HALF_FLOAT quickConvertFloatToHalf( float f ){
	const float * const keepCompilerHappy = &f;
	const unsigned int x = *( ( unsigned int * )keepCompilerHappy );
	const unsigned short sign = ( unsigned short )( x >> 31 );
	const unsigned int mantissa = x & 0x7fffff;
	const unsigned int exp = x & 0x7f800000;
	
	if( exp <= 0x38000000 ){
		return ( ( ( HALF_FLOAT )sign ) << 15 ) | ( HALF_FLOAT )( mantissa >> ( 14 + ( ( 0x38000000 - exp ) >> 23 ) ) );
		
	}else{
		return ( ( ( HALF_FLOAT )sign ) << 15 ) | ( HALF_FLOAT )( ( exp - 0x38000000 ) >> 13 ) | ( HALF_FLOAT )( mantissa >> 13 );
	}
}

/**
 * Converts a 16-bit float value to a 32-bit float value as used in OpenGL 16-bit textures.
 * The conversion is a quick conversion not converting NaN and Inf values. The input value
 * has to be inside the range of a non-NaN/non-Inf 16-bit float value or the result is incorrect.
 */
inline float quickConvertHalfToFloat( HALF_FLOAT h ){
	const unsigned int sign = ( unsigned int )( h >> 15 );
	const unsigned int mantissa = ( unsigned int )( h & 0x3ff ) << 13;
	const unsigned int exp = ( ( unsigned int )( h & 0x47800000 ) << 13 ) + 0x38000000;
	const unsigned int f = ( sign << 31 ) | exp | mantissa;
	const unsigned int * const keepCompilerHappy = &f;
	
	return *( ( float* )keepCompilerHappy );
}

#endif
