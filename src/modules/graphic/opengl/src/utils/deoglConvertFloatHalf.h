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

#ifndef _DEOGLCONVERTFLOATHALF_H_
#define _DEOGLCONVERTFLOATHALF_H_

#include <stdint.h>

/**
 * Convert Float to Half and vice verse.
 */
typedef uint16_t HALF_FLOAT;

/**
 * Converts a 32-bit float value to a 16-bit float value suitable for OpenGL 16-bit textures.
 * The conversion is a safe conversion properly converting NaN and Inf values.
 */
HALF_FLOAT convertFloatToHalf(float f);

/**
 * Converts a 16-bit float value to a 32-bit float value as used in OpenGL 16-bit textures.
 * The conversion is a safe conversion properly converting NaN and Inf values.
 */
float convertHalfToFloat(HALF_FLOAT h);

/**
 * Converts a 32-bit float value to a 16-bit float value suitable for OpenGL 16-bit textures.
 * The conversion is a quick conversion not converting NaN and Inf values. The input value
 * has to be inside the range of a non-NaN/non-Inf 16-bit float value or the result is incorrect.
 */
inline HALF_FLOAT quickConvertFloatToHalf(float f){
	const float * const keepCompilerHappy = &f;
	const unsigned int x = *((unsigned int *)keepCompilerHappy);
	const unsigned short sign = (unsigned short)(x >> 31);
	const unsigned int mantissa = x & 0x7fffff;
	const unsigned int exp = x & 0x7f800000;
	
	if(exp <= 0x38000000){
		return (((HALF_FLOAT)sign) << 15) | (HALF_FLOAT)(mantissa >> (14 + ((0x38000000 - exp) >> 23)));
		
	}else{
		return (((HALF_FLOAT)sign) << 15) | (HALF_FLOAT)((exp - 0x38000000) >> 13) | (HALF_FLOAT)(mantissa >> 13);
	}
}

/**
 * Converts a 16-bit float value to a 32-bit float value as used in OpenGL 16-bit textures.
 * The conversion is a quick conversion not converting NaN and Inf values. The input value
 * has to be inside the range of a non-NaN/non-Inf 16-bit float value or the result is incorrect.
 */
inline float quickConvertHalfToFloat(HALF_FLOAT h){
	const unsigned int sign = (unsigned int)(h >> 15);
	const unsigned int mantissa = (unsigned int)(h & 0x3ff) << 13;
	const unsigned int exp = ((unsigned int)(h & 0x47800000) << 13) + 0x38000000;
	const unsigned int f = (sign << 31) | exp | mantissa;
	const unsigned int * const keepCompilerHappy = &f;
	
	return *((float*)keepCompilerHappy);
}

#endif
