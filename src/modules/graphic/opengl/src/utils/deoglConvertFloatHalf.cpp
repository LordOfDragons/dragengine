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

#include "deoglConvertFloatHalf.h"



// Taken from http://my.safaribooksonline.com/book/programming/opengl/9780321563835/gl-half-float-oes/app01lev1sec2

// -15 stored using a single precision bias of 127
const unsigned int  HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP = 0x38000000;

// max exponent value in single precision that will be converted to Inf or Nan when stored as a half-float
const unsigned int  HALF_FLOAT_MAX_BIASED_EXP_AS_SINGLE_FP_EXP = 0x47800000;

// 255 is the max exponent biased value
const unsigned int  FLOAT_MAX_BIASED_EXP = ( 0xff << 23 );

const unsigned int  HALF_FLOAT_MAX_BIASED_EXP = ( 0x1f << 10 );



HALF_FLOAT convertFloatToHalf( float f ){
	const float * const keepCompilerHappy = &f;
	const unsigned int x = *( ( unsigned int * )keepCompilerHappy );
	const unsigned short sign = ( unsigned short )( x >> 31 );
	unsigned int mantissa;
	unsigned int exp;
	HALF_FLOAT h;
	
	// get mantissa
	mantissa = x & ( ( 1 << 23 ) - 1 );
	
	// get exponent bits
	exp = x & FLOAT_MAX_BIASED_EXP;
	if( exp >= HALF_FLOAT_MAX_BIASED_EXP_AS_SINGLE_FP_EXP ){
		// check if the original single precision float number is a NaN
		if( mantissa && ( exp == FLOAT_MAX_BIASED_EXP ) ){
			// we have a single precision NaN
			mantissa = ( 1 << 23 ) - 1;
			
		}else{
			// 16-bit half-float representation stores number as Inf
			mantissa = 0;
		}
		
		h = ( ( ( HALF_FLOAT )sign ) << 15 )
			| ( HALF_FLOAT )HALF_FLOAT_MAX_BIASED_EXP
			| ( HALF_FLOAT )( mantissa >> 13 );
		
	// check if exponent is <= -15
	}else if( exp <= HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP ){
		// store a denorm half-float value or zero
		exp = ( HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP - exp ) >> 23;
		mantissa >>= 14 + exp;
		
		h = ( ( ( HALF_FLOAT )sign ) << 15 ) | ( HALF_FLOAT )mantissa;
		
	}else{
		h = ( ( ( HALF_FLOAT )sign ) << 15 )
			| ( HALF_FLOAT )( ( exp - HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) >> 13 )
			| ( HALF_FLOAT )( mantissa >> 13 );
	}
	
	return h;
}



float convertHalfToFloat( HALF_FLOAT h ){
	const unsigned int sign = ( unsigned int )( h >> 15 );
	unsigned int mantissa = ( unsigned int )( h & ( ( 1 << 10 ) - 1 ) );
	unsigned int exp = ( unsigned int )( h & HALF_FLOAT_MAX_BIASED_EXP );
	
	if( exp == HALF_FLOAT_MAX_BIASED_EXP ){
		// we have a half-float NaN or Inf
		// half-float NaNs will be converted to a single precision NaN
		// half-float Infs will be converted to a single precision Inf
		exp = FLOAT_MAX_BIASED_EXP;
		if( mantissa ){
			mantissa = ( 1 << 23 ) - 1; // set all bits to indicate a NaN
		}
		
	}else if( exp == 0 ){
		// convert half-float zero/denorm to single precision value
		if( mantissa ){
			mantissa <<= 1;
			exp = HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
			
			// check for leading 1 in denorm mantissa
			while( ( mantissa & ( 1 << 10 ) ) == 0 ){
				// for every leading 0, decrement single precision exponent by 1 and shift half-float mantissa value to the left
				mantissa <<= 1;
				exp -= ( 1 << 23 );
			}
			
			// clamp the mantissa to 10-bits
			mantissa &= ( ( 1 << 10 ) - 1 );
			
			// shift left to generate single-precision mantissa of 23-bits
			mantissa <<= 13;
		}
		
	}else{
		// shift left to generate single-precision mantissa of 23-bits
		mantissa <<= 13;
		
		// generate single precision biased exponent value
		exp = ( exp << 13 ) + HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
	}
	
	union{
		unsigned int in;
		float out;
	} intToFloat;
	intToFloat.in = ( sign << 31 ) | exp | mantissa;
	return intToFloat.out;
}
